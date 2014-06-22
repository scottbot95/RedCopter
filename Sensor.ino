float gx_tmp[GYRO_MAF_NR];
float gy_tmp[GYRO_MAF_NR];
float gz_tmp[GYRO_MAF_NR];
int accx_tmp = 0;
int accy_tmp = 0;
int accz_tmp = 0;
float gyro_offsets[3];

unsigned long tPrevMillis = millis();
unsigned long tPrevMicros = micros();
unsigned long _startTime = millis();

void SensorInit() {
  _startTime = millis();
  for (int i = 0; i < GYRO_MAF_NR; i++) {
    gx_tmp[i] = 0.0;
    gy_tmp[i] = 0.0;
    gz_tmp[i] = 0.0;
  }
}

void updateSensorVals() {
  if ((micros()-tPrevMicros) > 1300) { // only update once per 1300us (~800 Hz)
    updateGyro();
    tPrevMicros = micros();
  }
  if ((millis()-tPrevMillis) > 20) { // only update once per 20ms (50 Hz)
    updateAcc();
    tPrevMillis = millis();
  }
  if(_startTime && (millis()-_startTime) > 1000)
  {
    gyro_offsets[0] = gx_avg;
    gyro_offsets[1] = gy_avg;
    gyro_offsets[2] = gz_avg;
    _startTime = 0;
  }
  
  unsigned long t = millis();
  float dt = (float)(t-tPrev)/1000.0;
  float accx = atan2(accx_tmp, accz_tmp)*RadToDeg;
  float accy = atan2(accy_tmp, accz_tmp)*RadToDeg;
  angles[0] = SPLIT*(-gy_avg*dt + angles[0]) + (1.0-SPLIT)*accx;
  angles[1] = SPLIT*(gx_avg*dt + angles[1]) + (1.0-SPLIT)*accy;
  tPrev = t;
}

void updateAcc() { // High pass filter
  int buffer[3]; //Axl buffer
  MPU.getAxlData(buffer);
  buffer[0]=buffer[0]-ACC_X_OFFSET; 
  buffer[1]=buffer[1]-ACC_Y_OFFSET;
  buffer[2]=buffer[2]-ACC_Z_OFFSET;
  accx_tmp=(ACC_HPF_NR*accx_tmp+(100-ACC_HPF_NR)*buffer[0])/100;
  accy_tmp=(ACC_HPF_NR*accy_tmp+(100-ACC_HPF_NR)*buffer[1])/100;
  accz_tmp=(ACC_HPF_NR*accz_tmp+(100-ACC_HPF_NR)*buffer[2])/100;
}

void updateGyro() {
  float buffer[3]; //Gyro buffer
  MPU.getGyroData(buffer);
  for (byte i=0; i<(GYRO_MAF_NR-1);i++) {
    gx_tmp[i] = gx_tmp[i+1];
    gy_tmp[i] = gy_tmp[i+1];
    gz_tmp[i] = gz_tmp[i+1];
  }
  buffer[0] -= gyro_offsets[0];
  buffer[1] -= gyro_offsets[1];
  buffer[2] -= gyro_offsets[2];
//  gx_tmp[GYRO_MAF_NR-1] = (float)(buffer[0]-buffer[1])*SINE45; // because it's not quite at a 45
//  gy_tmp[GYRO_MAF_NR-1] = (float)(buffer[0]+buffer[1])*SINE45;
  gx_tmp[GYRO_MAF_NR-1] = (float)(buffer[0]);
  gy_tmp[GYRO_MAF_NR-1] = (float)(buffer[1]);
  gz_tmp[GYRO_MAF_NR-1] = (float)(buffer[2]);
  gyroMAF();
}

void gyroMAF() { // Moving average filter
#if GYRO_HPF_NR > 0
  float gx_old = gx_avg;
  float gy_old = gy_avg;
  float gz_old = gz_avg;
#endif

  gx_avg = 0;
  gy_avg = 0;
  gz_avg = 0;
  for(byte i=0;i<GYRO_MAF_NR;i++){
    gx_avg=gx_avg+gx_tmp[i];
    gy_avg=gy_avg+gy_tmp[i];
    gz_avg=gz_avg+gz_tmp[i];
  }  
  gx_avg=(float)gx_avg/GYRO_MAF_NR;
  gy_avg=(float)gy_avg/GYRO_MAF_NR;
  gz_avg=(float)gz_avg/GYRO_MAF_NR;

  //gx_avg = (gx_avg - gy_avg) * SINE45;
  //gy_avg = (gx_avg + gy_avg) * SINE45;

#if GYRO_HPF_NR > 0
  gx_avg=(GYRO_HPF_NR*gx_old+(100.0-GYRO_HPF_NR)*gx_avg)/100.0;
  gy_avg=(GYRO_HPF_NR*gy_old+(100.0-GYRO_HPF_NR)*gy_avg)/100.0;
  gz_avg=(GYRO_HPF_NR*gz_old+(100.0-GYRO_HPF_NR)*gz_avg)/100.0;
#endif
}


