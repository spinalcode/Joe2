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
    if(y<bg.screenTop+1 || y>=bg.screenBottom){
        return;
    }

    uint32_t x = 0;

    uint32_t tileIndex = 2+(y/8) * 27;
    uint32_t lineOffset;
    uint32_t alpha;
    uint32_t temp;
    auto &tileRef = guiFont[0];
    #define invisible 1

    for(int d=0; d<27; d++){
        lineOffset = 2 + ((y%8)*4) + guiBG[tileIndex++]*34;
        for(int c=0; c<4; c++){
            temp = tileRef[lineOffset]>>4;
            if(temp==0){
                scanLine[PRESCAN + x] = 0;
            }else{
                if(temp==invisible){alpha = 16;}else{alpha=32;}
                scanLine[PRESCAN + x] = alphaBlendRGB565(guiFont_pal[temp], gamePalette.rgb[line[x]], alpha);
            }
            x++;
            temp = tileRef[lineOffset++]&15;
            if(temp==0){
                scanLine[PRESCAN + x] = 0;
            }else{
                if(temp==invisible){alpha = 16;}else{alpha=32;}
                scanLine[PRESCAN + x] = alphaBlendRGB565(guiFont_pal[temp], gamePalette.rgb[line[x]], alpha);
            }
            x++;
        }
    }

}


void spritesToLine(std::uint32_t y){
    if(spriteCount>=0){
        uint8_t offset = 0;
        for(uint8_t spriteIndex = 1; spriteIndex<spriteCount; spriteIndex++){
            auto & sprite = sprites[spriteIndex];
            if(y >= sprite.y && y < sprite.y + sprite.imageData[1]){
                if(sprite.x>-sprite.imageData[0] && sprite.x<PROJ_LCDWIDTH){
                    switch(sprite.bit){
                        case 16:{
                            // untested, I don't have any 16bit sprites
                            sprite.offset = 2+(sprite.imageData[0] * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0]; offset++){
                                    int pixPos = (sprite.imageData[0] + sprite.x - offset);
                                    if(sprite.imageData[sprite.offset]){
                                        // we |1 to the colour value to make sure it isn't transparent
                                        scanLine[PRESCAN + pixPos] = sprite.imageData[sprite.offset] | 1;
                                    }
                                    sprite.offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0]; offset++){
                                    int pixPos = sprite.x + offset;
                                    if(sprite.imageData[sprite.offset]){
                                        scanLine[PRESCAN + pixPos] = sprite.imageData[sprite.offset] | 1;
                                    }
                                    sprite.offset++;
                                }
                            }
                            break;
                        } // case 16
                        case 8:{
                            sprite.offset = 2+(sprite.imageData[0] * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0]; offset++){
                                    int pixPos = sprite.imageData[0] + sprite.x - offset;
                                    if(sprite.imageData[sprite.offset]){
                                        // we |1 to the colour value to make sure it isn't transparent
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]] | 1;
                                    }
                                    sprite.offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0]; offset++){
                                    int pixPos = sprite.x + offset;
                                    if(sprite.imageData[sprite.offset]){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]] | 1;
                                    }
                                    sprite.offset++;
                                }
                            }
                            break;
                        } // case 8
                        case 4:{
                            sprite.offset = 2+((sprite.imageData[0]/2) * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.imageData[0] + sprite.x - offset;
                                    if((sprite.imageData[sprite.offset]>>4)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>4] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if((sprite.imageData[sprite.offset]&15)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]&15] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.x + offset;
                                    if((sprite.imageData[sprite.offset]>>4)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>4] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if((sprite.imageData[sprite.offset]&15)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]&15] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }
                            break;
                        } // case 4
                        case 2:{
                            sprite.offset = 2+((sprite.imageData[0]/4) * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.imageData[0] + sprite.x - offset;
                                    if((sprite.imageData[sprite.offset]>>6)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>6] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>4)&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>4)&3] | 1;
                                    }
                                    pixPos--;
                                    offset++;
                                    if(((sprite.imageData[sprite.offset]>>2)&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>2)&3] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if((sprite.imageData[sprite.offset]&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]&3] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.x + offset;
                                    if((sprite.imageData[sprite.offset]>>6)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>6] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>4)&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>4)&3] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>2)&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>2)&3] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if((sprite.imageData[sprite.offset]&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]&3] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }
                            break;
                        } // case 2
                        case 1:{
                            sprite.offset = 2+((sprite.imageData[0]/8) * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.imageData[0] + sprite.x - offset;
                                    if((sprite.imageData[sprite.offset]>>7)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>7] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>6)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>6)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>5)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>5)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>4)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>4)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>3)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>3)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>2)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>2)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>1)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>1)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset])&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset])&1] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.x + offset;
                                    if((sprite.imageData[sprite.offset]>>7)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>7] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>6)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>6)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>5)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>5)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>4)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>4)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>3)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>3)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>2)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>2)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>1)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>1)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset])&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset])&1] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }
                            break;
                        } // case 1
                    }
                } // if X
            } // if Y
        } // for spriteCount
    } // sprite count >1
}


