# Into the code

## Open Challenge
### **1. Librerías y Constantes**

```cpp
#include <Servo.h>
#include "Simple_MPU6050.h"
#include <Wire.h>

#define MPU6050_ADDRESS_AD0_LOW     0x68
#define MPU6050_ADDRESS_AD0_HIGH    0x69
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW
```

**Explicación**:  
- Se incluyen las librerías necesarias para el proyecto:
  - `Servo.h`: Para controlar el servo.
  - `Simple_MPU6050.h`: Una biblioteca simplificada para comunicarse con el sensor MPU6050, que mide la orientación.
  - `Wire.h`: Permite la comunicación I2C, utilizada por el MPU6050.
- Se definen las direcciones del MPU6050 para especificar cómo se conecta al microcontrolador. En este caso, se usa la dirección `0x68` (valor por defecto).

---

### **2. Variables Globales**

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

**Explicación**:  
- **MPU6050**: 
  - `mpu`: Objeto para interactuar con el sensor.
  - `yawActual` y `yawInicial`: Almacenan el ángulo de orientación (yaw), clave para saber en qué dirección está orientado el robot.
- **Servo**: 
  - `servoDireccion`: Controla la dirección del robot.
  - Ángulos (`anguloIzquierda`, `anguloCentro`, `anguloDerecha`): Determinan las posiciones mínimas, centrales y máximas del servo.
- **Control PD**: 
  - `Kp` y `Kd`: Constantes del controlador proporcional-derivativo.
  - `lastError`: Almacena el error previo para calcular la derivada.
- **Estado del robot**: 
  - `curva`: Cuenta las curvas detectadas.
  - `enMarcha`: Indica si el robot está en movimiento.
- **Sensores ultrasónicos**: 
  - Pines para medir distancias (izquierda, centro, derecha).
- **Motores**: 
  - Pines que controlan la dirección y velocidad.
- **Control de tiempo**:
  - `tiempoUltimaCurva`: Evita que el robot detecte la misma curva varias veces seguidas.
  - `bloqueadorI` y `bloqueadorD`: Limitan cuándo el robot puede realizar giros en cada dirección.

---

### **3. Procesamiento del MPU6050**

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

**Explicación**:  
Esta función procesa los datos del MPU6050 para calcular la orientación del robot en el eje yaw (rotación horizontal).  
- Se convierten los datos del sensor en ángulos utilizando las funciones de la biblioteca `Simple_MPU6050`.
- El ángulo `yawActual` se actualiza para que el resto del programa pueda usarlo.

---

### **4. Control del Servo**

```cpp
void inicializarControlPD() {
    servoDireccion.attach(pinServo);
}
```

**Explicación**:  
Inicializa el servo conectándolo al pin especificado (`pinServo`).

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

**Explicación**:  
Mueve el servo de forma suave y controlada, ajustando gradualmente su posición.

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

**Explicación**:  
- Calcula cuánto debe ajustarse el ángulo del servo en función del error detectado y el control PD.
- Aplica el ajuste suavemente usando `move_steer`.

---

### **5. Sensores Ultrasónicos**

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

**Explicación**:  
- Mide la distancia desde un sensor ultrasónico enviando un pulso (trig) y midiendo cuánto tarda en regresar (echo).
- Calcula la distancia en centímetros usando la fórmula de tiempo y velocidad del sonido.

---

### **6. Control de Motores**

```cpp
void avanzar(int velocidad) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    analogWrite(enablePin, velocidad);
}
```

**Explicación**:  
Hace que los motores giren hacia adelante a la velocidad especificada.

---

```cpp
void detener() {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    analogWrite(enablePin, 0);
}
```

**Explicación**:  
Detiene los motores completamente.

---

### **7. Detección de Curvas**

```cpp
void detectarCurva(long distanciaIzquierda, long distanciaCentro, long distanciaDerecha) {
    float error = distanciaIzquierda - distanciaDerecha;
    lastError = error;
    float sum = distanciaIzquierda + distanciaDerecha;

    unsigned long tiempoActual = millis();
    if (tiempoActual - tiempoUltimaCurva >= bloqueoTiempo) {
        if (error > 1 && distanciaCentro < 58 && sum > 90 && distanciaIzquierda > bloqueadorI) {
            // Giro a la izquierda
        } else if (error < -1 && distanciaCentro < 58 && sum > 90 && distanciaDerecha > bloqueadorD) {
            // Giro a la derecha
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

**Explicación**:  
Detecta curvas basándose en las diferencias de distancia medidas por los sensores.  
- Determina si se debe girar a la izquierda o derecha. 
- Evita giros innecesarios gracias a los bloqueadores de tiempo (`bloqueoTiempo`).

---

### **8. Setup**

```cpp
void setup() {
    Serial.begin(115200);
    // Configuración de pines
    // Inicialización de servo y MPU6050
}
```

**Explicación**:  
- Configura los pines de entrada y salida.
- Inicializa el servo y el sensor MPU6050 para comenzar a recibir datos.

---

### **9. Loop**

```cpp
void loop() {
    if (!enMarcha) {
        // Espera hasta que se presione un botón.
    }
    if (enMarcha) {
        // Lee datos del MPU y ejecuta la lógica de actualización.
    }
}
```

**Explicación**:  
- El robot espera a que el botón sea presionado para empezar.
- Una vez en marcha, lee datos del MPU6050 y sensores ultrasónicos, ajusta el servo y controla los motores.
