![Copy of TERRENATOR (30 x 18 cm) (5)](https://github.com/user-attachments/assets/710e4716-3b6f-4e79-99c2-809fecb8279f)

---

# Into the code

## Open Challenge
### **1. Libraries and Constants**

```cpp
#include <Servo.h>
#include "Simple_MPU6050.h"
#include <Wire.h>

#define MPU6050_ADDRESS_AD0_LOW     0x68
#define MPU6050_ADDRESS_AD0_HIGH    0x69
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW
```

**Explanation**:  
- Includes the necessary libraries for the project:  
  - `Servo.h`: For controlling the servo motor.  
  - `Simple_MPU6050.h`: A simplified library for working with the MPU6050 sensor, which measures orientation.  
  - `Wire.h`: Enables I2C communication, which the MPU6050 uses.  
- Defines the MPU6050’s I2C addresses. The default address `0x68` is used here.  

---

### **2. Global Variables**

```cpp
Simple_MPU6050 mpu;
float yawActual = 0;
float yawInicial = 0;

Servo servoDireccion;
int pinServo = 6;
int anguloIzquierda = 87;
int anguloCentro = 90;
int anguloDerecha = 105;
int anguloMPU = 0;

float Kp = 0.63;
float Kd = 0.4;
float alpha = 0.15;
float lastError = 0;
int curva = 0;
const int botonPin = A7;
bool enMarcha = false;
float anguloSuavizado = anguloCentro;

int trigIzquierdo = 9, echoIzquierdo = 10;
int trigCentro = 15, echoCentro = 14;
int trigDerecho = 7, echoDerecho = 8;

int motorPin1 = 5, motorPin2 = 4, enablePin = 3, standbyPin = 16;

unsigned long tiempoUltimaCurva = 0;
const long bloqueoTiempo = 2000;
int bloqueadorI = 65;
int bloqueadorD = 65;
```

**Explanation**:  
- **MPU6050 Variables**:  
  - `mpu`: Object to interact with the MPU6050 sensor.  
  - `yawActual` and `yawInicial`: Store the current and initial yaw (horizontal orientation).  
- **Servo Variables**:  
  - `servoDireccion`: Controls the robot’s steering.  
  - `anguloIzquierda`, `anguloCentro`, `anguloDerecha`: Represent the leftmost, center, and rightmost angles of the servo.  
- **PD Control**:  
  - `Kp` and `Kd`: Proportional and derivative gains for the control system.  
  - `lastError`: Stores the previous error to calculate the derivative.  
- **Robot State**:  
  - `curva`: Tracks the number of detected turns.  
  - `enMarcha`: Indicates whether the robot is moving.  
- **Ultrasonic Sensors**:  
  - Define pins for the left, center, and right sensors.  
- **Motor Pins**:  
  - Define the pins controlling motor speed and direction.  
- **Time Management**:  
  - `tiempoUltimaCurva`: Prevents the robot from detecting the same turn repeatedly.  
  - `bloqueadorI` and `bloqueadorD`: Set thresholds to determine when the robot can turn in a specific direction.  

---

### **3. MPU6050 Processing**

```cpp
void procesarMPU(int16_t *gyro, int16_t *accel, int32_t *quat, uint32_t *timestamp) {
    Quaternion q;
    VectorFloat gravity;
    float ypr[3] = { 0, 0, 0 };
    float xyz[3] = { 0, 0, 0 };
    
    mpu.GetQuaternion(&q, quat);
    mpu.GetGravity(&gravity, &q);
    mpu.GetYawPitchRoll(ypr, &q, &gravity);
    mpu.ConvertToDegrees(ypr, xyz);
    
    yawActual = (int)xyz[0];
}
```

**Explanation**:  
This function processes data from the MPU6050 to calculate the robot's yaw (horizontal rotation).  
- The quaternion, gravity vector, and yaw-pitch-roll angles are computed using the `Simple_MPU6050` library.  
- The result is stored in `yawActual` to be used in navigation.  

---

### **4. Servo Control**

```cpp
void inicializarControlPD() {
    servoDireccion.attach(pinServo);
}
```

**Explanation**:  
This function initializes the servo motor by attaching it to the specified pin (`pinServo`).

---

```cpp
void move_steer(int pos) {
    int currentPos = servoDireccion.read();
    if (pos > currentPos) {
        for (int i = currentPos; i <= pos; i++) {
            servoDireccion.write(i);
            delay(10);
        }
    } else {
        for (int i = currentPos; i >= pos; i--) {
            servoDireccion.write(i);
            delay(10);
        }
    }
}
```

**Explanation**:  
Moves the servo motor gradually to the desired position (`pos`) to ensure smooth transitions.

---

```cpp
void ajustarAngulo(float error, int anguloDeseado) {
    float derivada = error - lastError;
    float ajuste = (Kp * error) + (Kd * derivada);
    lastError = error;
    
    anguloDeseado += ajuste;
    anguloDeseado = constrain(anguloDeseado, anguloIzquierda, anguloDerecha);
    move_steer(anguloDeseado);
}
```

**Explanation**:  
- Uses a proportional-derivative (PD) controller to calculate adjustments to the servo angle based on the error.  
- The angle is constrained within the servo’s limits and applied smoothly using `move_steer`.  

---

### **5. Ultrasonic Sensors**

```cpp
long medirDistancia(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(3);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duracion = pulseIn(echoPin, HIGH);
    return duracion * 0.034 / 2;
}
```

**Explanation**:  
- Measures the distance using an ultrasonic sensor.  
- Sends a trigger pulse and measures the time for the echo to return, calculating the distance in centimeters.

---

### **6. Motor Control**

```cpp
void avanzar(int velocidad) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    analogWrite(enablePin, velocidad);
}
```

**Explanation**:  
Drives the motors forward at the specified speed (`velocidad`).

---

```cpp
void detener() {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    analogWrite(enablePin, 0);
}
```

**Explanation**:  
Stops the motors by turning off all outputs.

---

### **7. Curve Detection**

```cpp
void detectarCurva(long distanciaIzquierda, long distanciaCentro, long distanciaDerecha) {
    float error = distanciaIzquierda - distanciaDerecha;
    lastError = error;
    float sum = distanciaIzquierda + distanciaDerecha;

    unsigned long tiempoActual = millis();
    if (tiempoActual - tiempoUltimaCurva >= bloqueoTiempo) {
        if (error > 1 && distanciaCentro < 58 && sum > 90 && distanciaIzquierda > bloqueadorI) {
            // Left turn
        } else if (error < -1 && distanciaCentro < 58 && sum > 90 && distanciaDerecha > bloqueadorD) {
            // Right turn
        } else {
            ajustarAngulo(error, anguloCentro);
            avanzar(200);
        }
    } else {
        ajustarAngulo(error, anguloCentro);
        avanzar(200);
    }
}
```

**Explanation**:  
Detects curves based on differences in the distances measured by the ultrasonic sensors.  
- Determines whether to turn left or right based on the error.  
- Prevents repeated turn detection using a time-based lock (`bloqueoTiempo`).  

---

### **8. Setup Function**

```cpp
void setup() {
    Serial.begin(115200);
    // Pin configurations
    // Initialize servo and MPU6050
}
```

**Explanation**:  
- Sets up pin modes for input and output.  
- Initializes the servo and MPU6050 to start reading data.  
- Prepares the robot for operation.  

---

### **9. Loop Function**

```cpp
void loop() {
    if (!enMarcha) {
        // Waits until the button is pressed.
    }
    if (enMarcha) {
        // Reads MPU data and executes control logic.
    }
}
```

**Explanation**:  
- The robot remains idle until a button press sets it into motion.  
- Once active, the robot reads data from the MPU6050 and ultrasonic sensors, adjusts the servo, and controls the motors to navigate.
 


#Obstacle Challenge

#include <Servo.h> 
#include <Pixy2.h>
#include "Simple_MPU6050.h"
#include <Wire.h>

// MPU6050 configuration
#define MPU6050_ADDRESS_AD0_LOW 0x68
#define MPU6050_ADDRESS_AD0_HIGH 0x69
#define MPU6050_DEFAULT_ADDRESS MPU6050_ADDRESS_AD0_LOW

Simple_MPU6050 mpu;
float yawActual = 0;  // Global variable to store the current Yaw
// Variable for yaw
float yawInicial = 0;

// PD controller global variables
Pixy2 pixy;
Servo servoDireccion;
int pinServo = 6;
int anguloIzquierda = 75;
int anguloCentro = 90;
int anguloDerecha = 119;
int anguloMPU = 0;

float Kp = 0.5;
float Kd = 0.5;
float alpha = 0.15;
float lastError = 0;
int curva = 0;
const int botonPin = A7;  // Button pin
bool enMarcha = false;    // Initial state of the robot: stopped
float anguloSuavizado = anguloCentro;
int AnguloObjetivo = 0;

const int deltaXMin = 50, deltaXMax = 98;
const int epsilonXMin = 101, epsilonXMax = 213;
const int zetaXMin = 250, zetaXMax = 274;

// Sensor pins
int trigIzquierdo = 10, echoIzquierdo = 9;
int trigCentro = 15, echoCentro = 14;
int trigDerecho = 7, echoDerecho = 8;

// Motor pins
int motorPin1 = 5, motorPin2 = 4, enablePin = 3, standbyPin = 16;

// Time control
unsigned long tiempoUltimaCurva = 0;
const long bloqueoTiempo = 5000;
unsigned long tiempoUltimaCurvablo = 0;
const long bloqueoTiempoblo = 4000;
bool bloqueadorI = false;

// Function to process MPU6050 data
void procesarMPU(int16_t *gyro, int16_t *accel, int32_t *quat, uint32_t *timestamp) {
  Quaternion q;
  VectorFloat gravity;
  float ypr[3] = { 0, 0, 0 };
  float xyz[3] = { 0, 0, 0 };

  mpu.GetQuaternion(&q, quat);
  mpu.GetGravity(&gravity, &q);
  mpu.GetYawPitchRoll(ypr, &q, &gravity);
  mpu.ConvertToDegrees(ypr, xyz);

  yawActual = (int)xyz[0];  // Update global Yaw
}

// Function to initialize the PD controller
void inicializarControlPD() {
  servoDireccion.attach(pinServo);
}

// Function to smoothly move the servo to a position
void move_steer(int pos) {
  int currentPos = servoDireccion.read();
  if (pos > currentPos) {
    for (int i = currentPos; i <= pos; i++) {
      servoDireccion.write(i);
      delay(10);
    }
  } else {
    for (int i = currentPos; i >= pos; i--) {
      servoDireccion.write(i);
      delay(10);
    }
  }
}

// Function to adjust the steering angle based on error
void ajustarAngulo(float error, int anguloDeseado) {
  float derivada = error - lastError;
  float ajuste = (Kp * error) + (Kd * derivada);
  lastError = error;

  anguloDeseado += ajuste;
  anguloDeseado = constrain(anguloDeseado, anguloIzquierda, anguloDerecha);
  move_steer(anguloDeseado);
}

// Function to measure distance using an ultrasonic sensor
long medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  return duracion * 0.034 / 2;
}