void spriteFill(std::uint8_t* line, std::uint32_t y, bool skip){

    // This is probably quite slow and will need some work.

    if(y<bg.screenTop+1 || y>=bg.screenBottom){
        clearPalette(256);
        return;
    }


    if(renderMenuLayer==true){
        GUILine(line, y, skip);
    }
    
    spritesToLine(y);

    auto Palette = &Pokitto::Display::palette[0];
    auto Line = &line[0];
    auto ScanLine = &scanLine[PRESCAN];
    uint16_t* rgbPalette = &gamePalette.rgb[0];

    if(bg.totalGemsCollected == bg.totalGemsToCollect){
        uint32_t py = y/8;
        uint32_t multiplied = (32*py) + 450; // the 450 is to select a different part of the sprite
        
        auto offsetedPal = pal + doorPalOffset;
        auto spriteSource = sprite_door + multiplied;

        #define SixteenBitLine1()\
            if(*ScanLine!=0){\
                *Palette++ = *ScanLine;\
            }else{\
                if(*Line==0)\
                    *Palette++ = offsetedPal[spriteSource[plasmaScreen[x]]];\
                else\
                    *Palette++ = rgbPalette[*Line];\
            }\
            *ScanLine++ = 0;\
            *Line++ = x++;

        for(uint32_t x=0; x<220;){
            SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1();
            SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1();
            SixteenBitLine1();
        }
    }else{
        #define SixteenBitLine2()\
            if(*ScanLine!=0){\
                *Palette++ = *ScanLine;\
            }else{\
                *Palette++ = rgbPalette[*Line];\
            }\
            *ScanLine++ = 0;\
            *Line++ = x++;

        for(uint32_t x=0; x<220;){
            SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); 
            SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); 
            SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); 

        }
    }

    Pokitto::Display::paletteptr = Pokitto::Display::palette;

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

    if(y<bg.screenTop+1 || y>=bg.screenBottom){
        return;
    }

    gamePalette.rgb[0] = gamePalette.hpal[hline[y]];

    uint32_t stX = -(bg.windowX&7);
    uint32_t x = stX;
    uint32_t tileIndex = (bg.windowX>>3) + ((y+bg.windowY)>>3) * bg.miniMap[0];
    uint32_t jStart = ((y+bg.windowY)&7) <<3; // current line in current tile

    uint32_t lineOffset;
    uint32_t lineStart = -stX;
    uint32_t thisTile;
    auto lineP = &line[0];
    if(x<0){lineOffset+=lineStart; x=0;}

    #define bgTileLine()\
        thisTile = bg.miniMap[2+tileIndex++]&0x83FF;\
        lineOffset = ((thisTile&1023)*tbt) + jStart;\
        lineP = &line[x];\
        x+=8;\
        if(thisTile&32768){\
            auto tilesP = &tiles[7 + lineOffset];\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
        }else{\
            auto tilesP = &tiles[lineOffset];\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
        }

    #define bgHalfTileLine()\
        thisTile = bg.miniMap[2+tileIndex++]&0x83FF;\
        lineOffset = ((thisTile&1023)*tbt) + jStart;\
        lineP = &line[x];\
        if(thisTile&32768){\
            auto tilesP = &tiles[7 + lineOffset];\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
            *lineP++ = *tilesP--;\
        }else{\
            auto tilesP = &tiles[lineOffset];\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
            *lineP++ = *tilesP++;\
        }


    // unrolling this loop got an extra 10fps
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgHalfTileLine(); 

}

