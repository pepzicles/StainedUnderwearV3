#include <Servo.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

// Define servo objects
Servo servoBottom;
Servo servoMiddle;
Servo servoTop;

// Define the pins for the servos
const int relayPin = 6;
const int servoBottomPin = 9;
const int servoMiddlePin = 10;
const int servoTopPin = 11;

// Define the angles for movement
const int bottomMotorDownPosition = 48;
const int bottomMotorUpPosition = 10;

const int middleMotorDownPosition = 42;
const int middleMotorUpPosition = 20;

const int topMotorMiddlePosition = 85;
const int topMotorBackPosition = 45;
const int topMotorFrontPosition = 125;
const int topMotorNoHittingPosition = 40;
const int topMotorDefaultPosition = 60;

struct Day {
  int h;
  int m;
  int scrubs;
};

Day one;
Day two;
Day three;
Day four;
Day five;
Day six;
Day seven;
Day eight;
Day nine;
Day ten;
Day eleven;
Day twelve;
Day thirteen;
Day fourteen;
Day fifteen;

int cycle_day = 1;
int current_hour;
int current_minute;
int day_switched = false;

bool clean_now = true;

void setup() {

  one.h = 15;
  one.m = 43;
  one.scrubs = 100;

  two.h = 16;
  two.m = 12;
  two.scrubs = 98;

  three.h = 19;
  three.m = 30;
  three.scrubs = 70;

  four.h = 15;
  four.m = 01;
  four.scrubs = 75;

  five.h = 14;
  five.m = 49;
  five.scrubs = 93;

  six.h = 16;
  six.m = 28;
  six.scrubs = 82;

  seven.h = 15;
  seven.m = 16;
  seven.scrubs = 90;

  eight.h = 14;
  eight.m = 35;
  eight.scrubs = 78;

  nine.h = 13;
  nine.m = 45;
  nine.scrubs = 87;

  ten.h = 13;
  ten.m = 56;
  ten.scrubs = 80;

  eleven.h = 13;
  eleven.m = 05;
  eleven.scrubs = 110;

  twelve.h = 15;
  twelve.m = 12;
  twelve.scrubs = 120;

  thirteen.h = 14;
  thirteen.m = 45;
  thirteen.scrubs = 89;

  fourteen.h = 15;
  fourteen.m = 01;
  fourteen.scrubs = 100;

  fifteen.h = 19;
  fifteen.m = 07;
  fifteen.scrubs = 98;

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  servoBottom.write(bottomMotorDownPosition);
  servoBottom.attach(servoBottomPin);
  delay(100);

  servoMiddle.write(middleMotorDownPosition);
  servoMiddle.attach(servoMiddlePin);
  delay(100);

  servoTop.write(topMotorDefaultPosition); //start from default for top motor
  servoTop.attach(servoTopPin);

  delay(3000);
  
  cleaning(1); // test running the water pump and the robotic arm one time

  
}

void loop() {

  if (rtc.begin()) {
    DateTime now = rtc.now();
    current_hour = now.hour();
    current_minute = now.minute();

    if (cycle_day == 1 && current_hour == one.h && current_minute == one.m) {
      cleaning(one.scrubs);
    }

    else if (cycle_day == 2 && current_hour == two.h && current_minute == two.m) {
      cleaning(two.scrubs);
    }

    else if (cycle_day == 3 && current_hour == three.h && current_minute == three.m) {
      cleaning(three.scrubs);
    }

    else if (cycle_day == 4 && current_hour == four.h && current_minute == four.m) {
      cleaning(four.scrubs);
    }

    else if (cycle_day == 5 && current_hour == five.h && current_minute == five.m) {
      cleaning(five.scrubs);
    }

    else if (cycle_day == 6 && current_hour == six.h && current_minute == six.m) {
      cleaning(six.scrubs);
    }

    else if (cycle_day == 7 && current_hour == seven.h && current_minute == seven.m) {
      cleaning(seven.scrubs);
    }

    else if (cycle_day == 8 && current_hour == eight.h && current_minute == eight.m) {
      cleaning(eight.scrubs);
    }

    else if (cycle_day == 9 && current_hour == nine.h && current_minute == nine.m) {
      cleaning(nine.scrubs);
    }

    else if (cycle_day == 10 && current_hour == ten.h && current_minute == ten.m) {
      cleaning(ten.scrubs);
    }

    else if (cycle_day == 11 && current_hour == eleven.h && current_minute == eleven.m) {
      cleaning(eleven.scrubs);
    }

    else if (cycle_day == 12 && current_hour == twelve.h && current_minute == twelve.m) {
      cleaning(twelve.scrubs);
    }

    else if (cycle_day == 13 && current_hour == thirteen.h && current_minute == thirteen.m) {
      cleaning(thirteen.scrubs);
    }

    else if (cycle_day == 14 && current_hour == fourteen.h && current_minute == fourteen.m) {
      cleaning(fourteen.scrubs);
    }

    else if (cycle_day == 15 && current_hour == fifteen.h && current_minute == fifteen.m) {
      cleaning(fifteen.scrubs);
    }

    // organizing how we count:
    if (day_switched == false && current_hour == 23 && current_minute == 59) {
      cycle_day += 1;
      day_switched = true;
    }

    if (current_hour == 0 && current_minute == 0) {
      day_switched = false; // resetting value for the day
      clean_now = true;
    }

    if (cycle_day == 16) {
      cycle_day = 1;
    }
  }

  delay(1000);
}

