#define motorL1 D3
#define motorL2 D4
#define motorR1 D5
#define motorR2 D6
#define speedPin1 D7
#define speedPin2 D8
//================================================================================
// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "Nokia";
const char* password = "12345678";

const char* PARAM_INPUT_1 = "pick";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//================================================================================
/* Include the HCPCA9685 library */
#include "HCPCA9685.h"
#define gripper 2
#define sholder 1
#define base 0
int sholderPosition =  180;
int basePosition = 0;
int gripperPosition = 180;
int pickingDistance = 300;

/* I2C slave address for the device/module. For the HCMODU0097 the default I2C address
   is 0x40 */
#define  I2CAdd 0x40


/* Create an instance of the library */
HCPCA9685 HCPCA9685(I2CAdd);
//==============================================================================
int shouldPick=0;
void setup() {
  // put your setup code here, to run once:
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(speedPin1, OUTPUT);
  pinMode(speedPin2, OUTPUT);
  /* Initialise the library and set it to 'servo mode' */
  HCPCA9685.Init(SERVO_MODE);

  /* Wake the device up */
  HCPCA9685.Sleep(false);
  Serial.begin(9600);
  homeArm();
  //forward();
  delay(1000);



  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      shouldPick=inputMessage1.toInt();
      Serial.print(shouldPick);
      
    }
    else {
      inputMessage1 = "No message sent";
      
    }
    
    Serial.print(inputMessage1);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();

}
void homeArm() {
  HCPCA9685.Servo(gripper, gripperPosition);
  HCPCA9685.Servo(sholder, sholderPosition);
  HCPCA9685.Servo(base, basePosition);
}

void followLine() {
  // put your main code here, to run repeatedly:
  int sensor1 = digitalRead(D0);
  int sensor2 = map(analogRead(A0), 0, 1023, 0, 1);
  if (sensor1 == 1 && sensor2 == 0) {
    //    Move towards left


    delay(100);
    stopMotors();
    left();
    Serial.println("Left");

  }
  else if (sensor2 == 1 && sensor1 == 0) {
    //    Move towards right
    right();
    Serial.println("Right");

    delay(100);
    stopMotors();
  }

  else if (sensor1 == 0 && sensor2 == 0) {
    //Move straight
    forward();
    delay(100);
    stopMotors();
  }
  Serial.println(sensor1);

  Serial.println(sensor2);
  delay(100);

}

void setMotorSpeed(int speedValue) {

  analogWrite(speedPin1, speedValue);
  analogWrite(speedPin2, speedValue);

}

void loop() {
  int distanceToProduct = 1400;
  setMotorSpeed(150);


  homeArm();
  delay(1000);

  forward();
  delay(distanceToProduct);
  stopMotors();
  delay(1000);
  setMotorSpeed(100);
  while (shouldPick!=1){
    Serial.print("Not allowed to pick");
    delay(100);
  }
  Serial.print("Picking");
  pickProduct();
  shouldPick=0;
  delay(1000);
  //  delay(2000);
  setMotorSpeed(150);
  back();
  delay(distanceToProduct );
  stopMotors();
  dropProduct();
  delay(1000);
  //  forward();

  //  motionTest();





}


void stopMotors() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);
}
void right() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, HIGH);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);
}
void left() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
}
void back() {
  digitalWrite(motorL2, LOW);
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);
}
void forward() {
  digitalWrite(motorL2, HIGH);
  digitalWrite(motorL1, LOW);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);

}

void openGripper(int targetPosition = 100) {

  for (int Pos1 = gripperPosition; Pos1 > targetPosition; Pos1 -= 10) {
    HCPCA9685.Servo(gripper, Pos1);
    delay(100);
    gripperPosition -= 10;
  }


}
void closeGriper(int targetPosition = 180) {

  for (int Pos1 = gripperPosition; Pos1 < targetPosition; Pos1 += 10) {
    HCPCA9685.Servo(gripper, Pos1);
    delay(100);
    gripperPosition += 10;
  }


}
void moveUp(int targetPosition = 180) {

  for (int Pos1 = sholderPosition; Pos1 > targetPosition; Pos1 -= 10) {
    HCPCA9685.Servo(sholder, Pos1);
    delay(100);
    sholderPosition -= 10;
  }

}
void moveDown(int targetPosition = 250) {

  for (int Pos1 = sholderPosition; Pos1 < targetPosition; Pos1 += 5) {
    HCPCA9685.Servo(sholder, Pos1);
    delay(100);
    sholderPosition += 5;
  }

}

void turnLeft(int targetPosition = 100) {

  for (int Pos1 = basePosition; Pos1 > targetPosition; Pos1 -= 10) {
    HCPCA9685.Servo(base, Pos1);
    delay(100);
  }
}
void turnRight(int targetPosition = 300) {

  for (int Pos1 = basePosition; Pos1 < targetPosition; Pos1 += 10) {
    HCPCA9685.Servo(base, Pos1);
    delay(100);
  }
}

void pickProduct() {
  moveUp();
  openGripper();
  forward();
  delay(pickingDistance);
  stopMotors();
  closeGriper();
}
void dropProduct() {
  moveDown();
  turnRight();
  //  turnLeft();
  openGripper();
  moveUp();

}


void motionTest() {
  // put your main code here, to run repeatedly:
  for (int Speed = 60; Speed < 150; Speed += 10) {
    setMotorSpeed(Speed);
    forward();
    delay(100);
  }
  for (int Speed = 60; Speed < 150; Speed += 10) {
    setMotorSpeed(Speed);
    right();
    delay(100);
  }

  for (int Speed = 60; Speed < 150; Speed += 10) {
    setMotorSpeed(Speed);
    left();
    delay(100);
  }

  for (int Speed = 60; Speed < 150; Speed += 10) {
    setMotorSpeed(Speed);
    back();
    delay(100);
  }

  stopMotors();
  openGripper();
  delay(1000);
  closeGriper();
  delay(1000);
  moveUp(180);
  delay(1000);
  moveDown(360);
  delay(1000);
  turnLeft();
  delay(1000);
  turnRight();
}