// background scenery
void myBGFiller2(std::uint8_t* line, std::uint32_t y, bool skip){

    if(y<bg.screenTop || y>bg.screenBottom){
        return;
    }

    uint32_t stX = -(bg.midwindowX&7);
    uint32_t x = stX;
    uint32_t tileIndex = (bg.midwindowX>>3) + ((y+bg.midwindowY)>>3) * bg.midminiMap[0];
    uint32_t jStart = ((y+bg.midwindowY)&7) <<3; // current line in current tile

    uint32_t lineOffset;
    uint32_t lineStart = -stX;
    uint32_t thisTile;
    auto lineP = &line[0];
    if(x<0){lineOffset+=lineStart; x=0;}

    #define bgTileLine2()\
        thisTile = bg.midminiMap[2+tileIndex++]&0x83FF;\
        lineOffset = ((thisTile&1023)*tbt) + jStart;\
        lineP = &line[x];\
        if(thisTile&32768){\
            auto tilesP = &tiles[7 + lineOffset];\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
        }else{\
            auto tilesP = &tiles[lineOffset];\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
        }

    #define bgHalfTileLine2()\
        thisTile = bg.midminiMap[2+tileIndex++]&0x83FF;\
        lineOffset = ((thisTile&1023)*tbt) + jStart;\
        lineP = &line[x];\
        if(thisTile&32768){\
            auto tilesP = &tiles[7 + lineOffset];\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP--;\
        }else{\
            auto tilesP = &tiles[lineOffset];\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
            if(line[x]==0) line[x] = *tilesP;\
            x++; *tilesP++;\
        }

    // unrolling this loop got an extra 10fps
    bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
    bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
    bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
    bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
    bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
    bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
    bgTileLine2(); bgTileLine2(); bgTileLine2(); bgTileLine2();
    bgHalfTileLine2(); 

}

/*
// render the collision map
void myBGFiller3(std::uint8_t* line, std::uint32_t y, bool skip){

    if(skip)return;

    uint32_t stX = (-bg.windowX)%BG_TILE_SIZE_W;
    uint32_t x = stX;

    uint32_t tileIndex = bg.windowX/BG_TILE_SIZE_W + ((y+bg.windowY)/BG_TILE_SIZE_H) * bg.miniMap[0];
    uint32_t jStart = ((y+bg.windowY) %BG_TILE_SIZE_H) * BG_TILE_SIZE_W; // current line in current tile

    uint32_t tileStart;
    uint32_t lineOffset;
    uint32_t lineStart = -stX;
//    uint32_t isFlipped = 0;
    uint32_t thisTile;
//    uint32_t tileOffset=0;
//    int xLine = 220-(bg.mapX%220);

    #define bgColLine()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = (bg.miniMap[2+tileIndex++]>>10)&31;\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        for(uint32_t b=0; b<BG_TILE_SIZE_W; b++){\
            if(collisionTile[lineOffset] > 0){\
                if((bg.windowX + x)%224==0)line[x]=240;\
            }\
            lineOffset++; x++;\
        }

    // unrolling this loop got an extra 10fps
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
}
*/

