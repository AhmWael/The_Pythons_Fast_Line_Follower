/***** Motors movement function *****/
void moveMotors(int control) {
  int leftSpeed;
  int rightSpeed;
  //  if(linePosition <= -IR3){
  //    digitalWrite(debugLed, HIGH);
  //    leftSpeed = baseSpeed + control;
  //    rightSpeed = baseSpeed;
  //  }
  //  else if(linePosition >= IR3){
  //    digitalWrite(debugLed, HIGH);
  //    leftSpeed = baseSpeed;
  //    rightSpeed = baseSpeed - control;
  //  }
  //  else{
  //    digitalWrite(debugLed, LOW);
  //    leftSpeed = baseSpeed + control;
  //    rightSpeed = baseSpeed - control;
  //  }
  if (highSpeed == 0) {
    ledcWriteChannel(leftMotorChannel, 255);
    digitalWrite(leftMotorIN, LOW);
    digitalWrite(leftMotorIN2, LOW);
    ledcWriteChannel(rightMotorChannel, 255);
    digitalWrite(rightMotorIN, LOW);
    digitalWrite(rightMotorIN2, LOW);
    return;
  }
  leftSpeed = baseSpeed + control;
  rightSpeed = baseSpeed - control;//+ 5

  leftSpeed = constrain(leftSpeed, -highSpeed, highSpeed);
  rightSpeed = constrain(rightSpeed, -highSpeed, highSpeed);
  if( linePosition == 0){
  leftSpeed=highSpeed;
  rightSpeed=highSpeed;
  }
  if (leftSpeed <= 0 && abs(linePosition) >= 1000 && abs(linePosition) <= 1255) { //Backward
    String linePositionStr = String(abs(linePosition));
    if(lastPosStr != linePositionStr){
      BackSpeed = 0;
      for (int i = 1; i < linePositionStr.length(); i++) {
        BackSpeed = BackSpeed * 10 + (linePositionStr[i] - '0');
      }
    }
    lastPosStr = linePositionStr;
    digitalWrite(leftMotorIN, LOW);
    digitalWrite(leftMotorIN2, HIGH);
    #if defined(debug) && defined(debugMotor)
    Serial.print("BackSpeed: ");
    Serial.println(abs(int(BackSpeed)));
    #endif
    ledcWriteChannel(leftMotorChannel, abs(int(BackSpeed)));
  }
  else if (linePosition == -2000 || leftSpeed <= 0 ) { //Stop
    ledcWriteChannel(leftMotorChannel, 255);
    digitalWrite(leftMotorIN, LOW);
    digitalWrite(leftMotorIN2, LOW);
  }
  else {
    digitalWrite(leftMotorIN, HIGH);
    digitalWrite(leftMotorIN2, LOW);
    ledcWriteChannel(leftMotorChannel, abs(leftSpeed));
  }

  if (rightSpeed <= 0 && abs(linePosition) >= 1000 && abs(linePosition) <= 1255) { //Backward
    String linePositionStr = String(abs(linePosition));
    if(lastPosStr != linePositionStr){
      BackSpeed = 0;
      for (int i = 1; i < linePositionStr.length(); i++) {
        BackSpeed = BackSpeed * 10 + (linePositionStr[i] - '0');
      } 
    }
    lastPosStr = linePositionStr;
    digitalWrite(rightMotorIN, LOW);
    digitalWrite(rightMotorIN2, HIGH);
    #if defined(debug) && defined(debugMotor)
    Serial.print("BackSpeed: ");
    Serial.println(abs(int(BackSpeed)));
    #endif
    ledcWriteChannel(rightMotorChannel, abs(int(BackSpeed)));
  }
  else if (linePosition == 2000 || rightSpeed <= 0 ) { //Stop
    ledcWriteChannel(rightMotorChannel, 255);
    digitalWrite(rightMotorIN, LOW);
    digitalWrite(rightMotorIN2, LOW);
  }
  else {
    digitalWrite(rightMotorIN, HIGH);
    digitalWrite(rightMotorIN2, LOW);
    ledcWriteChannel(rightMotorChannel, abs(rightSpeed));
  }

  /*
    leftSpeed = constrain(leftSpeed, 0, highSpeed);
    rightSpeed = constrain(rightSpeed, 0, highSpeed);
    if (leftSpeed <= 0) {
      ledcWriteChannel(leftMotorChannel, 255);
      digitalWrite(leftMotorIN, LOW);
    }
    else {
      digitalWrite(leftMotorIN, HIGH);
      ledcWriteChannel(leftMotorChannel, leftSpeed);
    }

    if (rightSpeed <= 0) {

      ledcWriteChannel(rightMotorChannel, 255);
      digitalWrite(rightMotorIN, LOW);
    }
    else {
      ledcWriteChannel(rightMotorChannel, rightSpeed);
      digitalWrite(rightMotorIN, HIGH);
    }
  */

#if defined(debug) && defined(debugMotor)
  if (leftSpeed < rightSpeed)
    Serial.println("LEFT");
  else if (leftSpeed > rightSpeed)
    Serial.println("RIGHT");
  else
    Serial.println("FORWARD");
#endif
}
