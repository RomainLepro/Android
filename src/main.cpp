#include <Arduino.h>
#include "RADIO.h"
#include <Servo.h>


//#define DEBUG

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool changeLight = false;
bool lightState = false;

#define myServosLength 8

RADIO radio;

//3,5,6,9,10,11
Servo myServos[myServosLength];//all servos
const int L_pin_servos[myServosLength] = {3,4,5,6,7,8,9,10};
const String L_name_servos[myServosLength] = {"S1","S2","S3","S4","S5","S6","S7","S8"};
String Lstring[myServosLength] = {"9","8","7","6","5","4","3","2"};
int index = 0;



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

float intToFloat(int val)
{
  if(val<0 || val>=1000)
  {
    return 90.f;
  }
  return (float(val)-500.f)/10.f + 90.f;
}


void stringToServos(String order)
{
  if(order.length()<5)
  {
    return;
  }
  String NAME = order.substring(0,2);
  String VALUE = order.substring(3,order.length()-1);
  #ifdef DEBUG
  Serial.print("value");
  Serial.print(":");
  Serial.println(VALUE);
  #endif
  float val = intToFloat(VALUE.toInt());
  

  for(int i = 0;i<myServosLength;i++)
  {
    if(NAME==L_name_servos[i])
    {
      myServos[i].write(val);
      #ifdef DEBUG
      Serial.print(NAME);
      Serial.print(":");
      Serial.println(val);
      #endif
      return;
    }
  }
  #ifdef DEBUG
  Serial.println("UNKNOWN");
  Serial.print(NAME);
  Serial.println(val);
  #endif
}

void loop() {

  radio.update();
  radio.quickSerialDebug(10,Lstring[0].toInt());

  myServos[0].write(Lstring[0].toFloat() / 500.f * 90.f);
  myServos[1].write(Lstring[0].toFloat() / 500.f * 90.f);
  myServos[2].write(Lstring[0].toFloat() / 500.f * 90.f);

  if (stringComplete) {
    //send back data received
    stringToServos(inputString);
    stringComplete = false;
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
    if (inChar == ';' && index<= myServosLength) {
      Lstring[index] = inputString;
      inputString = "";
      index ++;
    }
    else if (inChar == '\n') {
      index = 0;
      inputString = "";
    }
  }
}




/*
SEND

FORMAT OF DATA SENT TO ANDROID

NAME (2char) SEPARATOR (:) PAYLOAD (int 0-999) ENDLN (\n)

exemple:

OX:555\n
OY:120\n

FORMAT OF DATA RECEIVED FROM ANDROID

NAME (2char) SEPARATOR (:) PAYLOAD (int 0-999) ENDLN (\n)

exemple:

SX:555\n
SY:120\n

*/