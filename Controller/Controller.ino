#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// Joystick pins
const int joystickXPin = A0; // X-axis pin
const int joystickYPin = A1; // Y-axis pin
const int buttonPin = 2;      // Push button pin

// Command structure
struct Ucommand {
  int movement; // 0: Stop, 1: Forward, 2: Backward, 3: Right, 4: Left, 5: Forward Right, 6: Forward Left, 7: Backward Right, 8: Backward Left
  bool spraying; // True: Spray ON, False: Spray OFF
};

Ucommand command;

// sensitivity and threshold settings
const int sensitivity = 100; // Sensitivity
const int minJoystick = 400; // Minimum joystick value for movement
const int maxJoystick = 600; // Maximum joystick value for movement

// var for button state logic
bool lastButtonState = HIGH; // Assume button is not pressed initially

void setup() {

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
  
  pinMode(buttonPin, INPUT_PULLUP); 
  Serial.begin(9600); 

  // Initialize command
  command.movement = 0; 
  command.spraying = false;
}

void loop() {

  int joystickX = analogRead(joystickXPin);
  int joystickY = analogRead(joystickYPin);
  bool buttonState = digitalRead(buttonPin);

  // Logic to Toggle spraying
  if (buttonState == LOW && lastButtonState == HIGH) {
    command.spraying = !command.spraying; // Toggle spraying
  }
  lastButtonState = buttonState; // Store last button state

  command.movement = 0;

  /* --------------- JOYSTICK LOGIC TO GET THE DIRECTION --------------- */
  bool moving = false;

  if (joystickY < minJoystick - sensitivity) {
    command.movement = 2; // Move Backward
    moving = true;
  } else if (joystickY > maxJoystick + sensitivity) {
    command.movement = 1; // Move Forward
    moving = true;
  }

  if (joystickX < minJoystick - sensitivity) {
    command.movement = 4; // Move Left
    moving = true;
  } else if (joystickX > maxJoystick + sensitivity) {
    command.movement = 3; // Move Right
    moving = true;
  }
  // diagonal movements
  if (moving) {
    if (joystickY < minJoystick - sensitivity && joystickX > maxJoystick + sensitivity) {
      command.movement = 7; // Move Backward Right
    } else if (joystickY < minJoystick - sensitivity && joystickX < minJoystick - sensitivity) {
      command.movement = 8; // Move Backward Left
    } else if (joystickY > maxJoystick + sensitivity && joystickX > maxJoystick + sensitivity) {
      command.movement = 5; // Move Forward Right
    } else if (joystickY > maxJoystick + sensitivity && joystickX < minJoystick - sensitivity) {
      command.movement = 6; // Move Forward Left
    }
  }

  // Send the command to the robot
  radio.write(&command, sizeof(command));

  Serial.print("movement: ");
  Serial.print(command.movement);
  Serial.print(", Spraying: ");
  Serial.println(command.spraying);

  delay(100); 
}
