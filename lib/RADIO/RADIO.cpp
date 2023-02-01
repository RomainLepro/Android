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



int filter(float angle)
{
// take a float between -50 and +50
// and make it between 0 and 999
    return max(0,min(int(angle*10)+500,999));

}

void RADIO::quickSerial(int dt){
    
    if (millis()>tQuickSerial+dt){
        
                          Serial.print(filter(Ox));
        Serial.print(";");Serial.print(filter(Oy));
        Serial.print(";");Serial.print(filter(Oz));
        Serial.print(";");Serial.print(filter(Throtle));
        Serial.print(";");Serial.print(filter(A));
        Serial.print(";");Serial.print(filter(B));
        Serial.print(";");Serial.print(filter(health*10-50));
        Serial.print(";");Serial.println(filter(int((millis()/200)%100)-50));
        Serial.println(";");
        tQuickSerial = millis();
    }   
}


void RADIO::quickSerialDebug(int dt,int data){
    
    if (millis()>tQuickSerial+dt){
        
                          Serial.print(filter(Ox));
        Serial.print(";");Serial.print(500);
        Serial.print(";");Serial.print(600);
        Serial.print(";");Serial.print(700);
        Serial.print(";");Serial.print(800);
        Serial.print(";");Serial.print(900);
        Serial.print(";");Serial.print(filter(int((millis()/200)%100)-50));
        Serial.print(";");Serial.print(data);
        Serial.println(";");
        tQuickSerial = millis();
    }   
}