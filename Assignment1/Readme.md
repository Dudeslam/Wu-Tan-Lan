
<h1>Introduction</h1>

This is the handin for the first task in the course WSN conducted in the Spring of 2021. 
The elementes of the project consists of two tasks and is conducted on the embedded HW-platform TelosB with the Contiki OS running. 


## Task 1
This section seeks to explore the features of the TelosB mote by the use of the sensors 
1. Button Sensor
2. Light Sensor
3. RBG LEDs 

The objective of the task is by the use of the light- and button sensor to record a password sequence of button strokes, when the light sensor measures a low luminance(read: finger infront of sensor). This will be stored in the RAM of the Telosb mote and will make it ready for later comparison with attempts to make a sequence close to the recorded one. 
The RGB LEDs should change according when: 
- Password Sequence are recorded.
- Attempt are correct/incorrect.

## Task 2
The section seeks to implement the DTW algorithm to compare the attempt sequence to the password sequence. 
The algorithm evaluates a matrix based on the attempt sequence and the password sequence and finds the optimal path with lowest cost in between. 
If the path is "close enough"(designed to below 2) it will unlock the TelosB.

## Conclusion
The tasks is both completed and tested with success. The mote unlocks when the attempt is good enough and it does not when the attempt sequence is poor.  
