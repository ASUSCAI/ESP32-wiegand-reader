#include <Wiegand.h>
#include "ams_credentials.h"
#include "esp_wpa2.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <time.h>

WIEGAND wg;
int debug = 0;

void setup() {
    Serial.begin(115200);

    // UNCOMMENT BELOW FOR WPA2 WIFI CONNECTION
    // WiFi.mode(WIFI_STA);
    // WiFi.disconnect();

    // esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)username, strlen(username));
    // esp_wifi_sta_wpa2_ent_set_username((uint8_t *)username, strlen(username));
    // esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password));
    // esp_wifi_sta_wpa2_ent_enable();    

    // WiFi.begin(ssid);

    // UNCOMMENT BELOW FOR STANDARD WIFI CONNECTION
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
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
    // if (wg.available()) {
    //     if (debug) dispDebug(wg.getCode());
    //     Serial.printf("%lu,%lu\n", (unsigned long)time(NULL), wg.getCode());
    // }
}