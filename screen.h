// tile map
#define bgTileSizeH 8
#define bgTileSizeW 8
#define tbt bgTileSizeH*bgTileSizeW

/*
     ______                 __                 _______                              
    |   __ \.-----.-----.--|  |.-----.----.   |     __|.----.----.-----.-----.-----.
    |      <|  -__|     |  _  ||  -__|   _|   |__     ||  __|   _|  -__|  -__|     |
    |___|__||_____|__|__|_____||_____|__|     |_______||____|__| |_____|_____|__|__|

*/

inline void wiggleFill(std::uint8_t* line, std::uint32_t y, bool skip){
/*
    if(skip){
        memset(&line[0],0,220);
        return;
    }
*/
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


void myBGFiller(std::uint8_t* line, std::uint32_t y, bool skip){

    if(skip)return;

    // set bgcolor different for every line
    Pokitto::Display::palette[0] = hline_pal[hline[(y+(bg.mapY/4))]];


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

    #define bgTileLine()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = bg.miniMap[2+tileIndex++]&0x83FF;\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        if(thisTile&32768){\
            for(uint32_t b=0; b<bgTileSizeW; b++){\
                line[x++] = tiles[7+lineOffset--];\
            }\
        }else{\
            for(uint32_t b=0; b<bgTileSizeW; b++){\
                line[x++] = tiles[lineOffset++];\
            }\
        }

    #define bgHalfTileLine()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = bg.miniMap[2+tileIndex++]&0x83FF;\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        if(thisTile&32768){\
            for(uint32_t b=0; b<4; b++){\
                line[x++] = tiles[7+lineOffset--];\
            }\
        }else{\
            for(uint32_t b=0; b<4; b++){\
                line[x++] = tiles[lineOffset++];\
            }\
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

    if(clearScreen==true) memset(&line[0], 10, 220);

}

int mY;
// midmap

void myBGFiller2(std::uint8_t* line, std::uint32_t y, bool skip){

    if(skip)return;

    int mX = bg.mapX/4;
    mY = (bg.mapY-64)/4;

    if(mY+(int)y <=0) return;
//    if(mY+(int)y >=176) return;

    uint32_t stX = (-mX)%bgTileSizeW;
    uint32_t x = stX;
    uint32_t tileIndex = mX/bgTileSizeW + ((y+mY)/bgTileSizeH) *midmap[0];
    uint32_t jStart = ((y+mY) %bgTileSizeH) * bgTileSizeW; // current line in current tile

    uint32_t tileStart;
    uint32_t lineOffset;
    uint32_t lineStart = -stX;
    uint32_t isFlipped = 0;
    uint32_t thisTile;
    uint32_t tileOffset=0;

    #define bgTileLineBG()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = midmap[2+tileIndex++];\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        if(thisTile&32768){\
            for(uint32_t b=0; b<bgTileSizeW; b++){\
                if(line[x]==0){\
                    line[x++] = tiles[7+lineOffset--];\
                }else{\
                    x++; lineOffset--;\
                }\
            }\
        }else{\
            for(uint32_t b=0; b<bgTileSizeW; b++){\
                if(line[x]==0){\
                    line[x++] = tiles[lineOffset++];\
                }else{\
                    x++; lineOffset++;\
                }\
            }\
        }

    #define bgHalfTileLineBG()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = midmap[2+tileIndex++];\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        if(thisTile&32768){\
            for(uint32_t b=0; b<4; b++){\
                if(line[x]==0){\
                    line[x++] = tiles[7+lineOffset--];\
                }else{\
                    x++; lineOffset--;\
                }\
            }\
        }else{\
            for(uint32_t b=0; b<4; b++){\
                if(line[x]==0){\
                    line[x++] = tiles[lineOffset++];\
                }else{\
                    x++; lineOffset++;\
                }\
            }\
        }

    // unrolling this loop got an extra 10fps
    bgTileLineBG(); bgTileLineBG(); bgTileLineBG(); bgTileLineBG();
    bgTileLineBG(); bgTileLineBG(); bgTileLineBG(); bgTileLineBG();
    bgTileLineBG(); bgTileLineBG(); bgTileLineBG(); bgTileLineBG();
    bgTileLineBG(); bgTileLineBG(); bgTileLineBG(); bgTileLineBG();
    bgTileLineBG(); bgTileLineBG(); bgTileLineBG(); bgTileLineBG();
    bgTileLineBG(); bgTileLineBG(); bgTileLineBG(); bgTileLineBG();
    bgTileLineBG(); bgTileLineBG(); bgTileLineBG(); bgTileLineBG();
    bgHalfTileLineBG(); 
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
                line[x] = collisionTile[lineOffset];\
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





void myTitleFiller(std::uint8_t* line, std::uint32_t y, bool skip){

    uint32_t stX = 0;
    uint32_t x = stX;
    uint32_t tileIndex = ((y+bg.windowY)/bgTileSizeH) * 27;
    uint32_t jStart = ((y+bg.windowY) %bgTileSizeH) * bgTileSizeW; // current line in current tile

    uint32_t tileStart;
    uint32_t lineOffset;
    uint32_t lineStart = -stX;
    uint32_t thisTile;
    uint32_t tileOffset=0;

    #define titleTileLine()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = titlemap[tileIndex++]&0x83FF;\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        if(thisTile&32768){\
            for(uint32_t b=0; b<bgTileSizeW; b++){\
                line[x++] = titletiles[7+lineOffset--];\
            }\
        }else{\
            for(uint32_t b=0; b<bgTileSizeW; b++){\
                line[x++] = titletiles[lineOffset++];\
            }\
        }

    #define titleHalfTileLine()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = titlemap[tileIndex++]&0x83FF;\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        if(thisTile&32768){\
            for(uint32_t b=0; b<4; b++){\
                line[x++] = titletiles[7+lineOffset--];\
            }\
        }else{\
            for(uint32_t b=0; b<4; b++){\
                line[x++] = titletiles[lineOffset++];\
            }\
        }


    // unrolling this loop got an extra 10fps
    for(int t=0; t<27; t++){ titleTileLine();}
    titleHalfTileLine(); 

}
