#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

const int motor1A = 3; // Motor 1, IN1
const int motor1B = 4; // Motor 1, IN2
const int motor2A = 5; // Motor 2, IN3
const int motor2B = 6; // Motor 2, IN4
const int ENA = 9;      // Enable for Motor 1 and 3
const int ENB = 10;     // Enable for Motor 2 and 4

const int relayPin = 2; // Relay control pin

// Movement command structure
struct Ucommand {
  int movement; // 0: Stop, 1: Forward, 2: Backward, 3: Right, 4: Left, 5: Forward Right, 6: Forward Left, 7: Backward Right, 8: Backward Left
  bool spraying; // True: Spray ON, False: Spray OFF
};

Ucommand command; 

void setup() {
  Serial.begin(9600); 

  // Initialize motor control pins
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Initialize relay pin
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Start with relay off

  // Initialize radio
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); // Start listening for incoming data
}

void loop() {
  if (radio.available()) { 
    radio.read(&command, sizeof(command)); // Read the incoming data

    Serial.print("Received movement: ");
    Serial.print(command.movement);
    Serial.print(", Spraying: ");
    Serial.println(command.spraying);

    // Logic Relay control
    digitalWrite(relayPin, command.spraying ? LOW : HIGH); // Control relay based on spraying state

    // Logic for motor control
    switch (command.movement) {
      case 0: // Stop
        stopMotors();
        break;
      case 1: // Forward
        moveForward();
        break;
      case 2: // Backward
        moveBackward();
        break;
      case 3: // Right
        turnRight();
        break;
      case 4: // Left
        turnLeft();
        break;
      case 5: // Forward Right
        moveForwardRight();
        break;
      case 6: // Forward Left
        moveForwardLeft();
        break;
      case 7: // Backward Right
        moveBackwardRight();
        break;
      case 8: // Backward Left
        moveBackwardLeft();
        break;
      default:
        stopMotors(); // Fallback to stop
        break;
    }
  }
}


/* --------------- LOGIC FOR MOTOR FOR MOVEMENT OF THE ROBOT --------------- */

void stopMotors() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void moveForward() {
  analogWrite(ENA, 255); 
  analogWrite(ENB, 255);
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void moveBackward() {
  analogWrite(ENA, 255); 
  analogWrite(ENB, 255); 
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}

void turnRight() {
  analogWrite(ENA, 255); 
  analogWrite(ENB, 255); 
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}

void turnLeft() {
  analogWrite(ENA, 255); 
  analogWrite(ENB, 255); 
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void moveForwardRight() {
  analogWrite(ENA, 255); 
  analogWrite(ENB, 128); 
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void moveForwardLeft() {
  analogWrite(ENA, 128); 
  analogWrite(ENB, 255); 
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void moveBackwardRight() {
  analogWrite(ENA, 255); 
  analogWrite(ENB, 128); 
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}

void moveBackwardLeft() {
  analogWrite(ENA, 128); 
  analogWrite(ENB, 255); 
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}
