#ifndef IMAGE_DRAW
#define IMAGE_DRAW

#include <Adafruit_RA8875.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <arm_math.h>

void drawImage(Adafruit_RA8875 &tft, uint16_t (&Image)[64][64]);

void setupImgMax();

#endif
