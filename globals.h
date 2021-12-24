#define maxLives 10

#define LEVMAIN 0 // main level map
#define LEVMID 1 // background scenery map

// pointers to our data
const uint8_t* levelData = nullptr;
const uint16_t* midmap = nullptr;
const uint8_t* tiles = nullptr;
const uint8_t* collisionTile = nullptr;

bool interlaceScreen=false;
bool frameJump = false;

// Make a hotswap area for level data
// size = 64*1024k, unique id = 0
using LevelData = Hotswap<64*1024, 0>; // multiple of 32kb = Palette and level tiles - should really be 96KB

/*
int logline = 0;
const char*logFile = "joe2/log.txt";
void updateLog(const char*text){
    //return;
    File txtfile;
    int txtsize = strlen(text);
    if(txtfile.openRW(logFile,0,1)){
        txtfile.write(text, strlen(text));
        //txtfile.write("\r");
    }else{
        // if file didn;t op, assume it doesnt exist and create it
        txtfile.openRW(logFile,1,0);
        txtfile.write(text, strlen(text));
        //txtfile.write("\r");
    }
    txtfile.close();
}
*/

//int lastLoad=0;
//int mapPos;

//#define PRESCAN 88 // left side of scanline that is off screen
//uint16_t scanLine[396]; // there's an issue with screen garbage for some reason, so the buffer is WAY larger than it needs to be
//uint8_t myLine[396]; // there's an issue with screen garbage for some reason, so the buffer is WAY larger than it needs to be

int myVolume = 5;

#define MAXSTEP 64
#define MAXSPEED 512
#define PLAYER_SPEED 512
#define MAXANIMS 12
#define GEM_ANIM_SPEED 3

uint8_t cursorFrame=0;
bool renderMenuLayer = false;

uint8_t frameSkip;
int fpsCount=0;
long int fpsCounter;

bool mustDrawTitleScreen=true;
uint16_t pal[512]; // plasma palette for door
uint8_t doorPalOffset=0;

uint8_t titleLine[176];
const uint8_t titleRoll[]={0,1,3,4,5,6,7,8,9,10,10,11,11,12,12,12,12,13,13,13,13,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,17,17,17,17,18,18,18,18,19,19,20,21,22,23,24,25,26};
long int titleTimer = 0;
uint8_t titleScratch=0;
long int timerCounter=0;
uint16_t playerSpritePal[16];

// Death Animation thingy
uint8_t playerDying = 0;
uint8_t playerDeathFrame=0;
const uint8_t playerDeathFrames[] = {9,10,11,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13};
const uint8_t playerDeathHatFrame[]={6,14,6,6,6};
const uint8_t playerDeathHatX[]={1,0,0,0,0};
const uint8_t playerDeathHatY[]={4,4,4,4,4};
const uint8_t playerDeathHatFlip[]={0,0,1,1,1};
#define invincibleFrames  100 // how many frames to remain invincible
uint8_t invincibleCount;


bool gamePaused = false;


// for my own sprite renderer
#define NUMSPRITES 64
struct SPRITE_DATA {
    const uint16_t *paletteData; // palette data
    const uint8_t *imageData; // image data
    int16_t x;  // x postition
    int16_t y;  // y position
    bool hFlip;
    int16_t offset; // tile render pixel offset
    uint8_t bit;
} sprites[NUMSPRITES];
int spriteCount = -1;
uint8_t spriteLine[176];

struct ANIMATION_DATA {
    const uint16_t *paletteData; // palette data
    const uint8_t *imageData; // image data
    int x;  // x postition
    int y;  // y position
    int startX;
    int startY;
    int endX;
    int endY;
    uint8_t bitDepth;
    uint8_t frame;
    uint8_t speed;
    uint8_t maxFrame;
    uint8_t startFrame;
    int frameSize;
    int type;
    bool used=false;
    int frameCount=0;
} animSprite[MAXANIMS];

#define MAXDOORS 6
struct DOOR_DATA {
    int x;
    int y;
    bool visible;
    uint8_t speed = 6;
    uint8_t loadDoorCounter=0;
    uint8_t frame = 0;
} exitDoor[MAXDOORS];
uint8_t numDoors = 0;
uint8_t atDoor = 99;

uint8_t levNum=0;
uint8_t tileType[32]; // to be read from the first row in the collision map
#define NOTHING 0
#define SOLID  1
#define JUMPTHROUGH 2
#define DEATHCOLOUR 10
#define DOORCOLOUR 11

const uint8_t satRamp[]={0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,3,4,4,4,5,5,6,6,6,7,7,8,8,9,9,10,11,11,12,13,14,15,15,16,17,18,19,20,21,22,23,25,26,27,28,30,31,32,34,35,37,39,40,42,44,45,47,48,51,53,54,57,58,60,62,64,67,68,71,72,75,76,79,81,82,85,87,90,92,93,95,96,98,100};

