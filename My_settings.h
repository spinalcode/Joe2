#define PROJ_SCREENMODE TASMODE
#define PROJ_FPS 255

//#define PROJ_HIGH_RAM HIGH_RAM_ON
#define PROJ_HIGH_RAM HIGH_RAM_MUSIC
#define PROJ_ENABLE_SFX
#define PROJ_ENABLE_SD_MUSIC
#define PROJ_AUD_FREQ 22050
#define PROJ_STREAM_LOOP 1

// Enabling these 2 optinos causes my code to crash on first compile but work on the second compile.
#define PROJ_ENABLE_SOUND 1
#define PROJ_FILE_STREAMING


#define PROJ_MAX_SPRITES 128
// setting the tile size larger than the screen results in a tiny map
#define PROJ_TILE_H 1024
#define PROJ_TILE_W 1024
#define PROJ_DEVELOPER_MODE 0
#define MAX_TILE_COUNT 16
#define PROJ_LINE_FILLERS TAS::NOPFiller, TAS::SpriteFiller, TAS::NOPFiller, TAS::NOPFiller

/*
42.078125
26930
26880

352x304
44x38
640*38=24320
+44=24364

50.15
96,50


*/