// Library for using the servo motor on the Arduino Nano ESP32
#include <ESP32Servo.h> // This library provides an interface to control servo motors with ESP32-based boards.

// The 3 ultrasonic sensors pin declaration
// Left ultrasonic sensor pins
const int trigIzquierdo = 11; // Trigger pin for the left ultrasonic sensor
const int echoIzquierdo = 12; // Echo pin for the left ultrasonic sensor

// Center ultrasonic sensor pins
const int trigCentro = 9; // Trigger pin for the center ultrasonic sensor
const int echoCentro = 10; // Echo pin for the center ultrasonic sensor

// Right ultrasonic sensor pins
const int trigDerecho = 7; // Trigger pin for the right ultrasonic sensor
const int echoDerecho = 8; // Echo pin for the right ultrasonic sensor

// LED pin (indicated with a question mark "quejesto???", possibly for debugging or signaling)
const int led = 13; // Pin connected to an LED, likely used to indicate a specific event or condition

// TB6612 motor driver pins
const int motorPin1 = 4; // Pin connected to IN1 of the TB6612 motor driver
const int motorPin2 = 5; // Pin connected to IN2 of the TB6612 motor driver
const int enablePin = 2; // PWM pin to control motor speed
const int standbyPin = 3; // STBY pin to activate/deactivate the motor driver

// Servo and its angles
Servo servoDireccion; // Declares the servo motor as "servoDireccion"
const int pinServo = 6; // Pin where the servo motor is connected
const int anguloIzquierda = 60; // Angle for turning the servo to the left
const int anguloCentro = 95; // Center angle of the servo (neutral position)
const int anguloDerecha = 130; // Angle for turning the servo to the right

// Variable to store the lane number, initialized to 0
int carril = 0; 

// Variable for detecting curves, initialized to 0
int curva = 0;

unsigned long tiempoUltimaAccion = 0; // Stores the time of the last action
const long bloqueoTiempo = 2000; // Time block for 2 seconds to avoid frequent actions

// Variables for storing the distances measured by each ultrasonic sensor
long distanciaIzquierdo;
long distanciaCentro;
long distanciaDerecho;

// Function to measure distance using an ultrasonic sensor
long medirDistancia(int trigPin, int echoPin) {
  // Send out a pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse
  long duracion = pulseIn(echoPin, HIGH);
  
  // Convert the duration into distance (in centimeters)
  long distancia = duracion * 0.034 / 2; 

  return distancia;
}


// Function to make the motor move forward
void avanzar(int velocidad) {
  digitalWrite(motorPin1, HIGH); // Set IN1 to high
  digitalWrite(motorPin2, LOW);  // Set IN2 to low
  analogWrite(enablePin, velocidad); // Apply the speed using PWM
}

// Function to stop the motor
void detenerMotor() {
  digitalWrite(motorPin1, LOW); // Set both IN1 and IN2 to low to stop the motor
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 0); // Stop the PWM signal
}

// Function executed once at the beginning
void setup() {
  // Initialize serial communication at a baud rate of 9600
  Serial.begin(9600);
  delay(500); // Short delay to ensure serial communication is ready

  // Set the pins for the ultrasonic sensors
  pinMode(trigIzquierdo, OUTPUT);
  pinMode(echoIzquierdo, INPUT);
  pinMode(trigCentro, OUTPUT);
  pinMode(echoCentro, INPUT);
  pinMode(trigDerecho, OUTPUT);
  pinMode(echoDerecho, INPUT);

  // Set the motor and standby pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(standbyPin, OUTPUT);
  pinMode(led, OUTPUT); // Set the LED pin

  // Activate the motor driver (take it out of standby mode)
  digitalWrite(standbyPin, HIGH);

  // Attach and initialize the servo motor
  servoDireccion.attach(pinServo);
  servoDireccion.write(anguloCentro); // Start the servo in the center position

  // Measure distances from all ultrasonic sensors
  distanciaIzquierdo = medirDistancia(trigIzquierdo, echoIzquierdo);
  distanciaCentro = medirDistancia(trigCentro, echoCentro);
  distanciaDerecho = medirDistancia(trigDerecho, echoDerecho);
  delay(100); // Small delay

  // Print out the measured distances for debugging
  Serial.print("Distancia Izquierdo: ");
  Serial.println(distanciaIzquierdo);
  Serial.print("Distancia Centro: ");
  Serial.println(distanciaCentro);
  Serial.print("Distancia Derecho: ");
  Serial.println(distanciaDerecho);

  // Check distances to determine the lane based on sensor readings
  if (distanciaIzquierdo < 70 && distanciaCentro < 150) {
    carril = 3; // Set lane to 3 if both left and center distances meet the condition
    digitalWrite(led, HIGH); // Turn on the LED
  }
  if (distanciaIzquierdo < 50 && distanciaCentro < 150) {
    carril = 2; // Set lane to 2 if left distance is smaller
    digitalWrite(led, HIGH); // Turn on the LED
  }
  if (distanciaIzquierdo < 30 && distanciaCentro < 150) {
    carril = 1; // Set lane to 1 if left distance is even smaller
    digitalWrite(led, HIGH); // Turn on the LED
  }

  // Print the current lane number
  Serial.print("Carril: ");
  Serial.println(carril); // Print the lane value with a line break
}

