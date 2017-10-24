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
Servo servo_x1;  // create servo_x1 object to control aservo_x1
Servo servo_x2;  // create servo_x1 object to control aservo_x1
Servo servo_y1;  // create servo_x1 object to control aservo_x1
Servo servo_y2;  // create servo_x1 object to control aservo_x1

int servo_x1_pin = 10;
int servo_x2_pin = 11;
int servo_y1_pin = 12;
int servo_y2_pin = 13;

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
void showNewData() {
 if (newData == true) {
   newData = false;
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

  strtokIndx = strtok(receivedChars,"\t");      // get the first part - the string
  accel_x = atof(strtokIndx); // copy it to messageFromPC

  strtokIndx = strtok(NULL, "\t"); // this continues where the previous call left off
  accel_y = atof(strtokIndx);     // convert this part to an integer

  strtokIndx = strtok(NULL, "\t");
  accel_z = atof(strtokIndx);     // convert this part to a float

}

// void parseData() {
//
//     // split the data into its parts
//
//   char * strtokIndx; // this is used by strtok() as an index
//
//   strtokIndx = strtok(receivedChars, "\t");
//   accel_x = atof(strtokIndx);     // convert this part to a float
//
//   strtokIndx = strtok(NULL, "\t");
//   accel_y = atof(strtokIndx);     // convert this part to a float
//
//   strtokIndx = strtok(NULL, "\t");
//   accel_z = atof(strtokIndx);     // convert this part to a float
// }


  void writeToServo(float accel, Servo servo, int minRange, int maxRange ){
     //int val = analogRead(servo_x1_pin);            // reads the value of the potentiometer (value between 0 and 1023)
     int  accel_int  =  accel * 100;
     int val = map(accel_int, -100, 100, minRange, maxRange);     // scale it to use it with the servo_x1 (value between 0 and 180)
     val = constrain(val, 0.0, 180.0);
     Serial.println(val);
     servo.write(val);                  // sets the servo_x1 position according to the scaled value
     delay(100);
  }

  void setup()
  {
    servo_x1.attach(servo_x1_pin);
    // servo_x2.attach(servo_x2_pin);
    // servo_y1.attach(servo_y1_pin);
    // servo_y2.attach(servo_y2_pin);
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
      // Serial.println(XBee.read());
      recvWithEndMarker();
      showNewData();
      // Serial.println(receivedChars);
      parseData();
      showParsedData();
      // writeToServo(accel_x, servo_x1, 0, 180);
      // writeToServo(accel_y, servo_y1, 0, 180);
      // writeToServo(accel_x, servo_x2, 180, 0);
      // writeToServo(accel_x, servo_y2, 180, 0);

    }
  }
