#include <Wire.h>
#include <Servo.h>
#include <Stepper.h>

const int servoPin = 6;
const int servoPin2 = 7;
const int inductiveSensorPin = 4;
const int irSensor1 = 5;
const int moisturePin = A0;


bool itemDetected = false;

Servo servo1;
Servo servo2;

const int stepsPerRevolution = 2048;
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

int inductiveValue = LOW;

void setup(){
  pinMode(irSensor1, INPUT);
  servo1.attach(servoPin);
  servo2.attach(servoPin2);
  servo1.write(0);
  servo2.write(80);
  pinMode(inductiveSensorPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  MetalTest();
  WetTest();
  bool IsMetal = MetalTest();
  bool IsWet = WetTest();
  int irValue = digitalRead(irSensor1);  // Read the value from the IR sensor
  
  if ((irValue == 0))
  {
    itemDetected = true;
    delay(1500);
    
    if (IsMetal && itemDetected)
    {
      Serial.println("Metal Waste");
      MoveStepper(0.33);
      openBin();
      delay(2000);
      closeBin();
      MoveStepper(1-0.33);
      delay(2000);
    }
    else if (IsWet && itemDetected) 
    {
      Serial.println("Wet Waste");
      MoveStepper(0.66);
      openBin();
      delay(2000);
      closeBin();
      MoveStepper(1-0.66);
      delay(2000);
    }
    else
    {
      Serial.println("Dry Waste");
      delay(2000);
      openBin();
      closeBin();
      delay(2000);
    }
    
    itemDetected = false;
  }
}

void openBin() {
  servo2.write(0);
  delay(500);
  servo1.write(140);
  delay(2000);
}

void closeBin() {
  servo1.write(0);
  delay(500);
  servo2.write(80);
  delay(2000);
}

void MoveStepper(float x) {
	myStepper.setSpeed(10);
	myStepper.step(x*stepsPerRevolution);
	delay(1000);
}   

bool MetalTest() {
  int inductiveValue = digitalRead(inductiveSensorPin);
  if (inductiveValue == 1) {
    return false;
  } else {
    return true;
  }
}

bool WetTest() {
  int moistureValue = analogRead(moisturePin);
  if (moistureValue > 900) {
    return false;
  } else {
    return true;
  }
}

