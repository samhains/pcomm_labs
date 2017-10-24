/*****************************************************************
XBee_Serial_Passthrough.ino

Set up a software serial port to pass data between an XBee Shield
and the serial monitor.

Hardware Hookup:
  The XBee Shield makes all of the connections you'll need
  between Arduino and XBee. If you have the shield make
  sure the SWITCH IS IN THE "DLINE" POSITION. That will connect
  the XBee's DOUT and DIN pins to Arduino pins 2 and 3.

*****************************************************************/
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
#include "CurieIMU.h"

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX

float ax, ay, az;   //scaled accelerometer values

void setup()
{

  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(9600);
 // initialize device
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();
  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);

}

void loop()
{

  CurieIMU.readAccelerometerScaled(ax, ay, az);
  printCalculatedAccelsToXBee();
  printCalculatedAccelsToSerial();
}


void printCalculatedAccelsToSerial()
{
    Serial.print(ax, 3);
    Serial.print("\t");
    Serial.print(ay, 3);
    Serial.print("\t");
    Serial.print(az, 3);
    Serial.print("\t");
    Serial.println();
}

void printCalculatedAccelsToXBee()
{
    XBee.print(ax, 3);
    XBee.print("\t");
    XBee.print(ay, 3);
    XBee.print("\t");
    XBee.print(az, 3);
    XBee.print("\t");
    XBee.println();
}
