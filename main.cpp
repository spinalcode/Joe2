#include <Pokitto.h>
#include <LibHotswap>

#include <File>

#include "globals.h"
//#include "sound.h"
#include "sprites.h"
#include "font.h"

#include "buttonhandling.h"
#include <LibAudio>

// Make a hotswap area for palette data
// size = 512 bytes, unique id = 0
using LevelData = Hotswap<72*1024, 0>; // multiple of 4kb = Palette and level tiles
//using MidMap = Hotswap<4096, 1>; // background image tile map - multiple of 4kb

// pointers to our data
const uint8_t* levelData = nullptr;
const int16_t* midmap = nullptr;
const uint8_t* tiles = nullptr;
const uint8_t* collisionTile = nullptr;

#include "background.h"
#include "screen.h"

double saturation = 0;

#define  Pr  .299
#define  Pg  .587
#define  Pb  .114

//  public-domain function by Darel Rex Finley
//
//  The passed-in RGB values can be on any desired scale, such as 0 to
//  to 1, or 0 to 255.  (But use the same scale for all three!)
//
//  The "change" parameter works like this:
//    0.0 creates a black-and-white image.
//    0.5 reduces the color saturation by half.
//    1.0 causes no change.
//    2.0 doubles the color saturation.
//  Note:  A "change" value greater than 1.0 may project your RGB values
//  beyond their normal range, in which case you probably should truncate
//  them to the desired range before trying to use them in an image.

void changeSaturation(double *R, double *G, double *B, double change) {

  double  P=sqrt(
  (*R)*(*R)*Pr+
  (*G)*(*G)*Pg+
  (*B)*(*B)*Pb ) ;

  *R=P+((*R)-P)*change;
  *G=P+((*G)-P)*change;
  *B=P+((*B)-P)*change;
    
}
  
  
void reSaturate(double changeRed, double changeGreen, double changeBlue){
    
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

int printline = 4;

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

    //uint16_t tempLine[512];

    File file;
    if(file.openRO("level01.bin")){
        for(int y=0; y<bg.miniMap[1]; y++){
            file.seek(4+(mx*2)+largeMapWidth*(my+y));
            file.read(&bg.miniMap[t], miniMapWidth);
            t+=bg.miniMap[0];
            lastLoad += miniMapWidth;
        }
    }

/*
    for(int y=0; y<22; y++){
        for(int x=0; x<28; x++){
            printf("%02d ",(bg.miniMap[2+ x + bg.miniMap[0] * y]>>10)&31);
        }
        printf("\r\n");
    }
    printf("\r\n");
    printf("\r\n");
*/
}


void initMap(){
    File file;
    if(file.openRO("level01.bin")){
        printf("opened level01.bin");
        file.read(&bg.mapWidth, 2);
        file.read(&bg.mapHeight, 2);
    }
    printf("level01.bin failed to open");
    updateMap(bg.mapX>>3,bg.mapY>>3);
}


int checkCollision(int x, int y){

    int px1 = x-bg.mapX+bg.windowX;
    int py1 = y-bg.mapY+bg.windowY;
    int px2 = px1>>3; // /8
    int py2 = py1>>3; // /8
    int pixel = 0;
    
    int colTile = (bg.miniMap[2+ px2 + bg.miniMap[0] * py2]>>10)&31;

    int px = x&7; // is &7 faster than %8 ?
    int py = y&7;
    pixel = collisionTile[(64*colTile)+px+8*py];
        
    return pixel;
}

int leftCollision(int x, int y){
    return checkCollision(x+player.leftBound, y+player.lowerBound);
}
int rightCollision(int x, int y){
    return checkCollision(x+player.rightBound, y+player.lowerBound);
}
int downCollision(int x, int y){
    return checkCollision(x+player.centre, y+player.lowerBound);
}



