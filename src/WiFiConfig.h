#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// Access Point Einstellungen (für Server)
#define WIFI_AP_NAME     "MeinESP_Server"
#define WIFI_AP_PASSWORD ""  // leer = offen

#define WIFI_AP_IP       IPAddress(192, 168, 10, 1)
#define WIFI_AP_GATEWAY  IPAddress(192, 168, 10, 1)
#define WIFI_AP_SUBNET   IPAddress(255, 255, 255, 0)

// Client-Konfiguration
#define CLIENT_STATIC_IP IPAddress(192, 168, 10, 42)

#endif