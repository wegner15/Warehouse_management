#define motorL1 D3
#define motorL2 D4
#define motorR1 D5
#define motorR2 D6
#define speedPin1 D7
#define speedPin2 D8

//================================================================================
/* Include the HCPCA9685 library */
#include "HCPCA9685.h"
#define gripper 1
#define sholder 2
#define base 0

/* I2C slave address for the device/module. For the HCMODU0097 the default I2C address
   is 0x40 */
#define  I2CAdd 0x40


/* Create an instance of the library */
HCPCA9685 HCPCA9685(I2CAdd);
//==============================================================================

void setup() {
  // put your setup code here, to run once:
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(speedPin1,OUTPUT);
  pinMode(speedPin2, OUTPUT);
  /* Initialise the library and set it to 'servo mode' */ 
  HCPCA9685.Init(SERVO_MODE);

  /* Wake the device up */
  HCPCA9685.Sleep(false);
  Serial.begin(9600);
//forward();
delay(1000);

}

void followLine(){
  // put your main code here, to run repeatedly:
  int sensor1=digitalRead(D0);
  int sensor2=map(analogRead(A0),0,1023,0,1);
  if (sensor1==1&&sensor2==0){
//    Move towards left


delay(100);
    stopMotors();
    left();
Serial.println("Left");

  }
  else if (sensor2==1&&sensor1==0){
//    Move towards right
right();
Serial.println("Right");

delay(100);
    stopMotors();
  }

  else if (sensor1==0&&sensor2==0){
    //Move straight
    forward();
    delay(100);
    stopMotors();
  }
  Serial.println(sensor1);
  
  Serial.println(sensor2);
  delay(100);
  
}

void setMotorSpeed(int speedValue){
  
  analogWrite(speedPin1, speedValue);
  analogWrite(speedPin2, speedValue);
  
}
int targetSpeed=70;
void loop() {
  if (targetSpeed<120){
     setMotorSpeed(targetSpeed);
     targetSpeed+=10;
  }
  
 
  
  forward();
//  delay(600);
//  right();
//  delay(600);
//  forward();
  
//  motionTest();

 

}
void motionTest(){
   // put your main code here, to run repeatedly:
  for (int Speed=60;Speed<150;Speed+=10){
  setMotorSpeed(Speed);
  forward();
  delay(100);
  }
for (int Speed=60;Speed<150;Speed+=10){
  setMotorSpeed(Speed);
  right();
  delay(100);
  }

for (int Speed=60;Speed<150;Speed+=10){
  setMotorSpeed(Speed);
  left();
  delay(100);
  }

  for (int Speed=60;Speed<150;Speed+=10){
  setMotorSpeed(Speed);
  back();
  delay(100);
  }
  
  stopMotors();
openGripper();
delay(1000);
closeGriper();
delay(1000);
moveUp();
delay(1000);
moveDown();
delay(1000);
turnLeft();
delay(1000);
turnRight();
}
void stopMotors(){
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

void openGripper(){
  int Pos=450;
  for (int Pos1=0;Pos1<Pos;Pos1+=10){
  HCPCA9685.Servo(gripper, Pos1);
  delay(100);
  }
  
  
}
void closeGriper(){
  int Pos=0;
  HCPCA9685.Servo(gripper, Pos);
  
}
void moveUp(){
  int Pos=450;
  for (int Pos1=0;Pos1<Pos;Pos1+=10){
  HCPCA9685.Servo(sholder, Pos1);
  }
  
  
}
void moveDown(){
  int Pos=0;
  for (int Pos1=450;Pos1>0;Pos1-=10){
  HCPCA9685.Servo(sholder, Pos1);
  }
  
}

void turnLeft(){
  int Pos=0;
  for (int Pos1=450;Pos1>0;Pos1-=10){
  HCPCA9685.Servo(base, Pos1);
  delay(100);
  }
}
void turnRight(){
  int Pos=450;
  for (int Pos1=0;Pos1<Pos;Pos1+=10){
  HCPCA9685.Servo(sholder, Pos1);
  delay(100);
  }
}
