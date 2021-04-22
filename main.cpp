#include <Pokitto.h>
#include <LibHotswap>

#include <File>

#include "globals.h"
//#include "sound.h"
#include "sprites.h"
#include "font.h"

#include "buttonhandling.h"
#include <LibAudio>

// Make a hotswap area for level data
// size = 512 bytes, unique id = 0
using LevelData = Hotswap<72*1024, 0>; // multiple of 4kb = Palette and level tiles
//using MidMap = Hotswap<4096, 1>; // background image tile map - multiple of 4kb

// pointers to our data
const uint8_t* levelData = nullptr;
const int16_t* midmap = nullptr;
const uint8_t* tiles = nullptr;
const uint8_t* collisionTile = nullptr;

int levelNumber = 0;

#include "background.h"
#include "screen.h"

double saturation = 0;

char levelFilename[32];
char levelTilename[32];

int mapWidth=0;
int mapHeight=0;
int numGems=0;

void waitButton(){
    updateButtons();
    while(!_A_But[NEW]) updateButtons();
}

void reSaturate(double changeRed, double changeGreen, double changeBlue){
    
    #define  Pr  .299
    #define  Pg  .587
    #define  Pb  .114

    for(int t=0; t<256; t++){ // leave the ;ast 16 colours alone
        if(t<=240){
            gamePalette.rgb[t] = *reinterpret_cast<const uint16_t*>(&levelData[t*2]);
        
            gamePalette.r[t] = (gamePalette.rgb[t] & 0xF800) >> 11;
            gamePalette.g[t] = (gamePalette.rgb[t] & 0x7E0) >> 5;
            gamePalette.b[t] = (gamePalette.rgb[t] & 0x1F);
            //float grey = (0.2126 * gamePalette.r[t]) + (0.7152 * gamePalette.g[t] /2) + (0.0722 * gamePalette.b[t]);
            
            double R8 = ( gamePalette.r[t] * 527 + 23 ) >> 6;
            double G8 = ( gamePalette.g[t] * 259 + 33 ) >> 6;
            double B8 = ( gamePalette.b[t] * 527 + 23 ) >> 6;
    
            //  public-domain function by Darel Rex Finley
            double  P=sqrt(
              (R8)*(R8)*Pr+
              (G8)*(G8)*Pg+
              (B8)*(B8)*Pb ) ;
            
            R8=P+((R8)-P)*changeRed;
            G8=P+((G8)-P)*changeGreen;
            B8=P+((B8)-P)*changeBlue;
    
            uint8_t R5 = (( (int)R8 * 249 + 1014 ) >> 11)&31;
            uint8_t G6 = (( (int)G8 * 253 +  505 ) >> 10)&63;
            uint8_t B5 = (( (int)B8 * 249 + 1014 ) >> 11)&31;
    
            gamePalette.rgb[t] = (R5<<11)|(G6<<5)|B5;
        }else{
            gamePalette.rgb[t] = color_sprite_pal[t-240];
        }

    }
    
    Pokitto::Display::load565Palette(gamePalette.rgb);
}


// print text
void myPrint(char x, char y, const char* text) {
  uint8_t numChars = strlen(text);
  uint8_t x1 = 0;//2+x+28*y;
  for (uint8_t t = 0; t < numChars; t++) {
    uint8_t character = text[t] - 32;
    Pokitto::Display::drawSprite(x+((x1++)*8), y, font88[character]);
  }
}

void setFPS(int fps){
  myDelay = 1000 / fps;
}

int lastLoad=0;

void updateMap(int mx, int my){
    lastLoad=0;
    
    uint32_t t=0;
    bg.miniMap[t++] = 28*2;
    bg.miniMap[t++] = 22*2;
    uint32_t largeMapWidth = bg.mapWidth*2; // tiles * 2bytes
    uint32_t miniMapWidth = bg.miniMap[0]*2;

    File file;
    if(file.openRO(levelFilename)){
        for(int y=0; y<bg.miniMap[1]; y++){
            file.seek(4+(mx*2)+largeMapWidth*(my+y));
            file.read(&bg.miniMap[t], miniMapWidth);
            t+=bg.miniMap[0];
            lastLoad += miniMapWidth;
        }
    }
    file.close();
}

void initMap(){
    File file1;
    if(file1.openRO(levelFilename)){
        file1.read(&bg.mapWidth, 2);
        file1.read(&bg.mapHeight, 2);
    }
    updateMap(bg.mapX>>3,bg.mapY>>3);
    file1.close();
}


