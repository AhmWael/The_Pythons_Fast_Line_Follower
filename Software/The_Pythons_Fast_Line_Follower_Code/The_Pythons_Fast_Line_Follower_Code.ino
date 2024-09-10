unsigned long long int timer;

const int numSensors = 11; // Number of sensors
int sensorPins[numSensors] = { 14, 27, 26, 25, 33, 32, 35, 34, 39, 36, 15}; // Sensor pins 13 / 4
int sensorValues[numSensors]; // Array to store sensor readings
long linePosition = 0; // Calculated position
int sensorOffsets[numSensors][2];

/***** PID constants *****/ 
float Kp = 0.3; // Proportional gain
float Ki = 0.0; // Integral gain
float Kd = 0.0; // Derivative gain

/***** PID variables *****/ 
long lastPosition = 0; 
float integral = 0; 
long lastError = 0; 
float control;

/***** Motors pins *****/
#define leftMotor 22
#define rightMotor 21

#define leftMotorChannel 0
#define rightMotorChannel 1

int baseSpeed=105;
int highSpeed=150;

#define button 5
#define button2 17
#define debugLed 9

bool debug = false;
#include "BluetoothSerial.h"

String device_name = "ESP32-BT-Slave";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
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

calibrateIRS();

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
if (SerialBT.available()) {
    //auto received=;
    String rec = (String)SerialBT.readStringUntil('\n');
    //Serial.println(rec);
    char fun=rec[0];
    String after=rec.substring(1,rec.length());
    //Serial.println(after);
    float num = 0, factor = 1;
    int decimal_flag = 0;
    for (int i = 0; i<after.length(); i++) {
        if (after[i] == '.') {
            decimal_flag = 1;
            continue;
        }
        if (decimal_flag) {
            factor /= 10;
            num += (after[i] - '0') * factor;
        } else {
            num = num * 10 + (after[i] - '0');
        }
    }
    //float val=after.toFloat();
    if(fun=='S'){
      highSpeed=0;
  //delay(5000);
      }
     else if(fun=='P'){
      Kp=num;
      //Serial.printf("Kp: %f\n",Kp);
      }
      else if(fun=='I'){
      Ki=num;
      //Serial.printf("Ki: %f\n",Ki);
      }
      else if(fun=='D'){
      Kd=num;
      //Serial.printf("Kd: %f\n",Kd);
      }
      else if(fun=='M'){
      highSpeed=(int)num;
      //Serial.printf("high: %d\n",highSpeed);
      }
      else if(fun=='B'){
      baseSpeed=(int)num;
      //Serial.printf("base: %d\n",baseSpeed);
      }
    //val+=rec[rec.length()-1]/1000.0;
    //Serial.print("Float: ");
    //Serial.printf("num: %f\n",num);
    //Serial.write(SerialBT.read());
    //delay(500);
    //if(rec[0]=='S')
  }
//// Output the position and control value for debugging
  if(debug){
    Serial.print("Position: ");
    Serial.print(linePosition);
    Serial.print("    Control: ");
    Serial.println(control);
  }

}
