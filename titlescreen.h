

void renderTitleScreen(){

    if(mustDrawTitleScreen==true) {
        mustDrawTitleScreen = false;
        startSong("/joe2/flute.pcm");
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
            tsFile.close();
            Pokitto::Display::update(); // needed?
            
        }
    }
    
}

void titleScreen(){

    renderTitleScreen();

    if(_A_But[NEW]){
        
        reSaturate(0,0,0);
        gameMode=1;
    
        playingMusic1 = false;
        loadLevel(1);
        //if(++levelNumber==3)levelNumber=1;
        playingMusic1 = true;

        Pokitto::Display::lineFillers[0] = myBGFiller; // map layer
        Pokitto::Display::lineFillers[1] = myBGFiller2; // background map layer
        Pokitto::Display::lineFillers[2] = spriteFill; // sprite layer

        Pokitto::Display::setTASRowMask(0b1111'11111111'11111111);
        // a little 'wide-screening' to remove some lines for higher frame rate
        bg.screenTop = 20;
        bg.screenBottom = 176-bg.screenTop;
        bg.screenHeight = bg.screenBottom-bg.screenTop;
        
        mustDrawTitleScreen=true;
        titleTimer=0;
        startSong("/joe2/C_6000.pcm");
        player.numLives = maxLives;
        playerDying = 0;
    }

}




void gameOverScreen(){

    if(mustDrawTitleScreen==true) {
        mustDrawTitleScreen = false;
        Pokitto::Display::setTASRowMask(0b0000'00000000'00000000);
        File tsFile;
        // titlescreen.bmp MUST be 8bit and upside down. Ther is no error checking!
        if(tsFile.openRO("/joe2/gameoverscreen.bmp")){
            
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
            tsFile.close();
            Pokitto::Display::update(); // needed?
        }
    }

    if(_A_But[NEW]){
        mustDrawTitleScreen = true;
        gameMode=0;
    }

}









/*

void titleScreenOld(){

    if(Pokitto::Core::getTime() > titleTimer+6000){
        //titleTimer = Pokitto::Core::getTime();

        if(titleScratch==0){
            titleScratch=1;
//            startSong("/joe2/scratch.pcm");
        }
    }

    if(Pokitto::Core::getTime() > titleTimer+8000 && mustDrawTitleScreen==true){
//        startSong("/joe2/C_6000.pcm");
        //titleTimer = Pokitto::Core::getTime();

//    if(mustDrawTitleScreen==true) {
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
            int mainData = 1024+54; // main bitmap data in file

            int rollSize = sizeof(titleRoll);
            for(int y=0; y<176; y++){
                titleLine[y] = y;
            }
            int tempTime=Pokitto::Core::getTime();
            for(int top=176+rollSize; top>=-(rollSize+3); top-=1){
                for(int t=top; t<top+rollSize-1; t++){
                    int p = t+titleRoll[t-top];
                    if(p>=176) p-=176;
                    int s = top-(t-top);
                    if(s >0 && s <176)
                        titleLine[s] = p;
                }

                for(int y=top-rollSize+2; y<top; y++){
                    Pokitto::setDRAMpoint(0, y); SET_MASK_P2;
                    if(y>=0 && y<176){
                        tsFile.seek(mainData+(220*titleLine[y]));
                        tsFile.read(buff, 220);
                        flushLine(Pokitto::Display::palette, buff);
                    }
                }

                Pokitto::Display::update(); // needed?
//                updateStream(); // keep the sound going
            }


        }
    }

    if(_A_But[NEW]){
        
        reSaturate(0,0,0);
        gameMode=1;
    
//        playingMusic1 = false;
        loadLevel(1);
//        playingMusic1 = true;

    
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
        titleTimer=0;
//        startSong("/joe2/C_6000.pcm");
    }

}
*/

