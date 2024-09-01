/***** Read Sensors & Calculate Position *****/ 
long readSensors() {
  long weightedSum = 0;
  int totalValue = 0;

  /***** Read sensors & calculate Weighted sum *****/ 
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000);
    weightedSum += sensorValues[i] * (i - (numSensors - 1) / 2); // Multiply sensor reading by its index offset by the center
    totalValue += sensorValues[i];
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
  if (totalValue > 0) {
    return weightedSum; // Normalized position  / totalValue
  } else {
    return 0; // No line detected
  }
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
