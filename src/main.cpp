#include <Arduino.h>
#include "RADIO.h"
#include <Servo.h>


//#define DEBUG

String inputString = "";         // a String to hold incoming data
bool changeLight = false;
bool lightState = false;
int compteurSerial = 0;

#define myServosLength 8

RADIO radio;

//3,5,6,9,10,11
Servo myServos[myServosLength];//all servos
const int L_pin_servos[myServosLength] = {4,5,6,7,8,9,10,11};
const String L_name_servos[myServosLength] = {"S1","S2","S3","S4","S5","S6","S7","S8"};
String Lstring[myServosLength] = {"9","8","7","6","5","4","3","2"};
int index = 0;
uint32_t t_update = 0;



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
  radio.quickSerial(10);

  if(millis() - t_update < 100)
  { // normal update
    for(int i = 0;i<myServosLength;i++)
    {
      float value = Lstring[i].toFloat() / 500.f;
      //by default, throtle goes between 40 and 140
      value = (value-1)*60 +90;
      if(i == 2 || i == 3)
      {
        value = max(40,value);
        value = min(140,value);
      }
      myServos[i].write(value);
    }
  }
  else
  {
    compteurSerial++;
    myServos[0].write(90+radio.Ox);
    myServos[1].write(90+radio.Oy);
    myServos[2].write(90+radio.Throtle+radio.Oz/2);
    myServos[3].write(90+radio.Throtle-radio.Oz/2);
    myServos[4].write(90+radio.Oz);
    myServos[5].write(90+radio.A);
    myServos[6].write(90+radio.B);
    if(compteurSerial>50)
    {
      compteurSerial = 0;
      Serial.print("time last completion : ");
      Serial.println(millis() - t_update);
    }
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
      t_update = millis();
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