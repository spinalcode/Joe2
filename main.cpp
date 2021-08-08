#include <LibHotswap>
#include <File>
#include "PokittoCookie.h"

#ifndef POK_SIM
#include <SoftwareI2C.h>
#endif

#include "globals.h"
#include "sound.h"

#include "sprites.h"
#include "font.h"
#include "samples.h"

#include "buttonhandling.h"

#include "background.h"
#include "pause.h"
#include "screen.h"
#include "levelsandmaps.h"
#include "easing.h"
#include "titlescreen.h"
#include "settings.h"

void write_command_16(uint16_t data);
void write_data_16(uint16_t data);

#ifndef POK_SIM
// set hardware volume quite low
SoftwareI2C swvolpot(P0_4, P0_5); //swapped SDA,SCL
#endif


void waitButton(){
    updateButtons();
    while(!_A_But[NEW]) updateButtons();
}

void drawSprite(int x, int y, const uint8_t *imageData,const uint16_t *paletteData, bool hFlip, uint8_t bit){

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


void c_menu(){
    guiPrint(9,6, "    PAWS");
    
    int numOptions = sizeof(menuText)/sizeof(menuText[0]);
    
    for(int t=0; t<numOptions; t++){
        guiPrint(11,9+(t*2), menuText[t]);
    }
    char tempText[10];
    sprintf(tempText,"%d ",myVolume);
    guiPrint(18,11, tempText);


    drawSprite(55, 47, paw, paw_pal,0,2);
    drawSprite(59, 47, paw, paw_pal,1,2);
    drawSprite(133, 47, paw, paw_pal,0,2);
    drawSprite(137, 47, paw, paw_pal,1,2);

    
    drawSprite(58, 71+(optionNumber*16), tinyJoe[cursorFrame/2], tinyJoe_pal,0,2);
    if(++cursorFrame>=12)cursorFrame=0;

    updateButtons();
    if(_Up_But[NEW] && optionNumber>0) optionNumber--;
    if(_Down_But[NEW] && optionNumber<numOptions-1) optionNumber++;

    if(_A_But[RELEASED]){
        if(optionNumber==0){
            // Return to game
            gamePaused = false;
            renderMenuLayer = false;
            spriteCount=0;
            saveSettings();
        }
        if(optionNumber==2){
            // Exit the game
            gamePaused = false;
            renderMenuLayer = false;
            gameMode = 0;
        }
    }

    if(optionNumber==1){
        if(_Left_But[NEW]){
            myVolume-=5;
            if(myVolume <=0) myVolume = 0;
            #ifndef POK_SIM
            swvolpot.write(0x5e, myVolume);
            #endif
        }
        if(_Right_But[NEW]){
            myVolume+=5;
            if(myVolume >=60) myVolume = 60;
            #ifndef POK_SIM
            swvolpot.write(0x5e, myVolume);
            #endif
        }
    }
    
    if(_C_But[NEW]){
        printf("\n");
        printf("\n");
        int t=0;
        printf("%d,\n",bg.miniMap[t++]);
        printf("%d,\n",bg.miniMap[t++]);
        for(int y=0; y<bg.miniMap[1]; y++){
            for(int x=0; x<bg.miniMap[0]; x++){
                printf("%d,",bg.miniMap[t++]);
            }
            printf("\n");
        }
        printf("\n");
        printf("\n");
    }
    
};


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

void startAnimation(int x, int y, int x1, int y1, int speed, int itemType, int startFrame, int maxFrame, const uint8_t *imageData,const uint16_t *paletteData, uint8_t bitDepth){
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
    animSprite[useSprite].bitDepth = bitDepth;
    animSprite[useSprite].startFrame = startFrame;
    animSprite[useSprite].speed = speed;
    animSprite[useSprite].used = true;
    animSprite[useSprite].frameCount = 1;
    animSprite[useSprite].frameSize = (imageData[0]*imageData[1])/(8/bitDepth);
    //printf("size:%d\n",animSprite[useSprite].frameSize);

}

void renderSprites(){

    if(bg.totalGemsCollected == bg.totalGemsToCollect){
        for(int t=0; t<numDoors; t++){
            drawSprite(exitDoor[t].x-bg.mapX, exitDoor[t].y-bg.mapY, sprite_door, &pal[doorPalOffset],0,8);
        }
    }

    for(int t=0; t<maxItems; t++){
        if(items[t].collected == 0){

            items[t].frame++;
            if(items[t].frame >= (items[t].maxFrame*items[t].speed)){
                items[t].frame=items[t].startFrame;
            }
            
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
                    drawSprite(theX, theY-items[t].offy, &items[t].imageData[frame*(2+items[t].frameSize)], items[t].paletteData, items[t].direction ,items[t].bitDepth);
                }
            }
        }
    }
    
    
    if(playerDying==0 || playerDying==2){
        if(invincibleCount%4==0 || playerDying==0){
            // player sprite
            drawSprite((player.x>>8)-bg.mapX, (player.y>>8)-bg.mapY, player_sprite[player.frame], playerSpritePal, player.flip, 4);
            drawSprite((player.x>>8)-bg.mapX-player.hatX, (player.y>>8)-bg.mapY-player.hatY, player_sprite[player.hatFrame], playerSpritePal, player.flip, 4);
        }
    }else{
        // play the death animation
        drawSprite((player.x>>8)-bg.mapX, (player.y>>8)-bg.mapY, player_sprite[playerDeathFrames[playerDeathFrame]], playerSpritePal, player.flip, 4);
        int flipHat = playerDeathHatFlip[playerDeathFrames[playerDeathFrame]-9];
        if(player.flip) flipHat = 1-flipHat;
        drawSprite(((player.x>>8)-bg.mapX) - playerDeathHatX[playerDeathFrames[playerDeathFrame]-9], ((player.y>>8)-bg.mapY) - playerDeathHatY[playerDeathFrames[playerDeathFrame]-9], player_sprite[playerDeathHatFrame[playerDeathFrames[playerDeathFrame]-9]], playerSpritePal, flipHat , 4);
        // stars
        drawSprite((player.x>>8)-bg.mapX, (player.y>>8)-bg.mapY -5, stars[starCount], stars_pal,0,1);
        if(++starCount>=37)starCount=0;

        playerDeathFrame++;
        if(playerDeathFrame>=sizeof(playerDeathFrames)){
            invincibleCount=0;
            playerDeathFrame=0;
            playerDying=2;
            if(player.numLives==0){gameMode=2;}
        }
    }


    // animating sprites - items etc.
    int numAnimFrames = fpsCount/2;
    for(int t=MAX_AMINS; t; t--){
        if(animSprite[t].used==true){
            animSprite[t].frame++;
            if(animSprite[t].frame >= (animSprite[t].maxFrame*animSprite[t].speed)){
                animSprite[t].frame=animSprite[t].startFrame;
            }
            int frame = animSprite[t].frame/animSprite[t].speed;

            drawSprite(animSprite[t].x, animSprite[t].y, &animSprite[t].imageData[frame*(2+animSprite[t].frameSize)], animSprite[t].paletteData,0,animSprite[t].bitDepth);

            animSprite[t].x = (animSprite[t].startX-easeInOut(animSprite[t].frameCount, animSprite[t].endX, animSprite[t].startX-animSprite[t].endX, numAnimFrames))+animSprite[t].endX;
            animSprite[t].y = (animSprite[t].startY-easeInOut(animSprite[t].frameCount, animSprite[t].endY, animSprite[t].startY-animSprite[t].endY, numAnimFrames))+animSprite[t].endY;
            if(++animSprite[t].frameCount>=numAnimFrames){
                animSprite[t].used = false;
                if(animSprite[t].type>=10 && animSprite[t].type<=13){wordCollected[animSprite[t].type-10]=1;}
            }else{
                if(animSprite[t].type>=10 && animSprite[t].type<=13){
                    HUD_wordTimer = HUD_wordTimerStart;
                }else{
                    HUD_gemTimer = HUD_gemTimerStart;
                }
            }
        }
    }


    // HUD
    if(HUD_gemTimer){
        HUD_gemTimer--;
        int gemY = HUD_gemTimer;
        if(gemY > 16)gemY=16;
        drawSprite(0, bg.screenTop+gemY-16, color_gem[HUD_gemFrameCount/2], color_gem_pal,0,8);
        HUD_gemFrameCount++;
        if(HUD_gemFrameCount>=32)HUD_gemFrameCount=0;
        char tempText[10];
        snprintf(tempText,sizeof(tempText),"%d/%d",bg.countRed+bg.countGreen+bg.countBlue, bg.numRed+bg.numGreen+bg.numBlue);
        myPrint(20,bg.screenTop+gemY-12,tempText);
    }else{
        HUD_gemFrameCount=0;
    }
    if(HUD_wordTimer){
        HUD_wordTimer--;
        int gemY = HUD_wordTimer;
        if(gemY > 16)gemY=16;
        HUD_wordFrameCount++;
        if(HUD_wordFrameCount>=32)HUD_wordFrameCount=0;
        for(int t=0; t<3; t++){
            if(wordCollected[t]){
                drawSprite(89+(t*14), bg.screenTop+gemY-16, big_letter[(HUD_wordFrameCount/4)+(t*8)], big_letter_pal,0,4);
            }
        }
    }else{
        HUD_wordFrameCount=0;
    }

    if(HUD_heartTimer){
        HUD_heartTimer--;
        int hudY = HUD_heartTimer;
        if(hudY > 16)hudY=16;
        HUD_heartFrameCount++;
        if(HUD_heartFrameCount == HUD_heartTimerStart/2){
            player.numLives--;
//            playSound(1, sfx_pop, 100);
        }

        if(HUD_heartFrameCount>=32)HUD_heartFrameCount=0;
        for(int t=0; t<maxLives; t++){
            if(t < player.numLives)
                drawSprite(206-(t*14), bg.screenTop+2+hudY-16, heart[0], heart_pal,0,4);
            else
                drawSprite(206-(t*14), bg.screenTop+2+hudY-16, heart[1], heart_pal,0,4);
        }
    }else{
        HUD_heartFrameCount=0;
    }



    bg.totalGemsCollected = bg.countRed+bg.countGreen+bg.countBlue;
}

