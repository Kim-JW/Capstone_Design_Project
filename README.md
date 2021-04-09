# Development of an earthquake early warning system using various devices Earthquake detection algorithm improvement

## Notice
***Due to the nature of the project, security is important, so we do not disclose the code.***


## Contents

### ◦ Implement an earthquake early warning system targeting various devices
  -Porting the MyShake system developed with UC Berkeley to various devices such as ***Raspberry Pi, Arduino***, and ***artificial intelligence speakers.***
  
 -We intend to rewrite the earthquake detection algorithm implemented based on Java/Android in C/C++ language and implement it so that it can be operated on a specific platform.

### ◦ Development of an earthquake detection algorithm using micro-devices and low-cost sensors
 -Currently, the neural network-based earthquake detection algorithm developed at MyShake has a two-step structure that detects earthquakes using motion data for 2 seconds and determines the magnitude of the earthquake through the cloud. To use the following algorithm.
 
   -Step 1: Using a simple classification technique, it determines the presence or absence of an earthquake, generates an alarm immediately, and minimizes the transmission of earthquake information to the cloud.
   -Step 2: Understanding the magnitude and direction of earthquake through the cloud is calculated using only data from smartphones and devices equipped with high-performance sensors.
   
### ◦Development and performance comparison of earthquake detection algorithms for various devices

 -Rewrite the existing seismic detection algorithm based on various devices and compare the performance and precision of each technique to find the optimal algorithm to apply to the previously developed earthquake early warning system.

## System Description

![system](./img/system.jpg ) 

### RaspberryPi

● Porting an earthquake detection algorithm based on a neural network implemented in JAVA code to Raspberry Pi

### Arduino

● Attempt to port an earthquake detection algorithm based on a neural network implemented as JAVA to Arduino Uno and Due

● Implementation of Short Time Average over Long Time Average (STA/LTA) on Arduino Due
   -Due has better performance than Uno, allowing complex calculations.
   -By using this, STA/LTA algorithm superior to PGA is implemented and applied.

## Result

● Neural network-based earthquake detection algorithm written in Java code was rewritten in C/C++ language and applied to Raspberry Pi and tested.

● Using Arduino Uno/Due's PGA and STA-LTA algorithms, an efficient algorithm that considers device performance is applied to realize earthquake detection for various devices.

● Data collection and utilization by creating several Android applications using sensors
