#include <Arduino.h>
#include "RADIO.h"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool changeLight = false;
bool lightState = false;

RADIO radio;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // put your main code here, to run repeatedly:
   // print the string when a newline arrives:
  delay(1000);
  Serial.println("time is : " + String(millis()));
  if (stringComplete) {
    Serial.print("time is : ");
    Serial.print(millis());
    Serial.print(" : ");
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  if(changeLight){
    Serial.println(inputString);
    changeLight = false;
    lightState = int(millis()/1000)%2;
    digitalWrite(LED_BUILTIN, lightState);   // turn the LED on (HIGH is the voltage level)
    
    
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
    changeLight = true;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}