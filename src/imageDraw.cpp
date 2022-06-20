#include "ImageDraw.h"

void drawImage(Adafruit_RA8875 &tft, uint16_t (&Image)[64][64]){

  tft.graphicsMode();
  uint16_t color;
  for (int i=0;i<64;i++){
    for(int j = 0;j<64;j++){
      
      color = (Image[i][j]);

      tft.fillRect((5*i)+50,(5*j)+50,5,5,color);
    }
  }
  
}
