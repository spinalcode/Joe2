// tile map
#define bgTileSizeH 8
#define bgTileSizeW 8
#define tbt bgTileSizeH*bgTileSizeW

static inline void sd(uint16_t data)
{
    SET_MASK_P2;
    LPC_GPIO_PORT->MPIN[2] = (data<<3); // write bits to port
    CLR_MASK_P2;
}

void wc(uint16_t data)
{
    CLR_CS; // select lcd
    CLR_CD; // clear CD = command
    SET_RD; // RD high, do not read
    sd(data); // function that inputs the data into the relevant bus lines
    CLR_WR_SLOW;  // WR low
    SET_WR;  // WR low, then high = write strobe
    SET_CS; // de-select lcd
}

void wd(uint16_t data)
{
    CLR_CS;
    SET_CD;
    SET_RD;
    sd(data);
    CLR_WR;
    SET_WR;
    SET_CS;
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

void spriteFill(std::uint8_t* line, std::uint32_t y, bool skip){

    // This is probably quite slow and will need some work.

    if(y<bg.screenTop+1 || y>=bg.screenBottom){
        clearPalette(256);
        return;
    }

    #define leftOffset 64 // add space either side of the screen buffer

    uint16_t scanLine[leftOffset + 220 + leftOffset];

    if(spriteCount>1){

        for(uint8_t spriteIndex = spriteCount; spriteIndex>1; spriteIndex--){
            auto & sprite = sprites[spriteIndex];
            uint8_t spriteWidth = sprite.imageData[0];
            uint8_t spriteHeight = sprite.imageData[1];

            if(y >= sprite.y && y < sprite.y + spriteHeight){
                if(sprite.x>=-spriteWidth && sprite.x<PROJ_LCDWIDTH){
    
                    if(sprite.bit==8){
                        sprite.offset = 2+(spriteWidth * (y-sprite.y));
                        if(sprite.hFlip){
                            for(uint8_t offset = 0; offset < spriteWidth; offset++){
                                if(scanLine[leftOffset + sprite.x + spriteWidth - offset]==0){
                                    uint8_t thisPixel = sprite.imageData[sprite.offset];
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + spriteWidth - offset] = colour;
                                }
                                sprite.offset++;
                            }
                        }else{
                            for(uint8_t offset = 0; offset < spriteWidth; offset++){
                                if(scanLine[leftOffset + sprite.x + offset]==0){
                                    uint8_t thisPixel = sprite.imageData[sprite.offset];
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + offset] = colour;
                                }
                                sprite.offset++;
                            }
                        }
                    }

                    else if(sprite.bit==4){
                        sprite.offset = 2+((spriteWidth/2) * (y-sprite.y));
                        if(sprite.hFlip){
                            for(uint8_t offset = 0; offset < spriteWidth;){
                                if(scanLine[leftOffset + sprite.x + spriteWidth - offset]==0){
                                    uint8_t thisPixel = sprite.imageData[sprite.offset]>>4;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + spriteWidth - offset] = colour;
                                }
                                offset++;
                                if(scanLine[leftOffset + sprite.x + spriteWidth - offset]==0){
                                    uint8_t thisPixel = sprite.imageData[sprite.offset]>>4;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + spriteWidth - offset] = colour;
                                }
                                offset++;
                                sprite.offset++;
                            }
                        }else{
                            for(uint8_t offset = 0; offset < spriteWidth;){
                                if(scanLine[leftOffset + sprite.x + offset]==0){
                                    uint8_t thisPixel = sprite.imageData[sprite.offset]>>4;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + offset] = colour;
                                }
                                offset++;
                                if(scanLine[leftOffset + sprite.x + offset]==0){
                                    uint8_t thisPixel = sprite.imageData[sprite.offset]&15;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + offset] = colour;
                                }
                                offset++;
                                sprite.offset++;
                            }
                        }
                    }

                    else if(sprite.bit==2){
                        sprite.offset = 2+((spriteWidth/4) * (y-sprite.y));
                        if(sprite.hFlip){
                            for(uint8_t offset = 0; offset < spriteWidth;){
                                if(scanLine[leftOffset + sprite.x + spriteWidth - offset]==0){
                                    uint8_t thisPixel = sprite.imageData[sprite.offset]>>6;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x +  spriteWidth -offset] = colour;
                                }
                                offset++;
                                if(scanLine[leftOffset + sprite.x + spriteWidth - offset]==0){
                                    uint8_t thisPixel = (sprite.imageData[sprite.offset]>>4)&3;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x +  spriteWidth -offset] = colour;
                                }
                                offset++;
                                if(scanLine[leftOffset + sprite.x + spriteWidth - offset]==0){
                                    uint8_t thisPixel = (sprite.imageData[sprite.offset]>>2)&3;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x +  spriteWidth -offset] = colour;
                                }
                                offset++;
                                if(scanLine[leftOffset + sprite.x + spriteWidth - offset]==0){
                                    uint8_t thisPixel = (sprite.imageData[sprite.offset])&3;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x +  spriteWidth -offset] = colour;
                                }
                                offset++;
                                sprite.offset++;
                            }
                        }else{
                            for(uint8_t offset = 0; offset < spriteWidth;){
                                if(scanLine[leftOffset + sprite.x + offset]==0){
                                    uint8_t thisPixel = sprite.imageData[sprite.offset]>>6;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + offset] = colour;
                                }
                                offset++;
                                if(scanLine[leftOffset + sprite.x + offset]==0){
                                    uint8_t thisPixel = (sprite.imageData[sprite.offset]>>4)&3;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + offset] = colour;
                                }
                                offset++;
                                if(scanLine[leftOffset + sprite.x + offset]==0){
                                    uint8_t thisPixel = (sprite.imageData[sprite.offset]>>2)&3;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + offset] = colour;
                                }
                                offset++;
                                if(scanLine[leftOffset + sprite.x + offset]==0){
                                    uint8_t thisPixel = (sprite.imageData[sprite.offset])&3;
                                    uint16_t colour = sprite.paletteData[thisPixel];
                                    if(colour==0){colour = 1;} // can't have a zero value or it will be transparent
                                    if(thisPixel>0) scanLine[leftOffset + sprite.x + offset] = colour;
                                }
                                offset++;
                                sprite.offset++;
                            }
                        }
                    }

      
                }
            }
        }
    }


    uint16_t tempPal[512];
    auto lineP = &tempPal[0];
    auto lineT = &line[0];
    uint32_t px=32;
    uint32_t py = y/8;
    for(uint8_t x=0; x<220; x++){
        if(x%8==0){if(--px==0)px=32;}
        uint16_t colorIndex = *lineT;
        if(scanLine[leftOffset + x]){
            colorIndex = scanLine[leftOffset + x];
            if(!colorIndex) colorIndex = *lineT;
            scanLine[leftOffset + x]=0;
            *lineP++ = colorIndex;
        }else{
            if(*lineT==0 && bg.totalGemsCollected == bg.totalGemsToCollect){
                *lineP++ = pal[doorPalOffset + sprite_door[450+(px+32*py)]];
            }else{
                *lineP++ = gamePalette.rgb[colorIndex];
            }
        }
        *lineT++ = x;
    }
    loadPalette(tempPal, 220); // load first 220 colours into palette, nore more than that needed.

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