void lcdRefreshTASMode(const uint16_t* palette){
    int screenWidth = 220;
    int screenHeight = 176;

    // reset the screen to 0,0 each frame...
    #ifndef POK_SIM
        write_command_16(0x20);  // Horizontal DRAM Address
        write_data_16(0);
        write_command_16(0x22); // write data to DRAM
        CLR_CS_SET_CD_RD_WR;
        SET_MASK_P2;
    #else
        Pokitto::setDRAMptr(0,0); //needs to be called explicitly for pokitto_sim (no real controller!)
    #endif
    
    uint8_t lineBuffer[screenWidth + 16];
    uint8_t *line = lineBuffer + 8;
    auto mask = Pokitto::Display::TASMask;
    bool disabled = mask & 1;
    uint32_t maskY = 8;

    constexpr int lineFillerCount = sizeof(Pokitto::Display::lineFillers) / sizeof(Pokitto::Display::lineFillers[0]);
    int fillerCount = 0;
    TAS::LineFiller fillers[lineFillerCount];
    for(int i=0; i<lineFillerCount; i++){
        auto filler = Pokitto::Display::lineFillers[i];
        if(!filler || filler == TAS::NOPFiller)
            continue;
        fillers[fillerCount++] = filler;
    }

    for(uint32_t y=0; y<screenHeight; ++y ){
        #ifdef POK_SIM
            Pokitto::setDRAMptr(0,y); //needs to be called explicitly for pokitto_sim (no real controller!)
        #endif // POK_SIM

        if(interlaceScreen){
            if(frameJump){
                disabled = (y&1);
            }else{
                disabled = !(y&1);
            }
        }


        for( int i=0; i<fillerCount; ++i ){
            fillers[i]( line, y, disabled );
        }

        if(disabled){
            #ifndef POK_SIM
                write_command_16(0x20);  // Horizontal DRAM Address
                write_data_16(y+1);
                write_command_16(0x21);  // Vertical DRAM Address
                write_data_16(0);  // 0
                write_command_16(0x22); // write data to DRAM
                CLR_CS_SET_CD_RD_WR;
                SET_MASK_P2;
            #else
                Pokitto::setDRAMptr(0,y+1); //needs to be called explicitly for pokitto_sim (no real controller!)
            #endif
            continue;
        }

        flushLine(palette, line);
    }
}

void myRefreshTASMode(const uint16_t* palette){
    int screenWidth = 220;
    int screenHeight = 176;

    // reset the screen to 0,0 each frame...
    #ifndef POK_SIM
        write_command_16(0x20);  // Horizontal DRAM Address
        write_data_16(0);
        write_command_16(0x22); // write data to DRAM
        CLR_CS_SET_CD_RD_WR;
        SET_MASK_P2;
    #else
        Pokitto::setDRAMptr(0,0); //needs to be called explicitly for pokitto_sim (no real controller!)
    #endif
    
    uint8_t lineBuffer[screenWidth + 16];
    uint8_t *line = lineBuffer + 8;
    auto mask = Pokitto::Display::TASMask;
    bool disabled = mask & 1;
    uint32_t maskY = 8;

    constexpr int lineFillerCount = sizeof(Pokitto::Display::lineFillers) / sizeof(Pokitto::Display::lineFillers[0]);
    int fillerCount = 0;
    TAS::LineFiller fillers[lineFillerCount];
    for(int i=0; i<lineFillerCount; i++){
        auto filler = Pokitto::Display::lineFillers[i];
        if(!filler || filler == TAS::NOPFiller)
            continue;
        fillers[fillerCount++] = filler;
    }

    for(uint32_t y=0; y<screenHeight; ++y ){
        #ifdef POK_SIM
            Pokitto::setDRAMptr(0,y); //needs to be called explicitly for pokitto_sim (no real controller!)
        #endif // POK_SIM

        if(interlaceScreen){
            if(frameJump){
                disabled = (y&1);
            }else{
                disabled = !(y&1);
            }
        }


        for( int i=0; i<fillerCount; ++i ){
            fillers[i]( line, y, disabled );
        }

        flushLine(palette, line);
    }
}

