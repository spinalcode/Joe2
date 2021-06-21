#include <Pokitto.h>
#include <LibHotswap>
#include <File>
#include "PokittoCookie.h"

#include <SoftwareI2C.h>

#include "globals.h"
#include "sound.h"
#include "sprites.h"
#include "font.h"
#include "samples.h"

#include "buttonhandling.h"

#include "background.h"
#include "screen.h"
#include "levelsandmaps.h"
#include "easing.h"

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
    //int flipped = bg.miniMap[2+ px2 + bg.miniMap[0] * py2]&32768;

    int px = x&7; // is &7 faster than %8 ?
    int py = y&7;
    //if(flipped == 0){
        pixel = collisionTile[(64*colTile)+px+8*py];
    //}else{
    //    pixel = collisionTile[(64*colTile)+(7-px)+8*py];
    //}
        
    return pixel;
}

int leftCollision(int x, int y){
    return checkCollision(x+player.leftBound, y+player.lowerBound);
}
int rightCollision(int x, int y){
    return checkCollision(x+player.rightBound, y+player.lowerBound);
}


void startAnimation(int x, int y, int itemType){

    //printf("x:%d y:%d\n",x,y);

    int useSprite=0;
    for(int t=20; t; t--){
        if(animSprite[t].used==false){useSprite=t;}
    }
    animSprite[useSprite].type=itemType;
    animSprite[useSprite].x = x;
    animSprite[useSprite].y = y;
    animSprite[useSprite].startX = x;
    animSprite[useSprite].startY = y;
    animSprite[useSprite].frame = itemType;
    animSprite[useSprite].used = true;
    animSprite[useSprite].frameCount = 1;
}


void renderSprites(){
    
    //enemies
    for(int t=0; t<maxEnemies; t++){
        if(enemy[t].type != 0){

            if(++enemy[t].frame >= enemy[t].numFrames*enemy[t].speed){enemy[t].frame=0;}
            int theX = enemy[t].x-bg.mapX;
            int theY = enemy[t].y-bg.mapY;
            if(theX>-16 && theX<220 && theY>-16 && theY<176){
                if(enemy[t].type==1){
                    Pokitto::Display::drawSprite(theX, theY, enemy1[enemy[t].frame/enemy[t].speed],0,enemy[t].direction);
                }
                if(enemy[t].type==2){
                    Pokitto::Display::drawSprite(theX, theY, enemy2[enemy[t].frame/enemy[t].speed],0,enemy[t].direction);
                }
                if(enemy[t].type==3){
                    Pokitto::Display::drawSprite(theX, theY+enemy[t].offy, enemy3[enemy[t].frame/enemy[t].speed],0,enemy[t].direction);
                }
            }
        } // not dead
    }

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
                    Pokitto::Display::drawSprite(theX, theY, gems[ (items[t].type-1)*(sizeof(gemFrame)/2) + gemFrame[frame]],flipme,0,240);
                }
            }
        }
    }
    
    // player sprite
    Pokitto::Display::drawSprite((player.x>>8)-bg.mapX, (player.y>>8)-bg.mapY, player_sprite[player.frame], 0, player.flip);
    Pokitto::Display::drawSprite((player.x>>8)-bg.mapX-player.hatX, (player.y>>8)-bg.mapY-player.hatY, player_sprite[player.hatFrame], 0, player.flip);

    // animating sprites - items etc.
    for(int t=0; t<20; t++){
        if(animSprite[t].used==true){
            animSprite[t].frame++;
            if(animSprite[t].frame >= sizeof(gemFrame)*animSprite[t].speed){animSprite[t].frame=0;}
            int flipme=0;
            int frame = (animSprite[t].frame/animSprite[t].speed);
            if(frame >= sizeof(gemFrame)/2){
                flipme=1;
            }
            //Pokitto::Display::drawSprite(animSprite[t].x, animSprite[t].y, gems[(animSprite[t].startFrame-1)*(sizeof(gemFrame)/2) + gemFrame[frame]],flipme,0,240);
            Pokitto::Display::drawSprite(animSprite[t].x, animSprite[t].y, gems[(animSprite[t].type-1)*(sizeof(gemFrame)/2) + gemFrame[frame]],flipme,0,240);

            animSprite[t].x = (animSprite[t].startX-easeInOut(animSprite[t].frameCount, 0, animSprite[t].startX, 15));
            animSprite[t].y = (animSprite[t].startY-easeInOut(animSprite[t].frameCount, 16, animSprite[t].startY-16, 15))+16;
            if(++animSprite[t].frameCount==15){animSprite[t].used = false;}else{HUD_gemTimer = HUD_gemTimerStart;}
        }
    }

    // HUD
    if(HUD_gemTimer){
        //drawHUD=true;
        HUD_gemTimer--;
        int gemY = HUD_gemTimer;
        if(gemY > 16)gemY=16;
        Pokitto::Display::drawSprite(0, gemY-8, color_sprite[ gemFrame[HUD_gemFrameCount/3]], 0 ,0,240);
        //if(HUD_gemTimer%2 == 0)
        HUD_gemFrameCount++;
        if(HUD_gemFrameCount>=24)HUD_gemFrameCount=0;
        char tempText[32];
        snprintf(tempText,sizeof(tempText),"%d/%d",bg.countRed+bg.countGreen+bg.countBlue, bg.numRed+bg.numGreen+bg.numBlue);
        myPrint(20,gemY-4,tempText);
    }

    //Pokitto::Display::drawSprite(0, 32, door[myCounter], 0 ,0,240);
    //if(++myCounter==30)myCounter=0;


}


