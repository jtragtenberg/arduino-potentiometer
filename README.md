# Arduino Potentiometer
A code to handle potentiometer values on Arduino

It has implemented a Exponential Moving Average Filter and a change detection, so it can execute instructions only when there is a significant change in value. It is possible to change the amount of filtering and also to reduce the bit depth from the original Aduino's ADC 10 bits [0-1023] to 9 bits [0-511], 8 bits [0-255] or 7 bits [0-127] (as in the standard MIDI CC value).

The code was inspired by https://www.norwegiancreations.com/2015/10/tutorial-potentiometers-with-arduino-and-filtering/
