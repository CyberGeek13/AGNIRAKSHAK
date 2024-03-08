#include <AFMotor.h>
#include<Servo.h>

Servo motor_1;
int servo1 = 90;

AF_DCMotor motor1(1, MOTOR12_1KHZ);    // left front motor
AF_DCMotor motor2(2, MOTOR12_1KHZ);    // left rear motor
AF_DCMotor motor3(3, MOTOR34_1KHZ);    // right rear motor
AF_DCMotor motor4(4, MOTOR34_1KHZ);    // right front motor

int Speed = 200;                      // set the speed of motors from 0 - 255 

const int flamePin = A0;               // connect flame sensor to A0
int flameState = 0;

const int mq2Pin = A1;                // connect smoke sensor to A1
int mq2State = 0; 

const int buzzer = A2;               // connect buzzer to A2

const int pump = A3;                // connect pump to A3

const int trigPin = A4;

const int echoPin = A5;

long duration;
int distance;
int safetyDistance;

char command; 

void setup(){
  
  Serial.begin(9600);

  motor1.setSpeed(Speed);
  motor2.setSpeed(Speed); 
  motor3.setSpeed(Speed);
  motor4.setSpeed(Speed);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  
  pinMode(mq2Pin, INPUT_PULLUP);
  pinMode(flamePin, INPUT_PULLUP);
  
  pinMode(pump, OUTPUT);
  pinMode(buzzer, OUTPUT);

  motor_1.attach(9);                   // connect servo motor to pin 9 or serv 2 on the l293d shield
  motor_1.write(servo1);

  digitalWrite( pump, HIGH);          // turn the water pump off with relay
  digitalWrite( buzzer, LOW);
}

void loop() 
{
  mq2State = digitalRead(mq2Pin);
  
 ///////////////// object detection loop and smoke sensor loop ////////////////// 
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  safetyDistance = distance;

  if ((safetyDistance <= 30) && (mq2State == HIGH))
  {                                                      // mq2 smoke sensor off and ultrasonic on
    buzzer1pattern(); 
 }

   else if ((safetyDistance = 31) && (mq2State == LOW))
  {                                                       // smoke sensor on and ultrasonic off
     buzzer2pattern(); 
 }
 
  else if ((safetyDistance > 31) && (mq2State == HIGH))
  {                                                       // smoke sensor and ultrasonic sensor off
     buzzerstop(); 
 }

 /////////////////////// fire sensor loop /////////////////////
 
  flameState = digitalRead(flamePin);
  if (flameState == LOW)
  {      
      Serial.print("Fire");            //Send string to Application
      delay(1000);
  }
  else
  {   
    Serial.print("No");                 //Send string to Application
    delay(200);
    }

    
  while(Serial.available() > 0){ 
    command = Serial.read(); 
    Stop(); 

    switch(command){
    case 'F':       // forward
      forward();
      break;
    case 'B':      // backward
       back();
      break;
    case 'L':     // left
      left();
      break;
    case 'R':     // right
      right();
      break;
      case 'S':    // stop
      Stop();
      break;
    case 'i':      // servo left
      lefts();
      break;
    case 'j':      // servo right
      rights();
      break;
    case 'a':      // spray on
      sprayon();
      break;
    case 'b':      // spray off
      sprayoff();
      break;
  } 
 }
}

void forward()
{

  motor1.run(FORWARD); 
  motor2.run(FORWARD); 
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void back()
{
  motor1.run(BACKWARD); 
  motor2.run(BACKWARD); 
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void left()
{
  motor1.run(BACKWARD); 
  motor2.run(BACKWARD); 
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void right()
{
  motor1.run(FORWARD); 
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
} 

void Stop()
{
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void lefts()
 {
   if (servo1 < 180){
    servo1 = servo1+2;}
    motor_1.write(servo1); 
}

void rights()
{
   if (servo1 > 0){
   servo1 = servo1-2;} 
   motor_1.write(servo1); 
}

void sprayon()
{
  digitalWrite(pump , LOW);  // turn relay on
}
void sprayoff()
{
  digitalWrite(pump , HIGH); // turn relay OFF
}

void buzzer1pattern()    // change it according to need
{
  digitalWrite(buzzer , HIGH);
  delay(500);
  digitalWrite(buzzer , LOW);
  delay(300);
  digitalWrite(buzzer , HIGH);
  delay(500);
  digitalWrite(buzzer , LOW);
  delay(300);
  digitalWrite(buzzer , HIGH);
  delay(500);
  digitalWrite(buzzer , LOW);
  delay(300);
  digitalWrite(buzzer , HIGH);
  delay(500);
  digitalWrite(buzzer , LOW);
  delay(300); 
}


void buzzer2pattern()    // change it according to need
{
  digitalWrite(buzzer , HIGH);
  delay(300);
  digitalWrite(buzzer , LOW);
  delay(100);
  digitalWrite(buzzer , HIGH);
  delay(300);
  digitalWrite(buzzer , LOW);
  delay(100);
  digitalWrite(buzzer , HIGH);
  delay(300);
  digitalWrite(buzzer , LOW);
  delay(100);
  digitalWrite(buzzer , HIGH);
  delay(300);
  digitalWrite(buzzer , LOW);
  delay(100); 
}

void buzzerstop()    // to stop the buzzer
{
  digitalWrite(buzzer , LOW);
}
