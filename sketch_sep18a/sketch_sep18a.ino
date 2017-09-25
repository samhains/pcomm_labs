void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);      // open the serial port at 9600 bps:    

  pinMode(13, INPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
}

int num = 0;
int count = 0; 
int maxLightNum = 5;
int minLightNum = 2;
int lightNum = 3;
int prevLightNum = 2;



void loop() {

 
  int  buttonState =  digitalRead(13);
  num = num + 1;

  if (buttonState == HIGH) {
    /*Serial.print(lightNum);*/
    /*Serial.print(",");*/
     /*Serial.println(prevLightNum);*/
    lightNum = lightNum+ 1;
    prevLightNum = lightNum-1;
     if (lightNum > maxLightNum) {
      lightNum = minLightNum;
      prevLightNum = maxLightNum;
      
    }
    digitalWrite(lightNum, HIGH);
    delay(100);
    digitalWrite(prevLightNum, LOW);
    

   
  }
