unsigned long long int timer;

const int numSensors = 13; // Number of sensors
int sensorPins[numSensors] = {13, 14, 27, 26, 25, 33, 32, 35, 34, 39, 36, 15, 4}; // Sensor pins
int sensorValues[numSensors]; // Array to store sensor readings
long linePosition = 0; // Calculated position
int sensorOffsets[13][2];

/***** PID constants *****/ 
float Kp = 0.02; // Proportional gain
float Ki = 0.0; // Integral gain
float Kd = 0.0; // Derivative gain

/***** PID variables *****/ 
long lastPosition = 0; 
float integral = 0; 
long lastError = 0; 
float control;

/***** Motors pins *****/
#define leftMotor 22
#define rightMotor 23

#define leftMotorChannel 0
#define rightMotorChannel 1

#define baseSpeed 115

#define button 5
#define debugLed 9

bool debug = true;

void setup() {
  Serial.begin(115200);

  // Initialize motor pins channels
  ledcAttachChannel(leftMotor, 5000, 8, leftMotorChannel);
  ledcAttachChannel(rightMotor, 5000, 8, rightMotorChannel);

  pinMode(debugLed, OUTPUT);
  digitalWrite(debugLed, LOW);

  pinMode(button, INPUT_PULLUP);
  
  // Initialize sensor pins as input
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  analogReadResolution(12);

//  digitalWrite(debugLed, HIGH);
//  delay(1000);
//  digitalWrite(debugLed, LOW);
//  delay(1000);

//  calibrateIRS();
}

void loop() {
//  ledcWriteChannel(leftMotorChannel, 110);
//  ledcWriteChannel(rightMotorChannel, 110);
  
  // Calculate the position of the line
  linePosition = readSensors();

//  Serial.print("Button: ");
//  Serial.println(digitalRead(button));

  if(!digitalRead(button))
    calibrateIRS();

  // Call the PID function to calculate the control signal
  control = calculatePID(linePosition);

  // Use the control value to adjust motor speed
  moveMotors(control);  

//// Output the position and control value for debugging
  if(debug){
    Serial.print("Position: ");
    Serial.print(linePosition);
    Serial.print("    Control: ");
    Serial.println(control);
  }

}
