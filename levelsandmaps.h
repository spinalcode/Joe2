void reSaturate(double changeRed, double changeGreen, double changeBlue){
    
    #define  Pr  .299
    #define  Pg  .587
    #define  Pb  .114

    int palSize = sizeof(hline_pal)/2;
    for(int t=0; t<240; t++){ // leave the last 16 colours alone
        gamePalette.rgb[t] = *reinterpret_cast<const uint16_t*>(&levelData[t*2]);

        uint8_t r[256];
        uint8_t g[256];
        uint8_t b[256];
        
        r[t] = (gamePalette.rgb[t] & 0xF800) >> 11;
        g[t] = (gamePalette.rgb[t] & 0x7E0) >> 5;
        b[t] = (gamePalette.rgb[t] & 0x1F);
        //float grey = (0.2126 * gamePalette.r[t]) + (0.7152 * gamePalette.g[t] /2) + (0.0722 * gamePalette.b[t]);
            
        // convert 656 to 888 for easier maths
        double R8 = ( r[t] * 527 + 23 ) >> 6;
        double G8 = ( g[t] * 259 + 33 ) >> 6;
        double B8 = ( b[t] * 527 + 23 ) >> 6;
    
        //  public-domain function by Darel Rex Finley
        double  P=sqrt(
          (R8)*(R8)*Pr+
          (G8)*(G8)*Pg+
          (B8)*(B8)*Pb ) ;
            
        R8=P+((R8)-P)*changeRed;
        G8=P+((G8)-P)*changeGreen;
        B8=P+((B8)-P)*changeBlue;
    
        // convert 888 to 565 for reloading
        uint8_t R5 = (( (int)R8 * 249 + 1014 ) >> 11)&31;
        uint8_t G6 = (( (int)G8 * 253 +  505 ) >> 10)&63;
        uint8_t B5 = (( (int)B8 * 249 + 1014 ) >> 11)&31;
    
        gamePalette.rgb[t] = (R5<<11)|(G6<<5)|B5;

        if(t<=palSize){
            
            gamePalette.hpal[t] = hline_pal[t];

            // hline pal now
            r[t] = (gamePalette.hpal[t] & 0xF800) >> 11;
            g[t] = (gamePalette.hpal[t] & 0x7E0) >> 5;
            b[t] = (gamePalette.hpal[t] & 0x1F);
            //float grey = (0.2126 * gamePalette.r[t]) + (0.7152 * gamePalette.g[t] /2) + (0.0722 * gamePalette.b[t]);
                
            // convert 656 to 888 for easier maths
            R8 = ( r[t] * 527 + 23 ) >> 6;
            G8 = ( g[t] * 259 + 33 ) >> 6;
            B8 = ( b[t] * 527 + 23 ) >> 6;
        
            //  public-domain function by Darel Rex Finley
            P=sqrt(
              (R8)*(R8)*Pr+
              (G8)*(G8)*Pg+
              (B8)*(B8)*Pb ) ;
                
            R8=P+((R8)-P)*changeRed;
            G8=P+((G8)-P)*changeGreen;
            B8=P+((B8)-P)*changeBlue;
        
            // convert 888 to 565 for reloading
            R5 = (( (int)R8 * 249 + 1014 ) >> 11)&31;
            G6 = (( (int)G8 * 253 +  505 ) >> 10)&63;
            B5 = (( (int)B8 * 249 + 1014 ) >> 11)&31;
        
            gamePalette.hpal[t] = (R5<<11)|(G6<<5)|B5;
        }

    }

    // sprite palette
    for(int t=0; t<16; t++){ // colour palette for sprites
        gamePalette.rgb[t+240] = color_sprite_pal[t];
    }
    
    Pokitto::Display::load565Palette(gamePalette.rgb);

}