void cleaning(int swipeNum) {
  if (clean_now == true) {
    //Turn on and off water pump
    waterPump();
    clean_now = false;

    //Move robot arm Down
    downMovement();

    //Start Scrubbing
    for (int i = 0; i < (swipeNum - 2) ; i++) {
      backAndForth();
    }

    //Move robot arm Up
    upMovement();
  }
}

void backAndForth() {
  //TOP MOTOR: Middle to Front
  for (int i = topMotorMiddlePosition; i <= topMotorFrontPosition; i++) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorMiddlePosition, topMotorFrontPosition, 3, 20)); // Delay with decreasing interval for acceleration
  }

  // TOP MOTOR: Back to Middle
  for (int i = topMotorFrontPosition; i >= topMotorMiddlePosition; i--) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorFrontPosition, topMotorMiddlePosition, 20, 3)); // Delay with decreasing interval for acceleration
  }

  // TOP MOTOR: Middle to Front
  for (int i = topMotorMiddlePosition; i >= topMotorBackPosition; i--) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorMiddlePosition, topMotorBackPosition, 3, 20)); // Delay with decreasing interval for acceleration
  }

  // TOP MOTOR: Front to Center
  for (int i = topMotorBackPosition; i <= topMotorMiddlePosition; i++) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorBackPosition, topMotorMiddlePosition, 20, 3)); // Delay with decreasing interval for acceleration
  }
}


void waterPump() {
  delay(1000);
  digitalWrite(relayPin, HIGH);
  delay(5000);
  digitalWrite(relayPin, LOW);
  delay(1000);
}

void downMovement() {

  // TOP MOTOR: Default to NoHittingPosition
  for (int i = topMotorDefaultPosition; i >= topMotorNoHittingPosition; i--) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorDefaultPosition, topMotorNoHittingPosition, 3, 20)); // Delay with decreasing interval for acceleration
  }

  //BOTTOM MOTOR: Down Movement
  for (int i = bottomMotorUpPosition; i <= bottomMotorDownPosition; i ++) {
    servoBottom.write(i);
    delay(50);
  }

  // MIDDLE MOTOR: Down Movement
  for (int i = middleMotorUpPosition; i <= middleMotorDownPosition; i ++) {
    servoMiddle.write(i);
    delay(50);
  }

  // TOP MOTOR: NoHittingPosition to Middle
  for (int i = topMotorNoHittingPosition; i <= topMotorMiddlePosition; i++) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorNoHittingPosition, topMotorMiddlePosition, 20, 3)); // Delay with decreasing interval for acceleration
  }
}

void upMovement() {

  //TOP MOTOR: Middle to Back
  for (int i = topMotorMiddlePosition; i <= topMotorFrontPosition; i++) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorMiddlePosition, topMotorFrontPosition, 3, 20)); // Delay with decreasing interval for acceleration
  }

  // TOP MOTOR: Back to Middle
  for (int i = topMotorFrontPosition; i >= topMotorMiddlePosition; i--) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorFrontPosition, topMotorMiddlePosition, 20, 3)); // Delay with decreasing interval for acceleration
  }

  // TOP MOTOR: Middle to NoHittingPosition
  for (int i = topMotorMiddlePosition; i >= topMotorNoHittingPosition; i--) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorMiddlePosition, topMotorNoHittingPosition, 3, 20)); // Delay with decreasing interval for acceleration
  }

  // MIDDLE MOTOR: Up Movement
  for (int i = middleMotorDownPosition; i >= middleMotorUpPosition; i --) {
    servoMiddle.write(i);
    delay(100);
  }

  //BOTTOM MOTOR: Up Movement
  for (int i = bottomMotorDownPosition; i >= bottomMotorUpPosition; i --) {
    servoBottom.write(i);
    delay(100);
  }

  // TOP MOTOR: NoHittingPosition to Default
  for (int i = topMotorNoHittingPosition; i <= topMotorDefaultPosition; i++) {
    servoTop.write(i); // Move servo to the current angle
    delay(map(i, topMotorNoHittingPosition, topMotorDefaultPosition, 20, 3)); // Delay with decreasing interval for acceleration
  }
}
