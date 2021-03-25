
<h1>Introduction</h1>

This is the handin for the first task of the course WSN conducted in the Spring of 2021 at the University of Aarhus. 
The elementes of the project consists of two tasks and is conducted on the embedded HW-platform TelosB that runs the Contiki OS. 

## Task 1
This section seeks to explore the features of the TelosB mote by the use of the sensors:
- Button Sensor
- Light Sensor
- RBG LEDs 

The objective of the task is by the use of the light- and button sensor to record a password sequence of button strokes, when the light sensor measures a low luminance. This will be stored in the RAM of the Telosb mote and will make it ready for later comparison with attempts to make a sequence close to the recorded one. 
The RGB LEDs should change when: 
- Password Sequences are recorded.
- Attempts are correct/incorrect.

## Task 2
The section seeks to implement the DTW-algorithm to compare the attempt sequence to the stored password sequence. 
The algorithm evaluates a matrix based on the attempt sequence and the password sequence and finds the optimal path with lowest cost in between. 
If the path is "close enough"(designed to below 2) it will unlock the TelosB mote.

## Conclusion
Both tasks are completed and tested with success. The TelosB mote unlocks when the attempt sequence is good and it does not when the attempt sequence is poor.  
