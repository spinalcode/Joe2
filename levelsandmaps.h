void reSatPal(double percent, const uint16_t* palIn, uint16_t*palOut, int size){
    
    #define  Pr  .299
    #define  Pg  .587
    #define  Pb  .114

    for(int t=0; t<size; t++){ // leave the last 16 colours alone

        uint8_t r,g,b;

        r = (palIn[t] & 0xF800) >> 11;
        g = (palIn[t] & 0x7E0) >> 5;
        b = (palIn[t] & 0x1F);

        // convert 656 to 888 for easier maths
        double R8 = ( r * 527 + 23 ) >> 6;
        double G8 = ( g * 259 + 33 ) >> 6;
        double B8 = ( b * 527 + 23 ) >> 6;
    
        //  public-domain function by Darel Rex Finley
        double  P=sqrt(
          (R8)*(R8)*Pr+
          (G8)*(G8)*Pg+
          (B8)*(B8)*Pb ) ;
            
        R8=P+((R8)-P)*percent;
        G8=P+((G8)-P)*percent;
        B8=P+((B8)-P)*percent;
    
        // convert 888 to 565 for reloading
        uint8_t R5 = (( (int)R8 * 249 + 1014 ) >> 11)&31;
        uint8_t G6 = (( (int)G8 * 253 +  505 ) >> 10)&63;
        uint8_t B5 = (( (int)B8 * 249 + 1014 ) >> 11)&31;
    
        palOut[t] = (R5<<11)|(G6<<5)|B5;
    }
}


void reSaturate(double changeRed, double changeGreen, double changeBlue){
    reSatPal(changeRed, (uint16_t*)levelData, gamePalette.rgb, 255); // player sprite palette
    reSatPal(changeRed, player_sprite_pal, playerSpritePal, 16); // player sprite palette
    reSatPal(changeRed, hline_pal, gamePalette.hpal, (sizeof(hline_pal)/2)); // horizon effect palette
//    Pokitto::Display::load565Palette(gamePalette.rgb);
}


void updateMap(int mx, int my, int levNum, int map){

    if(map == LEVMAIN){
        
        auto minMap = &bg.miniMap[0];
        uint32_t t=0;
        // The map in RAM is 2x2 physical screens
        bg.miniMap[t++] = 28*2;
//        *minMap++ = 28*2;
        bg.miniMap[t++] = 22*2;
//        *minMap++ = 22*2;

        char levelFilename[32]; // doesn't need to be this long...
        sprintf(levelFilename,"joe2/%02d.bin",levNum);

        File file;
        if(file.openRO(levelFilename)){
            uint16_t mWidth;
            file.read(&mWidth, 2);
            for(int y=0; y<bg.miniMap[1]; y++){
                file.seek(4+(mx*2)+(mWidth*2)*(my+y));
                file.read(&bg.miniMap[t], bg.miniMap[0]*2);
                //file.read(minMap++, bg.miniMap[0]*2);
                //*minMap++;
                t+=bg.miniMap[0];
            }
        }
    }
/*
    if(map == LEVMID){

        uint32_t t=0;
        char levelFilename[32];
        sprintf(levelFilename,"joe2/%02dMid.bin",levNum);

        bg.midminiMap[t++] = 28*2;
        bg.midminiMap[t++] = 22*2;
        
        //uint32_t miniMapWidth = bg.miniMidMap[0];
        //printf("miniMidMap:%d\n",bg.miniMidMap[0]);


        File file;
        if(file.openRO(levelFilename)){
            uint16_t mWidth;
            file.read(&mWidth, 2);

            for(int y=0; y<bg.midminiMap[1]; y++){
                file.seek(4+(mx*2)+(mWidth*2)*(my+y));
                file.read(&bg.midminiMap[t], bg.midminiMap[0]*2);
                t+=bg.midminiMap[0];
                //lastLoad += miniMapWidth;
            }
        }
        file.close();
    }
*/    
    
    
}



