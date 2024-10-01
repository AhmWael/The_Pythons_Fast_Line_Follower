unsigned long long int timer;
/***** Line Sensors *****/
int threshold = 1500;
const int numSensors = 13; // Number of sensors
int sensorPins[numSensors] = { 13, 14, 27, 26, 25, 33, 32, 35, 34, 39, 36, 15, 4}; // Sensor pins 13 / 4
int sensorValues[numSensors]; // Array to store sensor readings
long linePosition = 0; // Calculated position
int sensorOffsets[numSensors][2];
int IR1 = 1160;
int IR2 = 40;
int IR3 = 30;
int IR4 = 20;
int IR5 = 10;
int IR6 = 5;
int sensorWeights[numSensors] = { -IR1, -IR2, -IR3, -IR4, -IR5, -IR6, 0, IR6, IR5, IR4, IR3, IR2, IR1};

/***** PID constants *****/
float Kp = 10; // Proportional gain
float Ki = 0.0; // Integral gain
float Kd = 100; // Derivative gain

/***** PID variables *****/
long lastPosition = 0;
float integral = 0;
long lastError = 0;
float control;

/***** Motors pins *****/
#define leftMotor 23
#define leftMotorIN 22
#define leftMotorIN2 18

#define rightMotor 21
#define rightMotorIN 19
#define rightMotorIN2 16

#define leftMotorChannel 0
#define rightMotorChannel 1

/***** Motor Speeds *****/
int calibSpeed = 160;
int baseSpeed = 220;
int highSpeed = 0; //240
int originalBase = baseSpeed;
int originalHigh = highSpeed;

#define button 17
#define button2 5
#define debugLed 9

/***** Camera Variables *****/
int slowdown_thresh = 35;
int slowdown_speed = 0;
int speedup_thresh = 10;
int speedup_speed = 15; //35

bool state = true;

// Uncomment to enable prints
//#define debug         //Uncomment this line for lines below to work
//#define debugIR
//#define debugIRCalib
//#define debugBT
//#define debugMotor
//#define debugPID
//#define debugCAM

#define BT


int BackSpeed = 0;
String lastPosStr = "";

int deflection = 0;
#ifdef BT
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
#endif

