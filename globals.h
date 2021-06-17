
//File bgmFile;
int myVolume = 5;

#define MAXSTEP 64
#define MAXSPEED 512
#define PLAYER_SPEED 512

bool clearScreen=false;
long int myDelay;
long int tempTime;
int frameSkip;
int fpsCount=0;
long int fpsCounter;
long int lastMillis;
bool screenMoving = false;
uint8_t line[255];
//uint8_t line2[255];
const uint16_t palette2[] = {0};
bool interlaceScreen = false;
bool mustDraw=true;

long int timerCounter=0;

uint8_t colourBlack=7;

uint16_t bgline_pal[92];

int lastCollectedX=0;
int lastCollectedY=0;

const uint16_t emptyPalette[]={0};
const uint8_t blankLine[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

char levelFilename[32];
char levelTilename[32];
uint32_t layerNumber=0;

uint8_t tileType[20]; // to be read from the first row in the collision map
#define NOTHING 0
int SOLID = 1;
int JUMPTHROUGH = 2;
int DEATHCOLOUR = 10;

const uint8_t satRamp[]={0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,8,8,8,9,9,9,9,10,10,10,11,11,12,12,12,13,13,14,14,15,16,17,17,18,19,20,21,22,23,25,27,29,31,33,36,40,43,48,53,59,67,75,94,99,100};

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
    //uint16_t collisionMap[2+56*44]; // the window on the whole map
    float satRed=0.0;
    float satGreen=0.0;
    float satBlue=0.0;
    int numRed;
    int numGreen;
    int numBlue;
    int countRed;
    int countGreen;
    int countBlue;
    float redPercent;
    float greenPercent;
    float bluePercent;
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
    uint8_t gotHat = 0;
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

uint8_t onGround[51]; // buffer of being on the ground, for safer jumping

struct COLLECTABLES_DATA {
    int x;
    int y;
    uint8_t type;
    uint8_t frame;
    uint8_t speed = 4;
    uint8_t collected;
    int mapPos;
} items[100];
int maxItems=0;
const uint8_t gemFrame[]={0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7};

struct ENEMY_DATA {
    int x;  // x postition
    int y;  // y position
    int offy=0;
    int vy; // for jumping
    bool flip; // flip sprite

    uint8_t frame; // tile number
    uint8_t direction;
    uint8_t step;
    int numFrames;

    uint8_t type=0;
    uint8_t speed=6;

    int centre = 7;
    int rightBound = 15;
    int leftBound = 0;
    int upperBound = 1;
    int lowerBound = 15;
} enemy[100];
int maxEnemies=0;

struct ANIMATION_DATA {
    int x;  // x postition
    int y;  // y position
    int startX;
    int startY;
    int frame;
    bool used=false;
    int frameCount=0;
} animSprite[20];

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

int offsetAngle = 0;
int sprite_anim_frame=0;

int gameMode=0;

struct PALETTE_DATA {
    uint16_t rgb[256];
} gamePalette;

double saturation = 0;

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




