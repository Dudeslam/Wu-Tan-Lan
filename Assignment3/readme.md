# Assignment 3: Channel Sensing
Made by: Morten Sahlertz, Trung Thai, Andreas Arberg and Jacob Kjærager

## Introduction
This is the handin for the third task of the course WSN conducted in the Spring of 2021 at the University of Aarhus. The project is conducted on the embedded HW-platform TelosB that runs the Contiki OS. The TelosB mote got a IEEE 802.15.4 inteface which uses the 2.4 GHz ISM band. This assignment seeks to measure the noise/interference at each of 16 channels(11-26).

## Technologies
To this assignment two different technologies has been used. The RSSI on the individual channels is found with the help of the Contiki CC2420 C-library on the mote. 
The second technology is the data visualization, which is done in Python

### Data visualization
To compare the 16 RSSI signals a Python based data visualization implementation has been made. This will display the results in a generated .html-file with the help from the libary Plotly. The html-file can be seen in the root folder.  

## Results 
Comparing the two images we notice the theory matches the practice quite good. The best channels must be the bars with the lowest value as this gives the loweste noise energy in the channel. 
![alt text](https://github.com/Dudeslam/Wu-Tan-Lan/blob/master/Assignment3/Illustrations/RSSI_16_channels.png?raw=true)\

![alt text](https://github.com/Dudeslam/Wu-Tan-Lan/blob/master/Assignment3/Illustrations/802_BLE_wifi.PNG?raw=true)\

## Conclusion
Maybe this assignment was a waste of time!
