#include <Pokitto.h>

#include <File>

#include "globals.h"
//#include "sound.h"
#include "sprites.h"
#include "font.h"
#include "background.h"
#include "buttonhandling.h"
#include "screen.h"

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

    uint16_t tempLine[512];

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
        file.read(&bg.mapWidth, 2);
        file.read(&bg.mapHeight, 2);
    }
    updateMap(bg.mapX>>3,bg.mapY>>3);
}


int collisionCheck(int x, int y){

    int px1 = x-bg.mapX+bg.windowX;
    int py1 = y-bg.mapY+bg.windowY;
    int px2 = px1>>3; // /8
    int py2 = py1>>3; // /8
    int pixel = 0;

    player.colTile = (bg.miniMap[2+ px2 + bg.miniMap[0] * py2]>>10)&31;

    int px = x&7; // is &7 faster than %8 ?
    int py = y&7;
    pixel = collisionTile[(64*player.colTile)+px+8*py];
        
    return pixel;
}


void gameLogic(){

        int oldMapX = bg.mapX;
        int oldMapY = bg.mapY;

        player.speed = 2;
        if(_B[HELD]) player.speed=4;
        
        if(_Left[HELD]){

            for(int count=0; count<player.speed; count++){
                if(collisionCheck(player.x-count-(player.width/2), player.y) && !collisionCheck(player.x-count-(player.width/2), player.y-1)){
                    player.y-=1;
                    exit;
                }
                if(!collisionCheck(player.x-count-(player.width/2), player.y)){
                    player.x-=1;
                }
            }

        }
        
        if(_Right[HELD]){

            for(int count=0; count<player.speed; count++){
                if(collisionCheck(player.x+(player.width/2)+count, player.y) && !collisionCheck(player.x+(player.width/2)+count, player.y-1)){
                    player.y-=1;
                    exit;
                }
                if(!collisionCheck(player.x+(player.width/2)+count, player.y)){
                    player.x+=1;
                }
            }
        }

        if(_A[NEW] && player.gravity==0){
            player.gravity = -12;
        }


        // apply gravity
        int gravity = 1;
        player.y += player.gravity;
        player.gravity += gravity;
        if (player.gravity > 8) {
            player.gravity = 8;
        }
    
        //int number = player.gravity*10;
        int collision=0;
        int oldCollision=0;
        for(int count=0; count<player.gravity; count++){
            oldCollision = collision;
            collision = collisionCheck(player.x+4, player.y+count+8);
    
            if(collision && !oldCollision){
                player.gravity = 0;
                player.y+=count;
                exit;
            };
    
        }

        
 /*       
        if(_A[HELD]){
            if(_Up[HELD])   player.y--;
            if(_Down[HELD]) player.y++;
            if(_Left[HELD]) player.x--;
            if(_Right[HELD])player.x++;
        }

        if(_Up[NEW])   player.y--;
        if(_Down[NEW]) player.y++;
        if(_Left[NEW]) player.x--;
        if(_Right[NEW])player.x++;
*/

        bg.mapX = player.x-110;
        bg.mapY = player.y-88;

        if(bg.mapX<0) bg.mapX=0;
        if(bg.mapX>(bg.mapWidth*bgTileSizeW)-220-player.speed) bg.mapX=(bg.mapWidth*bgTileSizeW)-220-player.speed;
        if(bg.mapY<0) bg.mapY=0;
        if(bg.mapY>(bg.mapHeight*bgTileSizeH)-176-player.speed) bg.mapY=(bg.mapHeight*bgTileSizeH)-176-player.speed;

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


/*
        // render the collision map with sprites as a test
        //uint32_t tileIndex = bg.windowX/bgTileSizeW + (bg.windowY/bgTileSizeH) * bg.collisionMap[0];
        for(int y=0; y<22*2; y++){
            for(int x=0; x<28*2; x++){
                int temp = (bg.miniMap[2+x+bg.miniMap[0]*y]>>10)&31;

                if(temp >0){
                    Pokitto::Display::drawSprite(-bg.windowX +  x*8,-bg.windowY + y*8, tempSprite2);
                }
                
            }
        }
*/
/*
        printline = 4;
        collisionCheck(player.x, player.y);
        collisionCheck(player.x+8, player.y);
        collisionCheck(player.x, player.y+8);
        collisionCheck(player.x+8, player.y+8);


        char tempText[10];
        int l=0;
        sprintf(tempText,"W:%03d,%03d",bg.windowX,bg.windowY); myPrint(0,++l*8,tempText);
        sprintf(tempText,"M:%03d,%03d",bg.mapX, bg.mapY); myPrint(0,++l*8,tempText);
        sprintf(tempText,"P:%03d,%03d",player.x, player.y); myPrint(0,++l*8,tempText);
*/


    
    // player sprite    
    Pokitto::Display::drawSprite(player.x-(player.width/2)-bg.mapX, player.y-player.height-bg.mapY, sprite[0]);

}







int main(){
    using PC=Pokitto::Core;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;
    using PS=Pokitto::Sound;

    PC::begin();
    PD::load565Palette(palette);
    PD::invisiblecolor = 0;
    PD::persistence = true;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;


    setFPS(50);

    frameCount=0;

    player.x=64;
    player.y=64;
    player.frame=0;

    // line filler test
    // 0 = tile layer
    // 1 = sprite layer
    // 2 = next layer
    PD::lineFillers[0] = myBGFiller; // map layer
    PD::lineFillers[3] = PD::lineFillers[1]; // sprite layer
    PD::lineFillers[1] = myBGFiller2; // background map layer
    //PD::lineFillers[2] = myBGFiller3; // collision layer
    
    initMap();

    //enableDAC();
    //playRandomTune();
    
    PS::playMusicStream("music/tiletest/dkcjh2.pcm",0);

    updateButtons(); // update buttons
    while(_A[HELD]){
        updateButtons(); // update buttons
    }

    while( PC::isRunning() ){
        
        //if( !PC::update(0) ) continue;
        //PS::updateStream();

        //updateStream();

        // limit FPS if we need to

        if(PC::getTime()-tempTime > myDelay){
            tempTime = PC::getTime();
            updateButtons(); // update buttons
            gameLogic();
            lcdRefreshTASMode(Pokitto::Display::palette); // update screen
            fpsCounter++;
            frameCount++;
        }


        char tempText[10];
        sprintf(tempText,"FPS:%d",fpsCount);
        myPrint(0,0,tempText);
        sprintf(tempText,"%d bytes",lastLoad);
        myPrint(0,8,tempText);
        sprintf(tempText,"%d,%d",bg.mapWidth,bg.mapHeight);
        myPrint(0,16,tempText);


        if(PC::getTime() >= lastMillis+1000){
            lastMillis = PC::getTime();
            fpsCount = fpsCounter;
            fpsCounter = 0;
        }

/*
        if(frameCount %3 == 0){
            interlaceScreen = false;
        }else{
            interlaceScreen = true;
        }
*/
/*
        if(fpsCounter < 50){
            interlaceScreen = true;
        }else{
            interlaceScreen = false;
        }
*/


//    offsetAngle +=1;
//    if(offsetAngle>=360)offsetAngle=0;

    


    }
    
    return 0;
}


/*
672x64 map or 857 8x8 tiles, with h-flipping
*/
