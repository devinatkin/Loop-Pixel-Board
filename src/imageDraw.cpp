#include "ImageDraw.h"

void drawImage(Adafruit_RA8875 &tft, uint16_t (&Image)[64][64]){
  unsigned long sum = 0;
  tft.graphicsMode();
  uint16_t color;
  uint16_t bw;
  for (int i=0;i<64;i++){
    for(int j = 0;j<64;j++){
      sum += Image[i][j];
      color = ((Image[i][j])/32);

      bw = ((color & 0x1F) << 11) | ((color & 0x1F)<< 6) | (color & 0x1F);
      tft.fillRect((5*i)+50,(5*j)+50,5,5,bw);
    }
  }
  Serial.println(sum);
  
}
