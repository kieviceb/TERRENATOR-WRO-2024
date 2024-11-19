#include <ESP32Servo.h>
#include <Wire.h>
#include <Arduino.h>

#define SLAVE_ADDR 9  // Slave device address

// Ultrasonic sensor pins
const int trigLeft = 11;
const int echoLeft = 12;
const int trigCenter = 9;
const int echoCenter = 10;
const int trigRight = 7;
const int echoRight = 8;
const int led = 13;  // Pin for an indicator LED

// Motor TB6612 pins
const int motorPin1 = 4;
const int motorPin2 = 5;
const int enablePin = 2; // PWM for speed control
const int standbyPin = 3; // STBY to enable the motor

// Servo and direction
Servo servo;  // Create a Servo object
const int pinServo = 6; // Servo pin
const int angleLeft = 60;  // Servo angle for left
const int angleCenter = 95; // Center angle (straight ahead)
const int angleRight = 130; // Servo angle for right
char region;  // Variable to store detected region
int signature;  // Variable to store block signature
volatile bool dataReceived = false;  // Flag to indicate if data is received
int lane = 0;  // Variable to determine the lane
int turnCount = 0;  // Counter for the number of turns made
unsigned long lastActionTime = 0;  // Variable to store the time of the last action
const long lockoutTime = 2000;  // 2 seconds lockout time between actions

// Distance variables for ultrasonic sensors
long distanceLeft;
long distanceCenter;
long distanceRight;

// Function to measure distance using the ultrasonic sensors
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);  // Measure the round-trip time of the pulse
  long distance = duration * 0.034 / 2;   // Convert the time into distance (cm)

  return distance;  // Return the measured distance
}

// Function to move the robot forward at a specific speed
void moveForward(int speed) {
  digitalWrite(motorPin1, HIGH);  // Activate the motor in forward direction
  digitalWrite(motorPin2, LOW);   // Motor forward direction
  analogWrite(enablePin, speed);  // Apply PWM for speed control
}

// Function to stop the motor
void stopMotor() {
  digitalWrite(motorPin1, LOW);  // Turn off motor
  digitalWrite(motorPin2, LOW);  // Turn off motor
  analogWrite(enablePin, 0);     // Stop the motor
}

// Functions for different actions based on the region and detected signature
void greendelta() {
  Serial.println("Executing left bypass maneuver...");
  moveForward(200);  // Move forward at speed 200
  servo.write(50);  // Turn the servo left (angle 50)
  delay(1000);      // Wait for 1 second
  servo.write(130); // Turn the servo right (angle 130)
  delay(500);       // Wait for 500 ms
  servo.write(angleCenter);  // Return the servo to the center

  moveForward(200);  // Continue moving forward
  delay(10);         // Small delay to avoid erratic readings
  servo.write(130);  // Turn right again
  delay(1000);       // Wait for 1 second
  servo.write(50);   // Turn left again
  delay(500);        // Wait for 500 ms
  servo.write(angleCenter);  // Return to the center
  Serial.println("Action: Green delta.");
}

// Similar functions for other regions, adjusting the servo movement and behavior
void greenepsilon() {
  servo.write(60);  // Move the servo to 60 degrees
  delay(2000);      // Wait for 2 seconds
  servo.write(95);  // Return to center
  Serial.println("Action: Green epsilon");
}

void greenzeta() {
  servo.write(60);  // Move the servo to 60 degrees
  delay(2000);      // Wait for 2 seconds
  servo.write(95);  // Return to center
  Serial.println("Action: Green zeta");
}

// Function to execute an action based on the region and detected signature
void executeAction (char region, int signature) {
  Serial.print("Executing action for Signature ");
  Serial.print(signature);
  Serial.print(" in Region ");
  Serial.println(region);
  
  if (signature == 2) {  // If the detected signature is 2 (green)
    switch (region) {
      case 'D': greendelta(); break;
      case 'E': greenepsilon(); break;
      case 'Z': greenzeta(); break;
      // More cases can be added if desired
    }
  } else if (signature == 4) {  // If the detected signature is 4 (red)
    // Similar code can be added to handle red signatures
  }
}

