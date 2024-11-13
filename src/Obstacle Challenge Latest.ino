#include <Pixy2.h>
#include <Servo.h>

Pixy2 pixy;
Servo servo;

const int servoPin = 6;
const int centerAngle = 95;  // Ángulo central
const int leftAngle = 70;    // Ángulo para girar a la izquierda
const int rightAngle = 130;  // Ángulo para girar a la derecha


// Pines del motor TB6612
const int motorPin1 = 4;
const int motorPin2 = 5;
const int enablePin = 3;    // PWM para velocidad
const int standbyPin = 13;  // STBY para activar el motor


const int deltaXMin = 78, deltaXMax = 98;
const int epsilonXMin = 101, epsilonXMax = 213;
const int zetaXMin = 210, zetaXMax = 274;

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

void forward(int velocidad) {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, velocidad);
}

void setup() {
  Serial.begin(115200);
  pixy.init();
  servo.attach(servoPin);
  servo.write(centerAngle);  // Posicionar el servo al centro inicialmente
}

void loop() {
  forward(100);

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
