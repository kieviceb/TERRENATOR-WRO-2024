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
