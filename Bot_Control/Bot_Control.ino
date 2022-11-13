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


}
void setMotorSpeed(int speedValue){
  analogWrite(speedPin1, speedValue);
  analogWrite(speedPin2, speedValue-20);
  
}
void loop() {
  // put your main code here, to run repeatedly:
  for (int Speed=40;Speed<150;Speed+=10){
  setMotorSpeed(Speed);
  forward();
  delay(1000);
  }

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
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, HIGH);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);
}
void forward() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);

}

void openGripper(){
  int Pos=450;
  HCPCA9685.Servo(gripper, Pos);
  
  
}
void closeGriper(){
  int Pos=0;
  HCPCA9685.Servo(gripper, Pos);
  
}
void moveUp(){
  int Pos=450;
  HCPCA9685.Servo(sholder, Pos);
  
  
}
void moveDown(){
  int Pos=0;
  HCPCA9685.Servo(sholder, Pos);
  
}

void turnLeft(){
  int Pos=0;
  HCPCA9685.Servo(base, Pos);
}
void turnRight(){
  int Pos=450;
  HCPCA9685.Servo(sholder, Pos);
}