// Function to calculate the target angle for left curves
int calcularAnguloObjetivoI(int curva) {
  if (curva % 4 == 1) return -75;        // Curves 1, 5, 9, ...
  else if (curva % 4 == 2) return -165;  // Curves 2, 6, 10, ...
  else if (curva % 4 == 3) return 105;   // Curves 3, 7, 11, ...
  else return 15;                        // Curves 4, 8, 12, ...
}

// Function to calculate the target angle for right curves
int calcularAnguloObjetivoD(int curva) {
  if (curva % 4 == 1) return 70;         // Curves 1, 5, 9, ...
  else if (curva % 4 == 2) return 165;   // Curves 2, 6, 10, ...
  else if (curva % 4 == 3) return -110;  // Curves 3, 7, 11, ...
  else return -15;                       // Curves 4, 8, 12, ...
}

// Function to move the robot forward
void avanzar(int velocidad) {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, velocidad);
}

// Function to move the robot backward
void retroceder(int velocidad) {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePin, velocidad);
}

// Function to stop the robot
void detener() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 0);
}
//*******
void detectarCurva(long distanciaIzquierda, long distanciaCentro, long distanciaDerecha) {
  float error = distanciaIzquierda - distanciaDerecha;
  lastError = error;
  float sum = distanciaIzquierda + distanciaDerecha;

  if (distanciaIzquierda < 35 && sum < 100) {
    move_steer(110);
    avanzar(100);
    delay(10);
  } else if (distanciaDerecha < 35 && sum < 100) {
    move_steer(85);
    avanzar(100);
    delay(10);
  } else if (error < -1 && sum > 100 && distanciaCentro < 50) {
    while (true) {
      mpu.dmp_read_fifo();
      pixy.ccc.getBlocks();
      avanzar(100);
      move_steer(69);
      if (signature == 1 || signature == 2 || abs(yawActual - AnguloObjetivo) > 5) break;
    }
    detener();
    delay(100);
  } else if (error > 1 && sum > 100 && distanciaCentro < 50) {
    while (true) {
      mpu.dmp_read_fifo();
      pixy.ccc.getBlocks();
      avanzar(100);
      move_steer(119);
      if (signature == 1 || signature == 2 || abs(yawActual - anguloObjetivo) > 2) break;
    }
    detener();
    delay(100);
  } else {
    move_steer(90);
    avanzar(100);
  }
}

