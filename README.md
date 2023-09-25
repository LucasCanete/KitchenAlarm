# KitchenAlarm
 A timer to avoid forgetting stuff in the oven

Components:
  -Atmega168 
  -Rotary Encoder 
  -Buzzer

When device is powered, initially nmothings happens until the button of the rotary encoder is pressed. If it is not pressed, after one minute, the device goes to sleep.
In case it is pressed, we can configure the minutes, which are displayed on the 7-segment display, with the rotary encoder. With one press, we can now confire the seconds. With another press
we activate the alarm, and the device starts counting down starting by the introduced minute and second. When the device reaches 0, a buzzer is activated and it won't shut down until the button is pressed.
After this the device goes again on "waiting for response" mode.

Goal of the project:

The goal was to solve a real world problem and in the journey program an avr without arduino, learn about interrupts and timers, and finally about pcb design.
