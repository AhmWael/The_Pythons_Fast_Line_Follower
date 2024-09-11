/** Read Sensors & Calculate Position **/ 
long lastweightSum=0;
long readSensors() {
  sensorWeights[0]=-IR1;
  sensorWeights[numSensors-1]=IR1;
  sensorWeights[1]=-IR2;
  sensorWeights[numSensors-2]=IR2;
  sensorWeights[2]=-IR3;
  sensorWeights[numSensors-3]=IR3;
  sensorWeights[3]=-IR4;
  sensorWeights[numSensors-4]=IR4;
  sensorWeights[4]=-IR5;
  sensorWeights[numSensors-5]=IR5;
  sensorWeights[5]=-IR6;
  sensorWeights[numSensors-6]=IR6;
  
  long weightedSum = 0;
  int totalValue = 0;
  int max_ir_constant=100;
  int flagL=0;
  int flagR=0;
  /** Read sensors & calculate Weighted sum **/ 
  for (int i = 0; i <= (numSensors/2); i++) {
    sensorValues[i] = map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000);
    //Serial.printf("Sensor[%d]: min:%d max:%d value:%d\n",i,sensorOffsets[i][0],sensorOffsets[i][1],sensorValues[i]);
    #ifdef debug
      Serial.printf("SensorWeight[%d]: %d\n",i,sensorWeights[i]);
    #endif
    if(sensorValues[i]>500){
      flagL=1;
      weightedSum +=sensorWeights[i];
      break;
    }
  }

  for (int i = numSensors-1; i >= numSensors/2; i--) {
    sensorValues[i] = map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000);
    //Serial.printf("Sensor[%d]: min:%d max:%d value:%d\n",i,sensorOffsets[i][0],sensorOffsets[i][1],sensorValues[i]);
    #ifdef debug
      Serial.printf("SensorWeight[%d]: %d\n",i,sensorWeights[i]);
    #endif
    if(sensorValues[i]>500){
      flagR=1;
      weightedSum +=sensorWeights[i]; 
      break;
      }
  }

  if (flagL==0 && flagR==0){
     weightedSum=lastweightSum;
    }
  else{
    lastweightSum=weightedSum;
    }
   
  #ifdef debug
    Serial.printf("weightedSum : %d\n", weightedSum);    
    for(int i = 0; i < numSensors; i++){
      Serial.print(i);
      Serial.print(" :  ");
      Serial.print(sensorValues[i]);
      //Serial.print(map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000));
      Serial.print("  ");
    }
    Serial.println();
  #endif
  return weightedSum;
}

void calibrateIRS(){
  Serial.println("Starting Calibration!");
  ledcWriteChannel(leftMotorChannel, 0);
  ledcWriteChannel(rightMotorChannel, 0);
  digitalWrite(debugLed, HIGH);
  delay(500);
  digitalWrite(debugLed, LOW);
  delay(500);
  digitalWrite(debugLed, HIGH);

  
  
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
    sensorOffsets[i][0] = sensorValues[i];
    sensorOffsets[i][1] = sensorValues[i];
  }
  timer = millis();
  while(millis() - timer < 5000){
    Serial.println("Calibrating");

    digitalWrite(leftMotorIN, LOW);
    digitalWrite(leftMotorIN2, HIGH);
    ledcWriteChannel(leftMotorChannel, 200);
    digitalWrite(rightMotorIN, HIGH);
    digitalWrite(rightMotorIN2, LOW);
    ledcWriteChannel(rightMotorChannel, 200);
    
    for(int i = 0; i < numSensors; i++){
      sensorValues[i] = analogRead(sensorPins[i]);
      if(sensorValues[i] < sensorOffsets[i][0])
        sensorOffsets[i][0] = sensorValues[i];
      if(sensorValues[i] > sensorOffsets[i][1])
        sensorOffsets[i][1] = sensorValues[i];
      Serial.print(i);
      Serial.print(" :  ");
      Serial.print(sensorValues[i]);
      //Serial.print(map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000));
      Serial.print("  ");
    }
    Serial.println();
  }
  ledcWriteChannel(leftMotorChannel, 255);
  digitalWrite(leftMotorIN, LOW);
  digitalWrite(leftMotorIN2, LOW);
  ledcWriteChannel(rightMotorChannel, 255);
  digitalWrite(rightMotorIN, LOW);
  digitalWrite(rightMotorIN2, LOW);
  
  digitalWrite(debugLed, LOW);
  Serial.println("Calibration Ended!");
  
  Serial.println("Sensors Offsets: ");
  for(int i = 0; i < numSensors; i++){
    Serial.print(i);
    Serial.print(" :    ");
    Serial.print(sensorOffsets[i][0]);
    Serial.print(" ,    ");
    Serial.println(sensorOffsets[i][1]);
  }
  Serial.println("/////////////////////////////////////////////");
  
}
