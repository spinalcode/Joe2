//#define INTERLACE_SCREEN
/*
     ______                 __                 _______                              
    |   __ \.-----.-----.--|  |.-----.----.   |     __|.----.----.-----.-----.-----.
    |      <|  -__|     |  _  ||  -__|   _|   |__     ||  __|   _|  -__|  -__|     |
    |___|__||_____|__|__|_____||_____|__|     |_______||____|__| |_____|_____|__|__|

*/

void myBGFiller(std::uint8_t* line, std::uint32_t y, bool skip){

    if(skip)return;

    // set bgcolor different for every line
    Pokitto::Display::palette[0] = hline_pal[hline[(y+(bg.mapY/4))]];

    //Pokitto::Display::palette[138] = 0x3CEF;
    //Pokitto::Display::palette[174] = 0xFFFF;

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
}

int mY;
// second layer test

void myBGFiller2(std::uint8_t* line, std::uint32_t y, bool skip){

    if(skip)return;

    int mX = bg.mapX/4;
    mY = (bg.mapY-64)/4;

    if(mY+(int)y <=0) return;
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

/*
// render the collision map
void myBGFiller3(std::uint8_t* line, std::uint32_t y, bool skip){

    if(skip)return;
    int lineColour = 138;

    if((y + bg.mapY) %176 == 0){
        memset(&line[0], lineColour ,220);
        return;
    }


    uint32_t stX = (-bg.windowX)%bgTileSizeW;
    uint32_t x = stX;
       // player.colTile = (bg.miniMap[2+ px2 + bg.miniMap[0] * py2]>>10)&31;

    uint32_t tileIndex = bg.windowX/bgTileSizeW + ((y+bg.windowY)/bgTileSizeH) * bg.miniMap[0];
    uint32_t jStart = ((y+bg.windowY) %bgTileSizeH) * bgTileSizeW; // current line in current tile

    uint32_t tileStart;
    uint32_t lineOffset;
    uint32_t lineStart = -stX;
    uint32_t isFlipped = 0;
    uint32_t thisTile;
    uint32_t tileOffset=0;
    int xLine = 220-(bg.mapX%220);

    #define bgTileLine()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = (bg.miniMap[2+tileIndex++]>>10)&31;\
        isFlipped = thisTile>>15;\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        for(uint32_t b=0; b<bgTileSizeW; b++){\
            if(x==xLine){\
                line[x++]=lineColour; lineOffset++;\
            }else{\
                if(collisionTile[lineOffset])\
                    line[x] = collisionTile[lineOffset];\
                lineOffset++; x++;\
            }\
        }

    #define bgHalfTileLine()\
        if(x<0){lineOffset+=lineStart; x=0;}\
        thisTile = (bg.miniMap[2+tileIndex++]>>10)&31;\
        isFlipped = thisTile>>15;\
        tileStart = (thisTile&32767)*tbt;\
        lineOffset = tileStart + jStart;\
        for(uint32_t b=0; b<4; b++){\
            if(collisionTile[lineOffset])\
                line[x] = collisionTile[lineOffset];\
            lineOffset++; x++;\
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
*/

void write_command_(uint16_t data)
{
#ifndef POK_SIM
    CLR_CS; // select lcd
    CLR_CD; // clear CD = command
    SET_RD; // RD high, do not read
    SET_MASK_P2;
    LPC_GPIO_PORT->MPIN[2] = (data<<3); // write bits to port
    CLR_MASK_P2;
    CLR_WR_SLOW;  // WR low
    SET_WR;  // WR low, then high = write strobe
    SET_CS; // de-select lcd
#endif
}

/**************************************************************************/
/*!
  @brief  Write data to the lcd, 16-bit bus
*/
/**************************************************************************/
void write_data_(uint16_t data)
{
#ifndef POK_SIM
    CLR_CS;
    SET_CD;
    SET_RD;
    SET_MASK_P2;
    LPC_GPIO_PORT->MPIN[2] = (data<<3); // write bits to port
    CLR_MASK_P2;
    CLR_WR;
    SET_WR;
    SET_CS;
#endif
}

void lcdRefreshTASMode(const uint16_t* palette){
    int screenWidth = 220;
    int screenHeight = 176;

    // reset the screen to 0,0 each frame...
    #ifndef POK_SIM
        write_command_(0x20);  // Horizontal DRAM Address
        write_data_(0);
        write_command_(0x22); // write data to DRAM
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
            if(frameCount&1){
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
                write_command_(0x20);  // Horizontal DRAM Address
                write_data_(y+1);
                write_command_(0x21);  // Vertical DRAM Address
                write_data_(0);  // 0
                write_command_(0x22); // write data to DRAM
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