int checkCollision(int x, int y){

    int px1 = x-bg.mapX+bg.windowX;
    int py1 = y-bg.mapY+bg.windowY;
    int px2 = px1>>3; // /8
    int py2 = py1>>3; // /8
    int pixel = 0;
    
    int colTile = (bg.miniMap[2+ px2 + bg.miniMap[0] * py2]>>10)&31;
    int flipped = bg.miniMap[2+ px2 + bg.miniMap[0] * py2]&32768;

    int px = x&7; // is &7 faster than %8 ?
    int py = y&7;
    if(flipped == 0){
        pixel = collisionTile[(64*colTile)+px+8*py];
    }else{
        pixel = collisionTile[(64*colTile)+(7-px)+8*py];
    }
        
    return pixel;
}

int leftCollision(int x, int y){
    return checkCollision(x+player.leftBound, y+player.lowerBound);
}
int rightCollision(int x, int y){
    return checkCollision(x+player.rightBound, y+player.lowerBound);
}



void gameLogic(){

    if(_B_But[NEW]){
        saturation += .05;
        if(saturation > 1.0)saturation = 1.0;
        reSaturate(saturation,saturation,saturation);
    }


    bool falling=false;
    bool jumping=false;

    int oldMapX = bg.mapX;
    int oldMapY = bg.mapY;

    player.onGround = false;

    if(_Left_But[NEW]){ player.flip = 1;}
    if(_Right_But[NEW]){ player.flip = 0;}

    if( _Left_But[RELEASED] || _Right_But[RELEASED]){
        player.frame = 0;
        player.step = 0;
    }

    //player.x += (_Right_But[HELD] - _Left_But[HELD]) * PLAYER_SPEED;
    if(_Right_But[HELD]){
        while(rightCollision(player.x>>8, player.y>>8) && rightCollision((player.x>>8)+1, (player.y>>8)-8)){
            player.x -= 128;
        }
        player.x += PLAYER_SPEED;
        player.step ++;
    }

    if(_Left_But[HELD]){
        while(leftCollision(player.x>>8, player.y>>8) && leftCollision((player.x>>8)-1, (player.y>>8)-8)){
            player.x += 128;
        }
        player.x -= PLAYER_SPEED;
        player.step ++;
    }

    if (player.step >= 3) {
        player.frame++;
        player.step = 0;
    }

    if (player.frame >= 4) {
        player.frame = 0;
    }

    // Add gravity
    player.vy += GRAVITY; // apply gravity to falling speed
    if(player.vy > MAXGRAVITY) player.vy = MAXGRAVITY; // limit falling speed to 4 pixels per frame
    player.y += player.vy; // apply falling speed to player position
    
    if(player.vy >= 0 && player.dropping == false){
        int colLeft = checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.lowerBound);
        while (colLeft >0 ) { // if player feet inside collision layer
    		player.y -= 128; // move player up by half pixel
            colLeft = checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.lowerBound);
        }
        colLeft = checkCollision((player.x>>8)+player.centre, ((player.y+128)>>8)+player.lowerBound);
        if (colLeft >0 ) { // if player feet inside collision layer
    	    player.vy = 0; // stop from falling
            player.onGround = true; // say were on the ground
        }
    }


    if(player.vy<0){
        player.jumping = true;
        player.falling = false;
        player.frame = 4;
    }
    if(player.vy>0){
        player.falling = true;
        player.jumping = false;
        player.frame = 5;
    }

    if(player.vy==0){
        player.falling = false;
        player.jumping = false;
    }


	if (_A_But[NEW] && player.onGround == true){ 
	    player.vy = -player.jumpHeight; // Start jumping
	}

    bg.mapX = (player.x>>8)-110;
    bg.mapY = (player.y>>8)-88;

    if(bg.mapX<0) bg.mapX=0;
    if(bg.mapX>(bg.mapWidth*bgTileSizeW)-220) bg.mapX=(bg.mapWidth*bgTileSizeW)-220;
    if(bg.mapY<0) bg.mapY=0;
    if(bg.mapY>(bg.mapHeight*bgTileSizeH)-176) bg.mapY=(bg.mapHeight*bgTileSizeH)-176;

    oldScreenX = screenX;
    oldScreenY = screenY;
    screenX = bg.mapX/224;
    screenY = bg.mapY/176;

	if(oldScreenX != screenX || oldScreenY != screenY){
        int mapPosX = screenX * 224;
        int mapPosY = screenY * 176;
        updateMap( mapPosX/8 , mapPosY/8);
	}
    bg.windowX = bg.mapX%224;
	bg.windowY = bg.mapY%176;

    // draw player sprite
    player.hatX = 1;
    player.hatY = 5;
    player.hatFrame = 6;
    if(player.flip==1){
        player.hatX = -1;
    }
    if(player.frame==2){ // dip in walk
        player.hatY = 4;
    }
    if(player.vy <0){
        player.hatFrame = 7;
        player.hatX *=5;
        player.hatY = 3;
    }



    if(player.vy>-150 && player.vy<150){
        if(player.jumping){
            player.frame=0;
            player.hatX = 1;
            player.hatY = 5;
            player.hatFrame = 6;
        }
    }


    // player sprite    
    Pokitto::Display::drawSprite((player.x>>8)-bg.mapX, (player.y>>8)-bg.mapY, player_sprite[player.frame], 0, player.flip);
    Pokitto::Display::drawSprite((player.x>>8)-bg.mapX-player.hatX, (player.y>>8)-bg.mapY-player.hatY, player_sprite[player.hatFrame], 0, player.flip);


    //items
    for(int t=0; t<maxItems; t++){
        if(items[t].collected == 0){

            items[t].frame++;
            if(items[t].frame >= sizeof(gemFrame)*items[t].speed){items[t].frame=0;}
            if(items[t].type!=0){
                int flipme=0;
                int frame = (items[t].frame/items[t].speed);
                if(frame >= sizeof(gemFrame)/2){
                    flipme=1;
                }
                Pokitto::Display::drawSprite(items[t].x-bg.mapX, items[t].y-bg.mapY, gems[ (items[t].type-1)*(sizeof(gemFrame)/2) + gemFrame[frame]],flipme,0,240);
            } // type = 1 (gem)
    
            // check for collisions in the animation loop
            if((player.x>>8)+player.centre >= items[t].x && (player.x>>8)+player.centre <= items[t].x + 16){
                if((player.y>>8)+player.centre >= items[t].y && (player.y>>8)+player.centre <= items[t].y + 16){
                    items[t].collected = 1;
                    switch(items[t].type){
                        case 1:
                            bg.satRed += 0.025;
                            break;
                        case 2:
                            bg.satGreen += 0.025;
                            break;
                        case 3:
                            bg.satBlue += 0.025;
                            break;
                    }
                    reSaturate(bg.satRed,bg.satGreen,bg.satBlue);
                }
            }
        } // collected
    }