void updateColours(){
    float bigNum = bg.numRed+bg.numGreen+bg.numBlue;
    float littleNum = bg.countRed+bg.countGreen+bg.countBlue;
    float tempSat = littleNum/bigNum;
    int percent = tempSat*100;
    float satAmount = (float)satRamp[percent]/100;
    reSaturate(satAmount,satAmount,satAmount);
}


void checkItemCollisions(){
    //items
    for(int t=0; t<maxItems; t++){
        if(items[t].collected == 0){

            items[t].frame++;
            if(items[t].frame >= items[t].maxFrame*items[t].speed){items[t].frame=items[t].startFrame;}
            if(items[t].type!=0){
                int frame = (items[t].frame/items[t].speed);

                int theX = items[t].x-bg.mapX;
                int theY = items[t].y-bg.mapY;
                
                if(theX>-16 && theX<220 && theY>-16 && theY<176){
                    // check for collisions in the animation loop
                    if((player.x>>8)+player.centre >= items[t].x-4 && (player.x>>8)+player.centre <= items[t].x + 20){
                        if((player.y>>8)+player.centre >= items[t].y-items[t].offy-4 && (player.y>>8)+player.centre <= items[t].y-items[t].offy + 20){

                            int theX = items[t].x-bg.mapX;
                            int theY = items[t].y-bg.mapY;
                            switch(items[t].type){
                                case 1:
                                    startAnimation(theX, theY, 0,bg.screenTop, items[t].speed, items[t].type, items[t].startFrame, items[t].maxFrame, gem[0], &gem_pal[(items[t].type-1)*6], items[t].bitDepth);
                                    //bg.satRed += bg.redPercent;
                                    bg.countRed++;
                                    items[t].collected = 1;
//                                    playSound(1, sfx_drop, 300, random(63)+192);
                                    updateColours();
                                    break;
                                case 2:
                                    startAnimation(theX, theY, 0,bg.screenTop, items[t].speed, items[t].type, items[t].startFrame, items[t].maxFrame, gem[0], &gem_pal[(items[t].type-1)*6], items[t].bitDepth);
                                    //bg.satGreen += bg.greenPercent;
                                    bg.countGreen++;
                                    items[t].collected = 1;
//                                    playSound(1, sfx_drop, 300, random(63)+192);
                                    updateColours();
                                    break;
                                case 3:
                                    startAnimation(theX, theY, 0,bg.screenTop, items[t].speed, items[t].type, items[t].startFrame, items[t].maxFrame, gem[0], &gem_pal[(items[t].type-1)*6], items[t].bitDepth);
                                    //bg.satBlue += bg.bluePercent;
                                    bg.countBlue++;
                                    items[t].collected = 1;
//                                    playSound(1, sfx_drop, 300, random(63)+192);
                                    updateColours();
                                    break;

                                case 4:
                                case 5:
                                case 6:
                                    if(invincibleCount==0 && playerDying==0){
                                        playerDying=1;
                                        //player.numLives--;
                                        HUD_heartTimer = HUD_heartTimerStart;

                                        //if(player.numLives==0){gameMode=2;}
                                    }
                                    break;
                                case 9:
                                    startAnimation(theX, theY, 89+((items[t].type-10)*14) ,bg.screenTop, items[t].speed, items[t].type, items[t].startFrame, items[t].maxFrame, items[t].imageData, items[t].paletteData, items[t].bitDepth);
                                    items[t].collected = 1;
//                                    playSound(1, sfx_drop, 300, random(63)+192);
                                    break;
                                case 10: // J/O/E
                                case 11: // J/O/E
                                case 12: // J/O/E
                                    startAnimation(theX, theY, 89+((items[t].type-10)*14) ,bg.screenTop, items[t].speed, items[t].type, items[t].startFrame, items[t].maxFrame, items[t].imageData, items[t].paletteData, items[t].bitDepth);
                                    items[t].collected = 1;
                                    //wordCollected[items[t].type-10]=1;
//                                    playSound(1, sfx_drop, 300, random(63)+192);
                                    break;
                            }
                            
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

    if(_C_But[NEW]){
        gamePaused = !gamePaused;
        if(gamePaused){renderMenuLayer = true;}else{renderMenuLayer=false;}
        for(int t=0; t<sizeof(menuBG); t++){
            guiBG[t] = menuBG[t];
        }
    }

    if(playerDying == 2){
        invincibleCount++;
        if(invincibleCount >= invincibleFrames){
            invincibleCount=0;
            playerDying=0;
        }
    }

    if(playerDying == 0 || playerDying == 2){

        if( !_Up_But[HELD] &&
            !_Down_But[HELD] &&
            !_Left_But[HELD] &&
            !_Right_But[HELD]){
                // reset the player to standing if no buttons pressed.
                player.frame = 0;
                player.step = 0;
        }
        
        if(_B_But[HELD]){player.speed =PLAYER_SPEED*1.5;}else{player.speed = PLAYER_SPEED;}
        
        if(_Right_But[HELD]){
            player.flip = 0;
            while(rightCollision(player.x>>8, player.y>>8)==SOLID && rightCollision((player.x>>8)+1, (player.y>>8)-8)==SOLID){
                player.x -= 128;
            }
            player.x += player.speed;
            if(frameSkip==0){player.step ++;}
        }
        
        if(_Left_But[HELD]){
            player.flip = 1;
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
                        // hidden room
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
    } // player dying

    // Add gravity
    // but only if you're on the screen (collision issues)
    if(player.y >>8 < bg.mapY+176 && player.y >>8 > bg.mapY){
        player.vy += GRAVITY; // apply gravity to falling speed
        if(player.vy > MAXGRAVITY) player.vy = MAXGRAVITY; // limit falling speed
        player.y += player.vy; // apply falling speed to player position
    }

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
        //gameMode = 0;
        player.x = player.lastGroundX;
        player.y = player.lastGroundY;
        playerDying=1;
    }

    int checkAmount = 7;
    for(int t=0; t<checkAmount; t++){
        onGround[t] = onGround[t+1];
    }
    onGround[checkAmount] = player.onGround;
    if(player.onGround){
        int colLeft = checkCollision((player.x>>8)+player.centre, ((player.y+128)>>8)+player.lowerBound);
        if (colLeft == SOLID) {
            player.lastGroundX = player.x;
            player.lastGroundY = player.y;
        }
    }

    if(playerDying == 0 || playerDying == 2){

    	if (_A_But[NEW]){
    	    bool checked = false;
            for(int t=0; t<checkAmount; t++){
                if(onGround[t] && checked == false){
                    checked = true;
            	    player.vy = -player.jumpHeight; // Start jumping
//                    playSound(0, sfx_jump, 100, random(63)+192);
                }
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

    if(bg.mapX<0) bg.mapX=0;
    if(bg.mapX>(bg.mapWidth*BG_TILE_SIZE_W)-220) bg.mapX=(bg.mapWidth*BG_TILE_SIZE_W)-220;
    if(bg.mapY<0) bg.mapY=0;
    if(bg.mapY>(bg.mapHeight*BG_TILE_SIZE_H)-(bg.screenHeight+bg.screenTop)) bg.mapY=(bg.mapHeight*BG_TILE_SIZE_H)-(bg.screenHeight+bg.screenTop);

    bg.oldScreenX = bg.screenX;
    bg.oldScreenY = bg.screenY;
    bg.screenX = bg.mapX/224;
    bg.screenY = bg.mapY/176;

	if(bg.oldScreenX != bg.screenX || bg.oldScreenY != bg.screenY){
        int mapPosX = bg.screenX * 224;
        int mapPosY = bg.screenY * 176;
        updateMap( mapPosX/8 , mapPosY/8, levelNumber, LEVMAIN);
	}
    bg.windowX = bg.mapX%224;
	bg.windowY = bg.mapY%176;

    bg.midoldScreenX = bg.midscreenX;
    bg.midoldScreenY = bg.midscreenY;
    int tempMX = (bg.mapX * bg.multiplyX)>>8;
    int tempMY = (bg.mapY * bg.multiplyY)>>8;
    bg.midscreenX = tempMX/224;
    bg.midscreenY = tempMY/176;

	if(bg.midoldScreenX != bg.midscreenX || bg.midoldScreenY != bg.midscreenY){
        int mapPosX = bg.midscreenX * 224;
        int mapPosY = bg.midscreenY * 176;
        updateMap( mapPosX/8 , mapPosY/8, levelNumber, LEVMID);
	}
    bg.midwindowX = tempMX%224;
	bg.midwindowY = tempMY%176;

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
    else if(player.vy>-200 && player.vy<200){
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

    //sprites[0].imageData = player_sprite[player.frame];

    checkItemCollisions();
    //moveEnemies();

}





int main(){
    using PC=Pokitto::Core;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;
    using PS=Pokitto::Sound;

    PD::setTASRowMask(0b0000'00000000'00000000);

    PC::begin();

    //PC::setFrameRate(20);

    PD::invisiblecolor = 0;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;

    // This can only be done once, as were swapping layers around
    Pokitto::Display::lineFillers[2] = Pokitto::Display::lineFillers[1]; // sprite layer

    make_pal();


    updateButtons(); // update buttons
    while(_A_But[HELD]){
        updateButtons(); // update buttons
    }

    mustDrawTitleScreen=true;

    frameSkip=0;
    
    Pokitto::Core::update(0); // needed first to setup IRQ that I will 'borrow' for my own sound.
//    playRandomTune();
    
//    clearAudioBuffer();
    //clearAudioBuffer(2);
//    startSong("/joe2/flute.pcm");

    //if(myVolume>64){myVolume=64;}
    //if(myVolume<0){myVolume=0;}
    #ifndef POK_SIM
    swvolpot.write(0x5e, myVolume);
    #endif

    long int lastMillis = PC::getTime();
    long int titleTimer = PC::getTime();

    myVolume = 10;
    loadSettings(); // test
    #ifndef POK_SIM
    swvolpot.write(0x5e, myVolume);
    #endif

    long int frameCount=0;

    while( PC::isRunning() ){
        
        //updateStream();

        if(frameSkip==0){
            fpsCounter++;
            if( !PC::update() ) continue;
		}else{
            PC::update(0); // don't update screen.
            spriteCount=0; // reset the visible sprites ready for redraw
            if(gamePaused) c_menu();
        }

        frameSkip = 1-frameSkip;

        if(gamePaused==false){
            switch(gameMode){
                
                case 0: // titlescreen
                    titleScreen();
                    break;
                case 1: // gameplay
                    gameLogic();
                    if(frameSkip==1){
                        renderSprites();
                    }
                    break;
                case 2: // game over screen
                    gameOverScreen();
                    break;
            }   
        }

        updateButtons(); // update buttons

        char tempText[64];
/*
        for(int t=0; t<10; t++){
            sprintf(tempText,"%d",bg.miniMap[t+15]);
            myPrint(t*20,bg.screenBottom-16,tempText);
        }
*/

        sprintf(tempText,"FPS:%d",fpsCount);
        myPrint(0,bg.screenBottom-8,tempText);

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

