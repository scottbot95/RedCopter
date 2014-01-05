void FlightControl() {

#ifdef SAFE
  if(rxVal[1]<1100)
    motorArm();
#endif

  if (rxVal[4]<1500) {
    rateAngleSwitch = 0;
    ledStatus(0);
  } else {
    rateAngleSwitch = 1;
    ledStatus(1);
    PIDAngleX.resetI();
    PIDAngleY.resetI();
  }
  
  throttle = map(rxVal[1],THROTTLE_RMIN,THROTTLE_RMAX,THROTTLE_WMIN,THROTTLE_WMAX);
  int m0_val,m1_val,m2_val,m3_val,PIDroll_val,PIDpitch_val,PIDyaw_val;
  m0_val=m1_val=m2_val=m3_val=MOTOR_ZERO_LEVEL;
  if (throttle > THROTTLE_WMIN) {
#ifdef RX_EXPO  // Software RX EXPO
  if(rxVal[5]<1480)
    setZ=map(rxVal[5],YAW_RMIN,1480,YAW_WMAX,0);
  else
    setZ=map(rxVal[5],1520,YAW_RMAX,0,YAW_WMIN);
    
  if (rateAngleSwitch == 0){

    if(rxVal[2]<1480)
      setX=map(rxVal[2],ROLL_RMIN,1480,ROLL_WMIN,0);
    else
      setX=map(rxVal[2],1520,ROLL_RMAX,0,ROLL_WMAX);
    if(rxVal[0]<1480)
      setY=map(rxVal[0],PITCH_RMIN,1480,PITCH_WMAX,0);
    else
      setY=map(rxVal[0],1520,PITCH_RMAX,0,PITCH_WMIN);   
  }
  
  else{

    if(rxVal[2]<1480)
      setX=map(rxVal[2],ROLL_RMIN,1480,ROLL_WMIN*RX_RATE_SENSITIVITY,0);
    else
      setX=map(rxVal[2],1520,ROLL_RMAX,0,ROLL_WMAX*RX_RATE_SENSITIVITY);

    if(rxVal[0]<1480)
      setY=map(rxVal[0],PITCH_RMIN,1480,PITCH_WMAX*RX_RATE_SENSITIVITY,0);
    else
      setY=map(rxVal[0],1520,PITCH_RMAX,0,PITCH_WMIN*RX_RATE_SENSITIVITY);
  }

  if((rxVal[2]>1480) & (rxVal[2]<1520)) setX=0.0;
  if((rxVal[0]>1480) & (rxVal[0]<1520)) setY=0.0;
  if((rxVal[5]>1480) & (rxVal[5]<1520)) setZ=0.0;
#else
  setZ=map(rxVal[5],YAW_RMIN,YAW_RMAX,YAW_WMAX,YAW_WMIN);
  if (rateAngleSwitch == 0){
    setX=map(rxVal[2],ROLL_RMIN,ROLL_RMAX,ROLL_WMIN,ROLL_WMAX);
    setY=map(rxVal[0],PITCH_RMIN,PITCH_RMAX,PITCH_WMAX,PITCH_WMIN);
  }
  else{
    setX=map(rxVal[2],ROLL_RMIN,ROLL_RMAX,ROLL_WMIN*RX_RATE_SENSITIVITY,ROLL_WMAX*RX_RATE_SENSITIVITY);
    setY=map(rxVal[0],PITCH_RMIN,PITCH_RMAX,PITCH_WMAX*RX_RATE_SENSITIVITY,PITCH_WMIN*RX_RATE_SENSITIVITY); 
  }
#endif

  if (rateAngleSwitch == 0){
    setX=(int)PIDAngleX.Compute((float)setX+angles[0],gy_avg,(float)setX/RX_ANGLE_DAMPNING);
    setY=(int)PIDAngleY.Compute((float)setY-angles[1],gx_avg,(float)setY/RX_ANGLE_DAMPNING);
  }
  PIDroll_val= (int)PIDRoll.Compute((float)setX-gy_avg);
  PIDpitch_val= (int)PIDPitch.Compute((float)setY-gx_avg);
  PIDyaw_val= (int)PIDYaw.Compute((float)setZ-gz_avg);
  
  m0_val=throttle+PIDroll_val+PIDpitch_val+PIDyaw_val;
  m1_val=throttle-PIDroll_val+PIDpitch_val-PIDyaw_val;
  m2_val=throttle+PIDroll_val-PIDpitch_val-PIDyaw_val;
  m3_val=throttle-PIDroll_val-PIDpitch_val+PIDyaw_val;
  }
  
#ifdef DEBUG_MOTOR
  Serial.print("Throttle:");
  Serial.print(throttle);
  Serial.print('\t');
  Serial.print("PIDroll:");
  Serial.print(PIDroll_val);
  Serial.print('\t');
  Serial.print("PIDpitch:");
  Serial.print(PIDpitch_val);
  Serial.print('\t');
  Serial.print("PIDyaw:");
  Serial.print(PIDyaw_val);
  Serial.print('\t');
  Serial.print("Motor0:");
  Serial.print(m0_val);
  Serial.print('\t');
  Serial.print("Motor1:");
  Serial.print(m1_val);
  Serial.print('\t');
  Serial.print("Motor2:");
  Serial.print(m2_val);
  Serial.print('\t');
  Serial.print("Motor3:");
  Serial.print(m3_val);
  Serial.print('\t');
#endif
  analogWrite(MOTOR0,m0_val);
  analogWrite(MOTOR1,m1_val);
  analogWrite(MOTOR2,m2_val);
  analogWrite(MOTOR3,m3_val);
}

