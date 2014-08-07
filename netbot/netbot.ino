//#include <PID_v1.h>

#include <Servo.h>

#define MOTOR1_PIN 9
#define MOTOR2_PIN 10
#define MOTOR3_PIN 11

#define SMOOTHING_SCALE 0.0025

#define MAX_TIMEOUT 500

#define MAX_INPUT 500
#define MIN_INPUT -500

#define MAX_OUTPUT 2000
#define MIN_OUTPUT 1000

int timeout = 0;
boolean timedOut = false;

const boolean useSerial = false;

//input all values -500 to 500
int xj = 0; //joystick X axis
int yj = 0; //joystick Y axis
int zj = 0; //joystick Z axis (rotation)

//target wheel power
int wheel1;
int wheel2;
int wheel3;

int output1;
int output2;
int output3;

Servo motor1;
Servo motor2;
Servo motor3;

void calcWheelOutput(){
  wheel1 = constrain(xj+zj, MIN_INPUT, MAX_INPUT);
  wheel2 = constrain((-xj/2+(0.866f*yj)+zj), MIN_INPUT, MAX_INPUT);
  wheel3 = constrain((-xj/2-(0.866f*yj)+zj), MIN_INPUT, MAX_INPUT);
  
  wheel1 = map(wheel1, MIN_INPUT, MAX_INPUT, MIN_OUTPUT, MAX_OUTPUT);
  wheel2 = map(wheel2, MIN_INPUT, MAX_INPUT, MIN_OUTPUT, MAX_OUTPUT);
  wheel3 = map(wheel3, MIN_INPUT, MAX_INPUT, MIN_OUTPUT, MAX_OUTPUT);
}

void printWheelOutput(){
  Serial.print("wheel1: ");
  Serial.println(wheel1);
  Serial.print("wheel2: ");
  Serial.println(wheel2);
  Serial.print("wheel3: ");
  Serial.println(wheel3);
  Serial.println();
}

void printOutput(){
  Serial.print("output1: ");
  Serial.print(output1);
  Serial.print(", output2: ");
  Serial.print(output2);
  Serial.print(", output3: ");
  Serial.println(output3);
}

void printInput(){
  Serial.print("xj: ");
  Serial.print(xj);
  Serial.print(", yj: ");
  Serial.print(yj);
  Serial.print(", zj: ");
  Serial.println(zj);
}

void setWheels(){
  smooth(wheel1, output1);
  smooth(wheel2, output2);
  smooth(wheel3, output3);
  //printOutput();
  motor1.writeMicroseconds(output1);
  motor2.writeMicroseconds(output2);
  motor3.writeMicroseconds(output3);
}

void smooth(int input, int &output){
//  Serial.print("input: ");
//  Serial.print(input);
//  Serial.print(", output: ");
//  Serial.print(output);
//  Serial.print(", diff: ");
//  Serial.println(input-output);
  double change = (input - output) * SMOOTHING_SCALE;
  if(change < 1 && change > 0){
    change = 1;
  }else if(change > -1 && change < 0){
    change = -1;
  }
  output += change;
}

void updateTimeout(){
  if(timeout > MAX_TIMEOUT){
    xj = yj = zj = 0;
    calcWheelOutput();
    setWheels();
    if(!timedOut){
      timedOut = true;
      Serial.println("timeout");
      printWheelOutput();
    }
  }else{
    timeout++;
    timedOut = false;
  }
}

void resetTimeout(){
  timeout = 0;
}

long scale(long input, long lowIn, long centerIn, long highIn, long lowOut, long centerOut, long highOut){
  if(input > centerIn){
    return map(input, centerIn, highIn, centerOut, highOut);
  }else if(input < centerIn){
    return map(input, lowIn, centerIn, lowOut, centerOut);
  }else{
    return centerOut;
  }
}

void scaleInput(){
  xj = scale(xj, 0, 514, 960, -500, 0, 500);
  yj = scale(yj, 0, 502, 1023, 500, 0, -500);
  zj = scale(zj, 52, 527, 1023, 500, 0, -500);
}

void setup(){
  delay(3000); //give time to upload in case of disaster
  Serial.begin(9600);
  motor1.attach(MOTOR1_PIN);
  motor2.attach(MOTOR2_PIN);
  motor3.attach(MOTOR3_PIN);
  
  calcWheelOutput();
  printWheelOutput();
  
  output1 = wheel1;
  output2 = wheel2;
  output3 = wheel3;
  
  
}

void loop(){
  if(useSerial){
    while(Serial.available() > 0){
      xj = Serial.parseInt();
      yj = Serial.parseInt();
      zj = Serial.parseInt();
      if(Serial.read() == '\n'){
        calcWheelOutput();
        //printWheelOutput();
        resetTimeout();
      }
    }
  }else{ //joystick control
    xj = analogRead(0);
    yj = analogRead(1);
    zj = analogRead(2);
    scaleInput();
  }
  

  delay(1);
  calcWheelOutput();
  //updateTimeout();
  setWheels();
  //printInput();
  //printWheelOutput();
}