void updateMap(int mx, int my){
    lastLoad=0;
    mapPos = mx;
    
    uint32_t t=0;
    bg.miniMap[t++] = 28*2;
    bg.miniMap[t++] = 22*2;
    uint32_t largeMapWidth = bg.mapWidth*2; // tiles * 2bytes
    uint32_t miniMapWidth = bg.miniMap[0]*2;

    File file;
    if(file.openRO(levelFilename)){
        for(int y=0; y<bg.miniMap[1]; y++){
            file.seek(4+(mx*2)+largeMapWidth*(my+y));
            file.read(&bg.miniMap[t], miniMapWidth);
            t+=bg.miniMap[0];
            lastLoad += miniMapWidth;
        }
    }
    file.close();
}

void initMap(){
    File file1;
    if(file1.openRO(levelFilename)){
        file1.read(&bg.mapWidth, 2);
        file1.read(&bg.mapHeight, 2);
    }
    //updateMap(bg.mapX>>3,bg.mapY>>3);
    file1.close();
}

void loadLevel(int levNum){

    bg.satRed=0;
    bg.satGreen=0;
    bg.satBlue=0;

    srand(Pokitto::Core::getTime());

    bg.numRed = 0;
    bg.numGreen;
    bg.numBlue;
    bg.totalGemsCollected = 0;

    int paletteSize = 512;

    sprintf(levelFilename,"joe2/0%d.bin",levNum);
    sprintf(levelTilename,"joe2/0%ddata.bin",levNum);

    initMap();


    // hotswap the level data    
    levelData = LevelData::load(levelTilename); 
    uint16_t scenerySize = (*reinterpret_cast<const uint16_t*>(&levelData[paletteSize]))*2;
    midmap = reinterpret_cast<const uint16_t*>(levelData + paletteSize +2);
    tiles = &levelData[paletteSize + scenerySize + 8];
    uint16_t tilesOffset = *reinterpret_cast<const uint16_t*>(&levelData[paletteSize + scenerySize +6]);
    collisionTile = &levelData[paletteSize + scenerySize + 8 + tilesOffset];
    reSaturate(0,0,0); // load palette at 100% saturation
    // load 16 colour sprite palette into 240+
    for(int t=0; t<16; t++){
        Pokitto::Display::palette[t+240] = color_sprite_pal[t];
    }

    uint16_t mapSize[2];
    File levfile;
    if(levfile.openRO(levelFilename))
        levfile.read(mapSize, sizeof(mapSize));
    
    // check collision map for collectable tilesOffset
    mapWidth = mapSize[0];
    mapHeight = mapSize[1];

    player.x = 0;
    player.y = 0;
    numGems = 0;
    maxItems=0;
    maxEnemies=0;
    
    bg.numRed=0;
    bg.numBlue=0;
    bg.numGreen=0;
    
    int x=0;
    int y=0;
    //int c=0;
    //tileType[0]=0;

    player.startX = 0;
    player.startY = 0;
    bool doorLoaded = false;

    for(int t=0; t<(mapWidth*mapHeight); t++){
        uint16_t curTile; // each tile is represented by 16bit, with the collision info being here -> 0111110000000000
        levfile.read(&curTile, sizeof(curTile));
        if(x==mapWidth){x=0; y++;}

        if(x<20 && y==0){
            tileType[x]=((curTile >> 10)&31);
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
                    items[maxItems].mapPos = t-2;
                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = y*8;
                    items[maxItems].collected = 0;
                    items[maxItems].frame = random(sizeof(gemFrame)*items[maxItems].speed);
                    items[maxItems].type = ((curTile >> 10)&31)-3; // gem
                    maxItems++;
            }
            if(tl == tileType[5]){ // 5 = green gem
                    bg.numGreen++;
                    items[maxItems].mapPos = t-2;
                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = y*8;
                    items[maxItems].collected = 0;
                    items[maxItems].frame = random(sizeof(gemFrame)*items[maxItems].speed);
                    items[maxItems].type = ((curTile >> 10)&31)-3; // gem
                    maxItems++;
            }
            if(tl == tileType[6]){ // 6 = blue gem
                    bg.numBlue++;
                    items[maxItems].mapPos = t;
                    items[maxItems].x = (x-1)*8;
                    items[maxItems].y = y*8;
                    items[maxItems].collected = 0;
                    items[maxItems].frame = random(sizeof(gemFrame)*items[maxItems].speed);
                    items[maxItems].type = ((curTile >> 10)&31)-3; // gem
                    maxItems++;
            }
            
            if(tl == tileType[7]){ // 7 = Keith - the first enemy
                    enemy[maxEnemies].x = (x-1)*8;
                    enemy[maxEnemies].y = (y*8)+2;
                    enemy[maxEnemies].offy = 0;
                    enemy[maxEnemies].type = 1; // 0 = dead?
                    enemy[maxEnemies].numFrames = 4;
                    enemy[maxEnemies].speed = 6;
                    maxEnemies++;
            }
            if(tl == tileType[8]){ // 8 = Tom - the second enemy
                    enemy[maxEnemies].x = (x-1)*8;
                    enemy[maxEnemies].y = (y*8)+2;
                    enemy[maxEnemies].offy = 0;
                    enemy[maxEnemies].type = 2; // 0 = dead?
                    enemy[maxEnemies].numFrames = 4;
                    enemy[maxEnemies].speed = 6;
                    maxEnemies++;
            }
            if(tl == tileType[9]){ // 9 = Bird - the third enemy
                    enemy[maxEnemies].x = (x-1)*8;
                    enemy[maxEnemies].y = (y*8)+2;
                    enemy[maxEnemies].offy = -24;
                    enemy[maxEnemies].type = 3; // 0 = dead?
                    enemy[maxEnemies].numFrames = 4;
                    enemy[maxEnemies].speed = 8;
                    maxEnemies++;
            }
            if(tl == tileType[11]){ // 11 = Level Complete Door
                if(doorLoaded==false){
                    doorLoaded = true;
                    exitDoor.x = x*8;
                    exitDoor.y = y*8;
                    exitDoor.frame = 0;
                    exitDoor.visible = true;
//                    printf("Door at %d,%d\n",exitDoor.x,exitDoor.y);
                }
            }

        }// y>0
        x++;
    }
    levfile.close();

    bg.redPercent = 1.0/bg.numRed;
    bg.greenPercent = 1.0/bg.numGreen;
    bg.bluePercent = 1.0/bg.numBlue;
    bg.countRed=0;
    bg.countGreen=0;
    bg.countBlue=0;

    bg.totalGemsToCollect = bg.numRed+bg.numGreen+bg.numBlue;


    player.x = player.startX;
    player.y = player.startY;

    // update the screen before actually playing the level or else!!!
    bg.mapX = (player.x>>8)-110;
    bg.mapY = (player.y>>8)-88;
    if(bg.mapX<0) bg.mapX=0;
    if(bg.mapX>(bg.mapWidth*bgTileSizeW)-220) bg.mapX=(bg.mapWidth*bgTileSizeW)-220;
    if(bg.mapY<0) bg.mapY=0;
    if(bg.mapY>(bg.mapHeight*bgTileSizeH)-176) bg.mapY=(bg.mapHeight*bgTileSizeH)-176;
    oldScreenX = screenX;
    oldScreenY = screenY;
    screenX = bg.mapX/224;
    screenY = bg.mapY/176;
    int mapPosX = screenX * 224;
    int mapPosY = screenY * 176;
    updateMap( mapPosX/8 , mapPosY/8);
    bg.windowX = bg.mapX%224;
	bg.windowY = bg.mapY%176;

    SOLID = collisionTile[tileType[1]*64]; // ?

    //exitDoor.x = 0;
    //exitDoor.y = 80;

    bg.oldMapX = player.x-28160;
    bg.oldMapY = player.y-22528;

}
