/***** Read Sensors & Calculate Position *****/ 
long lastweightSum=0;
int sensorWeights[numSensors]={85,80,70,40,30,30,30,40,70,80,85};
long readSensors() {
  long weightedSum = 0;
  int totalValue = 0;
  int max_ir_constant=100;
  /***** Read sensors & calculate Weighted sum *****/ 
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000);
    //Serial.printf("Sensor[%d]: min:%d max:%d value:%d\n",i,sensorOffsets[i][0],sensorOffsets[i][1],sensorValues[i]);
    if(sensorValues[i]>500)
      sensorValues[i]=sensorWeights[i];
    else
      sensorValues[i]=0;
    //if(sensorValues[i] && i
   
  }
  int left=-1;
  int right=-1;
  bool maxleft=false;
  bool maxright=false;
  for (int i = 0; i < numSensors/2; i++) {
    if(sensorValues[i]==sensorWeights[i]){
       left=i;
       break;
    }
       
  }
  for (int i = numSensors-1; i >= numSensors/2; i--) {
    if(sensorValues[i]==sensorWeights[i]){
      right=i;
      break;
      }
  }
  int half=(numSensors - 1) / 2;
  if (left!=-1 || right!=-1){
    if(left!=-1)
      weightedSum += sensorValues[left] * (left - half);
    if(right!=-1)
       weightedSum += sensorValues[right] * (right - half); // Multiply sensor reading by its index offset by the center
    //totalValue = sensorValues[left]+sensorValues[right];
    lastweightSum=weightedSum;
    }
  else{
     weightedSum=lastweightSum;
     totalValue=lastweightSum;
    }
    
  
   
   
  if(debug){
    for(int i = 0; i < numSensors; i++){
      Serial.print(i);
      Serial.print(" :  ");
      Serial.print(sensorValues[i]);
      //Serial.print(map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000));
      Serial.print("  ");
    }
    Serial.println();
  }

  /***** Calculate position *****/ 
  /*
  if (totalValue > 0) {
    return weightedSum; // Normalized position  / totalValue
  } else {
    return 0; // No line detected
  }
  */
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
