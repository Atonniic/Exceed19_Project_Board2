/*
    map, POST
*/

#include <Arduino.h>
#include "Define.h"

extern int tray_level; //F -> nothing, T -> refill

void Tray(void *param) {
    while(1) {
        //int map_LDR = map(analogRead(LDR_pin), 2000, 4000, 0, 255);
        if (analogRead(LDR_pin) <= 2500) { //do nothing
            tray_level = 0;
            //Serial.println(map_LDR);
            Serial.println(analogRead(LDR_pin));
            //Door()
            //POST_tray_level()
        }
        else { //refill
            tray_level = 1;
            //Serial.println(map_LDR);
            Serial.println(analogRead(LDR_pin));
            //Door()
            //POST_tray_level()  
        }
        delay(300);
    }
}