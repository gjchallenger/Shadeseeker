/*
Shadeseeker Robot Version 2.0
Date: 9/26/2020
Created By: Glenville Challenger Jr.
Last Updated: 12/3/2020 6:50PM
*/

#include "IRremote.h"
#define RECEIVER_PIN 13

// Pin Variables.

const int sensorPinUp = A0;    // Pins for the Light Sensors
const int sensorPinLeft = A1;
const int sensorPinRight = A2;

const int ledPinR = 8;        // Pins for the LEDs
const int ledPinG = 9;
const int ledPinB = 10;

const int motorLeft1 = 6;    //Pins for the Motors
const int motorLeft2 = 7;
const int motorRight1 = 4;
const int motorRight2 = 5;

const int echo = 3; //Pins for the Ultrasonic Sensors
const int trigger = 2;


// Variables:

int sensorValueUp = 0;         // the sensor values for Photoresistors        
int sensorValueLeft = 0;         
int sensorValueRight = 0;

int initialValueUp;           //the initial values for the Photoresistors taken at the beginning, used to end the loop
int initialValueLeft;
int initialValueRight;

bool initialValueExe = true; //Exists to take initial Values of photoresistors once only
bool task = false; //Exit Condition for while loop in Automatic mode

long duration;      //Calculations for the Ultrasonic Sensor
float distance; 
float range = 20; //Determines when the object should turn. Changable for testing

// IR Remote

IRrecv receiver(RECEIVER_PIN); // create a receiver object of the IRrecv class
decode_results results; // create a results object of the decode_results class
unsigned long key_value = 0; // variable to store the pressed key value


void setup() {
  Serial.begin(9600);
  receiver.enableIRIn(); 
  receiver.blink13(true); // enable blinking of the built-in LED when an IR signal is received
  
  pinMode(ledPinR,OUTPUT);
  pinMode(ledPinG,OUTPUT);
  pinMode(ledPinB,OUTPUT);
  
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  
  pinMode(motorLeft1,OUTPUT);
  pinMode(motorLeft2,OUTPUT);
  pinMode(motorRight1,OUTPUT);
  pinMode(motorRight2,OUTPUT);

  receiver.enableIRIn();
}
//***************************************************************************************************

//Main Method

void loop() {
  if (receiver.decode(&results)) {
    if (results.value == 0XFFFFFFFF) {
      results.value = key_value;
    }
    key_value = results.value;
    
    switch(results.value){

      case 0xFFA25D: //Power button - Start
      Serial.println("(Automatic Shadeseeker Subroutine)");
      AutomaticMode();
      break;
  }
}
}

//***************************************************

//Subroutines

void AutomaticMode(){
while (task == false){
 //Light Sensors
    sensorValueUp = (1023 - analogRead(A0));
    sensorValueLeft = (1023 - analogRead(A1));
    sensorValueRight = (1023 - analogRead(A2));


  if( initialValueExe == true){
    initialValueUp = sensorValueUp;
    initialValueLeft = sensorValueLeft;
    initialValueRight = sensorValueRight;
    initialValueExe = false;
    }

   Serial.println(sensorValueUp);
   Serial.println(sensorValueLeft);
   Serial.println(sensorValueRight);
   
  if (sensorValueUp > sensorValueLeft && sensorValueUp >sensorValueRight){ 
    RGB_Color(255,0,0);
    Serial.println("The Robot will go forward");
    GoForward();
  }
  else if (sensorValueLeft > sensorValueUp && sensorValueLeft > sensorValueRight){
    RGB_Color(0,255,0);
    Serial.println("The Robot will turn left");
    TurnLeft();
  }
  else if (sensorValueRight > sensorValueUp && sensorValueRight > sensorValueLeft){
    RGB_Color(0,0,255);
    Serial.println("The Robot will turn right");
    TurnRight();
  }
  else{
    Serial.println("Error: No direction detected");
  }
  
  //Ultrasonic Sensors
   digitalWrite(trigger, LOW);
   delayMicroseconds(2);
   digitalWrite(trigger, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigger, LOW);
   duration = pulseIn(echo, HIGH);
   distance = duration / 29 / 2;

  Serial.print(distance);
  Serial.println("cm");

  if (distance < range){
    RGB_Color(255,255,0);
    Serial.print("Turning...");
    TurnLeft();
    delay(1000);
    }
  else{}
  
  //End Condition
  if (sensorValueUp > initialValueUp + 100 && sensorValueLeft > initialValueLeft + 100 && sensorValueRight > initialValueRight + 100){
      TaskCompleted();
      task = true;
    }
}
}

void GoForward() {// Moves both motors forwards
  digitalWrite(motorLeft1,HIGH);
  digitalWrite(motorRight1,HIGH);
}

void TurnLeft() { //Turns right motor on, keeps left motor off
  digitalWrite(motorLeft1,LOW);
  digitalWrite(motorRight1,HIGH);
}

void TurnRight() { // Turns left motor on, keeps right motor off
    digitalWrite(motorLeft1,HIGH);
    digitalWrite(motorRight1,LOW);
}
  
void RGB_Color(int redValue, int greenValue, int blueValue){ // Turns the RGB LED a certain color to specify direction
   digitalWrite(ledPinR, redValue);
   digitalWrite(ledPinG, greenValue);
   digitalWrite(ledPinB, blueValue);
}

void TaskCompleted(){ //Shuts everything down, task completed
  RGB_Color(0,0,0);
  digitalWrite(motorLeft1,LOW);
  digitalWrite(motorLeft2,LOW);
  digitalWrite(motorRight1,LOW);
  digitalWrite(motorRight2,LOW);
  digitalWrite(trigger,LOW);
  digitalWrite(echo,LOW);
  Serial.println("Task Completed");
}
