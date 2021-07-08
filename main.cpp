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

void drawSprite(int x, int y, const uint8_t *imageData,const uint16_t *paletteData, bool hFlip, uint8_t bit=8){

    if(++spriteCount>NUMSPRITES-1)spriteCount=NUMSPRITES-1; // don't overflow the sprite max

    sprites[spriteCount].x = x;
    sprites[spriteCount].y = y;
    sprites[spriteCount].imageData = imageData;
    sprites[spriteCount].paletteData = paletteData;
    sprites[spriteCount].hFlip = hFlip;
    sprites[spriteCount].bit = bit;
}

// print text
void myPrint(char x, char y, const char* text) {
  uint8_t numChars = strlen(text);
  uint8_t x1 = 0;//2+x+28*y;
  for (uint8_t t = 0; t < numChars; t++) {
    uint8_t character = text[t] - 32;
    drawSprite(x+((x1++)*8), y, font88[character], font88_pal,0,2);
  }
}

void make_pal(void){
	int a,s,r,g,b;
	for(a=0; a<=63; a++){
		s = 0; 	r = a; 		g = 63-a;	b = 0;		pal[a+s] = Pokitto::Core::display.RGBto565(r*4,g*4,b*4); pal[255+a+s] = pal[a+s];
		s = 64; r = 63-a;	g = 0;		b = a; 		pal[a+s] = Pokitto::Core::display.RGBto565(r*4,g*4,b*4); pal[255+a+s] = pal[a+s];
		s = 128; r = 0;	 	g = 0;		b = 63-a;	pal[a+s] = Pokitto::Core::display.RGBto565(r*4,g*4,b*4); pal[255+a+s] = pal[a+s];
		s = 192; r = 0;		g = a;		b = 0;	 	pal[a+s] = Pokitto::Core::display.RGBto565(r*4,g*4,b*4); pal[255+a+s] = pal[a+s];
	}
}

/*
void setFPS(int fps){
  myDelay = 1000 / fps;
}
*/

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

void startAnimation(int x, int y, int x1, int y1, int speed, int itemType, int startFrame, int maxFrame, const uint8_t *imageData,const uint16_t *paletteData){
    int useSprite=0;
    for(int t=20; t; t--){
        if(animSprite[t].used==false){useSprite=t;}
    }
    animSprite[useSprite].imageData = imageData;
    animSprite[useSprite].paletteData = paletteData;
    animSprite[useSprite].type=itemType;
    animSprite[useSprite].maxFrame = maxFrame;
    animSprite[useSprite].x = x;
    animSprite[useSprite].y = y;
    animSprite[useSprite].startX = x;
    animSprite[useSprite].startY = y;
    animSprite[useSprite].endX = x1;
    animSprite[useSprite].endY = y1;
    animSprite[useSprite].frame = 0;
    animSprite[useSprite].startFrame = startFrame;
    animSprite[useSprite].speed = speed;
    animSprite[useSprite].used = true;
    animSprite[useSprite].frameCount = 1;
    animSprite[useSprite].frameSize = 2+(imageData[0]/2)*imageData[1];
    
}

