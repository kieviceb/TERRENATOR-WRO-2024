# Terrenator's readme
![BAnner TERRENATOR](https://github.com/user-attachments/assets/4b1e2dc8-cb9b-4220-bdbd-908330c50c6d)
## Team members
> Ericka Ceballos

> David Rico

> Jean Paul Sosa

## Overview of our repository
* `schemes`- contains the circuit diagram, wiring diagram and driving diagram.
* `models` - includes all the 3d printed parts of the robot.
* `src` - the codes for both challenges, with and without obstacles.
* `t-photos` - photos of the team one formal and a funny one.
* `v-photos` - photos of every angle of the robot.
* `video` - the link to our youtube channel where you can see our robot in action.
  
## Components
A list of all the electrical and mechanical components in the robot.
- Arduino NANO ESP32 x1
- Arduino NANO A000005 x1
- Mini breadboard x2
- TB6612 motor driver 
- HC-SRO4 ultrasonic sensor x3
- POLOLU Metal Gearmotor 25Dx65L mm MP 12V with 48 CPR Encoder x1
- Steren Metal gear servo motor x1
- OVONIC 3s 2200mAh 11.1V Lipo Battery x1
- Proto wires
- Pixy cam v2 x1
- 3d structure (you can find the printables in `models`)
- RC Car wheels x2
- Makeblock car kit wheels x2
- Toggle-Switch SPDT 6A/125VAC x1
- Iron bar for the engine
- (for a more detailed explanation of the components, review the engineering journal)

## Strategy

- robot movement strategy: our mobility system for the robot is powered by a single motor (POLOLU Metal Gearmotor (see list of components)), which connects our two RC wheelcar wheels through a gear, which uses a 3D printed traction system to interlock the action of the motor, that is, so that the two rear wheels have traction, to see how is the rear traction of the vehicle you can see the photos of the robot or the 3D model part in the git hub and also the engineering journal. 

- Steering: The theme of the steering is handled by a servomotor, which is connected to the tires in an ingenious way, because the servomotor (the model of the servomotor in the components part), is connected to a cylinder printed in 3D that works as a gear, in other words that has as teethistos on the outside of it, this because the lower part makes contact with a plate made in 3D with enough teeth that when the cylinder or gear moves, this plate moves in the form of arc or curve, this so that the mobility of the direction and when taking the curves is quite accurate also at the time of calibration.  Then the plate that I mentioned is connected in the form of a rectangle to two other completely flat plates without teeth, one lower and the other upper, that when moving the cylinder that moves the plate moves in the opposite direction to this one. We can imagine it as if we had a steering system on the four wheels, for example if the two front wheels turn to the left, for there to be movement the rear wheels must turn to the right for there to be movement, it is a maximalist way of explaining it, but all this is calibrated through the code. Our steering is simple but at the same time complex, but above all ingenious and quite precise.

- Wall detection: our robot uses  mechanisms to turn around and avoid colliding with walls, we will separate them into two parts that are controlled by the Arduino, the color and distance part. The color part is controlled by a color sensor that is almost attached to the floor, and through code we make it so that if the color sensor looks first at the blue color and then the orange color, it turns to the left, and if it looks first at the orange color and then the blue color, it turns to the right. The only problem with this is that there is a risk that at a certain moment it will derail and not detect anything and go into the wall, that´s why we discart this method. In this case, we use our other system, which would be the ultrasonic sensors, which are like our auxiliary method (now principal). We have three, arranged in a triangle, one on each side and one in front. The ones on the sides measure the distances to the sides, and the sensor in front measures in case the cart looks close to colliding with the wall in front. These two systems are integrated at the same time to make the robot or cart 100 percent autonomous when making its return. (to review how it is designed and components, you can find it in the top folders of the git hub and the engineering journal.)

- Camera / object detection: For the second round, we are using a pixy cam 2.1, which is a useful and accurate camera for what we needed, it has the advantage of being very smart and is integrated with artificial intelligence, and well, in the strategy of round 2 this will be explained in more detail.

**-First Challenge**
For the first challenge we use the three ultrasonic sensor to detect where te robot is using axels y points in the map, so the robot knows where it start. We divide the map onto three rails, and the robot will try to get onto the midel of each rail, that means always will finish in the starting position, to complete the 3 laps it has to turn 12 times and for that we have a counter in the code that detect when the robot complete the 12 corners and looks for the starting position, we use ultrasonic sensors, we have three sensors positioned in front and on the sides, that means the robot have the capability to make turns with more presition in least time.
For international tournament we are planning to use PID A PID (Proportional-Integral-Derivative) is a type of controller used in control systems to maintain a process variable (such as temperature, speed, position, etc.) at a desired value, called a setpoint. The PID controller adjusts the output of a system based on three terms: proportional, integral and derivative, each of which responds to different aspects of the error between the setpoint and the measured process variable. This allow us to have a more efficient way to make the first round in least time, we are thinking touse PID in two diferent ways, first as a subtraction between the measure of the ultraonic sensors, that means will always search for the middle o the rail, or use it as a wall follower dependind in which rail it start.


**- Second Challenge**
Now in this challenge we got the obstacles, the green blocks, red blocks and the parking spot, we found this one much harder than the other one so we decided to go with something more advance, our main component for this round is the PIXY cam, wich is an AI cam that is connected to the arduino and sets the signatures, we recorded 3 signatures in the PIXY cam "g.block", it refers to the green blocks, "r.block", it refers to the red blocks and "pkspot", that refers to the parking spot. for avoid the coor blocks, we devide de camara field of view in 9 sector using the camaras pixels, that means that depending on what sector the block is viewed the robot will make an idea of the distance from de block and where the block is positionated in the map, for every sector it will do a different movement to avoid the obstacle, that means that are 9 movements for green blocks and 9 with red blocks, with a total of 18 diferent movements. when we are trying this we foud that the program is to heavy for a normal arduino nano procesor, that´s why we diecide to use an arduino nano ESP32 for motion and measuring of distance, and a normal arduino nano only for procesing the data of the camara. We comunicate both arduino´s with i2c serial comunication between both, with arduino nano ESP32 as slave and arduino nano as master.
## Mobility strategy
Our robot is completely custom designed, thats why we decide to design their own steering for the robot, the steering we designed is made for have more precision in turns. We made a steering with a gear attached to the servo motor, at the same time the gear is over a custom design piece with theets, this allow us to make al type of turns quickly and on a precise way, this pieces are conected with the wheels that make what we call " The Sterring Wheels of the Robot".This pieces made our robot a front steering robot.
For the interNational Championship we are planning to make a 4 wheel steering motor, that meas that te robot will have direction in all there wheels, and is going to make closest turns to avoid obstacles in a better way.

<p align="center">
  <img src="![WhatsApp Image 2024-09-23 at 7 22 28 AM](https://github.com/user-attachments/assets/c44aed28-b9c0-4dab-8f1e-8f0ccee451fe)" alt="Descripción de la imagen" width="300">
</p>
![WhatsApp Image 2024-09-23 at 7 22 28 AM](https://github.com/user-attachments/assets/c44aed28-b9c0-4dab-8f1e-8f0ccee451fe) 
![WhatsApp Image 2024-09-23 at 6 12 33 AM](https://github.com/user-attachments/assets/22066840-dcbf-4895-86de-d436bbadb6e4)


For power we decided to use the POLOLU Metal Gearmotor 25Dx65L mm MP 12V with 48 CPR Encoder because it has good torque and speed, as our robot is based on 3D parts its structure is very light, which makes it go much faster, at first we decide tu use an axle bigger than the front one, but the car make drift on the turns, there´s when we notice that the back axle must need to be the same size as the front one. the motor bar is attached to a small gear, rhis gear is linked with a bigger one, this big gear is paste on a iron bar that is anckled to the back wheels that make the power of th robot. Thinking far away, we think we can use a four-wheels drive for international championship.

![WhatsApp Image 2024-08-05 at 3 48 41 PM](https://github.com/user-attachments/assets/d121cff4-57bb-49ff-abe9-caa6e58045c4)
