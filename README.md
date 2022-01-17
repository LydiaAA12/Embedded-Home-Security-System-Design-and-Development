# Embedded-Home-Security-System-Design-and-Development


1.	Introduction 

This is an embedded systems based home security system built on the basis of several specific software and hardware tools. The hardware and software resources that are needed to run and observe the functionalities of this project are listed as follows. 

a.	Software Tools and Libraries

- RIBS + RIMS 
- Code Composer Studio
- MATLAB
- ThingSpeak IoT
- MSP432 Driver Library, LCDdriver (Included in the project for convenience)

b.	Hardware Tools

- MSP432P4111 Launchpad Evaluation Board

- BOOSTXL-EDUMKII  BoosterPack kit 
	- TI OPT3001 Light Sensor (emulates the motion detection sensor. Ambient sensor value above a threshold of 100 lux means motion has been detected and vice versa! This value can be readjusted and modified in the code as needed.)
	- Color TFT LCD
	- RGB Multi-Color LED
	- Buzzer

Configure and interface with boosterpack peripherals based on the port pin alignment between msp432p4111 launchpad and boosterpack

2.	System Architecture

 ![image](https://user-images.githubusercontent.com/93744962/149738196-613a5b05-d141-4258-9505-c13b4c9298cf.png)



3.	Source Code

i.	main.c
     
•	 Contains the task-scheduling implementation for three state machines represented as the following tick functions

•	Calls on INITIALIZATION.h to initialize GPIO ports, UART communication as well as  SysTick timer


•	Calls on AMBIENT_SENSOR_LCD.h to initialize and configure the LCD and ambient sensor

•	Sends ’Motion Detected’ event data as well as real time light sensor value data to terminal port

•	Displays messages on LCD
 
•	The tick functions are implemented such as they are executed depending on their time periods (i.e. every 200 ms) and when motion is detected
•	Blue LED lights up and stays on for 10 seconds (emulates porch light)
•	Buzzer toggles every period interval to emulate sound alarm
•	‘Motion Detected!’ message + light sensor value displayed on LCD

ii.	AMBIENT_SENSOR_LCD.c/ AMBIENT_SENSOR_LCD.h

The following function interfaces implemented

a)	Initialization 
•	I2C communication initialization
•	Ambient sensor initialization and configuration
•	LCD configuration 

b)	Display temperature value: when motion is detected
c)	Display basic message : no motion detected
d)	Retrieve sensor value: in lux

iii.	INITIALIZATION.c/ INITIALIZATION .h
     Initializes ports, timer and UART communication 

iv.     GCD.c/ GCD .h
Implements greatest common divisor value for all state machine periods

v.     System_Data_Comm.m

This is a MATLAB script to run locally. It communicates and retrieves ambient sensor data from our CCS code and feeds it to ThinkSpeak IoT platform through their APIs. 

vi. ThingSpeak IoT MATLAB script and Visualizations can be accessed using the following details

  
![image](https://user-images.githubusercontent.com/93744962/149738121-4c45a195-05bc-4b19-83c1-77d51c3ed833.png)