void checkItemCollisions(){
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
                    // check for collisions in the animation loop
                    if((player.x>>8)+player.centre >= items[t].x-4 && (player.x>>8)+player.centre <= items[t].x + 20){
                        if((player.y>>8)+player.centre >= items[t].y-4 && (player.y>>8)+player.centre <= items[t].y + 20){

                            playSound(1, sfx_drop, 300, random(63)+192);

                            items[t].collected = 1;
                            int theX = items[t].x-bg.mapX;
                            int theY = items[t].y-bg.mapY;
                            startAnimation(theX, theY, items[t].type);
                            switch(items[t].type){
                                case 1:
                                    bg.satRed += bg.redPercent;
                                    bg.countRed++;
                                    break;
                                case 2:
                                    bg.satGreen += bg.greenPercent;
                                    bg.countGreen++;
                                    break;
                                case 3:
                                    bg.satBlue += bg.bluePercent;
                                    bg.countBlue++;
                                    break;
                            }
                            
                            //reSaturate(bg.satRed,bg.satGreen,bg.satBlue);
                            float bigNum = bg.numRed+bg.numGreen+bg.numBlue;
                            float littleNum = bg.countRed+bg.countGreen+bg.countBlue;
                            float tempSat = littleNum/bigNum;
                            int percent = tempSat*100;
                            float satAmount = (float)satRamp[percent]/100;
                            reSaturate(satAmount,satAmount,satAmount);
                        }
                    }
                }
            } // type = 1 (gem)
        } // collected
    }
}


void moveEnemies(){
    //enemies
    for(int t=0; t<maxEnemies; t++){
        if(enemy[t].type != 0){

            if(++enemy[t].frame >= 4*enemy[t].speed){enemy[t].frame=0;}
            int theX = enemy[t].x-bg.mapX;
            int theY = enemy[t].y-bg.mapY;
            if(theX>-16 && theX<220 && theY>-16 && theY<176){

                if(enemy[t].direction==0){
                    if((checkCollision(enemy[t].x+15, enemy[t].y+15) == SOLID || checkCollision(enemy[t].x+15, enemy[t].y+15) == JUMPTHROUGH) ||
                        (checkCollision(enemy[t].x+15, enemy[t].y+16) != SOLID && checkCollision(enemy[t].x+15, enemy[t].y+16) != JUMPTHROUGH) ){
                        enemy[t].direction=1;
                    }else{
                        enemy[t].x++;
                    }
                }
                if(enemy[t].direction==1){
                    if((checkCollision(enemy[t].x-1, enemy[t].y+15) == SOLID || checkCollision(enemy[t].x-1, enemy[t].y+15) == JUMPTHROUGH) || 
                        (checkCollision(enemy[t].x-1, enemy[t].y+16) != SOLID && checkCollision(enemy[t].x+1, enemy[t].y+16) != JUMPTHROUGH) ){
                        enemy[t].direction=0;
                    }else{
                        enemy[t].x--;
                    }
                }
            
            }
        } // not dead
    }
}

