 void debugProcess(){
#ifdef DEBUG_ANGLES
  Serial.print("XT: ");
  Serial.print((float)(angles[0]));
  Serial.print('\t');
  Serial.print("YT: ");
  Serial.print((float)(angles[1]));
  Serial.print('\t');
#endif
#ifdef DEBUG_GYRO
  Serial.print("GX:");
  if (!(gx_avg<0)) Serial.print(' ');
  Serial.print((float)(gx_avg));
  Serial.print('\t');
  Serial.print("GY:");
  if (!(gy_avg<0)) Serial.print(' ');
  Serial.print((float)(gy_avg));
  Serial.print('\t');
  Serial.print("GZ:");
  if (!(gz_avg<0)) Serial.print(' ');
  Serial.print((float)(gz_avg));
  Serial.print('\t');
#endif
#ifdef DEBUG_PID
  Serial.print("setX:");
  Serial.print(setX);
  Serial.print('\t');
  Serial.print("setY:");
  Serial.print(setY);
  Serial.print('\t');
  Serial.print("setZ:");
  Serial.print(setZ);
  Serial.print('\t');
#endif
#ifdef DEBUG_RX
  Serial.print(F("Pitch:"));
  Serial.print(rxVal[0]);
  Serial.print('\t');
  Serial.print(F("Throttle:"));
  Serial.print(rxVal[1]);
  Serial.print('\t');
  Serial.print(F("Roll:"));
  Serial.print(rxVal[2]);
  Serial.print('\t');
  Serial.print(F("AUX1:"));
  Serial.print(rxVal[3]);
  Serial.print('\t');
  Serial.print(F("AUX2:"));
  Serial.print(rxVal[4]);
  Serial.print('\t');  
  Serial.print(F("Yaw:"));
  Serial.print(rxVal[5]);
  Serial.print('\t');    
#endif

#ifdef DEBUG_SERIAL_CHART
  Serial.print(angles[0]);
  Serial.print(',');
  Serial.print(angles[1]);
  Serial.print(',');
  Serial.print((float)(gx_avg));  
  Serial.print(',');
  Serial.print((float)(gy_avg));  
  Serial.print(',');
  Serial.print((float)(gz_avg));  

#endif

  Serial.print('\n');

}

