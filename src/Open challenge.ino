//Library for using the sevo motor on arduino nano esp32
#include <ESP32Servo.h>

// the 3 ultrasonic sensors pin declaration
//left ultrasonic sensor pins
const int trigIzquierdo = 11;
const int echoIzquierdo = 12;

//center ultrasonic sensor pins
const int trigCentro = 9;
const int echoCentro = 10;

//right ultrasonic sensor pins
const int trigDerecho = 7;
const int echoDerecho = 8;

//quejesto???
const int led = 13;

// TB6612 motor driver pins
const int motorPin1 = 4;
const int motorPin2 = 5;
const int enablePin = 2; // PWM for speed control
const int standbyPin = 3; // STBY for motor activation

// Servo and its angles
Servo servoDireccion; // declaring servo as "servoDireccion"
const int pinServo = 6; // servo pin
const int anguloIzquierda = 60; //servo turning left angle
const int anguloCentro = 95; //servo center angle
const int anguloDerecha = 130; // servo turning right angle

// ahahahahha
int carril = 0;

//jajajajaj
int curva = 0;

unsigned long tiempoUltimaAccion = 0; // Variable for stoage the last action
const long bloqueoTiempo = 2000;      // Time block of two seconds

// distances variables
long distanciaIzquierdo;
long distanciaCentro;
long distanciaDerecho;

// Function to measure distance of each ultrasonic sensor
long medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  long distancia = duracion * 0.034 / 2; // Convertir a cm

  return distancia;
}

//****

// Function to start the motor forward
void avanzar(int velocidad) {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, velocidad);
}

// Function to stop the motor
void detenerMotor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 0);
}