void renderSprites(){
    
    for(int t=0; t<numDoors; t++){
        drawSprite(exitDoor[t].x-bg.mapX, exitDoor[t].y-bg.mapY, sprite_door, &pal[doorPalOffset],0,8);
    }

    for(int t=0; t<maxItems; t++){
        if(items[t].collected == 0){

            items[t].frame++;
            if(items[t].frame >= (items[t].maxFrame*items[t].speed)){items[t].frame=items[t].startFrame;}
            
            if(items[t].type!=0){
                int frame = items[t].frame/items[t].speed;
                int theX = items[t].x-bg.mapX;
                int theY = items[t].y-bg.mapY;
                
                if(theX>-16 && theX<220 && theY>-16 && theY<176){
                    // if item should move, move it...
                    if(items[t].type>=4 && items[t].type<=6){ // 1 = enemy 1
                        if(items[t].direction==0){
                            if((checkCollision(items[t].x+15, items[t].y+15) == SOLID || checkCollision(items[t].x+15, items[t].y+15) == JUMPTHROUGH) ||
                                (checkCollision(items[t].x+15, items[t].y+16) != SOLID && checkCollision(items[t].x+15, items[t].y+16) != JUMPTHROUGH) ){
                                items[t].direction=1;
                            }else{
                                items[t].x+=2;
                            }
                        }
                        if(items[t].direction==1){
                            if((checkCollision(items[t].x-1, items[t].y+15) == SOLID || checkCollision(items[t].x-1, items[t].y+15) == JUMPTHROUGH) || 
                                (checkCollision(items[t].x-1, items[t].y+16) != SOLID && checkCollision(items[t].x+1, items[t].y+16) != JUMPTHROUGH) ){
                                items[t].direction=0;
                            }else{
                                items[t].x-=2;
                            }
                        }
                    }
                    int flipMe = items[t].direction;
                    drawSprite(theX, theY-items[t].offy, &items[t].imageData[frame*(2+items[t].frameSize)], items[t].paletteData, flipMe ,items[t].bitDepth);
                }
            }
        }
    }
    
    // player sprite
    drawSprite((player.x>>8)-bg.mapX, (player.y>>8)-bg.mapY, player_sprite[player.frame], gamePalette.rgb, player.flip);
    drawSprite((player.x>>8)-bg.mapX-player.hatX, (player.y>>8)-bg.mapY-player.hatY, player_sprite[player.hatFrame], gamePalette.rgb, player.flip);

    // animating sprites - items etc.
    for(int t=MAX_AMINS; t; t--){
        if(animSprite[t].used==true){
            animSprite[t].frame++;
            if(animSprite[t].frame >= (animSprite[t].maxFrame*animSprite[t].speed)){animSprite[t].frame=animSprite[t].startFrame;}
            int frame = animSprite[t].frame/animSprite[t].speed;
            int flipme=0;

            drawSprite(animSprite[t].x, animSprite[t].y, animSprite[t].imageData+(frame*(animSprite[t].frameSize)), animSprite[t].paletteData,0,4);
            //drawSprite(theX, theY, &items[t].imageData[frame*(2+items[t].frameSize)], items[t].paletteData, flipMe ,items[t].bitDepth);

            animSprite[t].x = (animSprite[t].startX-easeInOut(animSprite[t].frameCount, animSprite[t].endX, animSprite[t].startX-animSprite[t].endX, 20))+animSprite[t].endX;
            animSprite[t].y = (animSprite[t].startY-easeInOut(animSprite[t].frameCount, animSprite[t].endY, animSprite[t].startY-animSprite[t].endY, 20))+animSprite[t].endY;
            if(++animSprite[t].frameCount>=20){
                animSprite[t].used = false;
                if(animSprite[t].type>=10 && animSprite[t].type<=13){wordCollected[animSprite[t].type-10]=1;}
            }else{
                HUD_gemTimer = HUD_gemTimerStart;
            }
        }
    }

    // HUD
    if(HUD_gemTimer){
        //drawHUD=true;
        HUD_gemTimer--;
        int gemY = HUD_gemTimer;
        if(gemY > 16)gemY=16;
        drawSprite(0, bg.screenTop+gemY-16, color_gem[HUD_gemFrameCount/2], color_gem_pal,0,8);
        //if(HUD_gemTimer%2 == 0)
        HUD_gemFrameCount++;
        if(HUD_gemFrameCount>=32)HUD_gemFrameCount=0;
        char tempText[10];
        snprintf(tempText,sizeof(tempText),"%d/%d",bg.countRed+bg.countGreen+bg.countBlue, bg.numRed+bg.numGreen+bg.numBlue);
        myPrint(20,bg.screenTop+gemY-12,tempText);

        for(int t=0; t<3; t++){
            if(wordCollected[t]){
                drawSprite(89+(t*14), bg.screenTop+gemY-16, big_letter[(HUD_gemFrameCount/4)+(t*8)], big_letter_pal,0,4);
            }

        }
    }

    bg.totalGemsCollected = bg.countRed+bg.countGreen+bg.countBlue;
}


