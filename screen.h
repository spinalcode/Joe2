// tile map
#define BG_TILE_SIZE_H 8
#define BG_TILE_SIZE_W 8
#define tbt BG_TILE_SIZE_H*BG_TILE_SIZE_W
#define htbt BG_TILE_SIZE_H*(BG_TILE_SIZE_W/2)

void inline bgTileLine2();
void inline bgTileLine();

void write_command_16(uint16_t data);
void write_data_16(uint16_t data);


int ease(int t, int b, int c, int d){
    return ((t*c)/d)+b;
}
/*
     ______                 __                 _______                              
    |   __ \.-----.-----.--|  |.-----.----.   |     __|.----.----.-----.-----.-----.
    |      <|  -__|     |  _  ||  -__|   _|   |__     ||  __|   _|  -__|  -__|     |
    |___|__||_____|__|__|_____||_____|__|     |_______||____|__| |_____|_____|__|__|

*/

void loadPalette(const uint16_t* p, int numColours) {
    for (int i=0;i<numColours;i++)
        Pokitto::Display::palette[i] = p[i];
    Pokitto::Display::paletteptr = Pokitto::Display::palette;
}

void clearPalette(int numColours) {
    for (int i=0;i<numColours;i++)
        Pokitto::Display::palette[i] = 0;
    Pokitto::Display::paletteptr = Pokitto::Display::palette;
}


uint16_t alphaBlendRGB565(uint32_t fg, uint32_t bg, uint8_t alpha) {

    fg = (fg | fg << 16) & 0x07e0f81f;
    bg = (bg | bg << 16) & 0x07e0f81f;
    bg += (fg - bg) * alpha >> 5;
    bg &= 0x07e0f81f;
        return (uint16_t)(bg | bg >> 16);
}


void GUILine(std::uint8_t* line, std::uint32_t y, bool skip){

    if(skip)return;
    if(y<bg.screenTop || y>=bg.screenBottom){
        return;
    }

    uint32_t x = 0;

    uint32_t tileIndex = 2+(y/8) * 27;
    uint32_t lineOffset;
    uint32_t alpha;
    uint32_t temp;
    auto &tileRef = guiFont[0];
    #define invisible 1
    auto scanLine = &Pokitto::Display::palette[16]; // start 32 pixels in because of underflow

    for(int d=0; d<27; d++){
        lineOffset = 2 + ((y%8)*4) + guiBG[tileIndex++]*34;
        for(int c=0; c<4; c++){
            temp = tileRef[lineOffset]>>4;
            if(temp==0){
//                scanLine[x] = 0;
            }else{
                if(temp==invisible){alpha = 16;}else{alpha=32;}
                scanLine[x] = alphaBlendRGB565(guiFont_pal[temp], scanLine[x], alpha);
            }
            x++;
            temp = tileRef[lineOffset++]&15;
            if(temp==0){
//                scanLine[x] = 0;
            }else{
                if(temp==invisible){alpha = 16;}else{alpha=32;}
                scanLine[x] = alphaBlendRGB565(guiFont_pal[temp], scanLine[x], alpha);
            }
            x++;
        }
    }

}

