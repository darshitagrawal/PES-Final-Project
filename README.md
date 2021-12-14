## DIGITAL ANGLE GAUGE

###### Author: - Darshit Nareshkumar Agrawal
###### Email: - daag8293@colorado.edu
###### Sources: - 1)Freescale ARM Cortex-M Embedded Programming by Mazidi and Naimi 2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers by Alexander Dean 3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo  4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo       
                  
---

#### _TABLE OF CONTENTS_
- Introduction
- Command Line
- Calibration
- Taking Commands Again
- Testing
- LED Lights
- References

### 1) Introduction
A digital angle gauge is implemented using MMA8451Q accelerometer interfaced via I2C bus. This project has an interactive command terminal which takes input from the user at which angle the saw blade is supposed to be inclined relative to the blade. Color of the LED changes to give user the idea whether the desired angle of the board is reached. White LED is turned on at zero-degree. Similarly, if the angle is less than desired angle, the color of the LED is nearer to red with its intensity increasing as the angle reaches closer to the angle given by the user. This can be seen with the intensity increasing in steps of RED LED. The color of the LED is Red at full intensity once the desired angle is reached. The color of the LED is nearer to Green if the angle is greater than the desired angle with its intensity increasing as the angle increases. The angle is also sent as output over UART every one second to at least one decimal for the user to see. This will give the user an idea whether they are getting close or moving away from the desired orientation and allow them to set the saw blade on a table saw to exactly any degree relative to the table. To calibrate the board for its 0 deg setting, a switch is connected over GPIO which generates an interrupt to indicate 0 deg. To give the command again for another input angle, one more switch is connected, triggering of which will stop displaying the angle over UART and prompt the user for another input. 

Note: The project is designed for range of angle between 0 to 180 degree. Any results (LED color and message over UART) for orientation more than 180 degree or lesser than 0 degree in either direction should be discarded. Negative angles should also be disregarded.

### 2) Command line
In this project we are using the following commands: 
1) author : Print the name of the author (Here, Darshit Nareshkumar Agrawal).
2) help: Print the relevant commands and their information.
3) input `<angle in degrees>`: Angle at which gauge has to be placed.
4) If the user gives command other than the above four, unknown command diagnostic message will be displayed.

### 3) Calibration
For calibrating the project, we have connected a switch at Port D pin 4. When the switch is pressed, the gauge will set the current angle at which it has been tilted as 0 and all the successive angles will be displayed as per this new reference. GPIO interrupt is used to handle this operation.

### 4) Taking Commands Again: 
If the user wishes to give command again for some other angle, another switch connected to Port D pin 5 is used. Pressing this switch will make the program to come out of inifinite loop, and prompt the user to give command again. Calibration will be back to zero reference. 

### 5) Testing
The below table gives testing details of different elements taken in the project.

|           Test   Cases          |                                                Description                                               |                                                                             Procedure                                                                            |                       Output on   success                       |                                     Output on   Failure                                    |
|:-------------------------------:|:--------------------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------------------------------------------------------:|:---------------------------------------------------------------:|:------------------------------------------------------------------------------------------:|
|                                 |                                                                                                          |                                                                                                                                                                  |                                                                 |                                                                                            |
|    Circular   Buffer Testing    | Test enqueuing and dequeuing of circular buffers used for transmitting and receiving data over UART.     |                                       Number of elements enqueued and dequeued are checked many times for a given string                                         | Program runs uinterrupted and goes to the successive function   | Program stops and shows the test case that has been failed through assert instruction.     |
|      Tricolor LED   testing     |                       Tests whether the LED is blinking for all the three colors.                        |                                                 Each color LED is glown alternatively at full intensity for around 1 second.                                     |              Via Visual inspection, we can check.               |                           Via Visual inspection, we can check.                             |
| MMA 'WHO AM I' register testing |                                  Test the value of WHO AM I register.                                    |                                Via I2C read, by reading the data from the WHO AM I register address, we should get its value.                                    |              Print relevant diagnosstic message.                |                            Print relevant diagnostic message.                              |
|  Accelerometer readings testing |                Verify whether the readings provided by accelerometer are correct or not.                 |                  Take 10 sample values from accelerometer and check whether the average of the readings match the standard reference values.                     |              Print relevant diagnosstic message.                |                            Print relevant diagnostic message.                              |
|    Self-test on accelerometer   |               Checks the accelerometer functionality without external mechanical stimulus                | Load the Control Register 1 in standby mode, then load it in self-test mode from control register 2 and then put it in active mode using control register 1.     | Can be checked as per the response from self_test() function.   |               Can be checked as per the response from self_test() function.                |

### 6) LED Lights
LED colors will be changes and by using PWM, intensity of colors will also change.
1) If the angle is zero degree, color of the LED will be white.
2) If the angle is less than the input angle given by the user, RED LED will glow with its intensity increasing as the angle closes in to the desired angle.
3) At the input angle given by the user, color of the LED will be RED at full intensity.
4) At  angle greater than the input angle given by the user, color of the LED will be GREEN, with its intensity increasing as the angle increases from the input angle.
5) LED color will be VIOLET in case if MMA initialization fails.

### 7) Video and Photo
Please find below links to access video and image. Please access it with the university email ID.

https://drive.google.com/file/d/1gFNJf9MGFDIajw-hcHhOf3vHu210NH00/view?usp=sharing
https://drive.google.com/file/d/1UKwcWd26QRLo0U0OP5YpzGzkKC_mZUZT/view?usp=sharing
https://drive.google.com/file/d/1ILiOJz_nWCQHjRXOzyu0Mrtcs7mGwM59/view?usp=sharing

### 7) References
1)	KL25 Sub-Family Reference Manual.
2)	FRDM-KL25Z User’s Manual.
3)	MMA851Q datasheet by NXP Semiconductors.
4)	NXP Application note Tilt Sensing using a Three-Axis accelerometer.
5)	Chapter 8 in Embedded Systems Fundamentals with ARM Cortex-M based Microcontrollers: A Practical Approach by Alexander G Dean.
6)	Freescale Arm Cortex-M Embedded Programming by Mazidi & Naimi Arm Series.