/*
    Pokitto::Display::drawSprite(0, 32, color_sprite[sprite_anim_frame/2],0,0,240);
    if(++sprite_anim_frame==16)sprite_anim_frame=0;
*/
}




void loadLevel(int levNum){

    int paletteSize = 512;

    sprintf(levelFilename,"level0%d.bin",levNum);
    sprintf(levelTilename,"level0%ddata.bin",levNum);

    // hotswap the level data    
    levelData = LevelData::load(levelTilename); 

    uint16_t scenerySize = (*reinterpret_cast<const uint16_t*>(&levelData[paletteSize]))*2;

    midmap = reinterpret_cast<const int16_t*>(levelData + paletteSize +2);
    tiles = levelData + paletteSize + scenerySize + 6;
    
    uint16_t tilesOffset = *reinterpret_cast<const uint16_t*>(&levelData[paletteSize + scenerySize +4]);
    
    collisionTile = levelData + paletteSize + scenerySize + 6 + tilesOffset ;

    reSaturate(0,0,0); // load palette at 100% saturation

    // load 16 colour sprite palette into 240+
    for(int t=0; t<16; t++){
        Pokitto::Display::palette[t+240] = color_sprite_pal[t];
    }
    
    uint16_t mapSize[2];
    File levfile;
    if(levfile.openRO(levelFilename))
        levfile.read(mapSize, 4);
    
    // check collision map for collectable tilesOffset
    mapWidth = mapSize[0];
    mapHeight = mapSize[1];
    numGems = 0;
    levfile.seek(mapSize[0]+4);
    
    player.x = 0;
    player.y = 0;
    numGems = 0;
    
    maxItems=0;
    int x=0;
    int y=0;
    for(int t=0; t<(mapSize[1]*mapSize[0]); t++){
        uint16_t curTile;
        int varSize = sizeof(curTile); // = 2
        levfile.read(&curTile, varSize);
        x++;
        if(x==mapSize[0]/2){x=0; y++;}

            // as the map data is 16bit, count to x*2 and then divitd x by 2
            // or multiply by half as much

        switch(((curTile >> 10)&31)){
            case 3: // 3 = start position
                printf("x:%d y:%d\n",x,y);
                //while(1){;}
                //numGems = x;
                player.startX = (x*8)<<8;
                player.startY = (y*4)<<8;
                player.frame=0;
                break;
            case 4: // 4 = first gem
            case 5: // 4 = first gem
            case 6: // 4 = first gem
                printf("x:%d y:%d\n",x*8,y*8);
                items[maxItems].x = x*8;
                items[maxItems].y = y*4;
                items[maxItems].collected = 0;
                items[maxItems].type = ((curTile >> 10)&31)-3; // gem
                maxItems++;
                break;
        }
    }
    levfile.close();
    //waitButton();
}




