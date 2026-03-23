Task 1.1p overview:


Project was a overall basic one using a nano 33, 2 LEDs, push button, 7 wires and 2 ressistors.

when you press the button the porch light turns on for 30s and hallway for 60s.

In our program we declared the pins we are using for the button, and both LEDs.

how this program works as follows:

we press the button, system becomes active and starts recroding the time and turning the LEDS on, after 30s has passed the porch LED turns off and after another 30s (60s) total our hallway also turns off.
after our system resets and goes back to its default status when button is not pressed.


Code explanation:

I have the code into couple of functions based on our lectures and modular programming.
for instance pinsetup makes sure the lights are off at the start of the program, and setting up of the pins.

checkbuttonpressed  reads the button state and does a check to see the button has been pressed and starts the lights if thats the case


the  active lights turns the lights on and starting the timer.

the control lights function handles our timings and the time that has passed and turns off porch after 30s and hallway after 60s

turnoffligths resets the system after the timer has finished and the lights turn off.
