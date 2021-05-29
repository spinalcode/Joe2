#include <Pokitto.h>
#include <LibHotswap>
#include <File>

Serial ser(EXT7, EXT6);

#include "globals.h"
//#include "sound.h"
#include "sprites.h"
#include "font.h"

#include "buttonhandling.h"
#include <LibAudio>

#include "background.h"
#include "screen.h"
#include "levelsandmaps.h"


void waitButton(){
    updateButtons();
    while(!_A_But[NEW]) updateButtons();
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
        while(rightCollision(player.x>>8, player.y>>8)==SOLID && rightCollision((player.x>>8)+1, (player.y>>8)-8)==SOLID){
            player.x -= 128;
        }
        player.x += PLAYER_SPEED;
        if(frameSkip==0){player.step ++;}
    }

    if(_Left_But[HELD]){
        while(leftCollision(player.x>>8, player.y>>8)==SOLID && leftCollision((player.x>>8)-1, (player.y>>8)-8)==SOLID){
            player.x += 128;
        }
        player.x -= PLAYER_SPEED;
        if(frameSkip==0){player.step ++;}
    }

    if (player.step >= 2) {
        player.frame++;
        player.step = 0;
    }

    if (player.frame >= 4) {
        player.frame = 0;
    }

    if(_Down_But[NEW]){
        player.dropping = true;
    }


    // Add gravity
    player.vy += GRAVITY; // apply gravity to falling speed
    if(player.vy > MAXGRAVITY) player.vy = MAXGRAVITY; // limit falling speed
    player.y += player.vy; // apply falling speed to player position

    if(player.vy < 0){
        // jumping to check headroom
        int colLeft = checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.upperBound);
        if (colLeft == SOLID) {
    	    player.vy = 0; // stop from jumping
        }
    }

    if(player.vy >= 0){
        int colLeft = checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.lowerBound);
        while (colLeft == SOLID || (colLeft == JUMPTHROUGH && player.dropping==false) ) { // if player feet inside collision layer
    		player.y -= 128; // move player up by half pixel
            colLeft = checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.lowerBound);
        }
        colLeft = checkCollision((player.x>>8)+player.centre, ((player.y+128)>>8)+player.lowerBound);
        if (colLeft == SOLID || (colLeft == JUMPTHROUGH && player.dropping==false) ) { // if player feet inside collision layer
    	    player.vy = 0; // stop from falling
            player.onGround = true; // say were on the ground
            player.dropping=false;
        }
    }

    if(player.dropping==true){
        int colLeft = checkCollision((player.x>>8)+player.centre, ((player.y+128)>>8)+player.lowerBound);
        if (colLeft == 0 ) { // falling through air
    	    player.dropping = false; // stop from dropping
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
        player.hatX *=6;
        player.hatY = 2;
    }

    if(player.vy>-200 && player.vy<200){
        if(player.jumping){
            player.frame=0;
            player.hatX = 4;
            player.hatY = 5;
            player.hatFrame = 8;
            if(player.flip==1){
                player.hatX *= -1;
            }
        }
    }


    // player sprite
    if(frameSkip==0){
        Pokitto::Display::drawSprite((player.x>>8)-bg.mapX, (player.y>>8)-bg.mapY, player_sprite[player.frame], 0, player.flip);
        Pokitto::Display::drawSprite((player.x>>8)-bg.mapX-player.hatX, (player.y>>8)-bg.mapY-player.hatY, player_sprite[player.hatFrame], 0, player.flip);
    }
    
    if((player.y>>8)-bg.mapY>220){gameMode = 0;}

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
                
                int theX = items[t].x-bg.mapX;
                int theY = items[t].y-bg.mapY;
                
                if(theX>-16 && theX<220 && theY>-16 && theY<176){
                    if(frameSkip==0){
                        Pokitto::Display::drawSprite(theX, theY, gems[ (items[t].type-1)*(sizeof(gemFrame)/2) + gemFrame[frame]],flipme,0,240);
                    }
                    //char tempText[10];
                    //sprintf(tempText,"%d",items[t].mapPos);
                    //myPrint(theX, theY,tempText);
                    
                }
                

            } // type = 1 (gem)
    
            // check for collisions in the animation loop
            if((player.x>>8)+player.centre >= items[t].x && (player.x>>8)+player.centre <= items[t].x + 16){
                if((player.y>>8)+player.centre >= items[t].y && (player.y>>8)+player.centre <= items[t].y + 16){
                    items[t].collected = 1;
                    switch(items[t].type){
                        case 1:
                            bg.satRed += bg.redPercent;
                            break;
                        case 2:
                            bg.satGreen += bg.greenPercent;
                            break;
                        case 3:
                            bg.satBlue += bg.bluePercent;
                            break;
                    }
                    reSaturate(bg.satRed,bg.satGreen,bg.satBlue);
                }
            }
        } // collected
    }




    //enemies
    for(int t=0; t<maxEnemies; t++){
        if(enemy[t].type != 0){

            if(++enemy[t].frame >= 4*enemy[t].speed){enemy[t].frame=0;}
            int theX = enemy[t].x-bg.mapX;
            int theY = enemy[t].y-bg.mapY;
            if(theX>-16 && theX<220 && theY>-16 && theY<176){

                if(enemy[t].direction==0){
                    if(checkCollision(enemy[t].x+15, enemy[t].y+15) == SOLID ||
                        checkCollision(enemy[t].x+15, enemy[t].y+16) != SOLID ){
                        enemy[t].direction=1;
                    }else{
                        enemy[t].x++;
                    }
                }
                if(enemy[t].direction==1){
                    if(checkCollision(enemy[t].x-1, enemy[t].y+15) == SOLID || 
                        checkCollision(enemy[t].x-1, enemy[t].y+16) != SOLID ){
                        enemy[t].direction=0;
                    }else{
                        enemy[t].x--;
                    }
                }
            
            
                if(frameSkip==0){
                    int flipme=0;
                    Pokitto::Display::drawSprite(theX, theY, enemy1[enemy[t].frame/enemy[t].speed],0,enemy[t].direction);
                }
            }

        } // not dead
    }