// Main loop function, runs repeatedly
void loop() {
  // Measure distances from all ultrasonic sensors
  distanciaIzquierdo = medirDistancia(trigIzquierdo, echoIzquierdo);
  distanciaCentro = medirDistancia(trigCentro, echoCentro);
  distanciaDerecho = medirDistancia(trigDerecho, echoDerecho);
  
  // Get the current time (in milliseconds)
  unsigned long tiempoActual = millis();

  // Print the sensor distances
  Serial.print("Distancia Izquierda: ");
  Serial.print(distanciaIzquierdo);
  Serial.print(" cm, Distancia Derecha: ");
  Serial.print(distanciaDerecho);
  Serial.print(" cm, Distancia Centro: ");
  Serial.println(distanciaCentro);

  // Condition to turn right if the left sensor detects an obstacle far away but the center is close
  if (tiempoActual - tiempoUltimaAccion >= bloqueoTiempo) {
    if (distanciaIzquierdo > 150 && distanciaCentro < 75 && distanciaIzquierdo < 500 && distanciaCentro > 1) {
      servoDireccion.write(anguloIzquierda); // Turn the servo to the left
      delay(1000);  // Delay to allow turning
      servoDireccion.write(anguloCentro); // Return servo to center
      tiempoUltimaAccion = millis(); // Update the last action time
      curva += 1; // Increment the curve count
    }

    // Condition to turn left if the right sensor detects an obstacle far away but the center is close
    if (distanciaDerecho > 110 && distanciaCentro < 120 && distanciaDerecho < 500 && distanciaCentro > 1) {
      servoDireccion.write(anguloDerecha); // Turn the servo to the right
      delay(1750);  // Delay for turning
      servoDireccion.write(anguloCentro); // Return the servo to the center
      tiempoUltimaAccion = millis(); // Update the last action time
      curva += 1; // Increment the curve count
    }
  }

  // If an object is detected on the left at less than 35 cm, make a small adjustment
  if (distanciaIzquierdo < 35 && distanciaIzquierdo > 1) {
    servoDireccion.write(110); // Turn to 110 degrees
    avanzar(255); // Move forward at full speed for a short time
    delay(10);
  }

  // If an object is detected on the right at less than 35 cm, make a small adjustment
  if (distanciaDerecho < 35 && distanciaDerecho > 1) {
    servoDireccion.write(80); // Turn to 80 degrees
    avanzar(255); // Move forward at full speed for a short time
    delay(10);
  }

  // Special maneuver for lane 1 after 12 curves
  if (curva == 12 && carril == 1) {
    servoDireccion.write(95); // Center the servo
    delay(400);
    servoDireccion.write(80); // Turn slightly left
    delay(1000);
    detenerMotor(); // Stop the motor for 60 seconds
    delay(60000); 
    servoDireccion.write(60); // Further left adjustment
    avanzar(255); // Move forward at full speed for 1.5 seconds
    delay(1500);
  }
  
  // Special maneuver for lane 2 after 12 curves
  if (curva == 12 && carril == 2) {
    servoDireccion.write(95); // Center the servo
    delay(400);
    servoDireccion.write(110); // Turn slightly right
    delay(1000);
    detenerMotor(); // Stop the motor for 60 seconds
    delay(60000); 
    servoDireccion.write(125); // Further right adjustment
    avanzar(255); // Move forward at full speed for 1.5 seconds
    delay(1500);
  }
  
  // Reset the curve count after executing the special maneuver
  if (curva >= 12) {
    curva = 0; // Reset the curve count
  }
}
