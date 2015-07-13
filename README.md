# WetBot
measure the frequency of one up to six capacitive humidity sensors and make decisions for watering (ArduinoYùn)

## Requirements
- Arduino Yùn
- microSD-Card

## Installation
The both sides of the Yùn must be installed properly
### For the Yùn-side (linino):
The Yùn needs to be in your local network and connected to the internet.
The SD-card need to be integrated properly. Use the "[Yún Disk Space Expander](http://www.arduino.cc/en/Tutorial/ExpandingYunDiskSpace)" and follow the instrudtions.
Copy the `install.sh` script into the root of your sd-card and execute it. This will install php and configure the uhttpd apropriate. MySQL will be installed with the data-directory on the sd-card, too.
### For the Arduino-side (32U4):
Here the sketch WetBot.ino needs to be uploaded using the Arduino IDE. I recommend the COM-connection to the Arduino.

## Usage
At the current status the sample usage with a breadborad and LEDs can be seen here:

IMAGE!

The LEDs are showing the grade of dryness for the last triggered sensor.

