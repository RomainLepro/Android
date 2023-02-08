#ifndef RADIO_H
#define RADIO_H


#include "Arduino.h"
#include "SPI.h"

#define pin_radio 3 //the pin_radio of the receiver
#define nb_channel 9
#define RC_NUM_CHANNELS  6

//the pin_radio of the receiver pwm => int2
class RADIO
{
    public:

    float health;
    static float radio_control;
    static uint8_t channel;
    int rc_values[nb_channel];
    static uint16_t rc_shared[nb_channel];
    static bool interupted;
    float Ox = 0,Oy = 0,Oz = 0,Throtle = 0;
    float Gx = 0,Gy = 0,Gz = 0; 
    int A=1;
    float B=0;
    static uint32_t t; 
    uint32_t tSerial = 0;
    uint32_t tQuickSerial = 0;
    bool errorState = true;//if some of the value from the channels are out of bound => errorState = true

    void showSerial(int dt = 500);

    void quickSerial(int dt = 10);
    void quickSerialDebug(int dt = 10,int data = 999);

    void init() {
        pinMode(pin_radio,INPUT);
        digitalWrite(pin_radio, LOW);
        
        attachInterrupt(digitalPinToInterrupt(pin_radio),pinFunct,CHANGE);
        Serial.print("interupt attached: ");
        Serial.println(digitalPinToInterrupt(pin_radio));
        }

    static void pinFunct(){ 
        if(digitalRead(pin_radio)){
            rc_shared[channel] = uint16_t(micros()-t);
            t = micros();  
            channel++;}
        else{if (uint16_t(micros()-t)>5000){channel = 0;}}
        if(channel == nb_channel){channel = 0;}
        }

    void update();

    //save the values and convert them
    void rc_read_values() {
        noInterrupts();
        memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
        interrupts();
        bool no_error = true;
        for (int i=1;i<=6;i++){
            if(abs(rc_values[i]-1500)>800){
                //Serial.println(abs(rc_values[i]-1500));
                rc_values[i] = 1500;//prevent error when wrog values / radio disconected
                no_error = false;
            }
        }
        if(no_error){
            errorState = false;
            health += (1-health)/10;
        }
        else{
            errorState = true;
            health -= health/10;
        }



        Ox = (float(rc_values[1])-1500)/10;
        Oy = (float(rc_values[2])-1500)/10;
        Throtle = (float(rc_values[3])-1500)/10;
        Oz = (float(rc_values[4])-1500)/10;
        A = int((float(rc_values[5])-1500)/1000*2.2);
        B = (float(rc_values[6])-1500)/10;
        }

    
};

#endif