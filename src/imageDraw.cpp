#include "ImageDraw.h"

uint16_t IMG_min[64][64];
uint16_t IMG_max[64][64];

void setupImgMax(){
  for(int i=0; i<64;i++){
    for(int j=0;j<64;j++){
      IMG_min[i][j] = 0xFFFF;
      IMG_max[i][j] = 0x0000;
    }
  }
}

void drawImage(Adafruit_RA8875 &tft, uint16_t (&Image)[64][64]){
  unsigned long sum = 0;
  tft.graphicsMode();
  uint16_t color;

  uint16_t bw;
  uint16_t range;
  float frac;
  
  for (int i=0;i<64;i++){
    for(int j = 0;j<64;j++){
      
      if(Image[i][j] < IMG_min[i][j]){
        IMG_min[i][j] = Image[i][j];
      }
      if(Image[i][j] > IMG_max[i][j]){
        IMG_max[i][j] = Image[i][j];
      }     
      //color = ((Image[i][j]));
      range = IMG_max[i][j] - IMG_min[i][j];
      frac= (Image[i][j]-IMG_min[i][j])/(range*0.5);
      color = (uint16_t)(frac * 32);

      sum += color;

      //Serial.println(color);
      bw = ((color & 0x1F) << 11) | ((color & 0x1F)<< 6) | (color & 0x1F);
      Image[i][j] = 32- bw;
      //tft.fillRect((5*i)+50,(5*j)+50,5,5,bw); // Does a large rectangle for each pixel 
      //tft.drawPixel((i)+50,(j)+50,bw);
    }
    
    tft.drawPixels(&Image[i][0],64,50,50+i);
  }
  //This Serial Print can be used to confirm the overall light level ia being detected
  SerialUSB1.println(sum/4096.0);
  
}
