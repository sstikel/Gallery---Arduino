# Gallery---Arduino
System for driving a shooting gallery - target procession, point tracking, etc.



Overview

System to operate a progressive shooting gallery (think ducks being run on a track from left to right).
This controls the on/start and off/stop operations of a motor to drive the targets, and displays the number of targets hit via points.
Included in the operation are three modes: Continuous, Timer, and Stop and Go.



Components Used

* 1x LafVIN Uno R3 Board (Arduino)
* 1x 830 Tie-points Breadboard
* 1x 5mm red LED
* 1x 5mm green LED
* 2x 220 ohm Resistor
* 1x Relay 5v (simple, not soldered to a PCB)
* 3x push-button switches
* 13x M-M wires (Male to Male jumper wires)
* 4x M-FM wires (Male to Female jumper wires)
* 1x LCD 1602 Module



Pinout


Uno R3:
0  - Unused
1  - Unused
2  - Button (Start/Stop)
3  - Unused
4  - Button (Mode)
5  - Unused
6  - Button (Point Tracking)
7  - Unused
8  - Relay (C1 pin; drive motor)
9  - Unused
10 - Unused
11 - Unused
12 - Unused
13 - Unused
A0 - Unused
A1 - Unused
A2 - Unused
A3 - Unused
A4 - LCD Display (SDA pin)
A5 - LCD Display (SCL pin)
5V - Bread Board Positive ('+') Rail
GND - Bread Board Negative ('-') Rail


LCD Display:
GND - Bread Board Negative ('-') Rail
VCC - Bread Board Positive ('+') Rail
SDA - Uno R3 pin A4
SCL - Uno R3 pin A5


Relay:
NC  - LED Red and 220 ohm resistor
NO  - LED Green and 220 ohm resistor (stand in for motor)
COM - Bread Board Positive ('+') Rail
C1  - Uno R3 pin 8
C2  - Bread Board Negative ('-') Rail



Functionality

Display:
During system start - Top row displays current score and high score for the mode. High score updates with score is score is greater than high score. Bottom row shows the mode/routine (Continuous, Timer, Stop 'n Go). During the Timer mode/routine the number of seconds left on the routine are displayed to the right of "TIMER".

During system stop - Top row displays the last operation of the mode/routine's scores. Bottom row displays "STOPPING.." before displaying "PRESS START".

During system mode change - On mode change the top row shows the last operation of the next mode's scores, bottom rows shows "Mode: <next mode name>" (Continuous, Timer, Stop 'n Go) for a couple seconds then displays "PRESS START". If the mode button is pressed during the operation of a routine, the display shows "STOPPING.." before showing the mode change display.

Buttons:
Start/Stop button - Starts the routine selected by activating the relay to drive the motor, allow point scoring/accumulation, and begins the timer (when in the timer mode/routine).
Mode button - Changes the mode/routine to the next one (continuous -> timer, timer -> stop 'n go, stop 'n go -> continuous) and, if a routine is running, stops the current routine run/operation.
Point button - This is a stand-in for the actual switch/button/sensor that will be used to determine when a target has been knocked over. Currently, a button press adds to the score while a routine is running.

Relay:
NO - Connected to Green LED (stand-in for motor). Indicates system call for the motor to run.
NC - Connected to Red LED. Indicates there is no system call for the motor to run.

