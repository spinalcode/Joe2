
uint8_t guiBG[27*22]; // GUI background layer

const uint8_t menuBG[]={
//27x22
27,22,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0,10,11,11,11,11,11,11,11,11,11,11,11,11,12, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

};

// print text
void guiPrint(char x, char y, const char* text) {
  uint8_t numChars = strlen(text);
  //uint8_t x1 = 0;//2+x+28*y;
  for (uint8_t t = 0; t < numChars; t++) {
    uint8_t character = text[t];
    guiBG[(x++)+27*y] = character;
    //drawSprite(x+((x1++)*8), y, font88[character], font88_pal,0,2);
  }
}


const char menuText[][16]={
    "Continue",
    "Volume",
    "Quit",
};
int optionNumber=0;
#define MAXOPTIONS 5

//Total colors 5
const uint16_t guiFont_pal[] = {
63519,1560,6242,63325,25357,
};

//Sprite sheet:16x8
const uint8_t guiFont [][34] ={
//[0] cell:0x0 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[1] cell:1x0 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x22,0x22, 0x0,0x2,0x43,0x33, 0x0,0x2,0x34,0x22, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 
},
//[2] cell:2x0 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x22,0x22,0x22,0x22, 0x33,0x33,0x33,0x33, 0x22,0x22,0x22,0x22, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[3] cell:3x0 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x22,0x22,0x20,0x0, 0x33,0x33,0x42,0x0, 0x22,0x24,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 
},
//[4] cell:4x0 
{
8,8, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 
},
//[5] cell:5x0 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[6] cell:6x0 
{
8,8, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 
},
//[7] cell:7x0 
{
8,8, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x34,0x22, 0x0,0x2,0x33,0x33, 0x0,0x2,0x34,0x22, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 
},
//[8] cell:8x0 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x22,0x22,0x22,0x22, 0x33,0x33,0x33,0x33, 0x22,0x22,0x22,0x22, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[9] cell:9x0 
{
8,8, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x22,0x24,0x32,0x0, 0x33,0x33,0x32,0x0, 0x22,0x24,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 
},
//[10] cell:10x0 
{
8,8, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x32,0x11, 0x0,0x2,0x34,0x22, 0x0,0x2,0x43,0x33, 0x0,0x0,0x22,0x22, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[11] cell:11x0 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x22,0x22,0x22,0x22, 0x33,0x33,0x33,0x33, 0x22,0x22,0x22,0x22, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[12] cell:12x0 
{
8,8, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x11,0x12,0x32,0x0, 0x22,0x24,0x32,0x0, 0x33,0x33,0x42,0x0, 0x22,0x22,0x20,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[13] cell:13x0 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[14] cell:14x0 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[15] cell:15x0 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[16] cell:0x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[17] cell:1x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[18] cell:2x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[19] cell:3x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[20] cell:4x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[21] cell:5x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[22] cell:6x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[23] cell:7x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[24] cell:8x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[25] cell:9x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[26] cell:10x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[27] cell:11x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[28] cell:12x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[29] cell:13x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[30] cell:14x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[31] cell:15x1 
{
8,8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 
},
//[32] cell:0x2 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[33] cell:1x2 
{
8,8, 0x11,0x12,0x22,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 
},
//[34] cell:2x2 
{
8,8, 0x12,0x22,0x22,0x22, 0x12,0x33,0x23,0x32, 0x12,0x43,0x24,0x32, 0x11,0x23,0x22,0x32, 0x11,0x12,0x21,0x22, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[35] cell:3x2 
{
8,8, 0x12,0x22,0x22,0x21, 0x22,0x32,0x23,0x22, 0x23,0x33,0x33,0x32, 0x22,0x32,0x23,0x22, 0x22,0x32,0x23,0x22, 0x23,0x33,0x33,0x32, 0x22,0x32,0x23,0x22, 0x12,0x22,0x22,0x21, 
},
//[36] cell:4x2 
{
8,8, 0x11,0x22,0x32,0x22, 0x12,0x43,0x33,0x32, 0x12,0x32,0x32,0x22, 0x12,0x43,0x33,0x42, 0x12,0x22,0x32,0x32, 0x12,0x33,0x33,0x42, 0x12,0x22,0x32,0x21, 0x11,0x12,0x22,0x11, 
},
//[37] cell:5x2 
{
8,8, 0x12,0x22,0x11,0x22, 0x24,0x34,0x22,0x32, 0x23,0x23,0x23,0x21, 0x24,0x34,0x32,0x21, 0x12,0x23,0x43,0x42, 0x12,0x32,0x32,0x32, 0x23,0x22,0x43,0x42, 0x22,0x11,0x22,0x21, 
},
//[38] cell:6x2 
{
8,8, 0x11,0x22,0x22,0x11, 0x12,0x43,0x34,0x21, 0x12,0x32,0x23,0x21, 0x22,0x33,0x22,0x21, 0x23,0x42,0x23,0x22, 0x23,0x22,0x43,0x32, 0x24,0x33,0x32,0x22, 0x12,0x22,0x22,0x11, 
},
//[39] cell:7x2 
{
8,8, 0x11,0x22,0x22,0x11, 0x11,0x23,0x32,0x11, 0x11,0x24,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x11,0x22,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[40] cell:8x2 
{
8,8, 0x11,0x12,0x22,0x11, 0x11,0x24,0x32,0x11, 0x11,0x23,0x42,0x11, 0x11,0x23,0x21,0x11, 0x11,0x23,0x21,0x11, 0x11,0x23,0x42,0x11, 0x11,0x24,0x32,0x11, 0x11,0x12,0x22,0x11, 
},
//[41] cell:9x2 
{
8,8, 0x11,0x22,0x21,0x11, 0x11,0x23,0x42,0x11, 0x11,0x24,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x24,0x32,0x11, 0x11,0x23,0x42,0x11, 0x11,0x22,0x21,0x11, 
},
//[42] cell:10x2 
{
8,8, 0x12,0x21,0x11,0x22, 0x12,0x32,0x12,0x32, 0x11,0x23,0x23,0x21, 0x11,0x12,0x32,0x11, 0x11,0x23,0x23,0x21, 0x12,0x32,0x12,0x32, 0x12,0x21,0x11,0x22, 0x11,0x11,0x11,0x11, 
},
//[43] cell:11x2 
{
8,8, 0x11,0x12,0x22,0x11, 0x11,0x12,0x32,0x11, 0x12,0x22,0x32,0x22, 0x12,0x33,0x33,0x32, 0x12,0x22,0x32,0x22, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 0x11,0x11,0x11,0x11, 
},
//[44] cell:12x2 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x22,0x22,0x11, 0x11,0x23,0x32,0x11, 0x11,0x24,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x11,0x22,0x11, 
},
//[45] cell:13x2 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x12,0x22,0x22,0x22, 0x12,0x33,0x33,0x32, 0x12,0x22,0x22,0x22, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[46] cell:14x2 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x12,0x22,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 0x11,0x11,0x11,0x11, 
},
//[47] cell:15x2 
{
8,8, 0x11,0x11,0x11,0x22, 0x11,0x11,0x12,0x32, 0x11,0x11,0x23,0x21, 0x11,0x12,0x32,0x11, 0x11,0x23,0x21,0x11, 0x12,0x32,0x11,0x11, 0x23,0x21,0x11,0x11, 0x22,0x11,0x11,0x11, 
},
//[48] cell:0x3 
{
8,8, 0x12,0x22,0x22,0x21, 0x24,0x33,0x33,0x42, 0x23,0x42,0x43,0x32, 0x23,0x21,0x23,0x32, 0x23,0x21,0x23,0x32, 0x23,0x42,0x43,0x32, 0x24,0x33,0x33,0x42, 0x12,0x22,0x22,0x21, 
},
//[49] cell:1x3 
{
8,8, 0x12,0x22,0x22,0x11, 0x12,0x33,0x32,0x11, 0x12,0x23,0x32,0x11, 0x11,0x23,0x32,0x11, 0x11,0x23,0x32,0x11, 0x11,0x23,0x32,0x11, 0x11,0x23,0x32,0x11, 0x11,0x22,0x22,0x11, 
},
//[50] cell:2x3 
{
8,8, 0x22,0x22,0x22,0x21, 0x23,0x33,0x33,0x42, 0x22,0x22,0x23,0x32, 0x24,0x33,0x33,0x42, 0x23,0x34,0x22,0x21, 0x23,0x32,0x22,0x22, 0x23,0x33,0x33,0x32, 0x22,0x22,0x22,0x22, 
},
//[51] cell:3x3 
{
8,8, 0x22,0x22,0x22,0x11, 0x23,0x33,0x34,0x21, 0x22,0x22,0x33,0x21, 0x23,0x33,0x33,0x42, 0x22,0x22,0x43,0x32, 0x22,0x22,0x43,0x32, 0x23,0x33,0x33,0x42, 0x22,0x22,0x22,0x21, 
},
//[52] cell:4x3 
{
8,8, 0x22,0x22,0x22,0x21, 0x23,0x22,0x33,0x21, 0x23,0x22,0x33,0x21, 0x23,0x22,0x33,0x21, 0x23,0x22,0x33,0x22, 0x23,0x33,0x33,0x32, 0x22,0x22,0x33,0x22, 0x11,0x12,0x22,0x21, 
},
//[53] cell:5x3 
{
8,8, 0x22,0x22,0x22,0x21, 0x23,0x33,0x33,0x21, 0x23,0x22,0x22,0x21, 0x23,0x33,0x33,0x42, 0x22,0x22,0x43,0x32, 0x22,0x22,0x43,0x32, 0x23,0x33,0x33,0x42, 0x22,0x22,0x22,0x21, 
},
//[54] cell:6x3 
{
8,8, 0x12,0x22,0x22,0x21, 0x24,0x33,0x33,0x21, 0x23,0x22,0x22,0x21, 0x23,0x33,0x33,0x42, 0x23,0x22,0x43,0x32, 0x23,0x22,0x43,0x32, 0x24,0x33,0x33,0x42, 0x12,0x22,0x22,0x21, 
},
//[55] cell:7x3 
{
8,8, 0x22,0x22,0x22,0x22, 0x23,0x33,0x33,0x32, 0x22,0x22,0x23,0x32, 0x11,0x11,0x23,0x32, 0x11,0x11,0x23,0x32, 0x11,0x11,0x23,0x32, 0x11,0x11,0x23,0x32, 0x11,0x11,0x22,0x22, 
},
//[56] cell:8x3 
{
8,8, 0x12,0x22,0x22,0x21, 0x24,0x33,0x33,0x42, 0x23,0x22,0x43,0x32, 0x24,0x33,0x33,0x42, 0x23,0x22,0x43,0x32, 0x23,0x22,0x43,0x32, 0x24,0x33,0x33,0x42, 0x12,0x22,0x22,0x21, 
},
//[57] cell:9x3 
{
8,8, 0x12,0x22,0x22,0x21, 0x24,0x33,0x33,0x42, 0x23,0x22,0x43,0x32, 0x23,0x22,0x43,0x32, 0x24,0x33,0x33,0x32, 0x12,0x22,0x23,0x32, 0x12,0x33,0x33,0x42, 0x12,0x22,0x22,0x21, 
},
//[58] cell:10x3 
{
8,8, 0x11,0x22,0x22,0x11, 0x11,0x23,0x32,0x11, 0x11,0x23,0x32,0x11, 0x11,0x22,0x22,0x11, 0x11,0x22,0x22,0x11, 0x11,0x23,0x32,0x11, 0x11,0x23,0x32,0x11, 0x11,0x22,0x22,0x11, 
},
//[59] cell:11x3 
{
8,8, 0x11,0x22,0x22,0x11, 0x11,0x23,0x32,0x11, 0x11,0x23,0x32,0x11, 0x11,0x22,0x22,0x11, 0x11,0x23,0x32,0x11, 0x11,0x24,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x11,0x22,0x11, 
},
//[60] cell:12x3 
{
8,8, 0x11,0x11,0x23,0x21, 0x11,0x12,0x32,0x11, 0x11,0x23,0x21,0x11, 0x12,0x32,0x11,0x11, 0x11,0x23,0x21,0x11, 0x11,0x12,0x32,0x11, 0x11,0x11,0x23,0x21, 0x11,0x11,0x12,0x21, 
},
//[61] cell:13x3 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x22,0x22, 0x12,0x33,0x33,0x32, 0x12,0x22,0x22,0x22, 0x12,0x33,0x33,0x32, 0x12,0x22,0x22,0x22, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[62] cell:14x3 
{
8,8, 0x12,0x32,0x11,0x11, 0x11,0x23,0x21,0x11, 0x11,0x12,0x32,0x11, 0x11,0x11,0x23,0x21, 0x11,0x12,0x32,0x11, 0x11,0x23,0x21,0x11, 0x12,0x32,0x11,0x11, 0x12,0x21,0x11,0x11, 
},
//[63] cell:15x3 
{
8,8, 0x11,0x22,0x22,0x21, 0x12,0x43,0x33,0x42, 0x12,0x32,0x22,0x32, 0x12,0x32,0x22,0x32, 0x12,0x22,0x33,0x42, 0x11,0x12,0x22,0x21, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 
},
//[64] cell:0x4 
{
8,8, 0x12,0x33,0x34,0x21, 0x23,0x22,0x23,0x42, 0x32,0x43,0x32,0x32, 0x32,0x32,0x22,0x32, 0x32,0x43,0x32,0x32, 0x43,0x22,0x23,0x42, 0x24,0x33,0x34,0x22, 0x12,0x22,0x22,0x21, 
},
//[65] cell:1x4 
{
8,8, 0x12,0x22,0x22,0x21, 0x24,0x33,0x33,0x42, 0x23,0x42,0x24,0x32, 0x23,0x22,0x22,0x32, 0x23,0x33,0x33,0x32, 0x23,0x22,0x22,0x32, 0x23,0x21,0x12,0x32, 0x22,0x21,0x12,0x22, 
},
//[66] cell:2x4 
{
8,8, 0x22,0x22,0x22,0x21, 0x23,0x33,0x33,0x42, 0x23,0x22,0x24,0x32, 0x23,0x33,0x33,0x22, 0x23,0x22,0x24,0x32, 0x23,0x22,0x24,0x32, 0x23,0x33,0x33,0x42, 0x22,0x22,0x22,0x21, 
},
//[67] cell:3x4 
{
8,8, 0x12,0x22,0x22,0x22, 0x24,0x33,0x33,0x32, 0x23,0x42,0x22,0x22, 0x23,0x21,0x11,0x11, 0x23,0x21,0x11,0x11, 0x23,0x42,0x22,0x22, 0x24,0x33,0x33,0x32, 0x12,0x22,0x22,0x22, 
},
//[68] cell:4x4 
{
8,8, 0x22,0x22,0x22,0x21, 0x23,0x33,0x33,0x42, 0x23,0x22,0x24,0x32, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x23,0x22,0x24,0x32, 0x23,0x33,0x33,0x42, 0x22,0x22,0x22,0x21, 
},
//[69] cell:5x4 
{
8,8, 0x22,0x22,0x22,0x22, 0x23,0x33,0x33,0x32, 0x23,0x22,0x22,0x22, 0x23,0x33,0x33,0x32, 0x23,0x22,0x22,0x22, 0x23,0x22,0x22,0x22, 0x23,0x33,0x33,0x32, 0x22,0x22,0x22,0x22, 
},
//[70] cell:6x4 
{
8,8, 0x22,0x22,0x22,0x22, 0x23,0x33,0x33,0x32, 0x23,0x22,0x22,0x22, 0x23,0x33,0x33,0x32, 0x23,0x22,0x22,0x22, 0x23,0x21,0x11,0x11, 0x23,0x21,0x11,0x11, 0x22,0x11,0x11,0x11, 
},
//[71] cell:7x4 
{
8,8, 0x12,0x22,0x22,0x22, 0x24,0x33,0x33,0x32, 0x23,0x42,0x22,0x22, 0x23,0x22,0x33,0x32, 0x23,0x22,0x22,0x32, 0x23,0x42,0x22,0x32, 0x24,0x33,0x33,0x32, 0x12,0x22,0x22,0x22, 
},
//[72] cell:8x4 
{
8,8, 0x22,0x21,0x12,0x22, 0x23,0x21,0x12,0x32, 0x23,0x22,0x22,0x32, 0x23,0x33,0x33,0x32, 0x23,0x22,0x22,0x32, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x22,0x21,0x12,0x22, 
},
//[73] cell:9x4 
{
8,8, 0x11,0x22,0x22,0x21, 0x11,0x23,0x33,0x21, 0x11,0x22,0x32,0x21, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x22,0x32,0x21, 0x11,0x23,0x33,0x21, 0x11,0x22,0x22,0x21, 
},
//[74] cell:10x4 
{
8,8, 0x11,0x12,0x22,0x22, 0x11,0x12,0x33,0x32, 0x11,0x12,0x23,0x22, 0x22,0x21,0x23,0x21, 0x23,0x21,0x23,0x21, 0x23,0x42,0x43,0x21, 0x24,0x33,0x34,0x21, 0x12,0x22,0x22,0x11, 
},
//[75] cell:11x4 
{
8,8, 0x22,0x21,0x12,0x22, 0x23,0x21,0x24,0x32, 0x23,0x22,0x43,0x21, 0x23,0x24,0x32,0x11, 0x23,0x43,0x34,0x21, 0x23,0x32,0x23,0x42, 0x23,0x21,0x12,0x32, 0x22,0x21,0x11,0x22, 
},
//[76] cell:12x4 
{
8,8, 0x22,0x21,0x11,0x11, 0x23,0x21,0x11,0x11, 0x23,0x21,0x11,0x11, 0x23,0x21,0x11,0x11, 0x23,0x21,0x11,0x11, 0x23,0x22,0x22,0x22, 0x23,0x33,0x33,0x32, 0x22,0x22,0x22,0x22, 
},
//[77] cell:13x4 
{
8,8, 0x22,0x11,0x11,0x22, 0x23,0x21,0x12,0x32, 0x23,0x32,0x23,0x32, 0x23,0x23,0x32,0x32, 0x23,0x22,0x22,0x32, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x22,0x21,0x12,0x22, 
},
//[78] cell:14x4 
{
8,8, 0x22,0x11,0x12,0x22, 0x23,0x21,0x12,0x32, 0x23,0x32,0x12,0x32, 0x23,0x23,0x22,0x32, 0x23,0x22,0x32,0x32, 0x23,0x21,0x23,0x32, 0x23,0x21,0x12,0x32, 0x22,0x21,0x11,0x22, 
},
//[79] cell:15x4 
{
8,8, 0x12,0x22,0x22,0x21, 0x24,0x33,0x33,0x42, 0x23,0x42,0x24,0x32, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x23,0x42,0x24,0x32, 0x24,0x33,0x33,0x42, 0x12,0x22,0x22,0x21, 
},
//[80] cell:0x5 
{
8,8, 0x22,0x22,0x22,0x21, 0x23,0x33,0x33,0x42, 0x23,0x22,0x24,0x32, 0x23,0x22,0x24,0x32, 0x23,0x33,0x33,0x42, 0x23,0x22,0x22,0x21, 0x23,0x21,0x11,0x11, 0x22,0x21,0x11,0x11, 
},
//[81] cell:1x5 
{
8,8, 0x12,0x22,0x22,0x21, 0x24,0x33,0x33,0x42, 0x23,0x42,0x24,0x32, 0x23,0x22,0x22,0x32, 0x23,0x22,0x32,0x32, 0x23,0x42,0x23,0x42, 0x24,0x33,0x34,0x32, 0x12,0x22,0x22,0x22, 
},
//[82] cell:2x5 
{
8,8, 0x22,0x22,0x22,0x21, 0x23,0x33,0x33,0x42, 0x23,0x22,0x24,0x32, 0x23,0x22,0x24,0x32, 0x23,0x33,0x33,0x42, 0x23,0x22,0x43,0x21, 0x23,0x21,0x24,0x32, 0x22,0x21,0x12,0x22, 
},
//[83] cell:3x5 
{
8,8, 0x12,0x22,0x22,0x22, 0x24,0x33,0x33,0x32, 0x23,0x42,0x22,0x22, 0x24,0x33,0x33,0x42, 0x12,0x22,0x24,0x32, 0x22,0x22,0x24,0x32, 0x23,0x33,0x33,0x42, 0x22,0x22,0x22,0x21, 
},
//[84] cell:4x5 
{
8,8, 0x12,0x22,0x22,0x22, 0x12,0x33,0x33,0x32, 0x12,0x22,0x32,0x22, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 
},
//[85] cell:5x5 
{
8,8, 0x22,0x21,0x12,0x22, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x23,0x42,0x24,0x32, 0x24,0x33,0x33,0x42, 0x12,0x22,0x22,0x21, 
},
//[86] cell:6x5 
{
8,8, 0x22,0x21,0x12,0x22, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x12,0x32,0x23,0x21, 0x12,0x34,0x43,0x21, 0x11,0x23,0x32,0x11, 0x11,0x12,0x21,0x11, 
},
//[87] cell:7x5 
{
8,8, 0x22,0x21,0x12,0x22, 0x23,0x21,0x12,0x32, 0x23,0x21,0x12,0x32, 0x23,0x22,0x22,0x32, 0x23,0x23,0x32,0x32, 0x23,0x32,0x23,0x32, 0x23,0x21,0x12,0x32, 0x22,0x11,0x11,0x22, 
},
//[88] cell:8x5 
{
8,8, 0x22,0x11,0x11,0x22, 0x23,0x21,0x12,0x32, 0x24,0x32,0x23,0x42, 0x12,0x43,0x34,0x21, 0x12,0x43,0x34,0x21, 0x24,0x32,0x23,0x42, 0x23,0x21,0x12,0x32, 0x22,0x11,0x11,0x22, 
},
//[89] cell:9x5 
{
8,8, 0x12,0x21,0x11,0x22, 0x12,0x32,0x12,0x32, 0x12,0x43,0x23,0x42, 0x11,0x24,0x34,0x21, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 
},
//[90] cell:10x5 
{
8,8, 0x22,0x22,0x22,0x22, 0x23,0x33,0x33,0x32, 0x22,0x22,0x23,0x21, 0x11,0x12,0x32,0x11, 0x11,0x23,0x21,0x11, 0x12,0x32,0x22,0x22, 0x23,0x33,0x33,0x32, 0x22,0x22,0x22,0x22, 
},
//[91] cell:11x5 
{
8,8, 0x11,0x22,0x22,0x11, 0x11,0x23,0x32,0x11, 0x11,0x23,0x21,0x11, 0x11,0x23,0x21,0x11, 0x11,0x23,0x21,0x11, 0x11,0x23,0x21,0x11, 0x11,0x23,0x32,0x11, 0x11,0x22,0x22,0x11, 
},
//[92] cell:12x5 
{
8,8, 0x22,0x11,0x11,0x11, 0x23,0x21,0x11,0x11, 0x12,0x32,0x11,0x11, 0x11,0x23,0x21,0x11, 0x11,0x12,0x32,0x11, 0x11,0x11,0x23,0x21, 0x11,0x11,0x12,0x32, 0x11,0x11,0x11,0x22, 
},
//[93] cell:13x5 
{
8,8, 0x11,0x22,0x22,0x11, 0x11,0x23,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x23,0x32,0x11, 0x11,0x22,0x22,0x11, 
},
//[94] cell:14x5 
{
8,8, 0x11,0x11,0x21,0x11, 0x11,0x12,0x32,0x11, 0x11,0x23,0x23,0x21, 0x12,0x32,0x12,0x32, 0x12,0x21,0x11,0x22, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[95] cell:15x5 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x22,0x22,0x22,0x22, 0x23,0x33,0x33,0x32, 0x22,0x22,0x22,0x22, 
},
//[96] cell:0x6 
{
8,8, 0x11,0x22,0x22,0x11, 0x11,0x23,0x32,0x11, 0x11,0x23,0x42,0x11, 0x11,0x23,0x21,0x11, 0x11,0x22,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[97] cell:1x6 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x22,0x22,0x21, 0x11,0x23,0x33,0x42, 0x11,0x22,0x24,0x32, 0x12,0x43,0x33,0x32, 0x12,0x32,0x22,0x32, 0x12,0x43,0x33,0x32, 0x11,0x22,0x22,0x22, 
},
//[98] cell:2x6 
{
8,8, 0x12,0x22,0x11,0x11, 0x12,0x32,0x11,0x11, 0x12,0x32,0x22,0x21, 0x12,0x34,0x33,0x42, 0x12,0x33,0x22,0x32, 0x12,0x32,0x22,0x32, 0x12,0x33,0x33,0x42, 0x12,0x22,0x22,0x21, 
},
//[99] cell:3x6 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x22,0x22,0x22, 0x12,0x43,0x33,0x32, 0x12,0x32,0x22,0x22, 0x12,0x32,0x11,0x11, 0x12,0x32,0x22,0x22, 0x12,0x43,0x33,0x32, 0x11,0x22,0x22,0x22, 
},
//[100] cell:4x6 
{
8,8, 0x11,0x11,0x12,0x22, 0x11,0x11,0x12,0x32, 0x11,0x22,0x22,0x32, 0x12,0x43,0x34,0x32, 0x12,0x32,0x23,0x32, 0x12,0x32,0x22,0x32, 0x12,0x43,0x33,0x32, 0x11,0x22,0x22,0x22, 
},
//[101] cell:5x6 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x22,0x22,0x21, 0x12,0x43,0x33,0x42, 0x12,0x32,0x22,0x32, 0x12,0x33,0x33,0x42, 0x12,0x32,0x22,0x22, 0x12,0x43,0x33,0x32, 0x11,0x22,0x22,0x22, 
},
//[102] cell:6x6 
{
8,8, 0x11,0x12,0x22,0x21, 0x11,0x24,0x33,0x21, 0x12,0x23,0x22,0x21, 0x12,0x33,0x33,0x21, 0x12,0x23,0x22,0x21, 0x11,0x23,0x21,0x11, 0x11,0x23,0x21,0x11, 0x11,0x22,0x21,0x11, 
},
//[103] cell:7x6 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x22,0x22,0x22, 0x12,0x43,0x33,0x32, 0x12,0x32,0x22,0x32, 0x12,0x43,0x33,0x32, 0x12,0x22,0x22,0x32, 0x12,0x33,0x33,0x42, 0x12,0x22,0x22,0x21, 
},
//[104] cell:8x6 
{
8,8, 0x12,0x22,0x11,0x11, 0x12,0x32,0x11,0x11, 0x12,0x32,0x22,0x21, 0x12,0x34,0x33,0x42, 0x12,0x33,0x22,0x32, 0x12,0x32,0x12,0x32, 0x12,0x32,0x12,0x32, 0x12,0x22,0x12,0x22, 
},
//[105] cell:9x6 
{
8,8, 0x11,0x12,0x22,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 
},
//[106] cell:10x6 
{
8,8, 0x11,0x11,0x22,0x21, 0x11,0x11,0x23,0x21, 0x11,0x11,0x22,0x21, 0x11,0x11,0x23,0x21, 0x12,0x22,0x23,0x21, 0x12,0x32,0x23,0x21, 0x12,0x43,0x34,0x21, 0x11,0x22,0x22,0x11, 
},
//[107] cell:11x6 
{
8,8, 0x12,0x22,0x11,0x11, 0x12,0x32,0x11,0x22, 0x12,0x32,0x12,0x32, 0x12,0x32,0x23,0x21, 0x12,0x32,0x32,0x11, 0x12,0x33,0x43,0x21, 0x12,0x32,0x24,0x32, 0x12,0x22,0x12,0x22, 
},
//[108] cell:12x6 
{
8,8, 0x11,0x22,0x21,0x11, 0x11,0x23,0x42,0x11, 0x11,0x22,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x21, 0x11,0x12,0x33,0x21, 0x11,0x12,0x22,0x21, 
},
//[109] cell:13x6 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x22,0x21, 0x12,0x33,0x43,0x42, 0x12,0x32,0x32,0x32, 0x12,0x32,0x32,0x32, 0x12,0x32,0x32,0x32, 0x12,0x32,0x32,0x32, 0x12,0x22,0x22,0x22, 
},
//[110] cell:14x6 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x22,0x21, 0x12,0x34,0x33,0x42, 0x12,0x33,0x22,0x32, 0x12,0x32,0x12,0x32, 0x12,0x32,0x12,0x32, 0x12,0x32,0x12,0x32, 0x12,0x22,0x12,0x22, 
},
//[111] cell:15x6 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x22,0x22,0x21, 0x12,0x43,0x33,0x42, 0x12,0x32,0x22,0x32, 0x12,0x32,0x12,0x32, 0x12,0x32,0x22,0x32, 0x12,0x43,0x33,0x42, 0x11,0x22,0x22,0x21, 
},
//[112] cell:0x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x22,0x21, 0x12,0x33,0x33,0x42, 0x12,0x32,0x22,0x32, 0x12,0x33,0x22,0x32, 0x12,0x34,0x33,0x42, 0x12,0x32,0x22,0x21, 0x12,0x22,0x11,0x11, 
},
//[113] cell:1x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x22,0x22,0x22, 0x12,0x43,0x33,0x32, 0x12,0x32,0x22,0x32, 0x12,0x32,0x23,0x32, 0x12,0x43,0x34,0x32, 0x11,0x22,0x22,0x32, 0x11,0x11,0x12,0x22, 
},
//[114] cell:2x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x11,0x11, 0x12,0x32,0x22,0x21, 0x12,0x34,0x33,0x42, 0x12,0x33,0x22,0x32, 0x12,0x32,0x12,0x22, 0x12,0x32,0x11,0x11, 0x12,0x22,0x11,0x11, 
},
//[115] cell:3x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x22,0x22,0x22, 0x12,0x43,0x33,0x32, 0x12,0x32,0x22,0x22, 0x12,0x43,0x33,0x42, 0x12,0x22,0x22,0x32, 0x12,0x33,0x33,0x42, 0x12,0x22,0x22,0x21, 
},
//[116] cell:4x7 
{
8,8, 0x11,0x12,0x22,0x11, 0x12,0x22,0x32,0x22, 0x12,0x33,0x33,0x32, 0x12,0x22,0x32,0x22, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x22, 0x11,0x12,0x43,0x32, 0x11,0x11,0x22,0x22, 
},
//[117] cell:5x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x12,0x22, 0x12,0x32,0x12,0x32, 0x12,0x32,0x12,0x32, 0x12,0x32,0x12,0x32, 0x12,0x32,0x23,0x32, 0x12,0x43,0x34,0x32, 0x11,0x22,0x22,0x22, 
},
//[118] cell:6x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x12,0x22, 0x12,0x32,0x12,0x32, 0x12,0x43,0x23,0x42, 0x11,0x23,0x23,0x21, 0x11,0x24,0x34,0x21, 0x11,0x12,0x32,0x11, 0x11,0x11,0x21,0x11, 
},
//[119] cell:7x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x22,0x22, 0x12,0x32,0x32,0x32, 0x12,0x32,0x32,0x32, 0x12,0x34,0x34,0x32, 0x11,0x23,0x43,0x21, 0x11,0x23,0x23,0x21, 0x11,0x12,0x12,0x11, 
},
//[120] cell:8x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x21,0x11,0x22, 0x12,0x32,0x12,0x32, 0x12,0x43,0x23,0x42, 0x11,0x24,0x34,0x21, 0x12,0x43,0x23,0x42, 0x12,0x32,0x12,0x32, 0x12,0x21,0x11,0x22, 
},
//[121] cell:9x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x12,0x22, 0x12,0x32,0x12,0x32, 0x12,0x43,0x23,0x42, 0x11,0x24,0x34,0x21, 0x11,0x23,0x42,0x11, 0x12,0x34,0x21,0x11, 0x12,0x22,0x11,0x11, 
},
//[122] cell:10x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x12,0x22,0x22,0x22, 0x12,0x33,0x33,0x32, 0x12,0x22,0x23,0x21, 0x11,0x12,0x32,0x11, 0x11,0x23,0x22,0x22, 0x12,0x33,0x33,0x32, 0x12,0x22,0x22,0x22, 
},
//[123] cell:11x7 
{
8,8, 0x11,0x12,0x32,0x11, 0x11,0x23,0x22,0x11, 0x11,0x23,0x21,0x11, 0x12,0x32,0x11,0x11, 0x11,0x23,0x21,0x11, 0x11,0x23,0x21,0x11, 0x11,0x12,0x32,0x11, 0x11,0x11,0x22,0x11, 
},
//[124] cell:12x7 
{
8,8, 0x11,0x12,0x22,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x12,0x22,0x11, 0x11,0x11,0x11,0x11, 
},
//[125] cell:13x7 
{
8,8, 0x11,0x23,0x21,0x11, 0x11,0x22,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x11,0x23,0x21, 0x11,0x12,0x32,0x11, 0x11,0x12,0x32,0x11, 0x11,0x23,0x21,0x11, 0x11,0x22,0x11,0x11, 
},
//[126] cell:14x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x22,0x11,0x21, 0x12,0x33,0x22,0x32, 0x23,0x22,0x33,0x21, 0x12,0x11,0x22,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
//[127] cell:15x7 
{
8,8, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 0x11,0x11,0x11,0x11, 
},
};
