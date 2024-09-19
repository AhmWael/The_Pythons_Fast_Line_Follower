/** Read Sensors & Calculate Position **/
long lastweightSum = 0;
long readSensors() {
  sensorWeights[0] = -IR1;
  sensorWeights[numSensors - 1] = IR1;
  sensorWeights[1] = -IR2;
  sensorWeights[numSensors - 2] = IR2;
  sensorWeights[2] = -IR3;
  sensorWeights[numSensors - 3] = IR3;
  sensorWeights[3] = -IR4;
  sensorWeights[numSensors - 4] = IR4;
  sensorWeights[4] = -IR5;
  sensorWeights[numSensors - 5] = IR5;
  sensorWeights[5] = -IR6;
  sensorWeights[numSensors - 6] = IR6;

  long weightedSum = 0;
  int totalValue = 0;
  int max_ir_constant = 100;
  int flagL = 0;
  int flagR = 0;
  /** Read sensors & calculate Weighted sum **/
  for (int i = 0; i <= (numSensors / 2); i++) {
    sensorValues[i] = map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000);
    //sensorValues[i] = analogRead(sensorPins[i]);
    //Serial.printf("Sensor[%d]: min:%d max:%d value:%d\n",i,sensorOffsets[i][0],sensorOffsets[i][1],sensorValues[i]);
    //int half_threshold = (sensorOffsets[i][0] + sensorOffsets[i][1]) / 2;
#ifdef debug
    Serial.printf("half threshold[%d]: %d : %d : -> %d\n", i, sensorOffsets[i][0], sensorOffsets[i][1],sensorValues[i]);
#endif
    if (sensorValues[i] >= 550) {
      flagL = 1;
      weightedSum += sensorWeights[i];
      break;
    }
  }

  for (int i = numSensors - 1; i >= numSensors / 2; i--) {
    sensorValues[i] = map(analogRead(sensorPins[i]), sensorOffsets[i][0], sensorOffsets[i][1], 0, 1000);
    //sensorValues[i] = analogRead(sensorPins[i]);
    //int half_threshold = (sensorOffsets[i][0] + sensorOffsets[i][1]) / 2;
    //Serial.printf("Sensor[%d]: min:%d max:%d value:%d\n",i,sensorOffsets[i][0],sensorOffsets[i][1],sensorValues[i]);
#ifdef debug
    Serial.printf("half threshold[%d]: %d : %d : -> %d\n", i, sensorOffsets[i][0], sensorOffsets[i][1],sensorValues[i]);
#endif
    if (sensorValues[i] >= 550) {
      flagR = 1;
      weightedSum += sensorWeights[i];
      break;
    }
  }

  if (flagL == 0 && flagR == 0) {
    weightedSum = lastweightSum;
  }
  else {
    lastweightSum = weightedSum;
  }

#ifdef debug
  Serial.printf("weightedSum : %d\n", weightedSum);
  for (int i = 0; i < numSensors; i++) {
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

void calibrateIRS() {
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
  while (millis() - timer < 10000) {
    Serial.println("Calibrating");

    digitalWrite(leftMotorIN, LOW);
    digitalWrite(leftMotorIN2, HIGH);
    ledcWriteChannel(leftMotorChannel, calibSpeed);
    digitalWrite(rightMotorIN, HIGH);
    digitalWrite(rightMotorIN2, LOW);
    ledcWriteChannel(rightMotorChannel, calibSpeed);

    for (int i = 0; i < numSensors; i++) {
      sensorValues[i] = analogRead(sensorPins[i]);
      if (sensorValues[i] < sensorOffsets[i][0])
        sensorOffsets[i][0] = sensorValues[i];
      if (sensorValues[i] > sensorOffsets[i][1])
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
  for (int i = 0; i < numSensors; i++) {
    Serial.print(i);
    Serial.print(" :    ");
    Serial.print(sensorOffsets[i][0]);
    Serial.print(" ,    ");
    Serial.println(sensorOffsets[i][1]);
  }
  // Calculate max and min for all sensors
  /*
    int max_thresh=0,min_thresh=4095;
    for(int i = 0; i < numSensors; i++){
    if(sensorOffsets[i][1]<min_thresh)
      min_thresh=sensorOffsets[i][1];
    if(sensorOffsets[i][0]>max_thresh)
      max_thresh=sensorOffsets[i][0];
    }
    threshold=(max_thresh+min_thresh)/2 + 100;
    Serial.print(min_thresh);
    Serial.print(" ,    ");
    Serial.print(max_thresh);
    Serial.print(" ,    ");
    Serial.println(threshold);
  */
  Serial.println("/////////////////////////////////////////////");

}
