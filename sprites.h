const uint8_t singlePixel[] = {
1,1, 10
};

const uint8_t sprite_palette[] =
{
32,8, 0, 8,16,24,32,40,48,56,64,72,80,88, 96,104,112,120,128,136,144,152,160,168,176,184,192,200,208,216,224,232,240,248,
      1, 9,17,25,33,41,49,57,65,73,81,89, 97,105,113,121,129,137,145,153,161,169,177,185,193,201,209,217,225,233,241,249,
      2,10,18,26,34,42,50,58,66,74,82,90, 98,106,114,122,130,138,146,154,162,170,178,186,194,202,210,218,226,234,242,250,
      3,11,19,27,35,43,51,59,67,75,83,91, 99,107,115,123,131,139,147,155,163,171,179,187,195,203,211,219,227,235,243,251,
      4,12,20,28,36,44,52,60,68,76,84,92,100,108,116,124,132,140,148,156,164,172,180,188,196,204,212,220,228,236,244,252,
      5,13,21,29,37,45,53,61,69,77,85,93,101,109,117,125,133,141,149,157,165,173,181,189,197,205,213,221,229,237,245,253,
      6,14,22,30,38,46,54,62,70,78,86,94,102,110,118,126,134,142,150,158,166,174,182,190,198,206,214,222,230,238,246,254,
      7,15,23,31,39,47,55,63,71,79,87,95,103,111,119,127,135,143,151,159,167,175,183,191,199,207,215,223,231,239,247,255, 
};


