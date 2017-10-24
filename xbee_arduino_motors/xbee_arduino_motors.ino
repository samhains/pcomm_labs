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
#define INPUT_SIZE 11
#include <Servo.h>


// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX
Servo servo;  // create servo object to control a servo

int servo_pin = 13;

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

float accel_x = 0.0;
float accel_y = 0.0;
float accel_z = 0.0;

boolean newData = false;


void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  if (XBee.available() > 0) {
    while (XBee.available() > 0 && newData == false) {
      rc = XBee.read();

      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        ndx = 0;
        newData = true;
      }
    }
  }
}

void showParsedData() {
 Serial.print("x ");
 Serial.println(accel_x);
 Serial.print("y ");
 Serial.println(accel_y);
 Serial.print("z ");
 Serial.println(accel_z);
}
void parseData() {

    // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index
  strtokIndx = strtok(receivedChars, "\t");
  accel_x = atof(strtokIndx);     // convert this part to a float
  strtokIndx = strtok(NULL, "\t");
  accel_y = atof(strtokIndx);     // convert this part to a float
  strtokIndx = strtok(NULL, "\t");
  accel_z = atof(strtokIndx);     // convert this part to a float
}

  void showNewData() {
    if (newData == true) {
      Serial.print("This just in ... ");
      Serial.println(receivedChars);
      newData = false;
    }
  }

  void writeToServo(){
     //int val = analogRead(servo_pin);            // reads the value of the potentiometer (value between 0 and 1023)
     //val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
     //servo.write(val);                  // sets the servo position according to the scaled value
     //delay(15);                           // waits for the servo to get there
  }

  void setup()
  {
    servo.attach(servo_pin);
    // Set up both ports at 9600 baud. This value is most important
    // for the XBee. Make sure the baud rate matches the config
    // setting of your XBee.
    XBee.begin(9600);
    Serial.begin(9600);

  }

  void loop()
  {

    if (XBee.available())

    {
      recvWithEndMarker();
      parseData();
      showParsedData();
      //writeToServo();

    }
  }
