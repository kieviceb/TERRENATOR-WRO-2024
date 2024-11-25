# Circuit diagram

here you can find the circuit that integrates our robot that will go to turkey, as well as we can find a small explanation of it.
<br>
 <br>


 > [!NOTE]
>The schemes of the circuit was made in [Fritzing](https://fritzing.org/)


<br>

### Explanation:

---

1- Microcontroller (Arduino Nano):
The central component managing all sensors and actuators. It receives signals, 
processes them, and sends output to the connected devices.

---

2- HC-SR04 Ultrasonic Sensors:
The top section of the diagram shows multiple HC-SR04 sensors (typically used for distance measurement). Each sensor has four pins:

VCC: Power supply, connected to the 5V pin of the Arduino.
GND: Ground, connected to Arduino's GND.
TRIG: Trigger pin, connected to individual Arduino digital pins.
ECHO: Echo pin, also connected to digital pins.
These sensors measure distance by sending ultrasonic waves and timing their return.

---

3- MPU6050 (Gyroscope + Accelerometer):
On the left, an MPU6050 module is visible.

VCC and GND are connected to the Arduino's power pins.
SCL and SDA are connected to the Arduino's I2C pins (A5 and A4, respectively, on the Nano).
This module provides orientation and motion data.

---

4- Servo Motor:
Below the MPU6050, a servo motor is connected.

Signal Pin: Goes to an Arduino PWM pin (likely pin 9).
Power and Ground: Connected to an external power supply for adequate current.

---

5- L298N Motor Driver:
Near the motor, there is an L298N or TB6612 motor driver module controlling the DC motor.

Power Input (12V): Supplied from the battery (bottom right).
Output Pins: Connected to the motor terminals for forward/backward movement.
Control Pins: Connected to Arduino digital pins for controlling speed and direction.

---

6- DC Motor:
Connected to the motor driver outputs. This motor is powered by the 12V supply and controlled by the Arduino via the driver.

---


7- Power Supply (Battery Pack):
A 12V Li-ion battery supplies power to the motor driver and, indirectly, to the Arduino.

A voltage regulator is likely used to step down 12V to 5V for the Arduino and other components.

---

8- Switch:
A power switch between the battery and the circuit ensures safe operation and easy ON/OFF control.

---

9- Resistors and Capacitors:
Passive components are included for stability and noise reduction, particularly around the MPU6050 and SD card module.

---

