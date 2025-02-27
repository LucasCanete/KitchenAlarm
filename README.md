# KitchenAlarm
A small Atmega168 based Kitchentimer to measure cooking time for your meals. Forget about downloading another app or using your phone. Do it the old fashioned way, like  it's the 90's. Put the pizza in the oven, configure your Kitchenalarm and let it beep like a fire alarm on steroids when your pizza is ready.

## Table of Contents
1. [About the Project](#about-the-project)
2. [Project Status](#project-status)
3. [Getting Started](#getting-started)

## About the project
This one was one of my first projects writing bare metal C. What i wanted to do is develop the simplest device possible, write the software, make a pcb and print a 3D case for it. A Timer is a device everyone is familiar with, so i decided to go for that. I took an atmega168, programmed it using avrdude and made a simple pcb for the circuitry using KiCad. Using the OnShape software i designed a case for the pcb and this was the end result.
  
## Project Status
The project is still under development after version 1. Some new improvements that are being implemented for version 2 are:
- [x] Grounded plane in both PCB layers to reduce noise
- [x] SMD Components
- [x] USB-C Port for battery charging 
- [x] Programming port to match usbasp
      
These new features have already been implemented in the new KiCad design and can be seen in the **Hardware** directory. Once i solder the new pcb i will upload the results with pictures.

## Getting Started
The Kitchenalarm has a **Rotary Encoder** that lets you configure the Minutes and Seconds through the encoder's rotation.
1. Press the Rotary Encoder on the board to configure the Minutes and adjust it rotating the encoder 
2. Press it once again to configure the Seconds and rotate it to adjust 
3. Press the encoder one last time to activate the Timer with the Minutes and Seconds you configured in step 1 and step 2. The Timer will start counting down on the 7-Segment Display (To reset the countdown press the Encoder)
4. When the Timer reaches 00:00 (on the Display) the buzzer on the Board will start beeping. Your pizza is ready. Press the Encoder once to stop the beeping
5. The Kitchenalarm goes automatically to deep sleep if it is not given instructions. To wake it up just press the Encoder


![pizza_clock_1](https://github.com/LucasCanete/KitchenAlarm/assets/57593487/5ca24377-a864-4aa4-a371-c07be57d8711)


![pizza_clock_2](https://github.com/LucasCanete/KitchenAlarm/assets/57593487/21c0dfde-b75c-4e0b-abf3-99eb028f383b)
