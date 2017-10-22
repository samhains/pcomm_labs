/*
Arduino Turn LED On/Off using Serial Commands
Created April 22, 2015
Hammad Tariq, Incubator (Pakistan)

It's a simple sketch which waits for a character on serial
and in case of a desirable character, it turns an LED on/off.

Possible string values:
a (to turn the LED on)
b (tor turn the LED off)
*/
#define INPUT_SIZE 30


char junk;
String inputString="";



void setup()                    // run once, when the sketch starts
{
 Serial.begin(9600);            // set the baud rate to 9600, same should be of your Serial Monitor
 pinMode(13, OUTPUT);
}

void loop()
{
//   static char buffer[80];
//  if (readline(Serial.read(), buffer, 80) > 0) {
//    Serial.print("You entered: >");
//    Serial.print(buffer);
//    Serial.println("<");
//  }
//  if(Serial.available()){
//  while(Serial.available())
//    {
//      char inChar = (char)Serial.read(); //read the input
//      inputString += inChar;        //make a string of the characters coming on serial
//    }
//    Serial.println(inputString);
//    while (Serial.available() > 0)  
//    { junk = Serial.read() ; }      // clear the serial buffer
//    if(inputString == "a"){         //in case of 'a' turn the LED on
//      Serial.println("A");
//      digitalWrite(13, HIGH);  
//    }else if(inputString == "b"){   //incase of 'b' turn the LED off
//      Serial.println("B");
//      digitalWrite(13, LOW);
//    }
//    inputString = "";
//  }

// Get next command from Serial (add 1 for final 0)
char input[INPUT_SIZE + 1];
byte size = Serial.readBytes(input, INPUT_SIZE);
//Serial.print(Serial.read());
// Add the final 0 to end the C string
input[size] = 0;

// Read each command pair 
char* command = strtok(input, "\n");
//Serial.println(command);
while (command != 0)
{
  Serial.println(command);
//    // Split the command in two values
//    char* separator = strchr(command, ':');
//    if (separator != 0)
//    {
//        // Actually split the string in 2: replace ':' with 0
//        *separator = 0;
//        int servoId = atoi(command);
//        ++separator;
//        int position = atoi(separator);
//
//        // Do something with servoId and position
//    }
//    // Find the next command in input string
//    command = strtok(0, "&");
}
}
