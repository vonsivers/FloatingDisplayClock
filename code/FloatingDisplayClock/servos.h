//==========================================================================
// Floating Display Clock - servos.h
//
// Copyright (c) 2023, Moritz v. Sivers
//
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#ifndef SERVOS_H
#define SERVOS_H

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  360 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

const uint16_t DELAY = 15;   // delay (ms) between servo position update


// put servos to sleep
//
void sleepServos() {

  Serial.println("put servos to sleep");
  for (uint8_t servonum = 0; servonum<4; servonum++) {
    pwm.setPWM(servonum,0,0);  
  }

}

void moveServoMax(uint8_t servonum) {
  for(uint16_t pos=SERVOMIN+1; pos<=SERVOMAX; pos++) {
    pwm.setPWM(servonum, 0, pos);
    delay(DELAY);
  }
  pwm.setPWM(servonum,0,0);
}

void moveServoMin(uint8_t servonum) {
  for(uint16_t pos=SERVOMAX-1; pos>=SERVOMIN; pos--) {
    pwm.setPWM(servonum, 0, pos);
    delay(DELAY);
  }
  pwm.setPWM(servonum,0,0);
}

void moveAllMax() {
  for (uint8_t servonum = 0; servonum<4; servonum++) {
    pwm.setPWM(servonum, 0, SERVOMAX);
    pwm.setPWM(servonum,0,0);
    delay(1000);
  }
}

void moveAllMin() {
  for (uint8_t servonum = 0; servonum<4; servonum++) {
    pwm.setPWM(servonum, 0, SERVOMIN);
    pwm.setPWM(servonum,0,0);
    delay(1000);
  }
}

void testServos() {

  for (uint8_t servonum = 0; servonum<4; servonum++) {
    moveServoMin(servonum);
    delay(500);
    moveServoMax(servonum);
    delay(500);
  }
}




#endif //SERVOS_H