void gameLogic(){

    //bool drawHUD=false;
    bool falling=false;
    bool jumping=false;

    int oldMapX = bg.mapX;
    int oldMapY = bg.mapY;

    player.onGround = false;

    //if(_Up_But[NEW]){ myVolume++;}
    //if(_Down_But[NEW]){ myVolume--;}

    if(_Left_But[NEW]){ player.flip = 1;}
    if(_Right_But[NEW]){ player.flip = 0;}

    if( _Left_But[RELEASED] || _Right_But[RELEASED]){
        player.frame = 0;
        player.step = 0;
    }

//    if(_B_But[HELD] && (_Left_But[HELD] || _Right_But[HELD])){player.speed +=8;}else{player.speed -=16;}

    if(_B_But[HELD]){player.speed =PLAYER_SPEED*1.5;}else{player.speed = PLAYER_SPEED;}

    //player.x += (_Right_But[HELD] - _Left_But[HELD]) * MAXSPEED;
    if(_Right_But[HELD]){
        while(rightCollision(player.x>>8, player.y>>8)==SOLID && rightCollision((player.x>>8)+1, (player.y>>8)-8)==SOLID){
            player.x -= 128;
        }
        player.x += player.speed;
        if(frameSkip==0){player.step ++;}
    }

    if(_Left_But[HELD]){
        while(leftCollision(player.x>>8, player.y>>8)==SOLID && leftCollision((player.x>>8)-1, (player.y>>8)-8)==SOLID){
            player.x += 128;
        }
        player.x -= player.speed;
        if(frameSkip==0){player.step ++;}
    }

/*
    if(_Right_But[HELD]){
        player.speed += (MAXSTEP * (_B_But[HELD]+1));
        while(rightCollision(player.x>>8, player.y>>8)==SOLID && rightCollision((player.x>>8)+1, (player.y>>8)-8)==SOLID){
            player.x -= 128;
            player.speed = 0;
        }
        if(frameSkip==0){player.step ++;}
    }

    if(_Left_But[HELD]){
        player.speed -= (MAXSTEP * (_B_But[HELD]+1));
        while(leftCollision(player.x>>8, player.y>>8)==SOLID && leftCollision((player.x>>8)-1, (player.y>>8)-8)==SOLID){
            player.x += 128;
            player.speed = 0;
        }
        if(frameSkip==0){player.step ++;}
    }

    if(player.speed > (MAXSPEED * (_B_But[HELD]+1))) player.speed=(MAXSPEED * (_B_But[HELD]+1));
    if(player.speed < -(MAXSPEED * (_B_But[HELD]+1))) player.speed=-(MAXSPEED * (_B_But[HELD]+1));
    player.x += player.speed;
    if(!_Left_But[HELD] && !_Right_But[HELD]) player.speed *= 0.75;
*/

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
        // falling
        // first check a couple of pixels down ti 'hug' the ground on ramps
        int colLeft = checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.lowerBound+2);
        if (colLeft == SOLID || (colLeft == JUMPTHROUGH && player.dropping==false)){
            player.y += (2<<8);
        }
        
        colLeft = checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.lowerBound);
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


    // check for 'death tiles'
    if (checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.centre) == DEATHCOLOUR){
        gameMode = 0;
    }


    int checkAmount = 7;
    for(int t=0; t<checkAmount; t++){
        onGround[t] = onGround[t+1];
    }
    onGround[checkAmount] = player.onGround;

	if (_A_But[NEW]){
	    bool checked = false;
        for(int t=0; t<checkAmount; t++){
            if(onGround[t] && checked == false){
                checked = true;
        	    player.vy = -player.jumpHeight; // Start jumping
                playSound(0, sfx_jump, 100, random(63)+192);
            }
        }
	}

    int mapX = player.x-28160;
    int mapY = player.y-22528;
    bg.oldMapX += ((mapX - bg.oldMapX)/15);
    bg.oldMapY += ((mapY - bg.oldMapY)/15);
    bg.mapX = bg.oldMapX >>8;
    bg.mapY = bg.oldMapY >>8;

    //printf("x:%d\n",bg.mapX);
    //printf("y:%d\n",bg.mapY);

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


    
    //f((player.y>>8)-bg.mapY>=204){gameMode = 0;}


    checkItemCollisions();
    moveEnemies();

    
    if(++exitDoor.loadDoorCounter==4){
        exitDoor.loadDoorCounter=0;
        if(exitDoor.doorFile.openRO("/joe2/door.raw")){
            exitDoor.doorFile.seek(64*48*exitDoor.frame);
            if(++exitDoor.frame>=30)exitDoor.frame=0;
            exitDoor.doorFile.read(&exitDoor.tempDoorSprite[0], 3072); // 32*48*2
        }
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
    
        playingMusic1 = false;
        loadLevel(1);
        playingMusic1 = true;

    
        Pokitto::Display::lineFillers[0] = myBGFiller; // map layer
        Pokitto::Display::lineFillers[1] = myBGFiller2; // background map layer
        Pokitto::Display::lineFillers[3] = doorFill; // background map layer
        // clear screen to black
        for(int y=0; y<176; y++){
            flushLine(emptyPalette, blankLine);
        }
        // a little 'wide-screening' to remove 16 lines for higher frame rate
        Pokitto::Display::setTASRowMask(0b0111'11111111'11111110);

        mustDraw=true;
    }

}


