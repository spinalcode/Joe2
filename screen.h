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
static inline void setup_data_16(uint16_t data)
{
    SET_MASK_P2;
    LPC_GPIO_PORT->MPIN[2] = (data<<3); // write bits to port
    CLR_MASK_P2;
}

inline void write_command_16(uint16_t data)
{
   CLR_CS; // select lcd
   CLR_CD; // clear CD = command
   SET_RD; // RD high, do not read
   setup_data_16(data); // function that inputs the data into the relevant bus lines
   CLR_WR_SLOW;  // WR low
   SET_WR;  // WR low, then high = write strobe
   SET_CS; // de-select lcd
}

/**************************************************************************/
/*!
    @brief  Write data to the lcd, 16-bit bus
*/
/**************************************************************************/
inline void write_data_16(uint16_t data)
{
   CLR_CS;
   SET_CD;
   SET_RD;
   setup_data_16(data);
   CLR_WR;
   SET_WR;
   SET_CS;
}


void directPixel(int16_t x, int16_t y, uint16_t* color){
	
	write_command(0x20);  // Horizontal DRAM Address
    write_data(y);  // 0
    write_command(0x21);  // Vertical DRAM Address
    write_data(x);
    write_command(0x22); // write data to DRAM
    CLR_CS_SET_CD_RD_WR;
    setup_data_16(0xFFFF);
    CLR_WR;SET_WR;
    
}


void doorFill(std::uint8_t* line, std::uint32_t y, bool skip){
    if(skip){
        return;
    }

    int offset = exitDoor.y-bg.mapY+(47-y);
    //int s=0;
    uint16_t tempPal[256];
    for(int t=0; t<220; t++){
        //tempPal[t] = gamePalette.rgb[line[t]];
        //Pokitto::Display::palette[t] = gamePalette.rgb[line[t]];
        //line[t]=t;
        auto colorIndex = line[t];
        Pokitto::Display::palette[t] = gamePalette.rgb[colorIndex];
        line[t]=t;
    }

    int s=0;
    if(exitDoor.x > bg.mapX && exitDoor.x < bg.mapX+220){
    	if(bg.mapY+y>=exitDoor.y && bg.mapY+y<exitDoor.y+48){
            int offset = exitDoor.y-bg.mapY+(47-y);
    	    // add the 16bit sprite
            int currentPix = (exitDoor.x-bg.mapX)+s;
            if(currentPix<220 && currentPix >=0){
                Pokitto::Display::palette[currentPix] = exitDoor.tempDoorSprite[s+(offset*32)];
                s++;
            }
        }
    }


    return;
}



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
    //Pokitto::Display::palette[0] = hline_pal[hline[(y+(bg.mapY/4))]];
    Pokitto::Display::palette[0] = bgline_pal[hline[y]];

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

    if(clearScreen==true) memset(&line[0], 10, 220);

}

void myBGFiller2(std::uint8_t* line, std::uint32_t y, bool skip){

    if(skip)return;

    int mX = bg.mapX/4;
    int mY = (bg.mapY-64)/4;

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

//    if(exitDoor.x > bg.mapX && exitDoor.x < bg.mapX+220){
//    	if(bg.mapY+y>=exitDoor.y && bg.mapY+y<exitDoor.y+48){
            // current line in door sprite
/*
            int offset = exitDoor.y-bg.mapY+(47-y);
            //int s=0;
    	    uint16_t tempPal[256];
    	    for(int t=0; t<220; t++){
    	        //tempPal[t] = gamePalette.rgb[line[t]];
    	        Pokitto::Display::palette[t] = gamePalette.rgb[line[t]];
    	        line[t]=t;
    	    }
*/
    	    /*
    	    // add the 16bit sprite
            int currentPix = (exitDoor.x-bg.mapX)+s;
            if(currentPix<220 && currentPix >=0){
                gamePalette.rgb[currentPix] = exitDoor.tempDoorSprite[s+(offset*32)];
                s++;
            }
            */
            //Pokitto::Display::load565Palette(tempPal); // gamePalette.rgb


    	        /*
    	        if(tempPal[t]==0 && s<32){
    	            int currentPix = (exitDoor.x-bg.mapX)+s;
    	            if(currentPix<220 && currentPix >=0){
    	                line[currentPix] = t;
    	                gamePalette.rgb[t] = exitDoor.tempDoorSprite[s+(offset*32)];
    	                s++;
    	            }
    	            //tempPal[t] = exitDoor.tempDoorSprite[s+(offset*32)];    
    	        }
    	        */

/*    	    
        	write_command(0x20);  // Horizontal DRAM Address
            write_data(y-1);  // current line
            write_command(0x21);  // Vertical DRAM Address
            //write_data(0); // x position
            write_data(exitDoor.x-bg.mapX); // x position
            write_command(0x22); // write data to DRAM
            CLR_CS_SET_CD_RD_WR;
            int offset = exitDoor.y-bg.mapY+(47-(y-1));
            for(int t=0; t<32; t++){
                setup_data_16(exitDoor.tempDoorSprite[t+(offset*32)]);
                //setup_data_16(random(0xffff)); // tempDoorSprite[t]
                CLR_WR;SET_WR;
            }
            // return screen coordinates back to normal
        	write_command(0x20);  // Horizontal DRAM Address
            write_data(y);  // current line
            write_command(0x21);  // Vertical DRAM Address
            write_data(0); // x position
            write_command(0x22); // write data to DRAM
            CLR_CS_SET_CD_RD_WR;
*/

//    	}
//    }

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

