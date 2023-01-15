#include "RADIO.h"


float RADIO::radio_control = 0;
uint8_t RADIO::channel = 0;
uint32_t RADIO::t = 0;
uint16_t RADIO::rc_shared[nb_channel] = {1,2,3,4,5,6,7,8,9};//verry bad, doesn't work for nb_channel

void RADIO::update(){
    rc_read_values();
}

void RADIO::showSerial(int dt){
    
    if (millis()>tSerial+dt){
        tSerial = millis();
        Serial.println("--Ox----Oy----Oz----Throtle----A----B-- ");
        Serial.print(Ox);Serial.print(" / ");Serial.print(Oy);Serial.print(" / ");Serial.print(Oz);Serial.print(" / ");
        Serial.print(Throtle);Serial.print(" / ");Serial.print(A);Serial.print(" / ");Serial.println(B);
    }
}
