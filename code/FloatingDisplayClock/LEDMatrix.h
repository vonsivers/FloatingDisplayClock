//==========================================================================
// Floating Display Clock - LEDMatrix.h
//
// Copyright (c) 2023, Moritz v. Sivers
//
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <Adafruit_GFX.h>
//#include <Adafruit_NeoMatrix.h>
//#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#define LED_PIN D8 

// Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 4, 1, LED_PIN,
//   NEO_TILE_TOP   + NEO_TILE_RIGHT   + NEO_TILE_COLUMNS   + NEO_TILE_PROGRESSIVE +
//   NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
//   NEO_GRB + NEO_KHZ800);

//const uint16_t colors[] = { matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

#define mw 32
#define mh 8
#define NUMMATRIX (mw*mh)

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

CRGB matrixleds[NUMMATRIX];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, 8, mh, mw/8, 1, 
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_RIGHT +  NEO_TILE_PROGRESSIVE);

const uint16_t colors[] = {
  matrix->Color(255, 0, 0), matrix->Color(0, 255, 0), matrix->Color(0, 0, 255) };

byte currentHue = 50;

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return matrix->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return matrix->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return matrix->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void DisplayTest() {
  matrix->fillScreen(0);
  matrix->setTextColor(colors[1]);

  matrix->setCursor(1, 0);
  matrix->print(F("1"));

  matrix->setCursor(8, 0);
  matrix->print(F("2"));
  
  matrix->setCursor(13, 0);
  matrix->print(F(":"));

  matrix->setCursor(18, 0);
  matrix->print(F("3"));
  
  matrix->setCursor(25, 0);
  matrix->print(F("4"));

  matrix->show();
}

void showColon() {
  //matrix->setTextColor(Wheel(currentHue));
  matrix->setCursor(13, 0);
  matrix->print(F(":"));
  matrix->show();
}

void blankDigit(int pos) {
  for(int x=0; x<8; x++) {
    for(int y=0; y<8; y++) {
      matrix->drawPixel(pos*8+x, y, (uint32_t) 0);
    }
  }
  matrix->show();
}

void showDigit(int pos, int digit) {
  //matrix->setTextColor(Wheel(currentHue));
  int cursorX;
  switch (pos) {
      case 0 :
          cursorX = 25; // minutes
          break;
      case 1 :
          cursorX = 18; // tom
          break;
      case 2 :
          cursorX = 8; // hours
          break;
      case 3 :
          cursorX = 1; // toh
          break;
    }
  matrix->setCursor(cursorX, 0);
  matrix->print(String(digit));
  matrix->show();
}



#endif //LEDMATRIX_H
