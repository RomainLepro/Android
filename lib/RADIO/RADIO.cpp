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
        
        Serial.println("--Ox----Oy----Oz----Throtle----A----B--HEALTH----dt");
        Serial.print(Ox);Serial.print(" / ");Serial.print(Oy);Serial.print(" / ");Serial.print(Oz);Serial.print(" / ");
        Serial.print(Throtle);Serial.print(" / ");Serial.print(A);Serial.print(" / ");Serial.print(B);
        Serial.print(" / ");Serial.print(health);Serial.print(" / ");Serial.println(millis()-tSerial);
        tSerial = millis();
    }   
}

void RADIO::quickSerial(int dt){
    
    if (millis()>tQuickSerial+dt){
        
        Serial.print("ox:");Serial.print(int(Ox*10));
        Serial.print(";oy:");Serial.print(int(Oy*10));
        Serial.print(";oz:");Serial.print(int(Oz*10));
        Serial.print(";th:");Serial.print(int(Throtle*10));
        Serial.print(";sa:");Serial.print(int(A*10));
        Serial.print(";sb:");Serial.print(int(B*10));
        Serial.print(";he:");Serial.println(int(health*10));
        tQuickSerial = millis();
    }   
}