void initMap(int levNum){
    char levelFilename[64];
    sprintf(levelFilename,"joe2/%02d.bin",levNum);
    File file1;
    if(file1.openRO(levelFilename)){
        file1.read(&bg.mapWidth, 2);
        printf("mapWidth:%d\n",bg.mapWidth);
        file1.read(&bg.mapHeight, 2);
        printf("mapHeight:%d\n",bg.mapHeight);
    }
    //file1.close();

    sprintf(levelFilename,"joe2/%02dmid.bin",levNum);
    if(file1.openRO(levelFilename)){
        file1.read(&bg.midmapWidth, 2);
        file1.read(&bg.midmapHeight, 2);
    }
    //file1.close();
}

void loadHotSwapData(const char*levelTilename){
    int paletteSize = 512;

    // hotswap the level data
    levelData = LevelData::load(levelTilename); 
    //midmap = reinterpret_cast<const uint16_t*>(&bg.midminiMap[0]);
    tiles = &levelData[paletteSize + 2];
    uint16_t tilesOffset = *reinterpret_cast<const uint16_t*>(&levelData[paletteSize]);
    collisionTile = &levelData[paletteSize + 2 + tilesOffset];
    reSaturate(0,0,0); // load palette at 100% saturation
    // load 16 colour sprite palette into 240+
    for(int t=0; t<16; t++){
        Pokitto::Display::palette[t+240] = color_sprite_pal[t];
    }
}

void resetLevelData(){

    player.x = 0;
    player.y = 0;
    numGems = 0;
    maxItems=0;

    bg.numRed=0;
    bg.numBlue=0;
    bg.numGreen=0;

    player.startX = 0;
    player.startY = 0;

    numDoors=0;
    for(int t=0; t<sizeof(exitDoor)/sizeof(exitDoor[0]); t++){
        exitDoor[t].visible = false;
    }

    bg.countRed=0;
    bg.countGreen=0;
    bg.countBlue=0;
    
    wordCollected[0]=0;
    wordCollected[1]=0;
    wordCollected[2]=0;
    atDoor = 99;
    
}




