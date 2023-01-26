#include "readImage.h"

uint8_t XC = 0;
uint8_t YC = 0;

uint8_t XR = 0;
uint8_t YR = 0;

uint8_t svals;

uint16_t IMG[64][64];
uint16_t IMGB[64][64];

picture& getImageRef(){
  return IMGB;
}

void setupColumn(){
  pinMode(C1,INPUT);
  pinMode(C2,INPUT);
  pinMode(C3,INPUT);
  pinMode(C4,INPUT);
  pinMode(C5,INPUT);
  pinMode(C6,INPUT);
  pinMode(C7,INPUT);
  pinMode(C8,INPUT);
  pinMode(C9,INPUT);
  pinMode(C10,INPUT);
}

void setupRow(){
  pinMode(R1,INPUT);
  pinMode(R2,INPUT);
  pinMode(R3,INPUT);
  pinMode(R4,INPUT);
  pinMode(R5,INPUT);
  pinMode(R6,INPUT);
  pinMode(R7,INPUT);
  pinMode(R8,INPUT);
  pinMode(R9,INPUT);
  pinMode(R10,INPUT);  
}

void setupS(){
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);
  pinMode(S4,OUTPUT);
  pinMode(S5,OUTPUT);
  pinMode(S6,OUTPUT);
}

void setS(uint8_t s){
  svals = s;
  digitalWrite(S1,svals & 0x01);
  digitalWrite(S2,svals & 0x02);
  digitalWrite(S3,svals & 0x04);
  digitalWrite(S4,svals & 0x08);
  digitalWrite(S5,svals & 0x10);
  digitalWrite(S6,svals & 0x20);

  YR = svals;
  XC = svals;
  
}

void setupCR(){
  pinMode(RS_CLK,OUTPUT);
  pinMode(CS_CLK,OUTPUT);

  pinMode(RS_INDATA,OUTPUT);
  pinMode(CS_INDATA,OUTPUT);
  
  pinMode(LOAD_CS,OUTPUT);
  pinMode(LOAD_RS,OUTPUT); 
}

uint16_t readColumn(){
  if(IMG[XC][YC] != 0){
    return 0xFFFF;
  }
  
  uint16_t col=0;
  col = col|(digitalRead(C10)<<9);
  col = col|(digitalRead(C9)<<8);
  col = col|(digitalRead(C8)<<7);
  col = col|(digitalRead(C7)<<6);
  col = col|(digitalRead(C6)<<5);
  col = col|(digitalRead(C5)<<4);
  
  col = col|(digitalRead(C4)<<3);
  col = col|(digitalRead(C3)<<2);
  col = col|(digitalRead(C2)<<1);
  col = col|(digitalRead(C1));

  if(col != 0){
    IMG[XC][YC] = col;
    return col;
  }
  else{
    return 0xFFFF;
  }

  }

#include <CircularBuffer.h>


void incrementS(bool keep){

    svals=svals+1;
    if(svals>=64){
        uint8_t temp = XR+1;
        uint8_t temp2 = YC+1;
        
        resetChip();
        loadCR();
        
        if(temp >= 64){
          temp = 0;
          SerialUSB2.print("i:");
          for(int i = 0 ; i<64;i++){
            for(int j=0;j<64;j++){
              SerialUSB2.print(IMG[i][j]);
              SerialUSB2.print(",");
              
              IMGB[i][j]=IMG[i][j];
              IMG[i][j]=0;
            }
          }
          SerialUSB2.print("\n");
        }
        if(temp2 >= 64){
          temp2 = 0;
        }
        

        svals = 0;
        while(XR != temp){
          clockRS();
        }
        while(YC != temp2){
          clockCS();
        }
      }
    
      //clockCS();
    

    setS(svals);


}

uint16_t readRow(){
  if(IMG[XR][YR] != 0){ //Dont Read when something's already there
    return 0xFFFF;
  }
  uint16_t row=0;
  row = row|(digitalRead(R10)<<9);
  row = row|(digitalRead(R9)<<8);
  row = row|(digitalRead(R8)<<7);
  row = row|(digitalRead(R7)<<6);
  row = row|(digitalRead(R6)<<5);
  row = row|(digitalRead(R5)<<4);
  
  row = row|(digitalRead(R4)<<3);
  row = row|(digitalRead(R3)<<2);
  row = row|(digitalRead(R2)<<1);
  row = row|(digitalRead(R1));
  
  //uint8_t currentS = svals;
  //setS(YC-XR);
  // while(YC != YR){
  //   clockCS();
  // }
  
  //uint16_t col = readColumn();
  //setS(currentS);
  if(row != 0){
    IMG[XR][YR] = row;
    return row;
  }
  else{
    return 0xFFFF;
  }
  
  
  
  
  
  
    }

void loadCR(){
  // Assume the register starts empty. 
  digitalWrite(RS_CLK,LOW);
  digitalWrite(CS_CLK,LOW); 

  digitalWrite(LOAD_CS,HIGH);
  digitalWrite(LOAD_RS,HIGH);

  digitalWrite(RS_INDATA,HIGH);
  digitalWrite(CS_INDATA,HIGH); 

  digitalWrite(RS_CLK,HIGH);
  digitalWrite(CS_CLK,HIGH);

  digitalWrite(LOAD_CS,LOW);
  digitalWrite(LOAD_RS,LOW);

  digitalWrite(RS_CLK,LOW);
  digitalWrite(CS_CLK,LOW);

  XR = 0;
  YC = 0; 
}

boolean clkRS;
void clockRS(){
  //Serial.println("RS");
  clkRS = !clkRS;
  if(clkRS){
    XR = XR + 1;
    if(XR >= 64){
      XR = 0;
      
    }
  }
  digitalWrite(RS_CLK,clkRS);
  
}

boolean clkCS;
void clockCS(){
  clkCS = !clkCS; 

  if(clkCS){
    YC = YC + 1;
    if(YC>=64){
      YC=0;
    }
  }
  digitalWrite(CS_CLK,clkCS);
   
}

#define RST_N 16

//Reset the chip
void resetChip(){
  digitalWrite(RST_N,LOW);
  digitalWrite(RST_N,HIGH);
  XR = 0;
  YC = 0;
}

//Run all the setup chip scripts
void setupChip(){ 
  setupColumn();
  setupRow();
  setupS();
  pinMode(RST_N,OUTPUT);
  resetChip();
  setupCR();
  loadCR();
}