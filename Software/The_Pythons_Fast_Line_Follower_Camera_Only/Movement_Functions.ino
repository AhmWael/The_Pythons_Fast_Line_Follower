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
  rightSpeed = baseSpeed - control;

  leftSpeed = constrain(leftSpeed, 0, highSpeed);
  rightSpeed = constrain(rightSpeed, 0, highSpeed);

  digitalWrite(rightMotorIN, HIGH);
  digitalWrite(rightMotorIN2, LOW);
  ledcWriteChannel(rightMotorChannel, abs(rightSpeed));
  digitalWrite(leftMotorIN, HIGH);
  digitalWrite(leftMotorIN2, LOW);
  ledcWriteChannel(leftMotorChannel, abs(leftSpeed));
//  if (leftSpeed <= 0 && abs(linePosition) >= IR1) {
//    digitalWrite(leftMotorIN, LOW);
//    digitalWrite(leftMotorIN2, HIGH);
//    ledcWriteChannel(leftMotorChannel, abs(leftSpeed));
//  }
//  else if (leftSpeed <= 0) {
//    ledcWriteChannel(leftMotorChannel, 255);
//    digitalWrite(leftMotorIN, LOW);
//    digitalWrite(leftMotorIN2, LOW);
//  }
//  else {
//    digitalWrite(leftMotorIN, HIGH);
//    digitalWrite(leftMotorIN2, LOW);
//    ledcWriteChannel(leftMotorChannel, abs(leftSpeed));
//  }
//
//  if (rightSpeed <= 0 && abs(linePosition) >= IR1) {
//    digitalWrite(rightMotorIN, LOW);
//    digitalWrite(rightMotorIN2, HIGH);
//    ledcWriteChannel(rightMotorChannel, abs(rightSpeed));
//  }
//  else if (rightSpeed <= 0) {
//    ledcWriteChannel(rightMotorChannel, 255);
//    digitalWrite(rightMotorIN, LOW);
//    digitalWrite(rightMotorIN2, LOW);
//  }
//  else {
//    digitalWrite(rightMotorIN, HIGH);
//    digitalWrite(rightMotorIN2, LOW);
//    ledcWriteChannel(rightMotorChannel, abs(rightSpeed));
//  }

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

#ifdef debug
  if (leftSpeed < rightSpeed)
    Serial.println("LEFT");
  else if (leftSpeed > rightSpeed)
    Serial.println("RIGHT");
  else
    Serial.println("FORWARD");
#endif
}
