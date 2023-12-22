#include <Wiegand.h>
#include <time.h>
#include <lwip/apps/sntp.h>
#include "esp_wpa2.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "ams_credentials.h"


WIEGAND wg;
const char* apiHost = "api.ams-lti.com";
const int httpPort = 443;
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

    configTime(0, 0, "pool.ntp.org", "time.nist.gov"); 

    Serial.print("Synchronizing time");
    while (time(NULL) < 946684800) { // year 2000
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nTime synchronized");


    wg.begin(34, 35);
}

void sendAttendanceRecord(const char* room, long timestamp, const char* sid) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("https://" + String(apiHost) + "/attendance");
        http.addHeader("Content-Type", "application/json");

        DynamicJsonDocument doc(1024);
        doc["room"] = room;
        doc["timestamp"] = timestamp;
        doc["sid"] = sid;

        String requestBody;
        serializeJson(doc, requestBody);

        int httpResponseCode = http.POST(requestBody);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(response);
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi not connected");
    }
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
    const char* room = "COOR170"; // Replace with actual room data
    const char* sid = "1221000031"; // Replace with actual student ID
    long timestamp = time(NULL); // Current UNIX timestamp

    sendAttendanceRecord(room, timestamp, sid);

    delay(10000);
}