void detectarBloques() {
  long distanciaIzquierda = medirDistancia(trigIzquierdo, echoIzquierdo);
  long distanciaCentro = medirDistancia(trigCentro, echoCentro);
  long distanciaDerecha = medirDistancia(trigDerecho, echoDerecho);
  unsigned long tiempoActualblo = millis();
  float sum = distanciaIzquierda + distanciaDerecha;
  if (tiempoActualblo - tiempoUltimaCurvablo >= bloqueoTiempoblo) {
    pixy.ccc.getBlocks();
    avanzar(100);
    if (pixy.ccc.numBlocks > 0) {
      Serial.println(pixy.ccc.numBlocks);

      for (int i = 0; i < pixy.ccc.numBlocks; i++) {
        uint8_t signature = pixy.ccc.blocks[i].m_signature;
        int x = pixy.ccc.blocks[i].m_x;

        // Comprobación de la firma y región para signature 1
        if (signature == 1) {
          while (x < zetaXMin) {
            avanzar(100);
            move_steer(119);  // Gira a la izquierda
            pixy.ccc.getBlocks();
            if (pixy.ccc.numBlocks > 0) {
              for (int i = 0; i < pixy.ccc.numBlocks; i++) {
                x = pixy.ccc.blocks[i].m_x;
                Serial.println("verde detectado");
                delay(10);
              }
            }
          }
          move_steer(73);
          delay(180);
          move_steer(90);
          delay(500);
          long distanciaIzquierda = medirDistancia(trigIzquierdo, echoIzquierdo);
          long distanciaCentro = medirDistancia(trigCentro, echoCentro);
          long distanciaDerecha = medirDistancia(trigDerecho, echoDerecho);
          float error = distanciaIzquierda - distanciaDerecha;
          lastError = error;
          float sum = distanciaIzquierda + distanciaDerecha;
          if (sum< 110 / error > 10) {
            move_steer(119);
            delay(300);
            move_steer(70);
            delay(100);
            move_steer(90);
          } else if (sum < 110 && error < -10) {
            move_steer(69);
            delay(300);
            move_steer(119);
            delay(50);
            move_steer(90);
          } else {
            move_steer(90);
          }
          tiempoUltimaCurvablo = tiempoActualblo;
        }
        // Comprobación de la firma y región para signature 2
        else if (signature == 2) {
          while (x > deltaXMin) {
            avanzar(100);
            move_steer(69);  // Gira a la derecha
            pixy.ccc.getBlocks();
            if (pixy.ccc.numBlocks > 0) {
              for (int i = 0; i < pixy.ccc.numBlocks; i++) {
                x = pixy.ccc.blocks[i].m_x;
                Serial.println("rojo detectado");
              }
            }
          }
          move_steer(119);
          delay(180);
          move_steer(90);
          delay(500);
          long distanciaIzquierda = medirDistancia(trigIzquierdo, echoIzquierdo);
          long distanciaCentro = medirDistancia(trigCentro, echoCentro);
          long distanciaDerecha = medirDistancia(trigDerecho, echoDerecho);
          float error = distanciaIzquierda - distanciaDerecha;
          lastError = error;
          if (sum < 110 && error > 10) {
            move_steer(119);
            delay(300);
            move_steer(70);
            delay(100);
            move_steer(90);
          } else if (sum < 110 && error < -10) {
            move_steer(69);
            delay(300);
            move_steer(119);
            delay(50);
            move_steer(90);
          } else {
            move_steer(90);
          }
        }
        tiempoUltimaCurvablo = tiempoActualblo;
      }
    }
  }

  else {
    detectarCurva(distanciaIzquierda, distanciaCentro, distanciaDerecha);
  }
}
//*******
void actualizar() {
  long distanciaIzquierda = medirDistancia(trigIzquierdo, echoIzquierdo);
  long distanciaCentro = medirDistancia(trigCentro, echoCentro);
  long distanciaDerecha = medirDistancia(trigDerecho, echoDerecho);

  detectarBloques();
  //detectarCurva(distanciaIzquierda, distanciaCentro, distanciaDerecha);

  if (curva == 12) {
    move_steer(90);
    delay(700);
    detener();
    delay(60000);
  }
  /*   // Imprimir todos los valores
    Serial.print("Yaw: "); Serial.print(yawActual);
    Serial.print(" | Distancia Izquierda: "); Serial.print(distanciaIzquierda);
    Serial.print(" | Distancia Centro: "); Serial.print(distanciaCentro);
    Serial.print(" | Distancia Derecha: "); Serial.print(distanciaDerecha);
    Serial.print(" | Error: "); Serial.println(lastError);*/
}
//*******
void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000);
  if (pixy.init() == 0) {
    Serial.println("Pixy2 inicializado correctamente");
  } else {
    Serial.println("Error al inicializar Pixy2");
    while (true)
      ;  // Detener el programa si falla la inicialización
  }


  // Inicialización de pines
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

  // Inicialización del PD
  inicializarControlPD();

  // Inicialización del MPU6050
  Serial.println(F("Iniciando calibración del MPU6050..."));
  mpu.SetAddress(MPU6050_ADDRESS_AD0_LOW)
    .CalibrateMPU()
    .load_DMP_Image();
  mpu.on_FIFO(procesarMPU);
  pixy.setLamp(1, 0);

  delay(100);
}
void loop() {
  if (!enMarcha) {  // Si el robot NO está en marcha
    delay(100);
    if (analogRead(botonPin) == 0) {  // Si el botón está presionado
      enMarcha = true;                // Cambiar el estado a "en marcha"
      Serial.println("¡Robot en marcha!");
      move_steer(anguloCentro);
    } else {
      detener();  // Mantener el robot detenido
    }
  }
  if (enMarcha == true) {  // Si el robot está en marcha
    //pixy.setLamp(0, 0);
    mpu.dmp_read_fifo();  // Leer datos del MPU6050
    actualizar();         // Ejecutar el control PD
    delay(5);
  }
}