void spriteFill(std::uint8_t* line, std::uint32_t y, bool skip){

    if(renderMenuLayer==true){
        GUILine(line, y, skip);
    }

    if(spriteLine[y]==0) return;

    #define width 16

//    if(bg.totalGemsCollected == bg.totalGemsToCollect){
/*
        uint32_t py = y/8;
        uint32_t multiplied = (32*py) + 450; // the 450 is to select a different part of the sprite
        
        auto offsetedPal = pal + doorPalOffset;
        auto spriteSource = sprite_door + multiplied;

        // for animating the background effect
        #define SixteenBitLine1()\
            if(*ScanLine){\
                *Palette++ = *ScanLine;\
            }else{\
                if(*Line)\
                    *Palette++ = rgbPalette[*Line];\
                else\
                    *Palette++ = offsetedPal[spriteSource[plasmaScreen[x]]];\
            }\
            *ScanLine++ = 0;\
            *Line++ = x++;

        for(uint32_t x=0; x<220;){
            SixteenBitLine1(); 
            SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1();
            SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1();
        }
*/
//    }else{
        
        auto scanLine = &Pokitto::Display::palette[16]; // start 32 pixels in because of underflow
        #define width 16
        #define halfWidth 8

        int y2 = y;
    
        spriteLine[y]=0;
        if(spriteCount>=0){
            for(int32_t spriteIndex = 1; spriteIndex<=spriteCount; spriteIndex++){
                auto &sprite = sprites[spriteIndex];
                if(sprite.bit==1){
                    if((int)y >= sprite.y && (int)y < sprite.y + sprite.imageData[0]){
                        if(sprite.x>-width && sprite.x<PROJ_LCDWIDTH){
                            if(sprite.hFlip){
                                uint32_t so = 2+(2 * (y2-sprite.y));
                                auto si = &sprite.imageData[so+3];
                                auto sl = &scanLine[sprite.x];
                                auto st = &scanLine[sprite.x + sprite.imageData[0]];
                                auto palette = sprite.paletteData;
                                auto pixel = *si;
                                #define midLoop()\
                                    if(((pixel=*si)>>7)&3) *sl = palette[(pixel>>7)&1];\
                                    sl++;\
                                    if((pixel>>6)&1) *sl = palette[(pixel>>6)&1];\
                                    sl++;\
                                    if((pixel>>5)&1) *sl = palette[(pixel>>5)&1];\
                                    sl++;\
                                    if((pixel>>4)&1) *sl = palette[(pixel>>4)&1];\
                                    sl++;\
                                    if((pixel>>3)&1) *sl = palette[(pixel>>3)&1];\
                                    sl++;\
                                    if((pixel>>2)&1) *sl = palette[(pixel>>2)&1];\
                                    sl++;\
                                    if((pixel>>1)&1) *sl = palette[(pixel>>1)&1];\
                                    sl++;\
                                    if(pixel&1) *sl = palette[pixel&1];\
                                    si--; sl++;
                                while(sl < st)
                                    midLoop();
                            }else{
                                uint32_t so = 2+(2 * (y2-sprite.y));
                                auto si = &sprite.imageData[so];
                                auto sl = &scanLine[sprite.x];
                                int sw = sprite.imageData[0];
                                auto palette = sprite.paletteData;
                                auto pixel = *si;
                                #define midLoop()\
                                    if(((pixel=*si)>>7)&3) *sl = palette[(pixel>>7)&1];\
                                    sl++;\
                                    if((pixel>>6)&1) *sl = palette[(pixel>>6)&1];\
                                    sl++;\
                                    if((pixel>>5)&1) *sl = palette[(pixel>>5)&1];\
                                    sl++;\
                                    if((pixel>>4)&1) *sl = palette[(pixel>>4)&1];\
                                    sl++;\
                                    if((pixel>>3)&1) *sl = palette[(pixel>>3)&1];\
                                    sl++;\
                                    if((pixel>>2)&1) *sl = palette[(pixel>>2)&1];\
                                    sl++;\
                                    if((pixel>>1)&1) *sl = palette[(pixel>>1)&1];\
                                    sl++;\
                                    if(pixel&1) *sl = palette[pixel&1];\
                                    si++; sl++;

                                    midLoop();
                            }
    
                        } // if X
                    } // if Y
                } // 1bpp
                if(sprite.bit==2){
                    if((int)y >= sprite.y && (int)y < sprite.y + sprite.imageData[0]){
                        if(sprite.x>-width && sprite.x<PROJ_LCDWIDTH){
                            if(sprite.hFlip){
                                uint32_t so = 2+(2 * (y2-sprite.y));
                                auto si = &sprite.imageData[so+3];
                                auto sl = &scanLine[sprite.x];
                                auto palette = sprite.paletteData;
                                auto pixel = *si;
                                #define midLoop()\
                                    if(((pixel=*si)>>6)&3) *sl = palette[(pixel>>6)&3];\
                                    sl++;\
                                    if((pixel>>4)&3) *sl = palette[(pixel>>4)&3];\
                                    sl++;\
                                    if((pixel>>2)&3) *sl = palette[(pixel>>2)&3];\
                                    sl++;\
                                    if((pixel)&3) *sl = palette[(pixel)&3];\
                                    *si--; sl++;
                                midLoop(); midLoop();
                            }else{
                                uint32_t so = 2+(2 * (y2-sprite.y));
                                auto si = &sprite.imageData[so];
                                auto sl = &scanLine[sprite.x];
                                auto palette = sprite.paletteData;
                                auto pixel = *si;
                                #define midLoop()\
                                    if(((pixel=*si)>>6)&3) *sl = palette[(pixel>>6)&3];\
                                    sl++;\
                                    if((pixel>>4)&3) *sl = palette[(pixel>>4)&3];\
                                    sl++;\
                                    if((pixel>>2)&3) *sl = palette[(pixel>>2)&3];\
                                    sl++;\
                                    if((pixel)&3) *sl = palette[(pixel)&3];\
                                    *si++; sl++;

                                midLoop(); midLoop();
                            }
    
                        } // if X
                    } // if Y
                } // 2bpp
                if(sprite.bit==4){
                    if((int)y >= sprite.y && (int)y < sprite.y + sprite.imageData[0]){
                        if(sprite.x>-width && sprite.x<PROJ_LCDWIDTH){
                            if(sprite.hFlip){
                                uint32_t so = 2+(halfWidth * (y2-sprite.y));
                                auto si = &sprite.imageData[so+7];
                                auto sl = &scanLine[sprite.x];
                                auto palette = sprite.paletteData;
                                #define midLoop()\
                                    if(auto pixel = *si&15) *sl = palette[pixel];\
                                    sl++;\
                                    if(auto pixel = *si>>4) *sl = palette[pixel];\
                                    si--; sl++;
                                midLoop(); midLoop(); midLoop(); midLoop();
                                midLoop(); midLoop(); midLoop(); midLoop(); 
                            }else{
                                uint32_t so = 2+(halfWidth * (y2-sprite.y));
                                auto si = &sprite.imageData[so];
                                auto sl = &scanLine[sprite.x];
                                auto palette = sprite.paletteData;
                                #define midLoop()\
                                    if(auto pixel = *si>>4) *sl = palette[pixel];\
                                    sl++;\
                                    if(auto pixel = *si&15) *sl = palette[pixel];\
                                    si++; sl++;
                                midLoop(); midLoop(); midLoop(); midLoop();
                                midLoop(); midLoop(); midLoop(); midLoop(); 
                            }
    
                        } // if X
                    } // if Y
                } // 4bpp
                if(sprite.bit==8){
                    if((int)y >= sprite.y && (int)y < sprite.y + sprite.imageData[0]){
                        if(sprite.x>-width && sprite.x<PROJ_LCDWIDTH){
                            if(sprite.hFlip){
                                uint32_t so = 2+(width * (y2-sprite.y));
                                auto si = &sprite.imageData[so+15];
                                auto sl = &scanLine[sprite.x];
                                auto palette = sprite.paletteData;
                                #define midLoop()\
                                    if(auto pixel = *si) *sl = palette[pixel];\
                                    si--; sl++;
                                midLoop(); midLoop(); midLoop(); midLoop();
                                midLoop(); midLoop(); midLoop(); midLoop(); 
                                midLoop(); midLoop(); midLoop(); midLoop();
                                midLoop(); midLoop(); midLoop(); midLoop(); 
                            }else{
                                uint32_t so = 2+(width * (y2-sprite.y));
                                auto si = &sprite.imageData[so];
                                auto sl = &scanLine[sprite.x];
                                auto palette = sprite.paletteData;
                                #define midLoop()\
                                    if(auto pixel = *si) *sl = palette[pixel];\
                                    si++; sl++;
                                midLoop(); midLoop(); midLoop(); midLoop();
                                midLoop(); midLoop(); midLoop(); midLoop(); 
                                midLoop(); midLoop(); midLoop(); midLoop();
                                midLoop(); midLoop(); midLoop(); midLoop(); 
                            }
    
                        } // if X
                    } // if Y
                } // 8bpp
            } // for spriteCount
        } // sprite count >1

//    }
    return;
}


