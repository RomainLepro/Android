#include <Arduino.h>
#include "RADIO.h"
#include <Servo.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool changeLight = false;
bool lightState = false;

#define myServosLength 3

RADIO radio;

Servo myServos[myServosLength];//all servos

//3,5,6,9,10,11
int L_pin_servos[myServosLength] = {3,5,6};

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  radio.init();
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  //intiqlisqtion of servos
  for (int i=0;i<myServosLength;i++){
    myServos[i].attach(L_pin_servos[i]);
    myServos[i].write(90); //to calm servos down
  }
}

void loop() {

  radio.update();

  // put your main code here, to run repeatedly:
   // print the string when a newline arrives:
  //delay(100);
  //Serial.println("time is : " + String(millis()));
  radio.quickSerial(1000);
  myServos[0].write(radio.Ox/500*90 + 90);


  if (stringComplete) {
    //send back data received
    Serial.print(millis());
    Serial.print(" : ");
    Serial.print(inputString);
    Serial.print(inputString.substring(0,2));
    Serial.print(" : ");
    Serial.println(inputString.substring(3,inputString.length()-1));

    myServos[1].write(inputString.substring(3,inputString.length()-1).toInt());

    // clear the string:
    inputString = "";
    stringComplete = false;
    lightState = !lightState;
    digitalWrite(LED_BUILTIN, lightState); 
  }

}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n' || inputString.length()>10 ) {
      stringComplete = true;
    }
  }
}