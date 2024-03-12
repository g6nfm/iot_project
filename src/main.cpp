#include <Servo.h>
#include <Arduino.h>

// define pins
const int moisturePin = A0;
const int servoPin = 9;


/*@ Author: Jakob Fahey
    I'm not sure how moisture sensors work, but this is just proof of concept, we declare
     some 'thresholds' at the top so the machine understands what is what.
*/
const int plasticThreshold = 200;
const int metalThreshold = 400;
const int biodegradeThreshold = 600;
const int idfklolThreshold = 800;

const int defaultServoAngle = 0;
const int rotationDuration = 5000;  // 5s

// creating the servo object
Servo myServoBins;
Servo trapDoorLeft;
Servo trapDoorRight;

void setup() {
  Serial.begin(9600);
  myServoBins.attach(servoPin);
}

/*@ Author: Jakob Fahey
    This function detects the moisture level of the material in the machine.
     I'm unsure about what we need to do for infrared but thaty can be implemented easily.
*/
int detectMaterialType(int moistureLevel) {
  if (moistureLevel <= plasticThreshold) {
    Serial.println("Plastic detected");
    return 1;  // Plastic
  } else if (moistureLevel <= metalThreshold) {
    Serial.println("Metal detected");
    return 2;  // Metal
  } else if (moistureLevel <= biodegradeThreshold) {
    Serial.println("Biodegradable detected");
    return 3;  // Biodegradable
  } else if (moistureLevel <= idfklolThreshold) {
    Serial.println("idfklol detected");
    return 4;  // Placeholder material type
  } else {
    Serial.println("Unknown material or out of range");
    return 0;  // Unknown material
  }
}

/*@ Author: Jakob Fahey
    This function rotates the Servo motor to the correct angle based on what was deteced in the detection phase.
    These values are just all placeholders for proof of concept.
*/

void rotateServo(int materialType) {
  int servoAngle;

  switch (materialType) {
    case 1:
      servoAngle = 30;
      break;

    case 2:
      servoAngle = 60;
      break;

    case 3:
      servoAngle = 90;
      break;

    case 4:
      servoAngle = 120;
      break;

    default:
      servoAngle = defaultServoAngle;
      break;
  }

  // rotate to angle
  myServoBins.write(servoAngle);

  // wait 5s
  delay(rotationDuration);

}

/*@ Author: Jakob Fahey
    This function rotates the Servo motor trapdoors to the open state, then the closed state.
*/

void openTrapdoors(int openDoorL, int openDoorR) {
  trapDoorLeft.write(openDoorL);
  trapDoorRight.write(openDoorR);

  // wait for material to drop in
  delay(5000);

  // close the trapdoor
  trapDoorLeft.write(defaultServoAngle);
  trapDoorRight.write(defaultServoAngle);
}

void loop() {
  // read moisture
  int moistureLevel = analogRead(moisturePin);

  // detect type
  int materialType = detectMaterialType(moistureLevel);

  // rotate
  rotateServo(materialType);

  //open trapdoor
  openTrapdoors(45, 135);  // adjust angles to what we need later

  //second delay just to make sure it gets in there
  delay(1000);

  // return to default
  myServoBins.write(defaultServoAngle);

 // delay for the loop
  delay(1000);
}