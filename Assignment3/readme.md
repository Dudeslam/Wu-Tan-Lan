# Assignment 3: Channel Sensing
Made by: Morten Sahlertz, Trung Thai, Andreas Arberg and Jacob Kjærager

## Introduction
This is the handin for the third task of the course WSN conducted in the Spring of 2021 at the University of Aarhus. The project is conducted on the embedded HW-platform TelosB that runs the Contiki OS. The TelosB mote got a IEEE 802.15.4 interface which uses the 2.4 GHz ISM band. This assignment seeks to measure the noise/interference at each of 16 channels(11-26).

## Technologies
To this assignment two different technologies has been used. The RSSI on the individual channels is found with the help of the Contiki CC2420 C-library on the mote. 
The second technology is the data visualization, which is done in Python

## Experiments
For the experiments an array was filled with 300 samples and then taking the mean of the values.
This mean value is then added to another buffer to store the mean RSSI the specific channel.
To calculate the RSSI from the CC2420 (The RF chip on the mote) register the following formula is needed according to the datasheet\
![alt text](https://github.com/Dudeslam/Wu-Tan-Lan/blob/master/Assignment3/Illustrations/RSSI_datasheet.PNG?raw=true)\
The function used in the assignment takes this into account and can be seen in the following code snippet\
![alt text](https://github.com/Dudeslam/Wu-Tan-Lan/blob/master/Assignment3/Illustrations/RSSI_function.PNG?raw=true)\
This is done from channel 11 to channel 26 and the results will then be written down in a .txt file.
The results are then visualized in python and compared to the illustration shown in the assignment description. 

### Data visualization
To compare the 16 RSSI signals a Python based data visualization implementation has been made. This will display the results in a generated .html-file with the help from the libary Plotly. The html-file can be seen in the root folder.  

## Results 
Comparing the two images we notice the theory matches the practice quite good. The best channels must be the bars with the lowest value(largest bar) as this gives the loweste noise energy in the channel. The expected best channels is marked on the figure below. The reason this is belived to be the best is because no overlapping with the very powerfull WiFi-channels which is belived to be a large source of interference.  
![alt text](https://github.com/Dudeslam/Wu-Tan-Lan/blob/master/Assignment3/Illustrations/802_BLE_wifi2.PNG?raw=true)\



![alt text](https://github.com/Dudeslam/Wu-Tan-Lan/blob/master/Assignment3/Illustrations/RSSI_16_channels.png?raw=true)\


## Conclusion
Maybe this assignment was a waste of time!
