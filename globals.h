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
    int height = 13;
    int width = 16;
    int frame; // tile number
    float gravity; // how fast player is falling
    float jumpHeight = 7.0;
    float walkSpeed = 1.75;
    int colTile;
    int speed=1;
} player;

int screenX = 0;
int screenY = 0;
int oldScreenX = screenX;
int oldScreenY = screenY;

//uint8_t tempTile[68];

