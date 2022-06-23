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
#define countCLKSPD 10000
#define CLKSPDper ((1000000.0)/countCLKSPD)
#define sd_cs 254

#define NormalRun
//#define SinglePixel

void setup()
{




  pinMode(PD,OUTPUT);
  digitalWrite(PD,LOW);
  
  pinMode(CLK_HS,OUTPUT);
  analogWriteResolution(3);
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
    tft.textWrite("Sensor Text A0.7.0b",20);
    tft.graphicsMode();
  #else
    tft.textWrite("TEST MODE",10);
  #endif
  

  
}

#define ImgTime 100000
unsigned long currentTime = 0;
unsigned long lastImg = 0;
unsigned long sum = 0;
unsigned long cnt = 0;

#ifdef NormalRun
unsigned long diff = 0;
#else
long diff = 0;
#endif

unsigned long lastS = 0;
unsigned long val = 0;



void loop()
{
  #ifdef NormalRun
    currentTime = micros();
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
   val = readRow();
   val = val && readColumn();
    if(val != 0xFFFF){
      sum += val;
      cnt += 1;
      lastS = currentTime;
      incrementS(false);
    }
    else{
      incrementS(true);
    }
  #elif defined(SinglePixel)
    currentTime = micros();
    float TimeSinceChange = 0;
    val = readRow();

    if (val == 0xFFFF){
      val = 0;
    }
    else{
      if(val != readRow()){
        val = 0;
      }
    }
    if(sum != val){
      TimeSinceChange = currentTime-lastImg;
      // diff = (diff-val);
      // if(diff<0){
      //   diff = diff + 1024;
      // }
      if(val != 0){
        diff = val;
      }
      

      SerialUSB1.print(diff);
      SerialUSB1.print("\n");
      
      lastImg = currentTime;
    }
    sum = val;
    
    
  #endif

}
