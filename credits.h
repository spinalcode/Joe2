
Main level music
Chiptronical

Non-copyrighted music by Patrick de Arteaga.

Creative Commons Attribution 4.0 license (CC-BY). This means:
You can use this song in any medium freely, even commercially, but you have to add a link to patrickdearteaga.com or to mention the original author (Patrick de Arteaga) there where you use it.

https://patrickdearteaga.com











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
