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
#include <SFE_MMA8452Q.h> // Includes the SFE_MMA8452Q library
MMA8452Q accel;

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(11, 12); // RX, TX


void setup()
{

  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(9600);
  accel.init();

}

void loop()
{
  if (accel.available()){
    accel.read();
    printCalculatedAccelsToXBee();
    // printCalculatedAccelsToSerial();
  }
}


void printCalculatedAccelsToSerial()
{
    Serial.print(accel.cx, 3);
    Serial.print("\t");
    Serial.print(accel.cy, 3);
    Serial.print("\t");
    Serial.print(accel.cz, 3);
    Serial.print("\t");
    Serial.println();
}

void printCalculatedAccelsToXBee()
{
    XBee.print(accel.cx, 3);
    XBee.print("\t");
    XBee.print(accel.cy, 3);
    XBee.print("\t");
    XBee.print(accel.cz, 3);
    XBee.print("\t");
    XBee.println();
}
