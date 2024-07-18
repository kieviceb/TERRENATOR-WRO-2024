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
- Arduino
- Mini breadboard
- L298N H Bridge 
- POLOLU Metal Gearmotor 25Dx65L mm MP 12V with 48 CPR Encoder  
- MPU6050 Gyroscope
- OVONIC 3s 2200mAh 11.1V Lipo Battery
- Proto wires
- Pixy cam
- Duracell 5000mAh power bank
- 3d parts (you can find the printables in `models`)
- RC Car wheels x4

## Strategy
**First Challenge**
For the first challenge our strategy is avoiding to walls, with the gyroscope we get a very precise angle in the turns so the robot can turn efectively, for distance we usea the laser sensors, those will keep the robot from crashing into the walls, this two combined should give us a good view of the track, and by having an accurate lecture of the track we can do it fast with no fear of crashing.

**Second Challenge**
Now in this challenge we got the obstacles, the green blocks, red blocks and the parking spot, we found this one much harder than the other one so we decided to go with something more advance, our main component for this round is the PIXY cam, wich is an AI cam that is connected to the arduino and sets the signatures, we recorded 3 signatures in the PIXY cam "g.block", it refers to the green blocks, "r.block", it refers to the red blocks and "pkspot", that refers to the parking spot, so when the camera sees any of this objects in the track it automatically detects it and then the code can process it, with the PIXY cam is much easier to process images than using a normal camera with artificial vision; In the code when the camera sends the signatures it can send many at the same time so in the code there is a conditional that uses the first signature detected and executes the action all that in a three round loop, after that the robot is able to look for the parking spot and execute the function "parkeate" and finish the challenge.

