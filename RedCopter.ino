#include "conf.h"
#include <MPULib.h>
#include <Wire.h>
#include <PIDCont.h>
#include <avr/pgmspace.h>

PIDCont PIDRoll, PIDPitch, PIDYaw, PIDAngleX, PIDAngleY;
MPULib MPU;

unsigned long tPrev;
float angles[2] = {0.0, 0.0};
float gx_avg = 0;
float gy_avg = 0;
float gz_avg = 0;

// Rate setpoint for PID
int setX = 0;
int setY = 0;
int setZ = 0;

volatile int rxVal[6] = {1500, 1500, 1500, 0, 0, 1000};

int throttle = MOTOR_ZERO_LEVEL;
byte rateAngleSwitch = 0;

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  while(!Serial);
#endif
  MPU.init();
  motorInit();
  ledsInit();
  rxInit();
  motorArm();
  PIDInit();
  tPrev = millis();
}

void loop() {
  updateSensorVals();
  FlightControl();
#ifdef DEBUG 
  debugProcess();
#endif
}