void loadLevel(int levNum){

    resetLevelData();

    levelNumber=levNum;
    srand(Pokitto::Core::getTime());

    bg.numRed = 0;
    bg.numGreen;
    bg.numBlue;
    bg.totalGemsCollected = 0;

    initMap(levNum);

    // multiplyer for bacground scenery offset
    bg.multiplyX = (bg.midmapWidth<<8) / (bg.mapWidth+(bg.midmapWidth*2));
    bg.multiplyY = (bg.midmapHeight<<8) / (bg.mapHeight+(bg.midmapHeight*2));

    char levelTilename[32];
    sprintf(levelTilename,"joe2/%02dData.bin",levNum);

    loadHotSwapData(levelTilename);


    uint16_t mapSize[2];
    char levelFilename[64];
    sprintf(levelFilename,"joe2/%02d.bin",levNum);
    File file1;
    if(file1.openRO(levelFilename))
        file1.read(mapSize, sizeof(mapSize));
    
    // check collision map for collectable tilesOffset
    mapWidth = mapSize[0];
    mapHeight = mapSize[1];

    int lettersFound=0; // we count the letter sprite items as they are loaded, it give the appearance of random placement

    int x=0;
    int y=0;
    for(int t=0; t<(mapWidth*mapHeight); t++){
        uint16_t curTile; // each tile is represented by 16bit, with the collision info being here -> 0111110000000000
        file1.read(&curTile, sizeof(curTile));
        if(x==mapWidth){x=0; y++;}

        if(x<20 && y==0){
            tileType[x]=((curTile >> 10)&31); // some tiles have 'types' as they are objects to be collected
        }

        if(y>1){ // skip first line as it has control blocks for numbering

            // as the collision tile is 0111110000000000, then we must >>10 &31 to get the tile number
            int tl = ((curTile >> 10)&31);
            if(tl == tileType[3]){ // 3 = start position
                player.startX = (x*8)<<8;
                player.startY = (y*8)<<8;
                player.frame=0;
            }
            if(tl == tileType[4]){ // 4 = red gem
                    bg.numRed++;
                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = y*8;
                    items[maxItems].collected = 0;
                    items[maxItems].speed = GEM_ANIM_SPEED;
                    items[maxItems].frame = 0;
                    items[maxItems].startFrame = 0;
                    items[maxItems].maxFrame = 16;
                    items[maxItems].type = ((curTile >> 10)&31)-3; // gem
                    items[maxItems].imageData = gem[0];
                    items[maxItems].paletteData = &gem_pal[0];
                    items[maxItems].bitDepth = 4;
                    items[maxItems].frameSize = (items[maxItems].imageData[0]*items[maxItems].imageData[1])/(8/items[maxItems].bitDepth);
                    maxItems++;
            }
            if(tl == tileType[5]){ // 5 = green gem
                    bg.numGreen++;
                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = y*8;
                    items[maxItems].collected = 0;
                    items[maxItems].speed = GEM_ANIM_SPEED;
                    items[maxItems].frame = random(15*items[maxItems].speed);
                    items[maxItems].startFrame = 0;
                    items[maxItems].maxFrame = 15;
                    items[maxItems].type = ((curTile >> 10)&31)-3; // gem
                    items[maxItems].imageData = gem[0];
                    items[maxItems].paletteData = &gem_pal[6];
                    items[maxItems].bitDepth = 4;
                    items[maxItems].frameSize = (items[maxItems].imageData[0]*items[maxItems].imageData[1])/(8/items[maxItems].bitDepth);
                    maxItems++;
            }
            if(tl == tileType[6]){ // 6 = blue gem
                    bg.numBlue++;
                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = y*8;
                    items[maxItems].collected = 0;
                    items[maxItems].speed = GEM_ANIM_SPEED;
                    items[maxItems].frame = random(15*items[maxItems].speed);
                    items[maxItems].startFrame = 0;
                    items[maxItems].maxFrame = 15;
                    items[maxItems].type = ((curTile >> 10)&31)-3; // gem
                    items[maxItems].imageData = gem[0];
                    items[maxItems].paletteData = &gem_pal[12];
                    items[maxItems].bitDepth = 4;
                    items[maxItems].frameSize = (items[maxItems].imageData[0]*items[maxItems].imageData[1])/(8/items[maxItems].bitDepth);
                    maxItems++;
            }
            
            if(tl == tileType[7]){ // 7 = Keith - the first enemy

                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = (y*8)+2;
                    items[maxItems].type = 4; // enemy 1
                    items[maxItems].collected = 0;
                    items[maxItems].speed = 4;
                    items[maxItems].frame = 0;
                    items[maxItems].startFrame = 0;
                    items[maxItems].maxFrame = 4;
                    items[maxItems].imageData = enemy1[0];
                    items[maxItems].paletteData = gamePalette.rgb;
                    items[maxItems].bitDepth = 8;
                    items[maxItems].frameSize = (items[maxItems].imageData[0]*items[maxItems].imageData[1])/(8/items[maxItems].bitDepth);
                    maxItems++;
            }
            if(tl == tileType[8]){ // 8 = Tom - the second enemy

                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = (y*8)+2;
                    items[maxItems].type = 5; // enemy 2
                    items[maxItems].collected = 0;
                    items[maxItems].frame = 0;
                    items[maxItems].speed = 8;
                    items[maxItems].startFrame = 0;
                    items[maxItems].maxFrame = sizeof(enemy2)/sizeof(enemy2[0]);
                    items[maxItems].imageData = enemy2[0];
                    items[maxItems].paletteData = gamePalette.rgb;
                    items[maxItems].bitDepth = 8;
                    items[maxItems].frameSize = (items[maxItems].imageData[0]*items[maxItems].imageData[1])/(8/items[maxItems].bitDepth);
                    maxItems++;

            }
        
            if(tl == tileType[9]){ // 9 = Bird - the third enemy
                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = (y*8)+2;
                    items[maxItems].type = 5; // enemy 2
                    items[maxItems].collected = 0;
                    items[maxItems].offy = 32;
                    items[maxItems].frame = 0;
                    items[maxItems].speed = 8;
                    items[maxItems].startFrame = 0;
                    items[maxItems].maxFrame = sizeof(enemy2)/sizeof(enemy2[0]);
                    items[maxItems].imageData = enemy3[0];
                    items[maxItems].paletteData = gamePalette.rgb;
                    items[maxItems].bitDepth = 8;
                    items[maxItems].frameSize = (items[maxItems].imageData[0]*items[maxItems].imageData[1])/(8/items[maxItems].bitDepth);
                    maxItems++;
            }
            if(tl == tileType[11]){ // 11 = Level Complete Door
                exitDoor[numDoors].x = x*8;
                exitDoor[numDoors].y = y*8;
                exitDoor[numDoors].frame = 0;
                exitDoor[numDoors].visible = true;
                printf("Doors %d:%d,%d\n", numDoors,exitDoor[numDoors].x,exitDoor[numDoors].y);
                numDoors++;
            }
            if(tl == tileType[12]){ // 12 = bonus gem
                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = y*8;
                    items[maxItems].collected = 0;
                    items[maxItems].speed = GEM_ANIM_SPEED;
                    items[maxItems].frame = random(15*items[maxItems].speed);
                    items[maxItems].startFrame = 0;
                    items[maxItems].maxFrame = 15;
                    items[maxItems].type = ((curTile >> 10)&31)-3; // gem
                    items[maxItems].imageData = color_gem2[0];
                    items[maxItems].paletteData = color_gem_pal;
                    items[maxItems].bitDepth = 8;
                    items[maxItems].frameSize = (items[maxItems].imageData[0]*items[maxItems].imageData[1])/(8/items[maxItems].bitDepth);
                    maxItems++;
            }

            if(tl == tileType[13]){ // 13 = Letter J,O,E
                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = y*8;
                    items[maxItems].collected = 0;
                    items[maxItems].frame = 0;//random(8);
                    items[maxItems].speed = GEM_ANIM_SPEED*3;
                    items[maxItems].bitDepth = 4;
                    items[maxItems].type = (((curTile >> 10)&31)-3) + lettersFound;
                    items[maxItems].imageData = big_letter[lettersFound*8];
                    items[maxItems].paletteData = big_letter_pal;
                    items[maxItems].frameSize = (items[maxItems].imageData[0]*items[maxItems].imageData[1])/(8/items[maxItems].bitDepth);
                    items[maxItems].startFrame = 0;
                    items[maxItems].maxFrame = 8;
                    maxItems++;
                    lettersFound++;
            }

        }// y>0
        x++;
    }
    //file1.close();

   bg.totalGemsToCollect = bg.numRed+bg.numGreen+bg.numBlue;

    player.x = player.startX;
    player.y = player.startY;

    // update the screen before actually playing the level or else!!!
    bg.mapX = (player.x>>8)-110;
    bg.mapY = (player.y>>8)-88;
    if(bg.mapX<0) bg.mapX=0;
    if(bg.mapX>(bg.mapWidth*BG_TILE_SIZE_W)-220) bg.mapX=(bg.mapWidth*BG_TILE_SIZE_W)-220;
    if(bg.mapY<0) bg.mapY=0;
    if(bg.mapY>(bg.mapHeight*BG_TILE_SIZE_H)-176) bg.mapY=(bg.mapHeight*BG_TILE_SIZE_H)-176;
    bg.oldScreenX = bg.screenX;
    bg.oldScreenY = bg.screenY;
    bg.screenX = bg.mapX/224;
    bg.screenY = bg.mapY/176;
    int mapPosX = bg.screenX * 224;
    int mapPosY = bg.screenY * 176;
    // update the main level map
    updateMap( mapPosX/8 , mapPosY/8, levelNumber, LEVMAIN);

    bg.windowX = bg.mapX%224;
	bg.windowY = bg.mapY%176;
    
    // update the background scenery map
    updateMap( mapPosX/8 , mapPosY/8, levelNumber, LEVMID);

    bg.oldMapX = player.x-28160;
    bg.oldMapY = player.y-22528;

}