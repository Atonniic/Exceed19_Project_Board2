#include <Arduino.h>
#include "Define.h"

extern int tray_level; //0 -> nothing, 1 -> refill

void Tray(void *param) {
    while(1) {
        //int map_LDR = map(analogRead(LDR_pin), 2000, 4000, 0, 255);
        if (analogRead(LDR_pin) <= 2500) { //do nothing
            tank_level = 0;
            //Serial.println(map_LDR);
            Serial.println(analogRead(LDR_pin));
            digitalWrite(GREEN_pin, 1);   
            digitalWrite(RED_pin, 0);
            //POST to server
        }
        else { //refill
            tank_level = 1;
            //Serial.println(map_LDR);
            Serial.println(analogRead(LDR_pin));
            digitalWrite(GREEN_pin, 0);   
            digitalWrite(RED_pin, 1);  
            //POST to server  
        }
        delay(300);
    }
}