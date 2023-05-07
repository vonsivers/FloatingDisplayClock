//==========================================================================
// Floating Display Clock - FloatingDisplayClock.ino
//
// Copyright (c) 2022, Moritz v. Sivers
//
// based on Slide Clock by Craig Colvin, https://github.com/moose408/SlideClock
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#include "stepper.h"
#include "clock.h"
#include "LEDMatrix.h"     





//-------------------------------------
// s e t u p
//-------------------------------------
void setup()
{
  Serial.begin(9600);
  Serial.print("Floating Display Clock version 1.0\n");

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(matrixleds, NUMMATRIX).setCorrection(TypicalSMD5050);
  matrix->begin();  
  matrix->setTextWrap(false);
  matrix->setBrightness(20);
  matrix->fillScreen(0);
  matrix->show();

  //DisplayTest();
  //delay(1000);
  //blankDigit(3);
  //matrix.show();

  pwm.begin();
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  moveAllMin();

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
  //reset settings - for testing
  //wm.resetSettings();

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //and goes into a blocking loop awaiting configuration
  if (!wm.autoConnect("FloatingClockAP")) {
    Serial.println("failed to connect Wifi and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

  configTime(MY_TZ, MY_NTP_SERVER); // configure NTP server and timezone

  delay(1000);

  showTime();

}


//-------------------------------------
// l o o p
//
// 
//-------------------------------------
void loop()
{
  runClock();
}
