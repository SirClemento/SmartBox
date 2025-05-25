#include "ServerLogic.h"
#include "WifiConfig.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>




ESP8266WebServer server(80);
String g_s1, g_s2, g_s3, g_s4;

void sendStringsToClient(const String& s1, const String& s2, const String& s3, const String& s4) {
  g_s1 = s1;
  g_s2 = s2;
  g_s3 = s3;
  g_s4 = s4;
}

void serverBegin() {
  WiFi.mode(WIFI_AP);

  // Feste IP setzen
  WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_GATEWAY, WIFI_AP_SUBNET);

  // Access Point starten
  WiFi.softAP(WIFI_AP_NAME, WIFI_AP_PASSWORD);

  Serial.println("Access Point gestartet:");
  Serial.println("SSID: " + String(WIFI_AP_NAME));
  Serial.println("IP: " + WiFi.softAPIP().toString());

  server.on("/data", []() {
    String response = g_s1 + "," + g_s2 + "," + g_s3 + "," + g_s4;
    server.send(200, "text/plain", response);
  });

  server.begin();
}

void serverHandle() {
  server.handleClient();
}