void C_Menu(){
    
}



int main(){
    using PC=Pokitto::Core;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;
    using PS=Pokitto::Sound;

    PC::begin();

    PC::setFrameRate(25);

    PD::invisiblecolor = 0;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;

    // This can only be done once, as were swapping layers around
    Pokitto::Display::lineFillers[2] = Pokitto::Display::lineFillers[1]; // sprite layer


    //bgmFile.openRO("/joe2/C_8000.pcm");
    //Audio::play(bgmFile, bgmFile.size());
    
    //auto music = Audio::play("/joe2/C_8000.pcm"); // streams are on channel 0 by default
    //if(music) music->setLoop(true);

    updateButtons(); // update buttons
    while(_A_But[HELD]){
        updateButtons(); // update buttons
    }


    mustDraw=true;

    frameSkip=0;
    
    Pokitto::Core::update(); // needed first to setup IRQ that I will 'borrow' for my own sound.
    playRandomTune();

    // set hardware volume quite low
    SoftwareI2C swvolpot(P0_4, P0_5); //swapped SDA,SCL
    //if(myVolume>64){myVolume=64;}
    //if(myVolume<0){myVolume=0;}
    swvolpot.write(0x5e, myVolume);
            
    
    while( PC::isRunning() ){
        
        updateStream();
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
                if(frameSkip==1) renderSprites();
                break;
        }   

        updateButtons(); // update buttons
        fpsCounter++;

        char tempText[32];

        //sprintf(tempText,"%d",((bg.numRed+bg.numGreen+bg.numBlue)*(bg.countRed+bg.countGreen+bg.countBlue))/100);
        //myPrint(0,16,tempText);

        sprintf(tempText,"FPS:%d",fpsCount);
        myPrint(0,160,tempText);
        
        
        sprintf(tempText,"Pal:%d",PALETTE_SIZE);
        myPrint(0,152,tempText);

        if(PC::getTime() >= lastMillis+1000){
            lastMillis = PC::getTime();
            fpsCount = fpsCounter;
            fpsCounter = 0;
        }

    }
    
    return 0;
}

