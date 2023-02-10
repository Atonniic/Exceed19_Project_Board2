#include <Arduino.h>
#include "Define.h"
#include "Tray.h"
#include "Door.h"
#include "HTTP.h"

bool tray_level; //0 -> nothing, 1 -> refill
bool auto_refill; //True -> automode, False -> manualmode
bool open_door; //True -> open, False -> closed

void setup() {
    Serial.begin(115200);
    pinMode(Laser_pin, OUTPUT);
    digitalWrite(Laser_pin, HIGH);
    //connectWifi();

    //xTaskCreatePinnedToCore(Tray, "Tray", 10000, NULL, 1, NULL, 0);
}

void loop() {
}