/*
inline void wiggleFill(std::uint8_t* line, std::uint32_t y, bool skip){

    if(y<bg.screenTop+1 || y>=bg.screenBottom){
        return;
    }

    int t=wiggle[y+offsetAngle];
    if(t>=0){
        memcpy(&line[0], &line[t], 220-t);
        memset(&line[220-t], 255, t);
    }else{
        for(int p=220; p>-t; p--){
            line[p] = line[t+p];
        }
        memset(&line[0], 255, -t);
    }
    return;
}
*/

void myBGFiller(std::uint8_t* line, std::uint32_t y, bool skip){

    if(y==bg.screenTop){
        for(uint32_t x=0; x<220; ++x){
            line[x]=x+16;
        }        
    }

    if(y==0 || y==bg.screenBottom){
        gamePalette.rgb[0]=0;
        for(uint32_t x=0; x<220; ++x){
            line[x]=0;
        }        
    }

    #define TILEWIDTH 8
    #define TILEHEIGHT 8

    if(y<bg.screenTop || y>=bg.screenBottom+1){
        return;
    }

    gamePalette.rgb[0] = 0xF81F; //gamePalette.hpal[hline[y]];

    int32_t x = -(bg.windowX%TILEWIDTH)+16;
    uint32_t tileIndex = (bg.windowX/TILEWIDTH) + ((y+bg.windowY)/TILEWIDTH) * bg.miniMap[0];
    uint32_t jStart = ((y+bg.windowY)%TILEWIDTH) *TILEWIDTH; // current line in current tile

    uint32_t lineOffset;
    uint32_t thisTile;
    auto lineP = &Pokitto::Display::palette[0];

    #define bgTileLine()\
        thisTile = bg.miniMap[2+tileIndex++]&0x83FF;\
        lineOffset = ((thisTile&1023)*tbt) + jStart;\
        lineP = &Pokitto::Display::palette[x];\
        x+=8;\
        if(thisTile&32768){\
            auto tilesP = &tiles[7 + lineOffset];\
            *lineP++ = gamePalette.rgb[*tilesP--];\
            *lineP++ = gamePalette.rgb[*tilesP--];\
            *lineP++ = gamePalette.rgb[*tilesP--];\
            *lineP++ = gamePalette.rgb[*tilesP--];\
            *lineP++ = gamePalette.rgb[*tilesP--];\
            *lineP++ = gamePalette.rgb[*tilesP--];\
            *lineP++ = gamePalette.rgb[*tilesP--];\
            *lineP++ = gamePalette.rgb[*tilesP--];\
        }else{\
            auto tilesP = &tiles[lineOffset];\
            *lineP++ = gamePalette.rgb[*tilesP++];\
            *lineP++ = gamePalette.rgb[*tilesP++];\
            *lineP++ = gamePalette.rgb[*tilesP++];\
            *lineP++ = gamePalette.rgb[*tilesP++];\
            *lineP++ = gamePalette.rgb[*tilesP++];\
            *lineP++ = gamePalette.rgb[*tilesP++];\
            *lineP++ = gamePalette.rgb[*tilesP++];\
            *lineP++ = gamePalette.rgb[*tilesP++];\
        }

    // unrolling this loop got an extra 10fps
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); 
}

