
int myVolume = 5;

#define MAXSTEP 64
#define MAXSPEED 512
#define PLAYER_SPEED 512
#define MAX_AMINS 20
#define GEM_ANIM_SPEED 3

//int myCounter=0;
//uint32_t mySin[360];
//int letter_frame=0;

//bool clearScreen=false;
//long int myDelay;
//long int tempTime;
uint8_t frameSkip;
int fpsCount=0;
long int fpsCounter;
//long int lastMillis; // doesn't need to be global
//bool screenMoving = false;
//uint8_t line[255];
//const uint16_t palette2[] = {0};
//bool interlaceScreen = false;
bool mustDrawTitleScreen=true;
uint16_t pal[512]; // plasma palette for door
uint8_t doorPalOffset=0;

long int timerCounter=0;
//uint16_t bgline_pal[92];
//int lastCollectedX=0;
//int lastCollectedY=0;

//const uint16_t emptyPalette[]={0};
//const uint8_t blankLine[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// for my own sprite renderer
#define NUMSPRITES 64
struct SPRITE_DATA {
    const uint16_t *paletteData; // palette data
    const uint8_t *imageData; // image data
    int x;  // x postition
    int y;  // y position
    int hFlip;
    int offset; // tile render pixel offset
    uint8_t bit;
} sprites[NUMSPRITES];
int spriteCount = 0;

struct ANIMATION_DATA {
    const uint16_t *paletteData; // palette data
    const uint8_t *imageData; // image data
    int x;  // x postition
    int y;  // y position
    int startX;
    int startY;
    int endX;
    int endY;
    uint8_t frame;
    uint8_t speed;
    uint8_t maxFrame;
    uint8_t startFrame;
    int frameSize;
    int type;
    bool used=false;
    int frameCount=0;
} animSprite[20];


struct DOOR_DATA {
    int x;
    int y;
    bool visible;
    uint8_t speed = 6;
    uint8_t loadDoorCounter=0;
    uint8_t frame = 0;
} exitDoor[4];
uint8_t numDoors = 0;
uint8_t atDoor = 99;

//char levelFilename[32];
//char levelTilename[32];
//uint32_t layerNumber=0;

int levNum=0;
uint8_t tileType[20]; // to be read from the first row in the collision map
#define NOTHING 0
int SOLID = 1;
int JUMPTHROUGH = 2;
int DEATHCOLOUR = 10;
int DOORCOLOUR = 11;

const uint8_t satRamp[]={0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,3,4,4,4,5,5,6,6,6,7,7,8,8,9,9,10,11,11,12,13,14,15,15,16,17,18,19,20,21,22,23,25,26,27,28,30,31,32,34,35,37,39,40,42,44,45,47,48,51,53,54,57,58,60,62,64,67,68,71,72,75,76,79,81,82,85,87,90,92,93,95,96,98,100};

struct BACKGROUND_DATA {
    int windowX; // position within the map window
    int windowY; // position within the map window
    int mapX; // scroll amount of main map
    int mapY; // scroll amount of main map
    int oldMapX; // scroll amount of main map
    int oldMapY; // scroll amount of main map
    int mapWidth; // width of map
    int mapHeight; // height of map
    uint16_t miniMap[2+56*44]; // the window on the whole map
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

    int8_t hatX = -2;
    int8_t hatY = 6;
    uint8_t hatFrame = 6;
    uint8_t frame; // tile number
    uint8_t direction;
    uint8_t step;
    int speed = 0;
    
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
} items[128];
int maxItems=0;

int wordCollected[3];

#define GRAVITY 56
#define MAXGRAVITY 0b1111111111111

struct VOLUME_DATA {
    int bgm = 75;  // bgm volume
    int sfx = 100;  // sfx volume
} volume;

int screenX = 0;
int screenY = 0;
int oldScreenX = screenX;
int oldScreenY = screenY;

//int offsetAngle = 0;
//int sprite_anim_frame=0;

int gameMode=0;

struct PALETTE_DATA {
    uint16_t rgb[256]; // background palette
    uint16_t hpal[100]; // background palette for scanline effect
} gamePalette;
//double saturation = 0;

int mapWidth=0;
int mapHeight=0;
int numGems=0;
int levelNumber = 0;


int HUD_gemTimer=0;
int HUD_livesTimer=0;
#define HUD_gemTimerStart 60;
int HUD_gemFrameCount=0;

// pointers to our data
const uint8_t* levelData = nullptr;
const uint16_t* midmap = nullptr;
const uint8_t* tiles = nullptr;
const uint8_t* collisionTile = nullptr;

// Make a hotswap area for level data
// size = 72*1024k, unique id = 0
using LevelData = Hotswap<72*1024, 0>; // multiple of 4kb = Palette and level tiles

int lastLoad=0;

int mapPos;