// Function that executes when data is received via I2C
void receiveData(int byteCount) {
  if (byteCount >= 2) {  // Expecting 2 bytes: region and signature
    region = Wire.read();      // Read the region
    signature = Wire.read();   // Read the signature

    Serial.print("Data received - Region: ");
    Serial.print(region);
    Serial.print(", Signature: ");
    Serial.println(signature);
    dataReceived = true;

    moveForward(200);  // Move forward at a moderate speed
  }
}

void setup() {
  Serial.begin(115200);  // Start serial communication
  delay(500);  // Small delay to ensure communication is ready

  // Setup pins for ultrasonic sensors
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(trigCenter, OUTPUT);
  pinMode(echoCenter, INPUT);
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);

  // Setup pins for the motor
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(standbyPin, OUTPUT);
  pinMode(led, OUTPUT);

  digitalWrite(standbyPin, HIGH);  // Enable the motor

  // Servo setup
  servo.attach(pinServo);  // Attach the servo to the specified pin
  servo.write(angleCenter);  // Position the servo initially in the center

  // Read the distances from the ultrasonic sensors
  distanceLeft = measureDistance(trigLeft, echoLeft);
  distanceCenter = measureDistance(trigCenter, echoCenter);
  distanceRight = measureDistance(trigRight, echoRight);
  delay(100);  // Small delay between measurements

  // Print measured distances
  Serial.print("Distance Left: ");
  Serial.println(distanceLeft);
  Serial.print("Distance Center: ");
  Serial.println(distanceCenter);
  Serial.print("Distance Right: ");
  Serial.println(distanceRight);
  Serial.println("-----------Program started--------- ");
  
  // Initialize as I2C slave
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveData);  // Set interrupt for receiving data
  Serial.println("Slave started.");

  // Determine the lane based on the distances
  if (distanceLeft < 70 && distanceCenter < 150) {
    lane = 3;
  } else if (distanceLeft < 50 && distanceCenter < 150) {
    lane = 2;
  } else if (distanceLeft < 30 && distanceCenter < 150) {
    lane = 1;
  }

  // Print the determined lane
  Serial.print("Lane: ");
  Serial.println(lane);
}

void loop() {
  // Update the distance measurements
  distanceLeft = measureDistance(trigLeft, echoLeft);
  distanceCenter = measureDistance(trigCenter, echoCenter);
  distanceRight = measureDistance(trigRight, echoRight);
  unsigned long currentTime = millis();  // Get the current time

  // Print measured distances
  Serial.print("Distance Left: ");
  Serial.print(distanceLeft);
  Serial.print(" cm, Distance Right: ");
  Serial.print(distanceRight);
  Serial.print(" cm, Distance Center: ");
  Serial.println(distanceCenter);

  // Lane and turn logic based on the distances
  if (distanceLeft < 150 && distanceCenter < 200 && distanceRight < 300) {
    servo.write(angleRight);  // Move the servo to the right
    delay(500);  // Small delay to stabilize the movement
    turnCount++;
    Serial.println("Turning right.");
  } else if (distanceLeft < 100) {
    servo.write(angleLeft);  // Move the servo to the left
    delay(500);  // Small delay to stabilize the movement
    turnCount++;
    Serial.println("Turning left.");
  } else if (distanceCenter < 100) {
    servo.write(angleCenter);  // Keep the servo in the center
    Serial.println("Moving straight.");
  }

  // Check if data has been received via I2C
  if (dataReceived) {
    // Execute the action corresponding to the detected signature and region
    executeAction(region, signature);
    delay(2000);  // Wait for a while before the next action
    dataReceived = false;  // Reset the data received flag
  }

  delay(100);  // Small delay before the next loop iteration
}