struct BACKGROUND_DATA {
    uint16_t miniMap[2+56*44]; // the window on the whole map
    int windowX; // position within the map window
    int windowY; // position within the map window
    int mapX; // scroll amount of main map
    int mapY; // scroll amount of main map
    int oldMapX; // scroll amount of main map
    int oldMapY; // scroll amount of main map
    int mapWidth; // width of map
    int mapHeight; // height of map
    int screenX = 0;
    int screenY = 0;
    int oldScreenX = 0;
    int oldScreenY = 0;

    uint16_t midminiMap[2+56*44]; // the window on the whole map
    int midwindowX; // position within the map window
    int midwindowY; // position within the map window
    int midmapX; // scroll amount of main map
    int midmapY; // scroll amount of main map
    int midoldMapX; // scroll amount of main map
    int midoldMapY; // scroll amount of main map
    int midmapWidth; // width of map
    int midmapHeight; // height of map
    int midscreenX = 0;
    int midscreenY = 0;
    int midoldScreenX = 0;
    int midoldScreenY = 0;

    int multiplyX;
    int multiplyY;

    uint8_t numRed;
    uint8_t numGreen;
    uint8_t numBlue;
    uint8_t countRed;
    uint8_t countGreen;
    uint8_t countBlue;
    uint8_t screenHeight;
    uint8_t screenTop;
    uint8_t screenBottom;
    uint32_t totalGemsCollected = 0;
    uint32_t totalGemsToCollect = 0;
} bg;

struct PLAYER_DATA {
    int x;  // x postition
    int y;  // y position
    int vy; // for jumping
    bool flip; // flip sprite
    int jumpHeight = 1250;

    int startX;
    int startY;
    int lastGroundX;
    int lastGroundY;

    int8_t hatX = -2;
    int8_t hatY = 6;
    uint8_t hatFrame = 6;
    uint8_t frame; // tile number
    uint8_t direction;
    uint8_t step;
    int speed = 0;
    uint8_t numLives;
    
    int centre = 7;
    int rightBound = 15;
    int leftBound = 0;
    int upperBound = 1;
    int lowerBound = 15;
    bool onGround;
    bool falling;
    bool jumping;
    bool dropping=false; // is falling through bridge?
} player;

uint8_t onGround[10]; // buffer of being on the ground, for safer jumping

#define MAXITEMS 100
struct COLLECTABLES_DATA {
    const uint16_t *paletteData; // palette data
    const uint8_t *imageData; // image data
    int x;
    int y;
    uint8_t type;
    uint8_t frame;
    uint8_t maxFrame;
    uint8_t startFrame;
    uint8_t speed = 4;
    uint8_t collected;
    uint8_t startX;
    uint8_t startY;
    int offy=0;
    int frameSize;
    bool used=false;
    uint8_t direction;
    uint8_t step;
    uint8_t frameCount=0;
    uint8_t bitDepth;
} items[MAXITEMS];
int maxItems=0;

int wordCollected[3];

#define GRAVITY 56
#define MAXGRAVITY 0b1111111111111

struct VOLUME_DATA {
    int bgm = 75;  // bgm volume
    int sfx = 100;  // sfx volume
} volume;

int starCount=0;

//int offsetAngle = 0;
//int sprite_anim_frame=0;

int gameMode=0;

struct PALETTE_DATA {
    uint16_t rgb[256]; // background palette
    uint16_t hpal[100]; // background palette for scanline effect in the sky
} gamePalette;
//double saturation = 0;

int mapWidth=0;
int mapHeight=0;
int numGems=0;
int levelNumber = 0;


#define HUD_gemTimerStart 60
#define HUD_wordTimerStart 60
#define HUD_heartTimerStart 60
uint8_t HUD_gemTimer=0;
uint8_t HUD_wordTimer=0;
uint8_t HUD_heartTimer=0;
uint8_t HUD_gemFrameCount=0;
uint8_t HUD_wordFrameCount=0;
uint8_t HUD_heartFrameCount=0;


const uint8_t plasmaScreen[] ={
0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,2,
3,3,3,3,3,3,3,3,
4,4,4,4,4,4,4,4,
5,5,5,5,5,5,5,5,
6,6,6,6,6,6,6,6,
7,7,7,7,7,7,7,7,
8,8,8,8,8,8,8,8,
9,9,9,9,9,9,9,9,
10,10,10,10,10,10,10,10,
11,11,11,11,11,11,11,11,
12,12,12,12,12,12,12,12,
13,13,13,13,13,13,13,13,
14,14,14,14,14,14,14,14,
15,15,15,15,15,15,15,15,
16,16,16,16,16,16,16,16,
17,17,17,17,17,17,17,17,
18,18,18,18,18,18,18,18,
19,19,19,19,19,19,19,19,
20,20,20,20,20,20,20,20,
21,21,21,21,21,21,21,21,
22,22,22,22,22,22,22,22,
23,23,23,23,23,23,23,23,
24,24,24,24,24,24,24,24,
25,25,25,25,25,25,25,25,
26,26,26,26,26,26,26,26,
27,27,27,27,27,27,27,27,
28,28,28,28,28,28,28,28,
29,29,29,29,29,29,29,29,
};