//****

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);
  delay(500); // Espera para asegurar que la comunicación serial esté lista

  // Configuración de pines
  pinMode(trigIzquierdo, OUTPUT);
  pinMode(echoIzquierdo, INPUT);
  pinMode(trigCentro, OUTPUT);
  pinMode(echoCentro, INPUT);
  pinMode(trigDerecho, OUTPUT);
  pinMode(echoDerecho, INPUT);

  // Configuración del motor y standby
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(standbyPin, OUTPUT);
  pinMode(led, OUTPUT);

  // Activar el controlador de motor
  digitalWrite(standbyPin, HIGH);

  // Configuración del servo
  servoDireccion.attach(pinServo);
  servoDireccion.write(anguloCentro); // Posición inicial en el centro

  // Leer distancias de los sensores
  distanciaIzquierdo = medirDistancia(trigIzquierdo, echoIzquierdo);
  distanciaCentro = medirDistancia(trigCentro, echoCentro);
  distanciaDerecho = medirDistancia(trigDerecho, echoDerecho);
  delay(100);
  
  // Imprimir distancias para depuración
  Serial.print("Distancia Izquierdo: ");
  Serial.println(distanciaIzquierdo);
  Serial.print("Distancia Centro: ");
  Serial.println(distanciaCentro);
  Serial.print("Distancia Derecho: ");
  Serial.println(distanciaDerecho);

  //****

  // Condiciones para determinar el carril
  if (distanciaIzquierdo < 70 /&& distanciaCentro < 150/) {
    carril = 3;
    digitalWrite(led, HIGH);
  }
  if (distanciaIzquierdo < 50 /&& distanciaCentro < 150/) {
    carril = 2;
    digitalWrite(led, HIGH);
  }
  if (distanciaIzquierdo < 30 /&& distanciaCentro < 150/) {
    carril = 1;
    digitalWrite(led, HIGH);
  }

  // Imprimir valor del carril
  Serial.print("Carril: ");
  Serial.println(carril); // Imprime el valor de carril con un salto de línea
}
//****
void loop() {
  // Leer distancias de los sensores
  distanciaIzquierdo = medirDistancia(trigIzquierdo, echoIzquierdo);
  distanciaCentro = medirDistancia(trigCentro, echoCentro);
  distanciaDerecho = medirDistancia(trigDerecho, echoDerecho);
  unsigned long tiempoActual = millis();

  // Imprimir los valores de las distancias
  Serial.print("Distancia Izquierda: ");
  Serial.print(distanciaIzquierdo);
  Serial.print(" cm, Distancia Derecha: ");
  Serial.print(distanciaDerecho);
  Serial.print(" cm, Distancia Centro: ");
  Serial.print(distanciaCentro);
  Serial.println(" cm");

  //*****

  // Condición para girar a la derecha si el sensor izquierdo mide más de 150 y el central menos de 75
  if (tiempoActual - tiempoUltimaAccion >= bloqueoTiempo) {
    if (distanciaIzquierdo > 150 && distanciaCentro < 75 && distanciaIzquierdo < 500 && distanciaCentro > 1) {
      servoDireccion.write(anguloIzquierda); // Gira a la derecha
      //avanzar(150); // Avanza por un tiempo determinado
      delay(1000);  // Tiempo de giro
      // Después del giro, avanzar en línea recta
      servoDireccion.write(anguloCentro); // Volver al centro
      /*delay(100);5
      while (distanciaIzquierdo > 100 && distanciaCentro < 180) {
        //avanzar(150); // Sigue avanzando
        distanciaIzquierdo = medirDistancia(trigIzquierdo, echoIzquierdo);
        distanciaCentro = medirDistancia(trigCentro, echoCentro);
      }*/
      // Actualizar el tiempo de la última acción
      tiempoUltimaAccion = millis();
      curva = curva + 1;
    }

    // Condición para girar a la izquierda si el sensor derecho mide más de 150 y el central menos de 75
    if (distanciaDerecho > 110 && distanciaCentro < 120 && distanciaDerecho < 500 && distanciaCentro > 1) {
      servoDireccion.write(anguloDerecha); // Gira a la izquierda
      //avanzar(150); // Avanza por un tiempo determinado
      delay(1750);  // Tiempo de giro

      // Después del giro, avanzar en línea recta
      servoDireccion.write(anguloCentro); // Volver al centro
      /*delay(100);

      while (distanciaDerecho > 100 && distanciaCentro < 180) {
        //avanzar(150); // Sigue avanzando
        distanciaDerecho = medirDistancia(trigDerecho, echoDerecho);
        distanciaCentro = medirDistancia(trigCentro, echoCentro);
      }*/

      // Actualizar el tiempo de la última acción
      tiempoUltimaAccion = millis();
      curva = curva + 1;
    }
  }

  //*****

  if (distanciaIzquierdo < 35 && distanciaIzquierdo > 1) {
    servoDireccion.write(110); // Gira a 110 grados
    avanzar(255); // Avanza por 200 ms
    delay(10);
  }

  // Condición adicional: Si la distancia del sensor derecho es menor de 25 cm
  if (distanciaDerecho < 35 && distanciaDerecho > 1) {
    servoDireccion.write(80); // Gira a 80 grados
    avanzar(255); // Avanza por 200 ms
    delay(10);
  }
  //****

  if (curva == 12 && carril == 1){ //movimiento para posicionarse deonde empeso
    servoDireccion.write(95);
    delay(400);
    servoDireccion.write(80);
    delay(1000);
    detenerMotor();
    delay(60000);
  }

   if (curva == 12 && carril == 2){ 
    servoDireccion.write(95);
    delay(1000);
    detenerMotor();
    delay(60000);
  }

   if (curva == 12 && carril == 3){
    
    servoDireccion.write(95);
    delay(400);
    servoDireccion.write(110);
    delay(1000);
    detenerMotor();
    delay(60000);
  }
  //*****

  // Si no se cumplen las condiciones, sigue avanzando en línea recta
  if (!(distanciaIzquierdo > 150 && distanciaCentro < 50) && !(distanciaDerecho > 150 && distanciaCentro < 50)) {
    servoDireccion.write(anguloCentro); // Centro
    avanzar(255); // Avanza a una velocidad moderada
  }
}
