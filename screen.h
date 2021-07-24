// tile map
#define bgTileSizeH 8
#define bgTileSizeW 8
#define tbt bgTileSizeH*bgTileSizeW

void inline bgTileLine2();
void inline bgTileLine();


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


void spriteFill(std::uint8_t* line, std::uint32_t y, bool skip){

    // This is probably quite slow and will need some work.

    if(y<bg.screenTop+1 || y>=bg.screenBottom){
        clearPalette(256);
        return;
    }

//    uint16_t scanLine[440];
//    memset(scanLine,0,440);

    if(spriteCount>=0){
        uint8_t offset = 0;
        for(uint8_t spriteIndex = 0; spriteIndex<spriteCount; spriteIndex++){
            auto & sprite = sprites[spriteIndex];
            if(y >= sprite.y && y < sprite.y + sprite.imageData[1]){
                if(sprite.x>-sprite.imageData[0] && sprite.x<PROJ_LCDWIDTH){
                    int startPos = sprite.x;
                    switch(sprite.bit){
                        case 16:{
                            // untested, I don't have any 16bit sprites
                            sprite.offset = 2+(sprite.imageData[0] * (y-sprite.y));
                            int pixelPos = sprite.x;
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
                            int pixelPos = sprite.x;
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
                            int pixelPos = sprite.x;
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
                            int pixelPos = sprite.x;
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
                            sprite.offset = 2+((sprite.imageData[0]/4) * (y-sprite.y));
                            int pixelPos = sprite.x;
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

    auto Palette = &Pokitto::Display::palette[0];
    auto Line = &line[0];
    auto ScanLine = &scanLine[PRESCAN + 0];
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
            *Line++ = x++;\
            *ScanLine++ = 0;

        for(uint32_t x=0; x<220;){
            SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1();
            SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1();
            SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1();
            SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1(); SixteenBitLine1();
        }
    }else{
        #define SixteenBitLine2()\
            if(*ScanLine!=0){\
                *Palette++ = *ScanLine;\
            }else{\
                *Palette++ = rgbPalette[*Line];\
            }\
            *Line++ = x++;\
            *ScanLine++ = 0;

        for(uint32_t x=0; x<220;){
            SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2();
            SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2();
            SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2();
            SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2(); SixteenBitLine2();
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

    uint32_t stX = (-bg.windowX)%bgTileSizeW;
    uint32_t x = stX;
    uint32_t tileIndex = bg.windowX/bgTileSizeW + ((y+bg.windowY)/bgTileSizeH) * bg.miniMap[0];
    uint32_t jStart = ((y+bg.windowY) %bgTileSizeH) * bgTileSizeW; // current line in current tile

    uint32_t tileStart;
    uint32_t lineOffset;
    uint32_t lineStart = -stX;
//    uint32_t isFlipped = 0;         /* isFlipped = thisTile>>15 */
    uint32_t thisTile;
    uint32_t tileOffset=0;
    uint32_t b=bgTileSizeW;
    auto lineP = &line[0];

    #define bgTileLine()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = bg.miniMap[2+tileIndex++]&0x83FF;\
        tileStart = (thisTile&1023)*tbt;\
        lineOffset = tileStart + jStart;\
        b=bgTileSizeW;\
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
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = bg.miniMap[2+tileIndex++]&0x83FF;\
        tileStart = (thisTile&1023)*tbt;\
        lineOffset = tileStart + jStart;\
        b=bgTileSizeW;\
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
//    for(uint32_t b=216; b<220; b++){ line[b]=colourBlack; }

    //if(clearScreen==true) memset(&line[0], 10, 220);

}


void myBGFiller2(std::uint8_t* line, std::uint32_t y, bool skip){

    if(y<bg.screenTop || y>bg.screenBottom){
        //memset(&line[0],0,220);
        return;
    }
//    if(skip)return;

    int mX = bg.mapX/4;
    int mY = (bg.mapY-64)/4;

//    if(mY+(int)y <=0) return;
//    if(mY+(int)y >=176) return;

    uint32_t stX = (-mX)%bgTileSizeW;
    uint32_t x = stX;
    uint32_t tileIndex = mX/bgTileSizeW + ((y+mY)/bgTileSizeH) * midmap[0];
    uint32_t jStart = ((y+mY) %bgTileSizeH) * bgTileSizeW; // current line in current tile

    uint32_t tileStart;
    uint32_t lineOffset;
    uint32_t lineStart = -stX;
    uint32_t isFlipped = 0;
    uint32_t thisTile;
    uint32_t tileOffset=0;
    uint32_t b=bgTileSizeW;
    auto lineP = &line[0];

    #define bgTileLine2()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = midmap[2+tileIndex++];\
        tileStart = (thisTile&1023)*tbt;\
        lineOffset = tileStart + jStart;\
        lineP = &line[x];\
        if(thisTile&32768){\
            auto tilesP = &tiles[7 + lineOffset];\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
        }else{\
            auto tilesP = &tiles[lineOffset];\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
        }


    #define bgHalfTileLine2()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = midmap[2+tileIndex++];\
        tileStart = (thisTile&1023)*tbt;\
        lineOffset = tileStart + jStart;\
        lineP = &line[x];\
        if(thisTile&32768){\
            auto tilesP = &tiles[7 + lineOffset];\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP--;\
        }else{\
            auto tilesP = &tiles[lineOffset];\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
            if(*lineP==0) *lineP = *tilesP;\
            x++; *lineP++; *tilesP++;\
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


// render the collision map
void myBGFiller3(std::uint8_t* line, std::uint32_t y, bool skip){

    if(skip)return;

    uint32_t stX = (-bg.windowX)%bgTileSizeW;
    uint32_t x = stX;

    uint32_t tileIndex = bg.windowX/bgTileSizeW + ((y+bg.windowY)/bgTileSizeH) * bg.miniMap[0];
    uint32_t jStart = ((y+bg.windowY) %bgTileSizeH) * bgTileSizeW; // current line in current tile

    uint32_t tileStart;
    uint32_t lineOffset;
    uint32_t lineStart = -stX;
    uint32_t isFlipped = 0;
    uint32_t thisTile;
    uint32_t tileOffset=0;
    int xLine = 220-(bg.mapX%220);

    #define bgColLine()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = (bg.miniMap[2+tileIndex++]>>10)&31;\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        for(uint32_t b=0; b<bgTileSizeW; b++){\
            if(collisionTile[lineOffset] > 0){\
                if((bg.windowX + x)%224==0)line[x]=240;\
            }\
            lineOffset++; x++;\
        }

//                line[x] = collisionTile[lineOffset];\

    // unrolling this loop got an extra 10fps
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
    bgColLine(); bgColLine(); bgColLine(); bgColLine();
}

