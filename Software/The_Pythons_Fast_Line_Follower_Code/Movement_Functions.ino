/***** Motors movement function *****/ 
void moveMotors(int control){
  int leftSpeed = baseSpeed + control;
  int rightSpeed = baseSpeed - control;

  leftSpeed = constrain(leftSpeed, 0, 150);
  rightSpeed = constrain(rightSpeed, 0, 150);

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