void titleScreen(){

    if(_A_But[NEW]){
        reSaturate(0,0,0);
        gameMode=1;
        Pokitto::Display::lineFillers[0] = myBGFiller; // map layer
    
        Pokitto::Display::setTASRowMask(0b1111'11111111'11111111);
        clearScreen=true; Pokitto::Core::update(); Pokitto::Core::update(); clearScreen=false;
        // a little 'wide-screening' to remove 16 lines for higher frame rate
        Pokitto::Display::setTASRowMask(0b0111'11111111'11111110);

    }

}

int main(){
    using PC=Pokitto::Core;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;
    using PS=Pokitto::Sound;

    PC::begin();

    //PC::setFrameRate(5);

    PD::invisiblecolor = 0;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;

    frameCount=0;


    // line filler test
    // 0 = tile layer
    // 1 = sprite layer
    // 2 = next layer
    PD::lineFillers[0] = myTitleFiller; // map layer
    //PD::lineFillers[0] = myBGFiller; // map layer
    PD::lineFillers[2] = PD::lineFillers[1]; // sprite layer
    PD::lineFillers[1] = myBGFiller2; // background map layer
    //PD::lineFillers[3] = wiggleFill; // collision layer
    //PD::lineFillers[3] = myBGFiller3; // collision layer
    

    loadLevel(1);
    initMap();
    loadLevel(1);

    reSaturate(1,1,1);

    auto music = Audio::play("music/tiletest/dkcjh2.pcm"); // streams are on channel 0 by default
    if(music) music->setLoop(true);


    updateButtons(); // update buttons
    while(_A_But[HELD]){
        updateButtons(); // update buttons
    }


    player.x = player.startX;
    player.y = player.startY;


    while( PC::isRunning() ){
        
        if( !PC::update() ) continue;

        switch(gameMode){
            
            case 0: // titlescreen
                titleScreen();
                break;
            case 1: // gameplay
                gameLogic();
                break;
        }   

        updateButtons(); // update buttons
        fpsCounter++;


/*
        // for wiggling the screen!
        offsetAngle +=1;
        if(offsetAngle>=360)offsetAngle=0;
*/  

        // limit FPS if we need to

    //    if(PC::getTime()-tempTime > myDelay){
    //        tempTime = PC::getTime();
//            updateButtons(); // update buttons
//            gameLogic();
            //lcdRefreshTASMode(Pokitto::Display::palette); // update screen
//            fpsCounter++;
            //frameCount++;
    //    }


        //Pokitto::Display::drawSprite(220-32, 0, palette_sprite);

        char tempText[10];
        sprintf(tempText,"FPS:%d",fpsCount);
        myPrint(0,8,tempText);

        //sprintf(tempText,"Gems:%d",varSize);
        //myPrint(0,16,tempText);

//        sprintf(tempText,"Collsision:%d",checkCollision(player.x>>8, player.y>>8));
//        myPrint(0,24,tempText);

//        sprintf(tempText,"Px:%d Py:%d",player.x>>8, player.y>>8);
//        myPrint(0,32,tempText);


//        sprintf(tempText,"X:%d Y:%d",items[2].x, items[2].y);
//        myPrint(0,40,tempText);

        //sprintf(tempText,"Frame:%d",((items[2].type-1)*items[2].numFrames)+(items[2].frame/items[2].speed));
        //myPrint(0,56,tempText);

/*        
        sprintf(tempText,"%d bytes",tilesOffset);
        myPrint(0,8,tempText);
        //sprintf(tempText,"%d,%d",bg.mapWidth,bg.mapHeight);
        //myPrint(0,16,tempText);

        
        if(levelData == NULL){
            myPrint(0,24,"level fail");
        }else{
            for(int t=0; t<14; t++){
                sprintf(tempText,"%d",levelData[512 + 2048 + t]);
                myPrint(0,32+(t*8),tempText);
            }
        }
*/        

        if(PC::getTime() >= lastMillis+1000){
            lastMillis = PC::getTime();
            fpsCount = fpsCounter;
            fpsCounter = 0;
        }

    }
    
    return 0;
}

