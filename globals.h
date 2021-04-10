bool clearScreen=false;
long int myDelay;
long int tempTime;
long int frameCount;
int fpsCount=0;
long int fpsCounter;
long int lastMillis;
bool screenMoving = false;
uint8_t line[255];
//uint8_t line2[255];
const uint16_t palette2[] = {0};
bool interlaceScreen = false;

struct BACKGROUND_DATA {
    int windowX; // position within the map window
    int windowY; // position within the map window
    int mapX; // scroll amount of main map
    int mapY; // scroll amount of main map
    int mapWidth; // width of map
    int mapHeight; // height of map
    uint16_t miniMap[2+56*44]; // the window on the whole map
    //uint16_t collisionMap[2+56*44]; // the window on the whole map
} bg;

struct PLAYER_DATA {
    int x;  // x postition
    int y;  // y position
    int vy; // for jumping
    bool flip; // flip sprite
    int jumpHeight = 1250;

    int8_t hatX = -2;
    int8_t hatY = 6;
    uint8_t hatFrame = 6;
    uint8_t frame; // tile number
    uint8_t direction;
    uint8_t step;
    uint8_t gotHat = 0;
    
    int centre = 7;
    int rightBound = 14;
    int leftBound = 1;
    int upperBound = 1;
    int lowerBound = 15;
    bool onGround;
    bool falling;
    bool jumping;
    bool okToJump=false;
} player;

#define GRAVITY 48
#define PLAYER_SPEED 512

int screenX = 0;
int screenY = 0;
int oldScreenX = screenX;
int oldScreenY = screenY;

int offsetAngle = 0;
int sprite_anim_frame=0;

struct PALETTE_DATA {
    uint8_t r[256];
    uint8_t g[256];
    uint8_t b[256];
    uint16_t rgb[256];
} gamePalette;

