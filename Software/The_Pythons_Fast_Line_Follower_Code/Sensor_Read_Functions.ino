/***** Read Sensors & Calculate Position *****/ 
long readSensors() {
  long weightedSum = 0;
  int totalValue = 0;

  /***** Read sensors & calculate Weighted sum *****/ 
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
    weightedSum += sensorValues[i] * (i - (numSensors - 1) / 2); // Multiply sensor reading by its index offset by the center
    totalValue += sensorValues[i];
  }
//  sensorValues[i] = map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000);
  for(int i = 0; i < numSensors; i++){
    Serial.print(i);
    Serial.print(" :  ");
    Serial.print(sensorValues[i]);
    Serial.print("  ");
  }
  Serial.println();

  /***** Calculate position *****/ 
  if (totalValue > 0) {
    return weightedSum / totalValue; // Normalized position
  } else {
    return 0; // No line detected
  }
}

void calibrateIRS(){
  Serial.println("Starting Calibration!");
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
    }
  }
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