void setup() {
  Serial.begin(115200);
#ifdef BT
  SerialBT.begin(device_name);  //Bluetooth device name
#endif
  pinMode(leftMotorIN, OUTPUT);
  pinMode(rightMotorIN, OUTPUT);
  pinMode(leftMotorIN2, OUTPUT);
  pinMode(rightMotorIN2, OUTPUT);
  // Initialize motor pins channels
  ledcAttachChannel(leftMotor, 5000, 8, leftMotorChannel);
  ledcAttachChannel(rightMotor, 5000, 8, rightMotorChannel);

  pinMode(debugLed, OUTPUT);
  digitalWrite(debugLed, LOW);

  pinMode(button, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  // Initialize sensor pins as input
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  analogReadResolution(12);

  digitalWrite(leftMotorIN2, LOW);
  digitalWrite(rightMotorIN2, LOW);
  //  digitalWrite(debugLed, HIGH);
  //  delay(1000);
  //  digitalWrite(debugLed, LOW);
  //  delay(1000);

  calibrateIRS();
//delay(5000);
  //  highSpeed = 220;
}

void loop() {
  //  ledcWriteChannel(leftMotorChannel, 110);
  //  ledcWriteChannel(rightMotorChannel, 110);

  // Calculate the position of the line
  linePosition = readSensors();

  //  Serial.print("Button: ");
  //  Serial.println(digitalRead(button));

  if (!digitalRead(button))
    calibrateIRS();

  if (!digitalRead(button2)) {
    delay(1500);
    highSpeed = 220;
  }



  // Call the PID function to calculate the control signal

  //  digitalWrite(leftMotorIN, HIGH);
  //  digitalWrite(leftMotorIN2, LOW);
  //  ledcWriteChannel(leftMotorChannel, abs(160));
  //  digitalWrite(rightMotorIN, HIGH);
  //  digitalWrite(rightMotorIN2, LOW);
  //  ledcWriteChannel(rightMotorChannel, abs(160));
#ifdef BT
  if (SerialBT.available()) {
    //auto received=;
    String rec = (String)SerialBT.readStringUntil('\n');
    //Serial.println(rec);
    char fun = rec[0];
    String after = rec.substring(1, rec.length());
    //Serial.println(after);
    float num = 0, factor = 1;
    int decimal_flag = 0;
    for (int i = 0; i < after.length(); i++) {
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
    if (fun == 'S') {
      highSpeed = 0;
      //delay(5000);
    }
    else if (fun == 'P') {
      Kp = num;
      //Serial.printf("Kp: %f\n",Kp);
    }
    else if (fun == 'I') {
      Ki = num;
      //Serial.printf("Ki: %f\n",Ki);
    }
    else if (fun == 'D') {
      Kd = num;
      //Serial.printf("Kd: %f\n",Kd);
    }
    else if (fun == 'M') {
      highSpeed = (int)num;
      originalHigh = highSpeed;
      //Serial.printf("high: %d\n",highSpeed);
    }
    else if (fun == 'B') {
      baseSpeed = (int)num;
      originalBase = baseSpeed;
      //Serial.printf("base: %d\n",baseSpeed);
    }
    else if (fun == 'Q') {
      IR1 = (int)num;
    }
    else if (fun == 'W') {
      IR2 = (int)num;
    }
    else if (fun == 'E') {
      IR3 = (int)num;
    }
    else if (fun == 'R') {
      IR4 = (int)num;
    }
    else if (fun == 'T') {
      IR5 = (int)num;
    }
    else if (fun == 'Y') {
      IR6 = (int)num;
    }
    else if (fun == 'H') {
      slowdown_thresh = (int)num;
    }
    else if (fun == 'J') {
      slowdown_speed = (int)num;
    }
    else if (fun == 'K') {
      speedup_thresh = (int)num;
    }
    else if (fun == 'L') {
      speedup_speed = (int)num;
    }

    //val+=rec[rec.length()-1]/1000.0;
    //Serial.print("Float: ");
    //Serial.printf("num: %f\n",num);
    //Serial.write(SerialBT.read());
    //delay(500);
    //if(rec[0]=='S')
  }
#endif
  //Serial.printf("SDT: %d SDS: %d SUT: %d SUS %d\n",slowdown_thresh,slowdown_speed,speedup_thresh,speedup_speed);
  if (Serial.available() && highSpeed != 0) {

    /*
      String cam_rec = (String)Serial.readStringUntil('\n');
      if(cam_rec[0]=='N'){
      deflection=slowdown_thresh-1;
      digitalWrite(debugLed, HIGH);
      }
      else
    */
    deflection = Serial.parseInt();
#if defined(debug) && defined(debugCAM)
    Serial.printf("%d\n", deflection);
#endif
    if ( abs(deflection) < 1000 && abs(deflection) >= slowdown_thresh ) {
     
      if (state) {
      
        ledcWriteChannel(leftMotorChannel, 145);
        digitalWrite(leftMotorIN, LOW);
        digitalWrite(leftMotorIN2, HIGH);
        ledcWriteChannel(rightMotorChannel, 145);
        digitalWrite(rightMotorIN, LOW);
        digitalWrite(rightMotorIN2, HIGH);
        delay(35);
      }
      baseSpeed = originalBase - slowdown_speed;
      highSpeed = originalHigh - slowdown_speed;
      /*
      if(abs(linePosition) <= 40){
        
      if(deflection<0)
        linePosition = -2000;
      else
        linePosition = 2000;
        
        }
        */
      
      digitalWrite(debugLed, HIGH);
      state = false;
    }
    else if (abs(deflection) < speedup_thresh  ) {
      if (abs(linePosition) < 40) {
        linePosition = deflection;
        baseSpeed = originalBase + speedup_speed;
        highSpeed = originalHigh + speedup_speed;
        digitalWrite(debugLed, LOW);
      }
      else {
        baseSpeed = originalBase;
        highSpeed = originalHigh;
        digitalWrite(debugLed, LOW);
      }


      state = true;
    }
    else {
      baseSpeed = originalBase;
      highSpeed = originalHigh;
      digitalWrite(debugLed, LOW);
      state = true;
    }
  }
  control = calculatePID(linePosition);

  // Use the control value to adjust motor speed
  moveMotors(control);

  // Output the position and control value for debugging
#ifdef debug
  Serial.print("Position: ");
  Serial.print(linePosition);
  Serial.print("    Control: ");
  Serial.println(control);
#endif


}
