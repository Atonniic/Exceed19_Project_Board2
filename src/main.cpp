// firmware for board 2 (Servo, Tray sensors)

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

#define Servo_pin 32
#define Laser_pin 33
#define LDR_pin 34

#define Servo_max 110
#define Servo_min 58

#define refill_interval_width 2000

#define LDR_threshold 2000

const int room_id = 0;

const char *ssid = "Pun-iPhone";
const char *password = "spiderman";

const String baseUrl = "https://ecourse.cpe.ku.ac.th/exceed12/";

bool tray_level = false; // true -> refill needed, false -> refill not needed
bool auto_refill = true;
bool open_door = false; // manual refill command

Servo refill_servo;

void connectWifi();
void Tray(void *param);
void Refill();
void PUT_tray_level();
void PUT_open_door();
void GET_auto_refill_open_door(void *param);

void setup() {
    Serial.begin(115200);

    pinMode(Servo_pin, OUTPUT);
    pinMode(Laser_pin, OUTPUT);
    pinMode(LDR_pin, INPUT);

    refill_servo.attach(Servo_pin);
    refill_servo.write(Servo_min);

    digitalWrite(Laser_pin, HIGH);

    connectWifi();

    xTaskCreatePinnedToCore(Tray, "Tray", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(GET_auto_refill_open_door, "GET_auto_refill_open_door", 10000, NULL, 1, NULL, 1);
}

void loop() {
}

void connectWifi() {
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");
}

void Tray(void *param) {
    bool last = tray_level;
    while (1) {
        tray_level = analogRead(LDR_pin) >= LDR_threshold;
        if (auto_refill && tray_level)
            Refill();
        if (last != tray_level)
            PUT_tray_level();
        last = tray_level;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void Refill() {
    refill_servo.write(Servo_max);
    int start = millis();
    while (millis() - start < refill_interval_width) {
    }
    refill_servo.write(Servo_min);
}

void PUT_tray_level() {
    const String url = baseUrl + String("update/tray_level/") + String(room_id) + String("/") + String(tray_level);
    HTTPClient http;
    http.begin(url);
    int httpCode = http.PUT("");
    if (httpCode >= 200 && httpCode <300) {
        Serial.println("PUT_tray_level OK");
    } else {
        Serial.println("PUT_tray_level ERROR");
    }
}

void PUT_open_door() {
    const String url = baseUrl + String("update/open_door/") + String(room_id) + String("/") + String(open_door);
    HTTPClient http;
    http.begin(url);
    int httpCode = http.PUT("");
    if (httpCode >= 200 && httpCode <300) {
        Serial.println("PUT_open_door OK");
    } else {
        Serial.println("PUT_open_door ERROR");
    }
}

void GET_auto_refill_open_door(void *param) {
    while (1) {
        const String url = baseUrl + String("getdata/commands/") + String(room_id);
	    String payload;
	    DynamicJsonDocument doc(2048);
	    HTTPClient http;
	    http.begin(url);
	    int httpCode = http.GET();
	    if (httpCode >= 200 && httpCode < 300) {
	    	Serial.println("GET_auto_refill_open_door OK");
	    	payload = http.getString();
	    	deserializeJson(doc, payload);
	    	auto_refill = doc["auto_refill"];
            open_door = doc["open_door"];
            if (open_door) {
                Refill();
                open_door = false;
                PUT_open_door();
            }
	    } else {
	    	Serial.println("GET_auto_refill_open_door ERROR");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
