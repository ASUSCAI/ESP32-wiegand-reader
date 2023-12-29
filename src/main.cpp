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
int debug = 0;

void connectToWiFi() {
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);

    // UNCOMMENT BELOW FOR WPA2 WIFI CONNECTION
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)username, strlen(username));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)username, strlen(username));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password));
    esp_wifi_sta_wpa2_ent_enable();    

    WiFi.begin(ssid);

    // UNCOMMENT BELOW FOR STANDARD WIFI CONNECTION
    // WiFi.begin(ssid, password);


    connectToWiFi();

    configTime(0, 0, "pool.ntp.org", "time.nist.gov"); 
    wg.begin(34, 35);
}

void sendAttendanceRecord(const char* room, long timestamp, const char* sid) {
    if (WiFi.status() != WL_CONNECTED) {
        connectToWiFi();
    }

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
}

void loop() {
    if (wg.available()) {
        long wiegandCode = wg.getCode();
        String wiegandCodeStr = String(wiegandCode);
        const char* sid = wiegandCodeStr.c_str();
        Serial.printf("%lu,%lu\n", (unsigned long)time(NULL), wiegandCode);
        const char* room = "COOR170"; 
        long timestamp = time(NULL); 

        sendAttendanceRecord(room, timestamp, sid);
    }
}