void checkItemCollisions(){
    //items
    for(int t=0; t<maxItems; t++){
        if(items[t].collected == 0){

            items[t].frame++;
            if(items[t].frame >= items[t].maxFrame*items[t].speed){items[t].frame=items[t].startFrame;}
            if(items[t].type!=0){
                int flipme=0;
                int frame = (items[t].frame/items[t].speed);

                int theX = items[t].x-bg.mapX;
                int theY = items[t].y-bg.mapY;
                
                if(theX>-16 && theX<220 && theY>-16 && theY<176){
                    // check for collisions in the animation loop
                    if((player.x>>8)+player.centre >= items[t].x-4 && (player.x>>8)+player.centre <= items[t].x + 20){
                        if((player.y>>8)+player.centre >= items[t].y-items[t].offy-4 && (player.y>>8)+player.centre <= items[t].y-items[t].offy + 20){

                            playSound(1, sfx_drop, 300, random(63)+192);

                            int theX = items[t].x-bg.mapX;
                            int theY = items[t].y-bg.mapY;
                            switch(items[t].type){
                                case 1:
                                    startAnimation(theX, theY, 0,bg.screenTop, items[t].speed, items[t].type, items[t].startFrame, items[t].maxFrame, gem[0], &gem_pal[(items[t].type-1)*6]);
                                    //bg.satRed += bg.redPercent;
                                    bg.countRed++;
                                    items[t].collected = 1;
                                    break;
                                case 2:
                                    startAnimation(theX, theY, 0,bg.screenTop, items[t].speed, items[t].type, items[t].startFrame, items[t].maxFrame, gem[0], &gem_pal[(items[t].type-1)*6]);
                                    //bg.satGreen += bg.greenPercent;
                                    bg.countGreen++;
                                    items[t].collected = 1;
                                    break;
                                case 3:
                                    startAnimation(theX, theY, 0,bg.screenTop, items[t].speed, items[t].type, items[t].startFrame, items[t].maxFrame, gem[0], &gem_pal[(items[t].type-1)*6]);
                                    //bg.satBlue += bg.bluePercent;
                                    bg.countBlue++;
                                    items[t].collected = 1;
                                    break;

                                case 10: // J/O/E
                                case 11: // J/O/E
                                case 12: // J/O/E
                                    startAnimation(theX, theY, 89+((items[t].type-10)*14) ,bg.screenTop, items[t].speed, items[t].type, items[t].startFrame, items[t].maxFrame, items[t].imageData, items[t].paletteData);
                                    items[t].collected = 1;
                                    //wordCollected[items[t].type-10]=1;
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

    if(_Up_But[NEW]){
        bool moved=false;
        for(int t=0; t<numDoors; t++){
            if((player.x>>8)+player.centre >= exitDoor[t].x && (player.x>>8)+player.centre <= exitDoor[t].x + sprite_door[0]){
                if((player.y>>8)+player.centre >= exitDoor[t].y && (player.y>>8)+player.centre <= exitDoor[t].y + sprite_door[1]){
                    if(moved==false){
                        moved = true;
                        int toDoor = t+1;
                        if(toDoor == numDoors)toDoor=0;
                        //printf("Door:%d\n",toDoor); // not printing?
                        player.x = exitDoor[toDoor].x<<8;
                        player.y = exitDoor[toDoor].y<<8;
                    }
                }
            }
        }
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
        // first check a couple of pixels down to 'hug' the ground on ramps
        int colLeft = checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.lowerBound+(4*_B_But[HELD]));
        if (colLeft == SOLID || (colLeft == JUMPTHROUGH && player.dropping==false)){
            player.y += ((4*_B_But[HELD])<<8);
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
    //int mapY = player.y-22528;
    int mapY = player.y-(bg.screenHeight*128);
    bg.oldMapX += ((mapX - bg.oldMapX)/15);
    bg.oldMapY += ((mapY - bg.oldMapY)/15);
    bg.mapX = bg.oldMapX >>8;
    bg.mapY = bg.oldMapY >>8;

    //printf("x:%d\n",bg.mapX);
    //printf("y:%d\n",bg.mapY);

    if(bg.mapX<0) bg.mapX=0;
    if(bg.mapX>(bg.mapWidth*bgTileSizeW)-220) bg.mapX=(bg.mapWidth*bgTileSizeW)-220;
    if(bg.mapY<0) bg.mapY=0;
    if(bg.mapY>(bg.mapHeight*bgTileSizeH)-(bg.screenHeight+bg.screenTop)) bg.mapY=(bg.mapHeight*bgTileSizeH)-(bg.screenHeight+bg.screenTop);

    oldScreenX = screenX;
    oldScreenY = screenY;
    screenX = bg.mapX/224;
    screenY = bg.mapY/176;

	if(oldScreenX != screenX || oldScreenY != screenY){
        int mapPosX = screenX * 224;
        int mapPosY = screenY * 176;
        updateMap( mapPosX/8 , mapPosY/8, levelNumber);
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

    sprites[0].imageData = player_sprite[player.frame];

    checkItemCollisions();
    //moveEnemies();

}






void titleScreen(){

    if(mustDrawTitleScreen==true) {
        mustDrawTitleScreen = false;
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
    }

    if(_A_But[NEW]){
        
        reSaturate(0,0,0);
        gameMode=1;
    
        playingMusic1 = false;
        loadLevel(1);
        playingMusic1 = true;

    
        Pokitto::Display::lineFillers[0] = myBGFiller; // map layer
        Pokitto::Display::lineFillers[1] = myBGFiller2; // background map layer
        Pokitto::Display::lineFillers[2] = spriteFill; // sprite layer
        // clear screen to black
        //for(int y=0; y<176; y++){
        //    flushLine(emptyPalette, blankLine);
        //}
        Pokitto::Display::setTASRowMask(0b1111'11111111'11111111);
        // a little 'wide-screening' to remove some lines for higher frame rate
        bg.screenTop=20;
        bg.screenBottom=176-20;
        bg.screenHeight = bg.screenBottom-bg.screenTop;
        
        mustDrawTitleScreen=true;
    }

}


void C_Menu(){
    // Not implemented yet
}



int main(){
    using PC=Pokitto::Core;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;
    using PS=Pokitto::Sound;

    PC::begin();

    //PC::setFrameRate(60);

    PD::invisiblecolor = 0;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;

    // This can only be done once, as were swapping layers around
    Pokitto::Display::lineFillers[2] = Pokitto::Display::lineFillers[1]; // sprite layer

    make_pal();

    //bgmFile.openRO("/joe2/C_8000.pcm");
    //Audio::play(bgmFile, bgmFile.size());
    
    //auto music = Audio::play("/joe2/C_8000.pcm"); // streams are on channel 0 by default
    //if(music) music->setLoop(true);

    updateButtons(); // update buttons
    while(_A_But[HELD]){
        updateButtons(); // update buttons
    }


    mustDrawTitleScreen=true;

    frameSkip=0;
    
    Pokitto::Core::update(); // needed first to setup IRQ that I will 'borrow' for my own sound.
    playRandomTune();

    // set hardware volume quite low
    SoftwareI2C swvolpot(P0_4, P0_5); //swapped SDA,SCL
    //if(myVolume>64){myVolume=64;}
    //if(myVolume<0){myVolume=0;}
    swvolpot.write(0x5e, myVolume);

    long int lastMillis;

    while( PC::isRunning() ){
        
        updateStream();
        if(frameSkip==0){
            if( !PC::update() ) continue;
            
        }else{
            PC::update(0); // don't update screen.
            spriteCount=0;        
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

        char tempText[64];

        sprintf(tempText,"FPS:%d",fpsCount);
        myPrint(0,bg.screenBottom-8,tempText);

        //sprintf(tempText,"Hit:%d",checkCollision((player.x>>8)+player.centre, (player.y>>8)+player.centre));
        //sprintf(tempText,"Hit:%d",sizeof(enemy1)/sizeof(enemy1[0]));
        //myPrint(0,bg.screenBottom-16,tempText);

        
        if(PC::getTime() >= lastMillis+1000){
            lastMillis = PC::getTime();
            fpsCount = fpsCounter;
            fpsCounter = 0;
        }

        // cycle through the 512 colour palette for the door sprite, auto looping at 255 because its uint8_t
        doorPalOffset++;
    }
    
    return 0;
}

