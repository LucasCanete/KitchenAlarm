# KitchenAlarm
 A timer to avoid forgetting stuff in the oven
<h2>Goal of the project:</h2>
The goal was to solve a real world problem and in the journey program an avr without arduino, learn about interrupts and timers, and finally about pcb design.

<h2>Components:</h2>
  <br>-Atmega168</br> 
  <br>-Rotary Encoder</br> 
  <br>-Buzzer</br>
  
<h2>How it works</h2>
When device is powered, initially nothing happens until the button of the rotary encoder is pressed. If it is not pressed, after one minute, the device goes to sleep.
In case it is pressed, we can configure the minutes, which are displayed on the 7-segment display, with the rotary encoder. With one press, we can now confire the seconds. With another press
we activate the alarm, and the device starts counting down starting by the introduced minute and second. When the device reaches 0, a buzzer is activated and it won't shut down until the button is pressed.
After this the device goes again on "waiting for response" mode.

<h2>What i learned</h2>
 <br>-Program atmega168 without arduino</br>
 <br>-Use of interrupts</br>
 <br>-Use of timers</br>
 <br>-Use of deep sleep</br>
 <br>-A bit about pcb design</br>

<h2>Things i would have done differently</h2>
<br>-Chosen a smaller atmega168 perhaps smd version instead of tht to save space</br>
<br>-Have all of the pcb grounded. The design is prone to produce noise because pcb is not grounded</br>
<br>-Plan the software better, perhaps using state machines</br>

<br></br>

![pizza_clock_1](https://github.com/LucasCanete/KitchenAlarm/assets/57593487/5ca24377-a864-4aa4-a371-c07be57d8711)


![pizza_clock_2](https://github.com/LucasCanete/KitchenAlarm/assets/57593487/21c0dfde-b75c-4e0b-abf3-99eb028f383b)
