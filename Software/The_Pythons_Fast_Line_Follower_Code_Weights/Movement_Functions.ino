/***** Motors movement function *****/ 
void moveMotors(int control){
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

  leftSpeed = baseSpeed + control;
  rightSpeed = baseSpeed - control;

  leftSpeed = constrain(leftSpeed, 0, highSpeed);
  rightSpeed = constrain(rightSpeed, 0, highSpeed);

  ledcWriteChannel(leftMotorChannel, leftSpeed);
  ledcWriteChannel(rightMotorChannel, rightSpeed);

  if(debug){
    if(leftSpeed < rightSpeed)
      Serial.println("LEFT");
    else if(leftSpeed > rightSpeed)
      Serial.println("RIGHT");
    else
      Serial.println("FORWARD");
  }
}
