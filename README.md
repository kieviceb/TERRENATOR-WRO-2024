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
- Arduino UNO R3
- Mini breadboard
- TB6612FNG motor driver
- TCS3472 colour sensor 
- POLOLU Metal Gearmotor 25Dx65L mm MP 12V with 48 CPR Encoder
- Steren Metal gear servo motor 
- OVONIC 3s 2200mAh 11.1V Lipo Battery
- Proto wires
- Pixy cam
- Duracell 5000mAh power bank
- 3d structure (you can find the printables in `models`)
- RC Car wheels x4

## Strategy
**First Challenge**
For the first challenge we usea the colour sensor to detect the orange and blue lines on te corners like traffic signals, so the robot knows when to turn, if it sees orange first and then blue it means turn to the right, if it sees blue and then orange means turn to the left and to complete the 3 rounds it has to turn 12 times and for that we have a counter in the code, and the ultrasonic sensors, we have one on each side to avoid the walls and dont crash.

**Second Challenge**
Now in this challenge we got the obstacles, the green blocks, red blocks and the parking spot, we found this one much harder than the other one so we decided to go with something more advance, our main component for this round is the PIXY cam, wich is an AI cam that is connected to the arduino and sets the signatures, we recorded 3 signatures in the PIXY cam "g.block", it refers to the green blocks, "r.block", it refers to the red blocks and "pkspot", that refers to the parking spot, so when the camera sees any of this objects in the track it automatically detects it and then the code can process it, with the PIXY cam is much easier to process images than using a normal camera with artificial vision; In the code when the camera sends the signatures it can send many at the same time so in the code there is a conditional that uses the first signature detected and executes the action all that in a three round loop, after that the robot is able to look for the parking spot and execute the function "parkeate" and finish the challenge.

## Mobility strategy
