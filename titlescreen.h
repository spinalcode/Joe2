
void titleScreen(){

    if(Pokitto::Core::getTime() > titleTimer+6000){
        //titleTimer = Pokitto::Core::getTime();

        if(titleScratch==0){
            titleScratch=1;
            //clearAudioBuffer(1);
            //clearAudioBuffer(2);
            startSong("/joe2/scratch.pcm");
        }
    }

    if(Pokitto::Core::getTime() > titleTimer+8000 && mustDrawTitleScreen==true){
        startSong("/joe2/C_6000.pcm");
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

            int tMinus = sizeof(titleRoll);
                for(int y=0; y<176; y++){
                    titleLine[y] = y;
                }
            for(int top=176+tMinus; top>=-tMinus; top-=2){
                /*
                updateButtons(); // update buttons
                if(_A_But[NEW]){
                    top=-tMinus;
                    updateButtons(); // update buttons
                }
                */
                
                for(int t=top; t<top+tMinus-1; t++){
                    int p = t+titleRoll[t-top];
                    if(p>176) p-=176;
                    if(top-(t-top) >0 && top-(t-top) <176)
                        titleLine[top-(t-top)] = p;
                }
                
                for(int y=top-tMinus+2; y<=top+2; y+=2){
                    Pokitto::setDRAMpoint(0, y);
                    SET_MASK_P2;
                    if(y>=0 && y<176){
                        tsFile.seek(mainData+(220*titleLine[y]));
                        tsFile.read(buff, 220);
                        flushLine(Pokitto::Display::palette, buff);
                        tsFile.read(buff, 220);
                        flushLine(Pokitto::Display::palette, buff);
                    }
                }
                Pokitto::Display::update(); // needed?
                updateStream(); // keep the sound going
            }


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
        titleTimer=0;
        startSong("/joe2/C_6000.pcm");
    }

}

void titleScreen2(){

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
            int mainData = 1024+54; // main bitmap data in file

            for(int y=0; y<176; y++){
                tsFile.read(buff, 220);
                flushLine(Pokitto::Display::palette, buff);
                }
            }
            Pokitto::Display::update(); // needed?
            updateStream(); // keep the sound going
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

        Pokitto::Display::setTASRowMask(0b1111'11111111'11111111);
        // a little 'wide-screening' to remove some lines for higher frame rate
        bg.screenTop=20;
        bg.screenBottom=176-20;
        bg.screenHeight = bg.screenBottom-bg.screenTop;
        
        mustDrawTitleScreen=true;
        titleTimer=0;
        startSong("/joe2/C_6000.pcm");
    }

}
