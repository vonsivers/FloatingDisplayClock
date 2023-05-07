//==========================================================================
// Floating Display Clock -  Clock.h
//
// Copyright (c) 2023, Moritz v. Sivers
//
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================
#ifndef CLOCK_H
#define CLOCK_H

#include "servos.h"
#include "LEDMatrix.h"
#include <ESP8266WiFi.h>            // we need wifi to get internet access
#include <time.h>                   // time() ctime()

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

/* Configuration of NTP */
#define MY_NTP_SERVER "de.pool.ntp.org"            // set the best fitting NTP server (pool) for your location
#define MY_TZ "CET-1CEST,M3.5.0,M10.5.0/3"        // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

time_t now;                         // this is the epoch
tm tm;                              // the structure tm holds time information in a more convient way

uint8_t lastMinute;                 // save minute when clock was last updated


// show time
void showTime() {
  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time

  uint16_t toh = tm.tm_hour / 10;
  uint16_t hour = tm.tm_hour % 10;
  uint16_t tom = tm.tm_min / 10;
  uint16_t min = tm.tm_min % 10;

  // get hue from minute
  byte hue = map(tm.tm_min,0,59,0,255);

  matrix->setTextColor(Wheel(hue));
  showDigit(0, min);
  showDigit(1, tom);
  showColon();
  showDigit(2, hour);
  showDigit(3, toh);

  lastMinute = tm.tm_min;
}

void updateHue() {
  uint16_t toh = tm.tm_hour / 10;
  uint16_t hour = tm.tm_hour % 10;
  uint16_t tom = tm.tm_min / 10;
  uint16_t min = tm.tm_min % 10;
  // get hue from minute
  byte hue = map(tm.tm_min,0,59,0,255);
  matrix->setTextColor(Wheel(hue));
  showDigit(0, min);
  showDigit(1, tom);
  showColon();
  showDigit(2, hour);
  showDigit(3, toh);
}

// draw the current time
void updateDisplay() {

  uint16_t toh = tm.tm_hour / 10;
  uint16_t hour = tm.tm_hour % 10;
  uint16_t tom = tm.tm_min / 10;
  uint16_t min = tm.tm_min % 10;

  // only minutes have changed
  moveServoMax(0);
  blankDigit(3);
  showDigit(0, min);
  moveServoMin(0);

  // time is xx:x0 
  if (min==0) {
    moveServoMax(1);
    blankDigit(2);
    showDigit(1, tom);
    moveServoMin(1);
    // time is xx:00
    if (tom==0) {
      moveServoMax(2);
      blankDigit(1);
      showDigit(2, hour);
      showColon();
      moveServoMin(2);
      // time is x0:00
      if (toh==0) {
        moveServoMax(3);
        blankDigit(0);
        showDigit(3, toh);
        moveServoMin(3);
      }
    }
  }
  updateHue();
  
}


// runs the clock
void runClock() {
  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time
  if(tm.tm_min!=lastMinute) {
    updateDisplay();
    lastMinute = tm.tm_min;
  }
  delay(1000);
  //EVERY_N_MILLISECONDS(50) {updateHue(); } // flickers!
}



#endif //CLOCK_H
