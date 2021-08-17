# Automated-BPM-Monitor
we are gonna interface MAX30102: pulse oximetry and heart rate monitor module with Arduino UNO board, and then measure BPM using this module + OLED display The MAX30102 can be found in different modules, we have a WAVGAT version, it's not a problem as long as the IC is MAX30102.<br>
The heart rate module used in the arduino code is a custom module imported from the arduino IDE 
This code works with MAX30102 + 128x32 OLED i2c and Arduino UNO
It's displays the Average BPM on the screen, with an animation and a buzzer sound everytime a heart pulse is detected
