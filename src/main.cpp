#include <Wiegand.h>
#include "ams_credentials.h"
#include <WiFi.h>
#include <time.h>

WIEGAND wg;
int debug = 0;

void setup() {
    Serial.begin(115200);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    wg.begin(34, 35);
}

void dispDebug(unsigned long code) {
    Serial.print("Wiegand HEX = ");
    Serial.println(wg.getCode(), HEX);

    Serial.print(", DECIMAL = ");
    Serial.println(wg.getCode());

    Serial.print(", BINARY = ");
    Serial.println(wg.getCode(), BIN);

    Serial.print(", Type W");
    Serial.println(wg.getWiegandType());
}

void loop() {
    if (wg.available()) {
        if (debug) dispDebug(wg.getCode());
        Serial.printf("%lu,%lu\n", (unsigned long)time(NULL), wg.getCode());
    }
}