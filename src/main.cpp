#include <Arduino.h>
#include <Adafruit_RA8875.h>
#include <Adafruit_GFX.h>

#include "imageDraw.h"
#include "readImage.h"
#include <SPI.h>

#define RA8875_LITE  5
#define RA8875_WAIT 6
#define RA8875_RST 7

#define RA8875_CS 10
#define RA8875_MOSI 11
#define RA8875_MISO 12
#define RA8875_SCLK 13

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RST);

#define PD 15
#define RST_N 16
//#define CLK_HS 17
#define CLK_HS 23
#define countCLKSPD 420000
#define CLKSPDper ((1000000.0)/countCLKSPD)
#define sd_cs 254

#define NormalRun
//#define SinglePixel
//#define SlowImage
//#define FreqImage

void setup()
{




  pinMode(PD,OUTPUT);
  digitalWrite(PD,LOW);
  
  pinMode(CLK_HS,OUTPUT);
  analogWriteResolution(4);
  analogWriteFrequency(CLK_HS, countCLKSPD);
  analogWrite(CLK_HS,5);

  setupChip();
  setupImgMax();



  // if (!SD.begin(sd_cs))
  // {
  //   Serial.println("initialization failed!");
  //   return;
  // }
  Serial.begin(115200);
  SerialUSB1.begin(115200);
  SerialUSB2.begin(115200);
  // Serial.println("Primary Serial");
  // SerialUSB1.println("Serial USB 1");
  // SerialUSB2.println("Serial USB 2");

  //Serial.println("RA8875 start");
   if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  tft.displayOn(true);
  tft.GPIOX(true);
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

  tft.fillScreen(RA8875_WHITE);
  
  tft.textMode();
  #ifdef NormalRun
    tft.textWrite("Sensor Text A0.8.0b",20);
    tft.graphicsMode();
  #else
    tft.textWrite("TEST MODE",10);
  #endif
  

  
}

#define ImgTime 100000
unsigned long currentTime = 0;
unsigned long lastImg = 0;
unsigned long sum;
unsigned long cnt = 0;

#ifdef SlowImage
  uint8_t Xcor = 0;
  uint8_t Ycor = 0;
  uint8_t Average = 200;
  uint16_t PrevVal=0;
  uint16_t val = 0;
  #define AveCnt 50
  long diff = 0;
#endif

#ifdef NormalRun
unsigned long diff = 0;
#elif defined(SinglePixel)
long diff = 0;
uint8_t Xcor = 32;
uint8_t Ycor = 32;
unsigned long val;
#elif defined(FreqImage)

uint16_t val = 0;
uint32_t freq = 1000;
uint8_t Xcor = 32;
uint8_t Ycor = 32;

unsigned long lastTick;

#endif

unsigned long lastS = 0;




void loop()
{
  currentTime = micros();
  #ifdef NormalRun
    
    diff = (currentTime-lastImg);
    if(diff>ImgTime){
    
      lastImg = currentTime;
      drawImage(tft,getImageRef());
      //Serial.println((micros()-lastImg)); //Get the time it takes to draw the image
    }
    // if(currentTime- lastS> 900){
    //   lastS = currentTime;
    //   incrementS();
    // }
   readRow();
   readColumn();


  lastS = currentTime;
  incrementS(false);

  #elif defined(SinglePixel)
    setCoorR(Xcor,Ycor);
    while(val != readRowDumb()){
      val = readRowDumb();
      delay(1);
    }
    if(sum != val){

      if(val != 0){
        diff = val;
      }
      

        SerialUSB1.print(Xcor);
        SerialUSB1.print(",");
        SerialUSB1.print(Ycor);
        SerialUSB1.print(",");
        SerialUSB1.print(diff);
        // SerialUSB1.print(",");
        // SerialUSB1.print(cnt);
        SerialUSB1.print("\n");
      
      
    }
    sum = val;

    if(currentTime - lastImg > 4000){
      lastImg = currentTime;
      Xcor = Xcor + 1;
        if(Xcor >= 16){
          Xcor = 0;
          Ycor = Ycor + 1;
          if(Ycor >= 16){
            Ycor = 0;
        }
    }

    }

  #elif defined(SlowImage)  

    setCoorR(Xcor,Ycor);
    currentTime = micros();
   
    val = readRowDumb();

    if (val == 0xFFFF){
      val = 0;
    }
    else{
      if(val != readRowDumb()){
        val = 0;
      }
    }
    if(PrevVal != val){
      
      // diff = (diff-val);
      // if(diff<0){
      //   diff = diff + 1024;
      // }
      if(val != 0){
        diff = val;
        sum = sum + val;
        cnt = cnt + 1;
      }
      Average = Average - 1;
      if(Average == 0){
        Average = AveCnt;
        SerialUSB1.print(Xcor);
        SerialUSB1.print(",");
        SerialUSB1.print(Ycor);
        SerialUSB1.print(",");
        SerialUSB1.print(sum);
        SerialUSB1.print(",");
        SerialUSB1.print(cnt);
        SerialUSB1.print("\n");

        sum = 0;
        cnt = 0;

        Xcor = Xcor + 1;
        if(Xcor >= 64){
          Xcor = 0;
          Ycor = Ycor + 1;
          if(Ycor >= 64){
            Ycor = 0;
        }
    }
      }

      
      lastImg = currentTime;
    }
    PrevVal = val;
    //SerialUSB1.print("");

    // SerialUSB1.print(pixelValue);
    // SerialUSB1.print("\n");

  #elif defined(FreqImage)
  setCoorR(Xcor,Ycor);
  analogWriteFrequency(CLK_HS, freq);
  while(val != readRowDumb()){
    val = readRowDumb();
  }
  //if(val != 0){
    if(val > 41){
      freq = freq - 5;
    }
    else if(val < 39){
      freq = freq + 5;
    }
    else{
        lastTick = currentTime;
        SerialUSB1.print(Xcor);
        SerialUSB1.print(",");
        SerialUSB1.print(Ycor);
        SerialUSB1.print(",");
        SerialUSB1.print(freq);
        SerialUSB1.print(",");
        SerialUSB1.print(val);
        SerialUSB1.print("\n");
      
      Xcor = Xcor + 1;
      if(Xcor >= 64){
        Xcor = 0;
        Ycor = Ycor + 1;
        if(Ycor >= 64){
          Ycor = 0;
        }
      }
      delay(1);
    }

    if((currentTime - lastTick) > 1000000){
      lastTick = currentTime;
        SerialUSB1.print(Xcor);
        SerialUSB1.print(",");
        SerialUSB1.print(Ycor);
        SerialUSB1.print(",");
        SerialUSB1.print(freq);
        SerialUSB1.print(",");
        SerialUSB1.print(val);
        SerialUSB1.print("\n");
      
      Xcor = Xcor + 1;
      if(Xcor >= 64){
        Xcor = 0;
        Ycor = Ycor + 1;
        if(Ycor >= 64){
          Ycor = 0;
        }
      }
    }

    delay(5);

  #endif

}
