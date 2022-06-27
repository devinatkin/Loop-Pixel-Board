#ifndef READ_IMAGE
#define READ_IMAGE

#include <Arduino.h>
#define R1 9
#define R2 27
#define R3 26
#define R4 25
#define R5 24
#define R6 28
#define R7 29
#define R8 30
#define R9 31
#define R10 32

#define C1 14
#define C2 41
#define C3 40
#define C4 39
#define C5 38
#define C6 37
#define C7 36
#define C8 35
#define C9 34
#define C10 33

#define S1 18
#define S2 19
#define S3 20
#define S4 21
#define S5 22
//#define S6 23
#define S6 17

#define LOAD_RS 8
#define CS_CLK 0
#define CS_INDATA 1
#define LOAD_CS 2
#define RS_CLK 3 
#define RS_INDATA 4

typedef uint16_t picture[64][64];

picture& getImageRef();



void setupColumn();
void setupRow();
void setupS();

void setS(uint8_t s);
void incrementS(bool keep);

uint16_t readRowDumb();
uint16_t readColumnDumb();
uint16_t readColumn();
uint16_t readRow();

void setupCR();
void loadCR();

void clockRS();
void clockCS();

void resetChip();

void setupChip();

void setCoorR(uint8_t X, uint8_t Y);

#endif
