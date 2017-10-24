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
int readIndex = 0;              // the index of the current reading

const int numReadings = 10;
int readings_x[numReadings];
int readings_y[numReadings];

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

float accel_x = 0.0;
float accel_y = 0.0;
float accel_z = 0.0;
int total_x = 0;
int total_y = 0;

int servo_rotation_x = 0;
int servo_rotation_y = 0;
int avg_servo_rotation_x = 0;
int avg_servo_rotation_x2 = 0;
int avg_servo_rotation_y = 0;
int avg_servo_rotation_y2 = 0;


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
void calculateServoRotationX(){
     int  accel_int  =  accel_x * 100;
     int val = map(accel_int, -100, 100, 0, 180);     // scale it to use it with the servo_x1 (value between 0 and 180)
     servo_rotation_x = constrain(val, 0.0, 180.0);
}

void calculateServoRotationY(){
     int  accel_int  =  accel_y * 100;
     int val = map(accel_int, -100, 100, 0, 180);     // scale it to use it with the servo_x1 (value between 0 and 180)
     servo_rotation_y = constrain(val, 0.0, 180.0);
}

  // void writeToServo(Servo servo, int minRange, int maxRange ){
  //    //int val = analogRead(servo_x1_pin);            // reads the value of the potentiometer (value between 0 and 1023)
  //    delay(100);
  // }

  void setup()
  {
    servo_x1.attach(servo_x1_pin);

    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      readings_x[thisReading] = 0;
      readings_y[thisReading] = 0;
    }
    // servo_x2.attach(servo_x2_pin);
    // servo_y1.attach(servo_y1_pin);
    // servo_y2.attach(servo_y2_pin);
    // Set up both ports at 9600 baud. This value is most important
    // for the XBee. Make sure the baud rate matches the config
    // setting of your XBee .
    XBee.begin(9600);
    Serial.begin(9600);

  }

  void smoothServoX(){
    total_x = total_x - readings_x[readIndex];
    // read from the sensor:
    readings_x[readIndex] = servo_rotation_x;
    // add the reading to the total:
    total_x = total_x + readings_x[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }

    // calculate the average:
    avg_servo_rotation_x = total_x / numReadings;
    // send it to the computer as ASCII digits
  }
  void calculateInverseRotations(){
    avg_servo_rotation_x2 = map(avg_servo_rotation_x,  0,  180, 180, 0);
    avg_servo_rotation_y2 = map(avg_servo_rotation_y,  0,  180, 180, 0);
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
      // showParsedData();
      calculateServoRotationX();
      calculateServoRotationY();
      smoothServoX();
      calculateInverseRotations();
      Serial.print("x1 ");
      Serial.print(avg_servo_rotation_x);
      Serial.print("x2 ");
      Serial.println(avg_servo_rotation_x2);

      delay(200);

      // writeToServo(accel_x, servo_x1, 0, 180);
      // writeToServo(accel_y, servo_y1, 0, 180);
      // writeToServo(accel_x, servo_x2, 180, 0);
      // writeToServo(accel_x, servo_y2, 180, 0);

    }
  }
