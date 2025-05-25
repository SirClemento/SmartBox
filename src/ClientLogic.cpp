#include "ClientLogic.h"
#include "WiFiConfig.h"
#include <ESP8266WiFi.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = WIFI_AP_NAME;
const char* password = WIFI_AP_PASSWORD;

void connectToServerAP() {
  WiFi.mode(WIFI_STA);

  // Statische IP für den Client
  if (!WiFi.config(CLIENT_STATIC_IP, WIFI_AP_IP, WIFI_AP_SUBNET)) {
    Serial.println("Fehler beim Setzen der statischen IP");
  }

  WiFi.begin(ssid, password);
  Serial.print("🔌 Verbinde mit AP");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected with " + String(ssid));
  Serial.println("Client IP: " + WiFi.localIP().toString());
}

bool requestAndGetData(String* s1, String* s2, String* s3, String* s4) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
  
      String url = "http://" + WIFI_AP_IP.toString() + "/data";
      http.begin(client, url);
  
      int httpCode = http.GET();
      if (httpCode > 0) {
        String payload = http.getString();
  
        // Aufteilen bei Komma
        int c1 = payload.indexOf(',');
        int c2 = payload.indexOf(',', c1 + 1);
        int c3 = payload.indexOf(',', c2 + 1);

  
      // Sicherstellen, dass es mindestens drei Kommata gibt
      if (c1 == -1 || c2 == -1 || c3 == -1) {
        Serial.println("unexpected payload format: " + payload);
        return false;
      }

      *s1 = payload.substring(0, c1);
      *s2 = payload.substring(c1 + 1, c2);
      *s3 = payload.substring(c2 + 1, c3);
      *s4 = payload.substring(c3 + 1);
  
        return true;
      } else {
        Serial.println("HTTP-Anfrage failed: " + String(httpCode));
      }
  
      http.end();
    } else {
      Serial.println("Not connected to WiFi");
    }
  
    return false;
  }