/*
    Pokitto::Display::drawSprite(0, 32, color_sprite[sprite_anim_frame/2],0,0,240);
    if(++sprite_anim_frame==16)sprite_anim_frame=0;
*/
}






void titleScreen(){

    if(mustDraw==true) {
        Pokitto::Display::setTASRowMask(0b0000'00000000'00000000);
        File tsFile;
        // titlescreen.bmp MUST be 8bit and upside down. Ther is no error checking!
        if(tsFile.openRO("/joe2/titlescreen.bmp")){
            
            tsFile.seek(54); // skip all data checking, if the bmp is wrong, we get a garbled mess
            uint8_t RGBA[256*4];
            tsFile.read(RGBA, 256*4);

            for(int t=0; t<256; t++){
                // convert 888 to 565 for reloading
                uint8_t R5 = (( (int)RGBA[2+(t*4)] * 249 + 1014 ) >> 11)&31;
                uint8_t G6 = (( (int)RGBA[1+(t*4)] * 253 +  505 ) >> 10)&63;
                uint8_t B5 = (( (int)RGBA[0+(t*4)] * 249 + 1014 ) >> 11)&31;
            
                gamePalette.rgb[t] = (R5<<11)|(G6<<5)|B5;
            }        
            Pokitto::Display::load565Palette(gamePalette.rgb);

            uint8_t buff[220];
            Pokitto::lcdPrepareRefresh();
            for(int y=0; y<176; y++){
                tsFile.read(buff, 220);
                flushLine(Pokitto::Display::palette, buff);
            }
            Pokitto::Display::update(); // needed?
        }
        mustDraw = false;
    }

    if(_A_But[NEW]){
        
        reSaturate(0,0,0);
        gameMode=1;
    
        loadLevel(1);
    
        Pokitto::Display::lineFillers[0] = myBGFiller; // map layer
        //Pokitto::Display::lineFillers[2] = Pokitto::Display::lineFillers[1]; // sprite layer
        Pokitto::Display::lineFillers[1] = myBGFiller2; // background map layer

        Pokitto::Display::setTASRowMask(0b1111'11111111'11111111);
        clearScreen=true; Pokitto::Core::update(); Pokitto::Core::update(); clearScreen=false;
        // a little 'wide-screening' to remove 16 lines for higher frame rate
        Pokitto::Display::setTASRowMask(0b0111'11111111'11111110);

        printf("px:%d py:%d\n",player.startX>>8,player.startY>>8);
        printf("px:%d py:%d\n",player.x>>8,player.y>>8);
        mustDraw=true;
    }

}

int main(){
    using PC=Pokitto::Core;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;
    using PS=Pokitto::Sound;

    PC::begin();

    //PC::setFrameRate(10);

    PD::invisiblecolor = 0;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;

    // line filler test
    // 0 = tile layer
    // 1 = sprite layer
    // 2 = next layer
    //PD::lineFillers[0] = myTitleFiller; // map layer
    //PD::lineFillers[0] = myBGFiller; // map layer
    //PD::lineFillers[2] = PD::lineFillers[1]; // sprite layer
    //PD::lineFillers[1] = myBGFiller2; // background map layer
    //PD::lineFillers[3] = wiggleFill; // collision layer
    //PD::lineFillers[3] = myBGFiller3; // collision layer
    
    //reSaturate(1,1,1);

    // This can only be done once, as were swapping layers around
    Pokitto::Display::lineFillers[2] = Pokitto::Display::lineFillers[1]; // sprite layer


    auto music = Audio::play("/joe2/C_8000.pcm"); // streams are on channel 0 by default
    if(music) music->setLoop(true);


    updateButtons(); // update buttons
    while(_A_But[HELD]){
        updateButtons(); // update buttons
    }


    mustDraw=true;
    ser.baud(115200);
    printf("Joe2\r\n");

    frameSkip=0;
    while( PC::isRunning() ){
        

        if(frameSkip==0){
            if( !PC::update() ) continue;
        }else{
            PC::update(0); // don't update screen.
        }
        frameSkip = 1-frameSkip;

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
    //    }


        //Pokitto::Display::drawSprite(220-32, 0, palette_sprite);

        char tempText[100];
        sprintf(tempText,"FPS:%d",fpsCount);
        myPrint(0,8,tempText);

        sprintf(tempText,"vx:%d", player.vy);
        myPrint(0,16,tempText);

        sprintf(tempText,"Collsision:%d",checkCollision(player.x>>8, player.y>>8));
        myPrint(0,24,tempText);

        //sprintf(tempText,"frameSkip:%d", frameSkip);
        //myPrint(0,32,tempText);


        if(PC::getTime() >= lastMillis+1000){
            lastMillis = PC::getTime();
            fpsCount = fpsCounter;
            fpsCounter = 0;
        }

    }
    
    return 0;
}

