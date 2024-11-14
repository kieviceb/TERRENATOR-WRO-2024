# Terrenator's readme <img src="https://upload.wikimedia.org/wikipedia/commons/a/ab/Flag_of_Panama.svg" alt="Bandera de Panamá" width="30"/>
![TERRENATOR (30 x 18 cm) (25 x 6 cm) (1)](https://github.com/user-attachments/assets/5c56937c-a3a7-425d-ac0a-d61c6865f1eb)
                
This is the official repository of the TERRENATOR team, which is representing Panama in the World Robotics Olympics (WRO) 2024 to be held this year in Izmir, Turkey. We chose to participate in the category of Future Engineers this year winning first place in the national robotics olympics in our country. In this repository you can find everything related to the development of our robot.

## Team members
> David Rico

> Ericka Ceballos

> Jean Paul Sosa

## Overview of our repository
* `schemes`- contains the circuit diagram.
* `models` - includes all the 3d printed parts of the robot.
* `src` - the codes for both challenges, with and without obstacles.
* `t-photos` - photos of the team one formal and a funny one.
* `v-photos` - photos of every angle of the robot, including our previuos version .
* `video` - the link to our youtube channel where you can see our robot in action completing both challenges.

## Components
A list of all the electrical and mechanical components in the robot.

| <img src="https://github.com/user-attachments/assets/3312d4d8-3a2c-48f9-8fe6-120640876eb5" alt="Alt 1" width="300"/> | <img src="https://github.com/user-attachments/assets/989e28c3-e055-45da-b284-13386d09e14e" alt="Alt 1" width="200"/> | <img src="https://github.com/user-attachments/assets/ddaac9e1-450c-4d3a-bbb0-b5ebca198fa2" alt="Alt 1" width="200"/> | 
| :------------: |:-------------:| :----------:|
|[Arduino NANO A000005 x1](https://store-usa.arduino.cc/products/arduino-nano?srsltid=AfmBOooU4-IrktQwXymxJgaV7MZPj3cBWDjg6AjQwBmYoQw8es2bz9ex)|[LTC1871 Booster Step x1](https://www.amazon.es/ICQUANZX-Converter-3-5-35VDC-Volt%C3%ADmetro-alimentaci%C3%B3n/dp/B07JL39RK2?language=en_GB)|[Step down LM2596 x1](https://a.co/d/e4jJKCS)|
| <img src="https://github.com/user-attachments/assets/b97f68dd-48d3-46a2-872b-7ddec7fb7df7" width="200"/> | <img src="https://github.com/user-attachments/assets/a8fcd7e0-c196-4b7e-b5ac-055d313b8194" alt="Alt 1" width="150"/> | <img src="https://github.com/user-attachments/assets/6791452a-f339-4d2d-ac77-07d16e8cec6d" alt="Alt 1" width="200"/> |
| [Double Sided Prototype Universal PCB Board x2](https://a.co/d/9mUTqVe) |[TB6612 motor driver](https://a.co/d/fpJSHg1)|[HC-SRO4 ultrasonic sensor x3](https://a.co/d/et6RN4v) |
| <img src="https://github.com/user-attachments/assets/22b69b73-122f-42be-bf56-a7297b1bbb23" width="350"/> |<img src="https://github.com/user-attachments/assets/dc905c65-28e3-42ff-bdde-d905bd22bd75" alt="Alt 1" width="200"/>| <img src="https://github.com/user-attachments/assets/dc0bdda4-aeda-43e9-b355-616e628742ca" alt="Alt 1" width="250"/> |
|[POLOLU Metal Gearmotor 25Dx65L mm MP 12V with 48 CPR Encoder x1](https://www.pololu.com/product/4863)|[INJORA 7KG 2065 micro servo motor x1](https://a.co/d/3OIRFif)|[Tosiicop Airsoft Lipo Battery 11.1V x1](https://a.co/d/4mkS5gP)|
| <img src="https://github.com/user-attachments/assets/75af90ba-1501-44d0-9cf9-13ee83422d15" width="250"/> | <img src="https://github.com/user-attachments/assets/1f0feb97-1b34-44a4-a4b9-ae579832d1af" alt="Alt 1" width="300"/> | <img src="https://github.com/user-attachments/assets/166d29d3-77c1-43ee-b914-0fc7dd4190bc" alt="Alt 1" width="200"/> |
|[Pixy cam v2.1 x1](https://a.co/d/hyOCC5F) | 3d structure (you can find the printables in `models`)|[Toggle-Switch SPDT 6A/125VAC x1](https://a.co/d/65AaiQM) |

## Robot Developoment

- For make all the structure of the robot, it took a loot of time and investigation, we decided to build our robot totally by our self, we develop the robot in Onshape Platform, all the design of the robot and all the models and pieces can be found here in * `models` 

## Mobility Strategy

- Movement: Our robot is powered by a single motor (POLOLU Metal Gearmotor (see list of components)), we used a 3D printed traction system in the back axle of the car, is specifically designed to enhance performance and maneuverability. This system involves a set of precisely engineered gears that provide consistent and reliable power transfer from the motor to the rear wheels. The gear configuration ensures that torque is effectively distributed, enabling the car to maintain traction even during rapid acceleration. It provides the power, control, and reliability needed for the commpetition.

 
- Steering: The steering is handled by a servomotor, during this project we discovered that using a plastic gear servo is not a good choice, because of the speed of the robot, it tends to break easily inside; thats how we ended up using at first a MG995 servo which is metal gear so it doesnt break so easily, finally for the last prototype to reduce weight and size we found a micro metal gear servo (INJORA 7KG 2065 micro servo motor) , with the power of the big one but weighting less and smaller. Having the steering on the front axle, in vertical position to make movement with more precission.  

- Wall detection:For this part it took us a lot of time to figure out a way of using the ultrasonic sensors to avoid the walls successfully, in our previous prototype we had the sensor horizontally and it gave us good lectures most of the time, but not always, thast way investigating a little further and using trigonometry we found out that position the ultrasonic sensors of the sides vertically is way more effective and the main reason we were having trouble at the curves, what was happening was that the TX and RX sides, when reaching the curves, one was further ahead than the other and the pulse did not reach the same place and was sending out erroneous data, therefore when placing them vertically this does not happen, both pulses collide equally, and the signal bounces back uniformly, sending out useful data.
<p align="center">
<img src="https://github.com/user-attachments/assets/da479d5d-0a66-4abc-842d-d36a51ef9c10" alt="Imagen 1" width="600">
</p> 


- Camera / object detection: For the second round, we are using a pixy cam v2, which is a useful and accurate camera for what we need, it has the advantage of having AI integrated, and well which makes it so much easier and faster for object detection.
  
**- Open Challenge**
For the first challenge we use the three ultrasonic sensor to detect where te robot is using axels y points in the map, so the robot knows where it start. We divide the map onto three lanes, and the robot will try to get onto the middle of each lane, that means always will finish in the starting position, to complete the 3 laps it has to turn 12 times and for that we have a counter in the code that detect when the robot completes 12 corners and looks for the starting position, we use ultrasonic sensors, we have three sensors positioned in front and on the sides, that means the robot have the capability to make turns with more presition in least time.
For international tournament we are planning to use PID,a PID (Proportional-Integral-Derivative) is a type of controller used in control systems to maintain a process variable (such as temperature, speed, position, etc.) at a desired value, called a setpoint. The PID controller adjusts the output of a system based on three terms: proportional, integral and derivative, each of which responds to different aspects of the error between the setpoint and the measured process variable. This allow us to have a more efficient way to make the first round in least time, we are thinking touse PID in two diferent ways, first as a subtraction between the measure of the ultraonic sensors, that means will always search for the middle o the rail, or use it as a wall follower dependind in which rail it start.


**- Obstacle Challenge**
Now in this challenge we got the obstacles, the green blocks, red blocks and the parking spot, we found this one much harder than the other one so we decided to go with something more advance, our main component for this round is the PIXY cam, wich is an AI cam that is connected to the arduino and sets the signatures, we recorded 3 signatures in the PIXY cam "g.block", it refers to the green blocks, "r.block", it refers to the red blocks and "pkspot", that refers to the parking spot. for avoid the color blocks, we devide de camara field of view in 9 sector using the camara pixels, 
<p align="center">
<img src="https://github.com/user-attachments/assets/cc3d6845-a564-4b15-b12f-3d4bc58545e4" alt="Imagen 1" width="300">
</p> 
that means that depending on what sector the block is viewed the robot will make an idea of the distance from de block and where the block is positionated in the map, for every sector it will do a different movement to avoid the obstacle, that means that are 9 movements for green blocks and 9 with red blocks, with a total of 18 diferent movements. While trying the code we noticed that the program is to heavy for a normal arduino nano procesor, that´s why we decided to use an arduino NANO ESP32 for motion and measuring of distance, and an arduino NANO A000005 only for procesing the data of the camara. We comunicate both arduino´s with i2c serial comunication, the arduino NANO ESP32 as slave and arduino NANO A000005 as master.


## Mobility strategy
Our robot is completely custom designed, thats why we decided to design our own steering for the robot, the steering design is made to have more precision in turns. We made a steering with a gear attached to the servo motor, at the same time the gear is over a custom design piece with teeth, this allow us to make all type of turns quickly and on a precise way, this pieces are conected to the wheels that make what we call " The Sterring Wheels of the Robot".This pieces make our robot a front steering robot.
For the international Championship we are planning to make a 4 wheel drive motor, that means that the robot will have direction in all there wheels, and is going to make closest turns to avoid obstacles in a better way.

<p align="center">
  <img src="https://github.com/user-attachments/assets/c44aed28-b9c0-4dab-8f1e-8f0ccee451fe" alt="Imagen 1" width="250">
  <img src="https://github.com/user-attachments/assets/22066840-dcbf-4895-86de-d436bbadb6e4" alt="Imagen 2" width="510">
</p>


For power we decided to use the POLOLU Metal Gearmotor 25Dx65L mm MP 12V with 48 CPR Encoder because it has better torque and speed, our robot is build in 3D parts, the structure is very light, which makes it go much faster, at first we decide to use an axle bigger than the front one, but the car make drift on the turns, there is when we notice that the back axle must be the same size as the front one. the motor bar is attached to a small gear, rhis gear is linked with a bigger one, this big gear is paste on a iron bar that is anckled to the back wheels that make the power of th robot. Thinking far away, we think we can use a four-wheels drive for international championship.

<p align="center">
  <img src="https://github.com/user-attachments/assets/d121cff4-57bb-49ff-abe9-caa6e58045c4" alt="Imagen 1" width="500">
</p>