// background scenery
void myBGFiller2(std::uint8_t* line, std::uint32_t y, bool skip){

    if(y<bg.screenTop || y>bg.screenBottom){
        return;
    }

    int32_t stX = -(bg.midwindowX&7)+16;
    int x = stX;
    uint32_t tileIndex = (bg.midwindowX>>3) + ((y+bg.midwindowY)>>3) * bg.midminiMap[0];
    uint32_t jStart = ((y+bg.midwindowY)&7) <<3; // current line in current tile

    uint32_t lineOffset;
    uint32_t thisTile;
    auto lineP = &Pokitto::Display::palette[0];
    gamePalette.rgb[0] = gamePalette.hpal[hline[y]];

    uint32_t py = y/8;
    uint32_t multiplied = (32*py) + 450; // the 450 is to select a different part of the sprite
    auto offsetedPal = pal + doorPalOffset;
    auto spriteSource = sprite_door + multiplied;
    auto tilesP = &tiles[lineOffset];

        #define bgTileLine2()\
            thisTile = bg.midminiMap[2+tileIndex++]&0x83FF;\
            lineOffset = ((thisTile&1023)*tbt) + jStart;\
            lineP = &Pokitto::Display::palette[x];\
            x+=8;\
            tilesP = &tiles[lineOffset];\
            if(*lineP==0xF81F) *lineP = offsetedPal[spriteSource[plasmaScreen[x]]];\
            *lineP++;\
            if(*lineP==0xF81F) *lineP = offsetedPal[spriteSource[plasmaScreen[x]]];\
            *lineP++;\
            if(*lineP==0xF81F) *lineP = offsetedPal[spriteSource[plasmaScreen[x]]];\
            *lineP++;\
            if(*lineP==0xF81F) *lineP = offsetedPal[spriteSource[plasmaScreen[x]]];\
            *lineP++;\
            if(*lineP==0xF81F) *lineP = offsetedPal[spriteSource[plasmaScreen[x]]];\
            *lineP++;\
            if(*lineP==0xF81F) *lineP = offsetedPal[spriteSource[plasmaScreen[x]]];\
            *lineP++;\
            if(*lineP==0xF81F) *lineP = offsetedPal[spriteSource[plasmaScreen[x]]];\
            *lineP++;\
            if(*lineP==0xF81F) *lineP = offsetedPal[spriteSource[plasmaScreen[x]]];\
            *lineP++;

        #define bgTileLine3()\
            thisTile = bg.midminiMap[2+tileIndex++]&0x83FF;\
            lineOffset = ((thisTile&1023)*tbt) + jStart;\
            lineP = &Pokitto::Display::palette[x];\
            x+=8;\
            if(thisTile&32768){\
                auto tilesP = &tiles[7 + lineOffset];\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP--];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP--];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP--];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP--];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP--];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP--];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP--];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP--];\
                *lineP++;\
            }else{\
                auto tilesP = &tiles[lineOffset];\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP++];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP++];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP++];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP++];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP++];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP++];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP++];\
                *lineP++;\
                if(*lineP==0xF81F) *lineP = gamePalette.rgb[*tilesP++];\
                *lineP++;\
            }
    if(bg.totalGemsCollected == bg.totalGemsToCollect){
        // unrolling this loop got an extra 10fps
        bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
        bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
        bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
        bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
        bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
        bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
        bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
        bgTileLine2();
    }else{
        bgTileLine3(); bgTileLine3(); bgTileLine3(); bgTileLine3(); 
        bgTileLine3(); bgTileLine3(); bgTileLine3(); bgTileLine3(); 
        bgTileLine3(); bgTileLine3(); bgTileLine3(); bgTileLine3(); 
        bgTileLine3(); bgTileLine3(); bgTileLine3(); bgTileLine3(); 
        bgTileLine3(); bgTileLine3(); bgTileLine3(); bgTileLine3(); 
        bgTileLine3(); bgTileLine3(); bgTileLine3(); bgTileLine3(); 
        bgTileLine3(); bgTileLine3(); bgTileLine3(); bgTileLine3(); 
        bgTileLine3();
    }
}