//Sprite sheet:8x1
const uint8_t player_sprite [][258] ={
//[0] cell:0x0 
{
16,16, 0,0,0,0,0,0,10,10,10,10,0,0,0,0,0,0, 0,0,0,0,10,10,26,27,27,26,10,10,0,0,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,0,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,10,28,28,28,28,28,28,28,28,28,28,10,0,0, 0,10,26,28,28,28,28,28,28,28,28,28,28,26,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,26,28,28,28,28,28,10,28,10,28,28,26,10,0, 0,0,10,28,28,28,28,28,10,28,10,28,28,10,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,0,0,0, 0,0,0,0,10,10,26,27,27,26,10,10,0,0,0,0, 0,0,0,0,9,21,10,10,10,10,9,0,0,0,0,0, 0,0,0,0,10,21,21,21,29,10,29,10,0,0,0,0, 0,0,0,0,10,10,10,10,10,10,10,10,0,0,0,0, 
},
//[1] cell:1x0 
{
16,16, 0,0,0,0,0,0,10,10,10,10,0,0,0,0,0,0, 0,0,0,0,10,10,26,27,27,26,10,10,0,0,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,0,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,10,28,28,28,28,28,28,28,28,28,28,10,0,0, 0,10,26,28,28,28,28,28,28,28,28,28,28,26,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,26,28,28,28,28,28,10,28,10,28,28,26,10,0, 0,0,10,28,28,28,28,28,10,28,10,28,28,10,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,10,0,0, 0,0,10,21,10,10,26,27,27,26,10,10,29,29,10,0, 0,0,10,21,21,30,10,10,10,10,30,21,21,30,10,0, 0,0,10,30,21,21,29,10,0,10,21,21,30,10,0,0, 0,0,0,10,10,10,10,10,0,0,10,10,10,0,0,0, 
},
//[2] cell:2x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,10,10,10,10,0,0,0,0,0,0, 0,0,0,0,10,10,26,27,27,26,10,10,0,0,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,0,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,10,28,28,28,28,28,28,28,28,28,28,10,0,0, 0,10,26,28,28,28,28,28,28,28,28,28,28,26,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,26,28,28,28,28,28,10,28,10,28,28,26,10,0, 0,0,10,28,28,28,28,28,10,28,10,28,28,10,9,0, 0,10,10,26,28,28,28,28,28,28,28,28,26,10,21,10, 0,10,21,10,26,28,28,28,28,28,28,26,10,30,21,10, 0,10,21,30,10,10,26,27,27,26,10,10,30,21,10,0, 0,10,21,29,10,0,10,10,10,10,0,10,21,21,10,0, 0,0,10,10,0,0,0,0,0,0,0,0,10,10,0,0, 
},
//[3] cell:3x0 
{
16,16, 0,0,0,0,0,0,10,10,10,10,0,0,0,0,0,0, 0,0,0,0,10,10,26,27,27,26,10,10,0,0,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,0,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,10,28,28,28,28,28,28,28,28,28,28,10,0,0, 0,10,26,28,28,28,28,28,28,28,28,28,28,26,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,26,28,28,28,28,28,10,28,10,28,28,26,10,0, 0,0,10,28,28,28,28,28,10,28,10,28,28,10,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,10,0,0, 0,0,10,21,10,10,26,27,27,26,10,10,29,29,10,0, 0,0,10,21,21,30,10,10,10,10,30,21,21,30,10,0, 0,0,10,30,21,21,29,10,0,10,21,21,30,10,0,0, 0,0,0,10,10,10,10,10,0,0,10,10,10,0,0,0, 
},
//[4] cell:4x0 
{
16,16, 0,0,0,0,0,0,10,10,10,10,0,0,0,0,0,0, 0,0,0,0,10,10,26,27,27,26,10,10,0,0,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,0,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,10,28,28,28,28,28,10,28,10,28,28,10,0,0, 0,10,26,28,28,28,28,28,10,28,10,28,28,26,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,26,28,28,28,28,28,28,28,28,28,28,26,10,0, 0,0,10,28,28,28,28,28,28,28,28,28,28,10,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,0,0,0, 0,0,0,10,10,10,26,27,27,26,10,10,0,0,0,0, 0,0,0,10,21,30,10,10,10,10,0,0,0,0,0,0, 0,0,0,10,21,29,10,21,10,0,0,0,0,0,0,0, 0,0,0,0,10,10,0,10,0,0,0,0,0,0,0,0, 
},
//[5] cell:5x0 
{
16,16, 0,0,0,0,0,0,10,10,10,10,0,0,0,0,0,0, 0,0,0,0,10,10,26,27,27,26,10,10,0,0,0,0, 0,0,0,10,26,28,28,28,28,28,28,26,10,0,0,0, 0,0,10,26,28,28,28,28,28,28,28,28,26,10,0,0, 0,0,10,28,28,28,28,28,28,28,28,28,28,10,0,0, 0,10,26,28,28,28,28,28,28,28,28,28,28,26,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,27,28,28,28,28,28,28,28,28,28,28,27,10,0, 0,10,26,28,28,28,28,28,10,28,10,28,28,26,10,0, 0,0,10,28,28,28,28,28,10,28,10,28,28,10,0,0, 0,0,10,26,28,28,28,26,10,10,26,28,26,10,0,0, 0,0,0,10,26,28,26,10,21,30,10,26,10,29,10,0, 0,0,0,0,10,10,10,29,21,29,10,10,29,21,10,0, 0,0,0,0,0,0,10,21,21,10,30,21,21,10,0,0, 0,0,0,0,0,0,10,29,21,10,21,21,10,0,0,0, 0,0,0,0,0,0,10,10,10,0,10,10,0,0,0,0, 
},
//[6] cell:6x0 
{
16,16, 0,0,0,0,0,11,10,10,0,0,0,0,0,0,0,0, 0,0,0,11,10,10,10,10,11,0,0,0,0,0,0,0, 0,11,10,10,10,10,10,10,10,0,0,0,0,0,0,0, 10,10,10,10,10,10,10,10,10,11,0,0,0,0,0,0, 10,10,10,10,10,10,10,10,10,10,0,0,0,0,0,0, 11,10,10,10,10,10,10,10,10,11,11,0,11,10,11,0, 0,10,10,10,10,10,10,10,11,13,12,10,10,11,0,0, 0,11,10,10,10,10,11,13,13,10,10,11,0,0,0,0, 0,0,10,10,11,13,13,10,10,11,0,0,0,0,0,0, 0,0,11,13,13,10,10,11,0,0,0,0,0,0,0,0, 0,0,11,10,10,11,0,0,0,0,0,0,0,0,0,0, 11,10,10,10,11,0,0,0,0,0,0,0,0,0,0,0, 11,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[7] cell:7x0 
{
16,16, 0,0,0,10,10,11,0,0,0,0,0,11,11,0,0,0, 0,0,11,10,10,10,10,11,0,0,0,10,11,0,0,0, 0,0,10,10,10,10,10,10,10,11,11,10,0,0,0,0, 0,11,10,10,10,10,10,10,10,13,10,10,0,0,0,0, 0,10,10,10,10,10,10,10,11,13,10,11,0,0,0,0, 11,10,10,10,10,10,10,10,13,10,11,0,0,0,0,0, 10,10,10,10,10,10,10,11,13,10,0,0,0,0,0,0, 10,10,10,10,10,10,10,13,10,11,0,0,0,0,0,0, 0,11,10,10,10,10,11,13,10,0,0,0,0,0,0,0, 0,0,0,11,10,11,13,10,11,0,0,0,0,0,0,0, 0,0,0,0,0,11,12,10,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,10,11,0,0,0,0,0,0,0,0, 0,0,0,0,0,11,10,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,10,11,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
};

//Total colors 16
const uint16_t color_sprite_pal[] = {
16396,45696,63497,62272,65254,22274,15661,3348,0,415,11647,1690,65535,50712,31136,23209,

};

//Sprite sheet:8x1
const uint8_t color_sprite [][258] ={
//[0] cell:0x0 
{
16,16, 0,0,0,0,1,6,7,15,9,0,0,0,0,0,0,0, 0,0,0,7,15,7,7,15,14,4,2,9,1,0,0,0, 0,0,7,7,7,7,6,12,10,13,4,5,4,0,0,0, 0,6,7,7,6,1,0,12,10,10,12,10,5,0,0,0, 0,6,7,6,0,0,9,13,13,13,12,12,13,0,0,0, 0,6,6,0,9,5,5,5,13,13,12,13,9,0,0,0, 0,0,12,10,5,5,5,5,13,13,13,10,12,12,0,0, 0,1,5,5,5,5,13,13,13,13,9,12,12,12,15,0, 1,4,5,5,5,13,13,13,2,9,12,12,12,15,15,9, 2,4,4,4,4,2,2,9,12,12,12,12,12,15,15,9, 2,5,4,4,2,9,12,9,9,12,12,15,15,15,15,1, 1,4,4,4,2,10,12,1,6,7,15,15,15,15,9,0, 0,0,9,12,12,12,7,7,7,7,7,15,15,9,0,0, 0,0,0,0,0,15,7,7,7,7,7,14,0,0,0,0, 0,0,0,0,0,15,7,7,7,14,13,9,0,0,0,0, 0,0,0,0,0,6,6,9,9,9,0,0,0,0,0,0, 
},
//[1] cell:1x0 
{
16,16, 0,0,0,0,9,7,7,15,12,0,0,0,0,0,0,0, 0,0,1,15,15,15,7,7,15,2,9,0,0,0,0,0, 0,1,7,15,15,7,7,6,10,13,5,4,4,1,0,0, 0,7,7,7,6,1,0,9,10,13,2,5,5,4,0,0, 6,7,7,6,0,0,9,9,12,10,12,12,13,2,0,0, 6,7,6,0,0,9,10,13,13,13,13,10,13,0,0,0, 0,6,0,0,9,13,5,5,5,5,13,13,9,0,0,0, 0,1,12,2,5,5,5,5,5,5,13,12,12,12,0,0, 0,1,4,5,5,13,13,13,13,2,12,10,12,15,12,0, 0,4,5,5,13,13,13,10,9,12,10,12,12,15,15,0, 9,5,5,5,13,2,9,9,9,12,12,12,15,15,15,0, 9,5,5,4,2,10,12,9,1,6,15,15,15,15,9,0, 0,1,4,14,15,12,12,6,7,7,15,15,15,12,0,0, 0,0,0,0,9,12,15,7,7,7,7,15,9,0,0,0, 0,0,0,0,0,6,15,7,7,7,6,0,0,0,0,0, 0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0, 
},
//[2] cell:2x0 
{
16,16, 0,0,0,0,0,9,10,14,6,15,12,0,0,0,0,0, 0,0,0,0,10,12,15,15,7,15,15,0,0,0,0,0, 0,0,0,9,15,15,15,15,7,7,14,0,0,0,0,0, 0,0,15,15,15,15,15,7,6,12,10,5,13,2,0,0, 0,7,7,15,15,7,6,1,1,12,10,13,5,4,4,1, 6,7,7,7,7,9,9,9,9,9,2,13,13,4,4,6, 7,15,7,12,12,12,12,9,9,10,13,13,13,5,4,1, 12,15,12,12,12,9,9,2,13,13,5,5,5,5,2,0, 0,14,12,12,9,2,13,13,13,5,5,5,5,2,0,0, 0,0,2,10,2,13,13,13,13,13,5,5,10,12,0,0, 0,0,0,2,13,13,12,12,13,13,10,12,12,15,9,0, 0,0,1,5,13,12,12,12,10,12,12,12,12,15,9,0, 0,0,9,13,13,13,2,10,12,12,12,15,15,15,0,0, 0,0,9,13,4,4,15,12,12,7,15,15,15,9,0,0, 0,0,0,0,0,9,12,15,15,7,15,15,1,0,0,0, 0,0,0,0,0,0,0,1,7,6,1,0,0,0,0,0, 
},
//[3] cell:3x0 
{
16,16, 0,0,0,0,9,10,10,2,9,9,9,0,0,0,0,0, 0,0,0,9,12,12,12,14,15,7,15,9,0,0,0,0, 0,0,1,12,12,12,15,15,7,7,15,9,0,0,0,0, 0,0,9,12,15,15,15,15,7,7,12,2,0,0,0,0, 0,1,15,15,15,15,15,15,6,9,10,13,13,2,1,0, 1,15,15,15,15,15,12,12,9,9,9,2,2,4,4,1, 15,15,15,15,12,12,12,12,9,9,10,13,5,4,4,2, 15,15,15,12,12,10,12,9,2,13,13,13,5,4,4,1, 15,15,12,12,12,9,2,13,13,13,5,5,4,4,4,0, 1,15,12,10,9,2,13,10,13,13,5,5,4,4,0,0, 0,0,2,2,2,13,12,12,12,13,13,5,14,9,0,0, 0,0,0,9,13,12,12,12,12,13,10,12,12,12,0,0, 0,0,0,13,13,12,12,12,12,10,12,15,15,9,0,0, 0,0,0,10,13,14,14,12,12,15,15,15,15,0,0,0, 0,0,0,9,14,2,15,15,15,15,15,12,0,0,0,0, 0,0,0,0,0,0,0,1,9,9,0,0,0,0,0,0, 
},
//[4] cell:4x0 
{
16,16, 0,0,0,0,1,12,10,13,1,0,0,0,0,0,0,0, 0,0,0,10,13,12,12,13,14,15,12,9,0,0,0,0, 0,0,12,12,12,12,12,10,14,12,15,15,15,0,0,0, 0,9,12,12,12,12,12,10,14,15,7,15,15,0,0,0, 0,12,12,12,12,12,12,15,15,15,7,7,15,0,0,0, 0,9,12,12,12,15,15,15,15,15,7,15,9,0,0,0, 0,0,12,12,15,15,15,15,15,15,12,10,13,2,0,0, 0,1,15,15,15,15,15,15,12,12,9,2,2,4,13,0, 1,15,15,15,15,15,12,12,9,9,2,2,2,4,4,9, 12,15,15,15,12,12,12,9,2,13,13,2,4,4,4,6, 12,15,15,12,10,9,2,10,10,10,13,5,4,4,4,1, 1,15,15,12,10,2,13,12,12,12,13,5,4,4,1,0, 0,0,1,2,4,13,10,12,12,12,12,13,4,1,0,0, 0,0,0,0,0,13,12,12,12,12,12,14,0,0,0,0, 0,0,0,0,0,10,12,12,12,15,15,1,0,0,0,0, 0,0,0,0,0,9,9,9,9,9,0,0,0,0,0,0, 
},
//[5] cell:5x0 
{
16,16, 0,0,0,0,9,10,12,10,2,0,0,0,0,0,0,0, 0,0,1,13,13,13,12,12,5,12,9,0,0,0,0,0, 0,9,12,13,13,12,12,12,13,12,15,15,15,9,0,0, 0,12,12,12,12,12,12,10,2,10,12,15,15,15,0,0, 9,12,12,12,12,12,10,10,9,7,7,7,15,12,0,0, 9,12,12,12,10,12,12,15,15,15,15,15,15,0,0,0, 0,12,12,10,12,15,15,15,15,15,15,15,12,0,0,0, 0,1,12,12,15,15,15,15,15,15,12,2,13,13,0,0, 0,1,15,15,15,15,15,15,12,9,2,2,2,4,2,0, 0,15,15,15,15,15,12,9,9,2,2,2,4,4,4,0, 9,15,15,15,12,12,2,10,10,13,13,4,4,4,4,0, 9,15,15,12,12,2,13,10,12,12,13,5,4,4,1,0, 0,9,15,14,4,4,13,12,12,12,10,5,4,2,0,0, 0,0,0,0,1,2,13,12,12,12,12,13,1,0,0,0, 0,0,0,0,0,9,10,12,12,10,9,0,0,0,0,0, 0,0,0,0,0,0,9,9,0,0,0,0,0,0,0,0, 
},
//[6] cell:6x0 
{
16,16, 0,0,0,0,0,6,7,12,12,10,2,0,0,0,0,0, 0,0,0,0,7,15,5,13,12,10,5,0,0,0,0,0, 0,0,0,2,4,5,13,13,12,12,13,0,0,0,0,0, 0,0,10,5,5,13,13,12,12,10,14,15,15,12,0,0, 0,10,12,10,10,12,12,12,12,10,10,10,12,15,15,1, 9,12,12,12,12,10,10,10,10,9,9,12,12,15,15,9, 10,13,10,13,13,13,10,9,9,12,12,15,15,15,15,9, 2,13,13,13,13,2,9,12,12,15,15,15,15,15,12,0, 0,14,13,13,9,12,12,15,15,15,15,15,15,12,0,0, 0,0,12,10,12,15,15,15,15,15,15,15,2,4,0,0, 0,0,0,12,15,15,7,7,15,15,2,2,2,4,1,0, 0,0,1,15,15,7,7,7,14,13,13,2,4,4,6,0, 0,0,9,15,15,15,10,2,13,13,13,4,4,4,0,0, 0,0,1,15,15,15,4,5,13,12,13,5,4,1,0,0, 0,0,0,0,0,1,2,13,13,12,13,14,1,0,0,0, 0,0,0,0,0,0,0,9,10,9,1,0,0,0,0,0, 
},
//[7] cell:7x0 
{
16,16, 0,0,0,0,1,7,15,12,1,9,9,0,0,0,0,0, 0,0,0,6,7,7,7,14,14,12,13,9,0,0,0,0, 0,0,1,7,7,14,5,13,12,12,13,9,0,0,0,0, 0,0,9,7,4,5,13,13,12,12,13,9,0,0,0,0, 0,1,13,5,5,13,13,10,12,12,10,12,12,12,0,0, 1,10,13,13,13,13,13,10,10,10,9,12,12,15,15,1, 13,13,13,13,13,13,13,10,10,9,12,10,12,15,15,12, 4,5,13,13,2,2,2,9,12,12,12,12,15,15,15,9, 4,4,4,2,2,9,12,12,12,12,15,15,15,15,15,0, 1,4,4,2,2,10,12,12,15,15,15,15,15,15,0,0, 0,0,12,12,12,12,7,7,7,15,15,15,4,1,0,0, 0,0,0,9,15,7,7,7,7,15,14,2,4,2,0,0, 0,0,0,15,15,7,7,7,14,13,5,4,4,6,0,0, 0,0,0,15,15,15,15,5,13,13,13,4,4,0,0,0, 0,0,0,1,12,12,4,13,13,13,13,2,0,0,0,0, 0,0,0,0,0,0,0,1,9,9,0,0,0,0,0,0, 
},
};



//Total colors 16
const uint16_t gems_pal[] = {
16396,45696,63497,62272,65254,22274,15661,3348,0,415,11647,1690,65535,50712,31136,23209,
};

//Sprite sheet:10x3
const uint8_t gems [][258] ={
//[0] cell:0x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,3,2,8,0,0,0,0,0,0, 0,0,0,0,0,8,3,3,2,2,8,0,0,0,0,0, 0,0,0,0,8,3,3,3,2,2,2,8,0,0,0,0, 0,0,0,8,3,3,3,3,2,2,2,2,8,0,0,0, 0,0,0,8,3,3,3,3,2,2,2,2,8,0,0,0, 0,0,8,3,3,3,3,3,2,2,2,2,2,8,0,0, 0,0,8,2,2,2,2,2,3,3,3,3,3,8,0,0, 0,0,0,8,2,2,2,2,3,3,3,3,8,0,0,0, 0,0,0,8,2,2,2,2,3,3,3,3,8,0,0,0, 0,0,0,0,8,2,2,2,3,3,3,8,0,0,0,0, 0,0,0,0,0,8,2,2,3,3,8,0,0,0,0,0, 0,0,0,0,0,0,8,2,3,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[1] cell:1x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,3,2,8,0,0,0,0,0,0, 0,0,0,0,0,8,3,3,2,2,8,0,0,0,0,0, 0,0,0,0,8,3,3,3,2,2,2,8,0,0,0,0, 0,0,0,8,3,3,3,3,3,2,2,2,8,0,0,0, 0,0,0,8,3,3,3,3,3,2,2,2,8,0,0,0, 0,0,8,3,3,3,3,3,3,2,2,2,2,8,0,0, 0,0,8,2,2,2,2,2,2,3,3,3,3,8,0,0, 0,0,0,8,2,2,2,2,2,3,3,3,8,0,0,0, 0,0,0,8,2,2,2,2,2,3,3,3,8,0,0,0, 0,0,0,0,8,2,2,2,3,3,3,8,0,0,0,0, 0,0,0,0,0,8,2,2,3,3,8,0,0,0,0,0, 0,0,0,0,0,0,8,2,3,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[2] cell:2x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,3,2,8,0,0,0,0,0,0, 0,0,0,0,0,8,3,3,2,2,8,0,0,0,0,0, 0,0,0,0,8,3,3,3,3,2,2,8,0,0,0,0, 0,0,0,8,3,3,3,3,3,2,2,2,8,0,0,0, 0,0,0,8,3,3,3,3,3,3,2,2,8,0,0,0, 0,0,8,3,3,3,3,3,3,3,2,2,2,8,0,0, 0,0,8,2,2,2,2,2,2,2,3,3,3,8,0,0, 0,0,0,8,2,2,2,2,2,2,3,3,8,0,0,0, 0,0,0,8,2,2,2,2,2,3,3,3,8,0,0,0, 0,0,0,0,8,2,2,2,2,3,3,8,0,0,0,0, 0,0,0,0,0,8,2,2,3,3,8,0,0,0,0,0, 0,0,0,0,0,0,8,2,3,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[3] cell:3x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,3,2,8,0,0,0,0,0,0, 0,0,0,0,0,8,3,3,3,2,8,0,0,0,0,0, 0,0,0,0,8,3,3,3,3,2,2,8,0,0,0,0, 0,0,0,8,3,3,3,3,3,3,2,2,8,0,0,0, 0,0,0,8,3,3,3,3,3,3,2,2,8,0,0,0, 0,0,8,3,3,3,3,3,3,3,3,2,2,8,0,0, 0,0,8,2,2,2,2,2,2,2,2,3,3,8,0,0, 0,0,0,8,2,2,2,2,2,2,3,3,8,0,0,0, 0,0,0,8,2,2,2,2,2,2,3,3,8,0,0,0, 0,0,0,0,8,2,2,2,2,3,3,8,0,0,0,0, 0,0,0,0,0,8,2,2,2,3,8,0,0,0,0,0, 0,0,0,0,0,0,8,2,3,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[4] cell:4x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,3,2,8,0,0,0,0,0,0, 0,0,0,0,0,8,3,3,3,2,8,0,0,0,0,0, 0,0,0,0,8,3,3,3,3,3,2,8,0,0,0,0, 0,0,0,8,3,3,3,3,3,3,3,2,8,0,0,0, 0,0,0,8,3,3,3,3,3,3,3,2,8,0,0,0, 0,0,8,3,3,3,3,3,3,3,3,3,2,8,0,0, 0,0,8,2,2,2,2,2,2,2,2,2,3,8,0,0, 0,0,0,8,2,2,2,2,2,2,2,3,8,0,0,0, 0,0,0,8,2,2,2,2,2,2,2,3,8,0,0,0, 0,0,0,0,8,2,2,2,2,2,3,8,0,0,0,0, 0,0,0,0,0,8,2,2,2,3,8,0,0,0,0,0, 0,0,0,0,0,0,8,2,3,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[5] cell:5x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,3,3,8,0,0,0,0,0,0, 0,0,0,0,0,8,3,3,3,3,8,0,0,0,0,0, 0,0,0,0,8,3,3,3,3,3,3,8,0,0,0,0, 0,0,0,8,3,3,3,3,3,3,3,3,8,0,0,0, 0,0,0,8,3,3,3,3,3,3,3,3,8,0,0,0, 0,0,8,3,3,3,3,3,3,3,3,3,3,8,0,0, 0,0,8,2,2,2,2,2,2,2,2,2,2,8,0,0, 0,0,0,8,2,2,2,2,2,2,2,2,8,0,0,0, 0,0,0,8,2,2,2,2,2,2,2,2,8,0,0,0, 0,0,0,0,8,2,2,2,2,2,2,8,0,0,0,0, 0,0,0,0,0,8,2,2,2,2,8,0,0,0,0,0, 0,0,0,0,0,0,8,2,2,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[6] cell:6x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,2,3,8,0,0,0,0,0,0, 0,0,0,0,0,8,2,3,3,3,8,0,0,0,0,0, 0,0,0,0,8,2,3,3,3,3,3,8,0,0,0,0, 0,0,0,8,2,3,3,3,3,3,3,3,8,0,0,0, 0,0,0,8,2,3,3,3,3,3,3,3,8,0,0,0, 0,0,8,2,3,3,3,3,3,3,3,3,3,8,0,0, 0,0,8,3,2,2,2,2,2,2,2,2,2,8,0,0, 0,0,0,8,3,2,2,2,2,2,2,2,8,0,0,0, 0,0,0,8,3,2,2,2,2,2,2,2,8,0,0,0, 0,0,0,0,8,3,2,2,2,2,2,8,0,0,0,0, 0,0,0,0,0,8,3,2,2,2,8,0,0,0,0,0, 0,0,0,0,0,0,8,3,2,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[7] cell:7x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,2,3,8,0,0,0,0,0,0, 0,0,0,0,0,8,2,3,3,3,8,0,0,0,0,0, 0,0,0,0,8,2,2,3,3,3,3,8,0,0,0,0, 0,0,0,8,2,2,3,3,3,3,3,3,8,0,0,0, 0,0,0,8,2,2,3,3,3,3,3,3,8,0,0,0, 0,0,8,2,2,3,3,3,3,3,3,3,3,8,0,0, 0,0,8,3,3,2,2,2,2,2,2,2,2,8,0,0, 0,0,0,8,3,3,2,2,2,2,2,2,8,0,0,0, 0,0,0,8,3,3,2,2,2,2,2,2,8,0,0,0, 0,0,0,0,8,3,3,2,2,2,2,8,0,0,0,0, 0,0,0,0,0,8,3,2,2,2,8,0,0,0,0,0, 0,0,0,0,0,0,8,3,2,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[8] cell:8x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,2,3,8,0,0,0,0,0,0, 0,0,0,0,0,8,2,2,3,3,8,0,0,0,0,0, 0,0,0,0,8,2,2,3,3,3,3,8,0,0,0,0, 0,0,0,8,2,2,2,3,3,3,3,3,8,0,0,0, 0,0,0,8,2,2,3,3,3,3,3,3,8,0,0,0, 0,0,8,2,2,2,3,3,3,3,3,3,3,8,0,0, 0,0,8,3,3,3,2,2,2,2,2,2,2,8,0,0, 0,0,0,8,3,3,2,2,2,2,2,2,8,0,0,0, 0,0,0,8,3,3,3,2,2,2,2,2,8,0,0,0, 0,0,0,0,8,3,3,2,2,2,2,8,0,0,0,0, 0,0,0,0,0,8,3,3,2,2,8,0,0,0,0,0, 0,0,0,0,0,0,8,3,2,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[9] cell:9x0 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,2,3,8,0,0,0,0,0,0, 0,0,0,0,0,8,2,2,3,3,8,0,0,0,0,0, 0,0,0,0,8,2,2,2,3,3,3,8,0,0,0,0, 0,0,0,8,2,2,2,3,3,3,3,3,8,0,0,0, 0,0,0,8,2,2,2,3,3,3,3,3,8,0,0,0, 0,0,8,2,2,2,2,3,3,3,3,3,3,8,0,0, 0,0,8,3,3,3,3,2,2,2,2,2,2,8,0,0, 0,0,0,8,3,3,3,2,2,2,2,2,8,0,0,0, 0,0,0,8,3,3,3,2,2,2,2,2,8,0,0,0, 0,0,0,0,8,3,3,3,2,2,2,8,0,0,0,0, 0,0,0,0,0,8,3,3,2,2,8,0,0,0,0,0, 0,0,0,0,0,0,8,3,2,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[10] cell:0x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,6,5,8,0,0,0,0,0,0, 0,0,0,0,0,8,6,6,5,5,8,0,0,0,0,0, 0,0,0,0,8,6,6,6,5,5,5,8,0,0,0,0, 0,0,0,8,6,6,6,6,5,5,5,5,8,0,0,0, 0,0,0,8,6,6,6,6,5,5,5,5,8,0,0,0, 0,0,8,6,6,6,6,6,5,5,5,5,5,8,0,0, 0,0,8,5,5,5,5,5,6,6,6,6,6,8,0,0, 0,0,0,8,5,5,5,5,6,6,6,6,8,0,0,0, 0,0,0,8,5,5,5,5,6,6,6,6,8,0,0,0, 0,0,0,0,8,5,5,5,6,6,6,8,0,0,0,0, 0,0,0,0,0,8,5,5,6,6,8,0,0,0,0,0, 0,0,0,0,0,0,8,5,6,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[11] cell:1x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,6,5,8,0,0,0,0,0,0, 0,0,0,0,0,8,6,6,5,5,8,0,0,0,0,0, 0,0,0,0,8,6,6,6,5,5,5,8,0,0,0,0, 0,0,0,8,6,6,6,6,6,5,5,5,8,0,0,0, 0,0,0,8,6,6,6,6,6,5,5,5,8,0,0,0, 0,0,8,6,6,6,6,6,6,5,5,5,5,8,0,0, 0,0,8,5,5,5,5,5,5,6,6,6,6,8,0,0, 0,0,0,8,5,5,5,5,5,6,6,6,8,0,0,0, 0,0,0,8,5,5,5,5,5,6,6,6,8,0,0,0, 0,0,0,0,8,5,5,5,6,6,6,8,0,0,0,0, 0,0,0,0,0,8,5,5,6,6,8,0,0,0,0,0, 0,0,0,0,0,0,8,5,6,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[12] cell:2x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,6,5,8,0,0,0,0,0,0, 0,0,0,0,0,8,6,6,5,5,8,0,0,0,0,0, 0,0,0,0,8,6,6,6,6,5,5,8,0,0,0,0, 0,0,0,8,6,6,6,6,6,5,5,5,8,0,0,0, 0,0,0,8,6,6,6,6,6,6,5,5,8,0,0,0, 0,0,8,6,6,6,6,6,6,6,5,5,5,8,0,0, 0,0,8,5,5,5,5,5,5,5,6,6,6,8,0,0, 0,0,0,8,5,5,5,5,5,5,6,6,8,0,0,0, 0,0,0,8,5,5,5,5,5,6,6,6,8,0,0,0, 0,0,0,0,8,5,5,5,5,6,6,8,0,0,0,0, 0,0,0,0,0,8,5,5,6,6,8,0,0,0,0,0, 0,0,0,0,0,0,8,5,6,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[13] cell:3x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,6,5,8,0,0,0,0,0,0, 0,0,0,0,0,8,6,6,6,5,8,0,0,0,0,0, 0,0,0,0,8,6,6,6,6,5,5,8,0,0,0,0, 0,0,0,8,6,6,6,6,6,6,5,5,8,0,0,0, 0,0,0,8,6,6,6,6,6,6,5,5,8,0,0,0, 0,0,8,6,6,6,6,6,6,6,6,5,5,8,0,0, 0,0,8,5,5,5,5,5,5,5,5,6,6,8,0,0, 0,0,0,8,5,5,5,5,5,5,6,6,8,0,0,0, 0,0,0,8,5,5,5,5,5,5,6,6,8,0,0,0, 0,0,0,0,8,5,5,5,5,6,6,8,0,0,0,0, 0,0,0,0,0,8,5,5,5,6,8,0,0,0,0,0, 0,0,0,0,0,0,8,5,6,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[14] cell:4x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,6,5,8,0,0,0,0,0,0, 0,0,0,0,0,8,6,6,6,5,8,0,0,0,0,0, 0,0,0,0,8,6,6,6,6,6,5,8,0,0,0,0, 0,0,0,8,6,6,6,6,6,6,6,5,8,0,0,0, 0,0,0,8,6,6,6,6,6,6,6,5,8,0,0,0, 0,0,8,6,6,6,6,6,6,6,6,6,5,8,0,0, 0,0,8,5,5,5,5,5,5,5,5,5,6,8,0,0, 0,0,0,8,5,5,5,5,5,5,5,6,8,0,0,0, 0,0,0,8,5,5,5,5,5,5,5,6,8,0,0,0, 0,0,0,0,8,5,5,5,5,5,6,8,0,0,0,0, 0,0,0,0,0,8,5,5,5,6,8,0,0,0,0,0, 0,0,0,0,0,0,8,5,6,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[15] cell:5x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,6,6,8,0,0,0,0,0,0, 0,0,0,0,0,8,6,6,6,6,8,0,0,0,0,0, 0,0,0,0,8,6,6,6,6,6,6,8,0,0,0,0, 0,0,0,8,6,6,6,6,6,6,6,6,8,0,0,0, 0,0,0,8,6,6,6,6,6,6,6,6,8,0,0,0, 0,0,8,6,6,6,6,6,6,6,6,6,6,8,0,0, 0,0,8,5,5,5,5,5,5,5,5,5,5,8,0,0, 0,0,0,8,5,5,5,5,5,5,5,5,8,0,0,0, 0,0,0,8,5,5,5,5,5,5,5,5,8,0,0,0, 0,0,0,0,8,5,5,5,5,5,5,8,0,0,0,0, 0,0,0,0,0,8,5,5,5,5,8,0,0,0,0,0, 0,0,0,0,0,0,8,5,5,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[16] cell:6x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,5,6,8,0,0,0,0,0,0, 0,0,0,0,0,8,5,6,6,6,8,0,0,0,0,0, 0,0,0,0,8,5,6,6,6,6,6,8,0,0,0,0, 0,0,0,8,5,6,6,6,6,6,6,6,8,0,0,0, 0,0,0,8,5,6,6,6,6,6,6,6,8,0,0,0, 0,0,8,5,6,6,6,6,6,6,6,6,6,8,0,0, 0,0,8,6,5,5,5,5,5,5,5,5,5,8,0,0, 0,0,0,8,6,5,5,5,5,5,5,5,8,0,0,0, 0,0,0,8,6,5,5,5,5,5,5,5,8,0,0,0, 0,0,0,0,8,6,5,5,5,5,5,8,0,0,0,0, 0,0,0,0,0,8,6,5,5,5,8,0,0,0,0,0, 0,0,0,0,0,0,8,6,5,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[17] cell:7x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,5,6,8,0,0,0,0,0,0, 0,0,0,0,0,8,5,6,6,6,8,0,0,0,0,0, 0,0,0,0,8,5,5,6,6,6,6,8,0,0,0,0, 0,0,0,8,5,5,6,6,6,6,6,6,8,0,0,0, 0,0,0,8,5,5,6,6,6,6,6,6,8,0,0,0, 0,0,8,5,5,6,6,6,6,6,6,6,6,8,0,0, 0,0,8,6,6,5,5,5,5,5,5,5,5,8,0,0, 0,0,0,8,6,6,5,5,5,5,5,5,8,0,0,0, 0,0,0,8,6,6,5,5,5,5,5,5,8,0,0,0, 0,0,0,0,8,6,6,5,5,5,5,8,0,0,0,0, 0,0,0,0,0,8,6,5,5,5,8,0,0,0,0,0, 0,0,0,0,0,0,8,6,5,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[18] cell:8x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,5,6,8,0,0,0,0,0,0, 0,0,0,0,0,8,5,5,6,6,8,0,0,0,0,0, 0,0,0,0,8,5,5,6,6,6,6,8,0,0,0,0, 0,0,0,8,5,5,5,6,6,6,6,6,8,0,0,0, 0,0,0,8,5,5,6,6,6,6,6,6,8,0,0,0, 0,0,8,5,5,5,6,6,6,6,6,6,6,8,0,0, 0,0,8,6,6,6,5,5,5,5,5,5,5,8,0,0, 0,0,0,8,6,6,5,5,5,5,5,5,8,0,0,0, 0,0,0,8,6,6,6,5,5,5,5,5,8,0,0,0, 0,0,0,0,8,6,6,5,5,5,5,8,0,0,0,0, 0,0,0,0,0,8,6,6,5,5,8,0,0,0,0,0, 0,0,0,0,0,0,8,6,5,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[19] cell:9x1 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,5,6,8,0,0,0,0,0,0, 0,0,0,0,0,8,5,5,6,6,8,0,0,0,0,0, 0,0,0,0,8,5,5,5,6,6,6,8,0,0,0,0, 0,0,0,8,5,5,5,6,6,6,6,6,8,0,0,0, 0,0,0,8,5,5,5,6,6,6,6,6,8,0,0,0, 0,0,8,5,5,5,5,6,6,6,6,6,6,8,0,0, 0,0,8,6,6,6,6,5,5,5,5,5,5,8,0,0, 0,0,0,8,6,6,6,5,5,5,5,5,8,0,0,0, 0,0,0,8,6,6,6,5,5,5,5,5,8,0,0,0, 0,0,0,0,8,6,6,6,5,5,5,8,0,0,0,0, 0,0,0,0,0,8,6,6,5,5,8,0,0,0,0,0, 0,0,0,0,0,0,8,6,5,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[20] cell:0x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,9,10,8,0,0,0,0,0,0, 0,0,0,0,0,8,9,9,10,10,8,0,0,0,0,0, 0,0,0,0,8,9,9,9,10,10,10,8,0,0,0,0, 0,0,0,8,9,9,9,9,10,10,10,10,8,0,0,0, 0,0,0,8,9,9,9,9,10,10,10,10,8,0,0,0, 0,0,8,9,9,9,9,9,10,10,10,10,10,8,0,0, 0,0,8,10,10,10,10,10,9,9,9,9,9,8,0,0, 0,0,0,8,10,10,10,10,9,9,9,9,8,0,0,0, 0,0,0,8,10,10,10,10,9,9,9,9,8,0,0,0, 0,0,0,0,8,10,10,10,9,9,9,8,0,0,0,0, 0,0,0,0,0,8,10,10,9,9,8,0,0,0,0,0, 0,0,0,0,0,0,8,10,9,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[21] cell:1x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,9,10,8,0,0,0,0,0,0, 0,0,0,0,0,8,9,9,10,10,8,0,0,0,0,0, 0,0,0,0,8,9,9,9,10,10,10,8,0,0,0,0, 0,0,0,8,9,9,9,9,9,10,10,10,8,0,0,0, 0,0,0,8,9,9,9,9,9,10,10,10,8,0,0,0, 0,0,8,9,9,9,9,9,9,10,10,10,10,8,0,0, 0,0,8,10,10,10,10,10,10,9,9,9,9,8,0,0, 0,0,0,8,10,10,10,10,10,9,9,9,8,0,0,0, 0,0,0,8,10,10,10,10,10,9,9,9,8,0,0,0, 0,0,0,0,8,10,10,10,9,9,9,8,0,0,0,0, 0,0,0,0,0,8,10,10,9,9,8,0,0,0,0,0, 0,0,0,0,0,0,8,10,9,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[22] cell:2x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,9,10,8,0,0,0,0,0,0, 0,0,0,0,0,8,9,9,10,10,8,0,0,0,0,0, 0,0,0,0,8,9,9,9,9,10,10,8,0,0,0,0, 0,0,0,8,9,9,9,9,9,10,10,10,8,0,0,0, 0,0,0,8,9,9,9,9,9,9,10,10,8,0,0,0, 0,0,8,9,9,9,9,9,9,9,10,10,10,8,0,0, 0,0,8,10,10,10,10,10,10,10,9,9,9,8,0,0, 0,0,0,8,10,10,10,10,10,10,9,9,8,0,0,0, 0,0,0,8,10,10,10,10,10,9,9,9,8,0,0,0, 0,0,0,0,8,10,10,10,10,9,9,8,0,0,0,0, 0,0,0,0,0,8,10,10,9,9,8,0,0,0,0,0, 0,0,0,0,0,0,8,10,9,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[23] cell:3x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,9,10,8,0,0,0,0,0,0, 0,0,0,0,0,8,9,9,9,10,8,0,0,0,0,0, 0,0,0,0,8,9,9,9,9,10,10,8,0,0,0,0, 0,0,0,8,9,9,9,9,9,9,10,10,8,0,0,0, 0,0,0,8,9,9,9,9,9,9,10,10,8,0,0,0, 0,0,8,9,9,9,9,9,9,9,9,10,10,8,0,0, 0,0,8,10,10,10,10,10,10,10,10,9,9,8,0,0, 0,0,0,8,10,10,10,10,10,10,9,9,8,0,0,0, 0,0,0,8,10,10,10,10,10,10,9,9,8,0,0,0, 0,0,0,0,8,10,10,10,10,9,9,8,0,0,0,0, 0,0,0,0,0,8,10,10,10,9,8,0,0,0,0,0, 0,0,0,0,0,0,8,10,9,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[24] cell:4x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,9,10,8,0,0,0,0,0,0, 0,0,0,0,0,8,9,9,9,10,8,0,0,0,0,0, 0,0,0,0,8,9,9,9,9,9,10,8,0,0,0,0, 0,0,0,8,9,9,9,9,9,9,9,10,8,0,0,0, 0,0,0,8,9,9,9,9,9,9,9,10,8,0,0,0, 0,0,8,9,9,9,9,9,9,9,9,9,10,8,0,0, 0,0,8,10,10,10,10,10,10,10,10,10,9,8,0,0, 0,0,0,8,10,10,10,10,10,10,10,9,8,0,0,0, 0,0,0,8,10,10,10,10,10,10,10,9,8,0,0,0, 0,0,0,0,8,10,10,10,10,10,9,8,0,0,0,0, 0,0,0,0,0,8,10,10,10,9,8,0,0,0,0,0, 0,0,0,0,0,0,8,10,9,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[25] cell:5x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,9,9,8,0,0,0,0,0,0, 0,0,0,0,0,8,9,9,9,9,8,0,0,0,0,0, 0,0,0,0,8,9,9,9,9,9,9,8,0,0,0,0, 0,0,0,8,9,9,9,9,9,9,9,9,8,0,0,0, 0,0,0,8,9,9,9,9,9,9,9,9,8,0,0,0, 0,0,8,9,9,9,9,9,9,9,9,9,9,8,0,0, 0,0,8,10,10,10,10,10,10,10,10,10,10,8,0,0, 0,0,0,8,10,10,10,10,10,10,10,10,8,0,0,0, 0,0,0,8,10,10,10,10,10,10,10,10,8,0,0,0, 0,0,0,0,8,10,10,10,10,10,10,8,0,0,0,0, 0,0,0,0,0,8,10,10,10,10,8,0,0,0,0,0, 0,0,0,0,0,0,8,10,10,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[26] cell:6x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,10,9,8,0,0,0,0,0,0, 0,0,0,0,0,8,10,9,9,9,8,0,0,0,0,0, 0,0,0,0,8,10,9,9,9,9,9,8,0,0,0,0, 0,0,0,8,10,9,9,9,9,9,9,9,8,0,0,0, 0,0,0,8,10,9,9,9,9,9,9,9,8,0,0,0, 0,0,8,10,9,9,9,9,9,9,9,9,9,8,0,0, 0,0,8,9,10,10,10,10,10,10,10,10,10,8,0,0, 0,0,0,8,9,10,10,10,10,10,10,10,8,0,0,0, 0,0,0,8,9,10,10,10,10,10,10,10,8,0,0,0, 0,0,0,0,8,9,10,10,10,10,10,8,0,0,0,0, 0,0,0,0,0,8,9,10,10,10,8,0,0,0,0,0, 0,0,0,0,0,0,8,9,10,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[27] cell:7x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,10,9,8,0,0,0,0,0,0, 0,0,0,0,0,8,10,9,9,9,8,0,0,0,0,0, 0,0,0,0,8,10,10,9,9,9,9,8,0,0,0,0, 0,0,0,8,10,10,9,9,9,9,9,9,8,0,0,0, 0,0,0,8,10,10,9,9,9,9,9,9,8,0,0,0, 0,0,8,10,10,9,9,9,9,9,9,9,9,8,0,0, 0,0,8,9,9,10,10,10,10,10,10,10,10,8,0,0, 0,0,0,8,9,9,10,10,10,10,10,10,8,0,0,0, 0,0,0,8,9,9,10,10,10,10,10,10,8,0,0,0, 0,0,0,0,8,9,9,10,10,10,10,8,0,0,0,0, 0,0,0,0,0,8,9,10,10,10,8,0,0,0,0,0, 0,0,0,0,0,0,8,9,10,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[28] cell:8x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,10,9,8,0,0,0,0,0,0, 0,0,0,0,0,8,10,10,9,9,8,0,0,0,0,0, 0,0,0,0,8,10,10,9,9,9,9,8,0,0,0,0, 0,0,0,8,10,10,10,9,9,9,9,9,8,0,0,0, 0,0,0,8,10,10,9,9,9,9,9,9,8,0,0,0, 0,0,8,10,10,10,9,9,9,9,9,9,9,8,0,0, 0,0,8,9,9,9,10,10,10,10,10,10,10,8,0,0, 0,0,0,8,9,9,10,10,10,10,10,10,8,0,0,0, 0,0,0,8,9,9,9,10,10,10,10,10,8,0,0,0, 0,0,0,0,8,9,9,10,10,10,10,8,0,0,0,0, 0,0,0,0,0,8,9,9,10,10,8,0,0,0,0,0, 0,0,0,0,0,0,8,9,10,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
//[29] cell:9x2 
{
16,16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,8,10,9,8,0,0,0,0,0,0, 0,0,0,0,0,8,10,10,9,9,8,0,0,0,0,0, 0,0,0,0,8,10,10,10,9,9,9,8,0,0,0,0, 0,0,0,8,10,10,10,9,9,9,9,9,8,0,0,0, 0,0,0,8,10,10,10,9,9,9,9,9,8,0,0,0, 0,0,8,10,10,10,10,9,9,9,9,9,9,8,0,0, 0,0,8,9,9,9,9,10,10,10,10,10,10,8,0,0, 0,0,0,8,9,9,9,10,10,10,10,10,8,0,0,0, 0,0,0,8,9,9,9,10,10,10,10,10,8,0,0,0, 0,0,0,0,8,9,9,9,10,10,10,8,0,0,0,0, 0,0,0,0,0,8,9,9,10,10,8,0,0,0,0,0, 0,0,0,0,0,0,8,9,10,8,0,0,0,0,0,0, 0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
},
};

