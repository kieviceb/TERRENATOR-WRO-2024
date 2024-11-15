#include <Pixy2.h>
#include <Servo.h>

Pixy2 pixy;
Servo servo;

const int servoPin = 6;
const int centerAngle = 95;  // Ángulo central
const int leftAngle = 70;    // Ángulo para girar a la izquierda
const int rightAngle = 130;  // Ángulo para girar a la derecha

float Kp = 0.5;
float Kd = 0.5;
float alpha = 0.15;
float lastError = 0;
float anguloSuavizado = centerAngle;

int trigIzquierdo = 7, echoIzquierdo = 8;
int trigCentro = 9, echoCentro = 10;
int trigDerecho = 11, echoDerecho = 12;

// Pines del motor TB6612
const int motorPin1 = 4;
const int motorPin2 = 5;
const int enablePin = 3;    // PWM para velocidad
const int standbyPin = 13;  // STBY para activar el motor

const int deltaXMin = 78, deltaXMax = 98;
const int epsilonXMin = 101, epsilonXMax = 213;
const int zetaXMin = 210, zetaXMax = 274;

void inicializarControlPD() {
  servo.attach(servoPin);
  servo.write(centerAngle);
}

void move_steer(int pos) {
  int currentPos = servo.read();
// angulo = currentPos;
  if (pos > currentPos) {
    for (int i = currentPos; i <= pos; i++) {
      servo.write(i);
      delay(10);
    }
  } else {
    for (int i = currentPos; i >= pos; i--) {
      servo.write(i);
      delay(10);
    }
  }
}

void ajustarAngulo(float error, int anguloDeseado) {
  float derivada = error - lastError;
  float ajuste = (Kp * error) + (Kd * derivada);
  lastError = error;
  
  anguloDeseado += ajuste;
  anguloDeseado = constrain(anguloDeseado, leftAngle, rightAngle);

  //anguloSuavizado = (alpha * anguloDeseado) + ((1 - alpha) * anguloSuavizado);
 // servoDireccion.write(anguloSuavizado);
  move_steer(anguloDeseado);
}

long medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  return duracion
   * 0.034 / 2;
}

void forward(int velocidad) {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, velocidad);
}

void detectarCurva(long distanciaIzquierda, long distanciaCentro, long distanciaDerecha) {
  // Calcular el error basado en las distancias de los sensores izquierdo y derecho
  float error = distanciaIzquierda - distanciaDerecha;
  lastError  = error;
  float sum  = distanciaIzquierda + distanciaDerecha;

  // Si el error es positivo, significa que el robot está más cerca de la pared derecha y debe girar hacia la izquierda
  if (error > 1 && distanciaCentro < 70 && sum > 100) {
    servo.write(119);  // Gira a la izquierda
    forward(150);  // Mantén la velocidad constante
    delay(1000);   // Mantén el giro durante 1 segundo
    servo.write(90);  // Regresa al centro
  }
  // Si el error es negativo, significa que el robot está más cerca de la pared izquierda y debe girar hacia la derecha
  else if (error < -1 && distanciaCentro < 70  && sum > 100) {
    servo.write(64);  // Gira a la derecha
    forward(150);  // Mantén la velocidad constante
    delay(1000);   // Mantén el giro durante 1 segundo
    servo.write(90);  // Regresa al centro
  }
  // Si el error es pequeño (el robot está casi centrado), ajusta el ángulo para mantenerse en el centro
  else {
    ajustarAngulo(error, 90);  // Ajusta la dirección según el error
    forward(150);  // Mantén la velocidad constante
  }
}

void actualizar() {
  long distanciaIzquierda = medirDistancia(trigIzquierdo, echoIzquierdo);
  long distanciaCentro = medirDistancia(trigCentro, echoCentro);
  long distanciaDerecha = medirDistancia(trigDerecho, echoDerecho);

  detectarCurva(distanciaIzquierda, distanciaCentro, distanciaDerecha);

  // Depuración
  Serial.print("Distancia Izquierda: "); Serial.print(distanciaIzquierda);
  Serial.print(" | Distancia Centro: "); Serial.print(distanciaCentro);
  Serial.print(" | Distancia Derecha: "); Serial.println(distanciaDerecha);
  Serial.print("error: "); Serial.println(lastError);
  //Serial.print("currentPos: "); Serial.println(angulo);
}

void setup() {
  Serial.begin(115200);
  pixy.init();
  servo.attach(servoPin);
  servo.write(centerAngle);  // Posicionar el servo al centro inicialmente

  pinMode(trigIzquierdo, OUTPUT);
  pinMode(echoIzquierdo, INPUT);
  pinMode(trigCentro, OUTPUT);
  pinMode(echoCentro, INPUT);
  pinMode(trigDerecho, OUTPUT);
  pinMode(echoDerecho, INPUT);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(standbyPin, OUTPUT);
  digitalWrite(standbyPin, HIGH);
  
  inicializarControlPD();
}

void loop() {
  //forward(100);

  actualizar();
  delay(50);

  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks > 0) {
    Serial.println(pixy.ccc.numBlocks);

    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      uint8_t signature = pixy.ccc.blocks[i].m_signature;
      int x = pixy.ccc.blocks[i].m_x;
      int y = pixy.ccc.blocks[i].m_y;
      // Comprobación de la firma y región para signature 1
      if (signature == 1) {

        while (x < zetaXMin) {
          move_steer(leftAngle);  // Gira a la izquierda
          pixy.ccc.getBlocks();
          if (pixy.ccc.numBlocks > 0) {
            for (int i = 0; i < pixy.ccc.numBlocks; i++) {
              x = pixy.ccc.blocks[i].m_x;
              Serial.println(signature);

              Serial.println("verde detectado");
              delay(10);
            }
          }
        }

       
      }
      // Comprobación de la firma y región para signature 2
      else if (signature == 2) {

        while (x > deltaXMin) {
          move_steer(rightAngle);  // Gira a la izquierda
          pixy.ccc.getBlocks();
          if (pixy.ccc.numBlocks > 0) {
            for (int i = 0; i < pixy.ccc.numBlocks; i++) {
              x = pixy.ccc.blocks[i].m_x;
              Serial.println(pixy.ccc.blocks[i].m_x);
              Serial.println("rojo detectado");
            }
          }
        }

        servo.write(centerAngle);
        delay(100);
      }
    }
  }

   else {
        servo.write(centerAngle);
        delay(100);
        Serial.println("sali del while");
        }
}