void gameLogic(){

    if(_B_But[NEW]){
        saturation += .05;
        if(saturation > 1.0)saturation = 1.0;
        reSaturate(0,0,saturation);
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
    if(player.vy > 1023) player.vy = 1023; // limit falling speed to 4 pixels per frame
    player.y += player.vy; // apply falling speed to player position
    
    while (downCollision(player.x>>8, player.y>>8)) { // if player feet inside collision layer
		player.y -= 128; // move player up by half pixel
    }

    if (downCollision(player.x>>8, (player.y+128)>>8)) { // if player feet inside collision layer
	    player.vy = 0; // stop from falling
        player.onGround = true; // say were on the ground
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
    if(player.frame==2){
        player.hatY = 4;
    }
    if(player.vy <0){
        player.hatFrame = 7;
        player.hatX *=5;
        player.hatY = 3;
    }


    // player sprite    
    Pokitto::Display::drawSprite((player.x>>8)-bg.mapX, (player.y>>8)-bg.mapY, player_sprite[player.frame], 0, player.flip);
    Pokitto::Display::drawSprite((player.x>>8)-bg.mapX-player.hatX, (player.y>>8)-bg.mapY-player.hatY, player_sprite[player.hatFrame], 0, player.flip);


    Pokitto::Display::drawSprite(0, 32, color_sprite[sprite_anim_frame/2],0,0,240);
    if(++sprite_anim_frame==16)sprite_anim_frame=0;

}







int main(){
    using PC=Pokitto::Core;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;
    using PS=Pokitto::Sound;

    PC::begin();

    int paletteSize = 512;
    int scenerySize = 2048;

    // hotswap the level data    
    levelData = LevelData::load("level01stuff.bin"); 
    midmap = reinterpret_cast<const int16_t*>(levelData + paletteSize);
    tiles = levelData + paletteSize + scenerySize + 2;
    
    uint16_t tilesOffset = *reinterpret_cast<const uint16_t*>(&levelData[paletteSize + scenerySize]);
    
    collisionTile = levelData + paletteSize + scenerySize + 2 + tilesOffset ;//29184;

    reSaturate(0,0,0); // load palette at 100% saturation
    //Pokitto::Display::load565Palette((uint16_t*)levelData);


    // load 16 colour sprite palette into 240+
    for(int t=0; t<16; t++){
        Pokitto::Display::palette[t+240] = color_sprite_pal[t];
    }

    PD::invisiblecolor = 0;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;

    frameCount=0;

    player.x=0;
    player.y=0;
    player.frame=0;

    // line filler test
    // 0 = tile layer
    // 1 = sprite layer
    // 2 = next layer
    PD::lineFillers[0] = myBGFiller; // map layer
    PD::lineFillers[2] = PD::lineFillers[1]; // sprite layer
    PD::lineFillers[1] = myBGFiller2; // background map layer
    //PD::lineFillers[3] = wiggleFill; // collision layer
    //PD::lineFillers[3] = myBGFiller3; // collision layer
    
    initMap();

    auto music = Audio::play("music/tiletest/dkcjh2.pcm"); // streams are on channel 0 by default
    if(music) music->setLoop(true);


    updateButtons(); // update buttons
    while(_A_But[HELD]){
        updateButtons(); // update buttons
    }

    
    PD::setTASRowMask(0b1111'11111111'11111111);
    clearScreen=true; PC::update(); PC::update(); clearScreen=false;
    // a little 'wide-screening' to remove 16 lines for higher frame rate
    PD::setTASRowMask(0b0111'11111111'11111110);
    

    while( PC::isRunning() ){
        
        if( !PC::update() ) continue;

/*
        // for wiggling the screen!
        offsetAngle +=1;
        if(offsetAngle>=360)offsetAngle=0;
*/  

        // limit FPS if we need to

    //    if(PC::getTime()-tempTime > myDelay){
    //        tempTime = PC::getTime();
            updateButtons(); // update buttons
            gameLogic();
            //lcdRefreshTASMode(Pokitto::Display::palette); // update screen
            fpsCounter++;
            //frameCount++;
    //    }


        //Pokitto::Display::drawSprite(220-32, 0, palette_sprite);

        char tempText[10];
        sprintf(tempText,"FPS:%d",fpsCount);
        myPrint(0,8,tempText);

        sprintf(tempText,"Sat:%f",saturation);
        myPrint(0,16,tempText);

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
/*
    saturation += .005;
    if(saturation > 1.0)saturation = 1.0;
    reSaturate(1-saturation);
*/
    }
    
    return 0;
}

