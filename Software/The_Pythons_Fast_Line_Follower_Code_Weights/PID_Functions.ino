/***** PID calculation function *****/ 
float calculatePID(long currentPosition) {
  long error = currentPosition - 0; // Error is the difference from the setpoint (0)
  integral += error; // Accumulate the error for the integral term
  float derivative = error - lastError; // Calculate the derivative term
  if(debug)
    Serial.printf("Error: %ld\n",error);
  float control = (Kp * error) + (Ki * integral) + (Kd * derivative);

  lastError = error;

  return control;
}
