#include <Arduino.h>
#include <Wire.h>
#include <timer.h>
#include <main.h>

unsigned long previousMillis50ms = 0;
unsigned long previousMillis250ms = 0;
unsigned long previousMillis500ms = 0;
unsigned long previousMillis1s = 0;
unsigned long previousMillis2s = 0;
unsigned long previousMillis5s = 0;
unsigned long previousMillis10s = 0;
unsigned long previousMillis20s = 0;
unsigned long previousMillis30s = 0;
unsigned long previousMillis60s = 0;


bool Timer50ms() {
    unsigned long currentMillis50ms = millis();
    if (currentMillis50ms - previousMillis50ms >= interval50ms) {
        previousMillis50ms = currentMillis50ms;
        return true;
    }
    return false;
}
bool Timer250ms() {
    unsigned long currentMillis250ms = millis();
    if (currentMillis250ms - previousMillis250ms >= 250) {
        previousMillis250ms = currentMillis250ms;
        return true;
    }
    return false;
}
bool Timer500ms() {
    unsigned long currentMillis500ms = millis();
    if (currentMillis500ms - previousMillis500ms >= interval500ms) {
        previousMillis500ms = currentMillis500ms;
        return true;
    }
    return false;
}
bool Timer1s() {
    unsigned long currentMillis1s = millis();
    if (currentMillis1s - previousMillis1s >= interval1s) {
        previousMillis1s = currentMillis1s;
        return true;
    }
    return false;
}
bool Timer2s() {
    unsigned long currentMillis2s = millis();
    if (currentMillis2s - previousMillis2s >= 2000) {
        previousMillis2s = currentMillis2s;
        return true;
    }
    return false;
}
bool Timer5s() {
    unsigned long currentMillis5s = millis();
    if (currentMillis5s - previousMillis5s >= 5000) {
        previousMillis5s = currentMillis5s;
        return true;
    }
    return false;
}
bool Timer10s() {
    unsigned long currentMillis10s = millis();
    if (currentMillis10s - previousMillis10s >= 10000) {
        previousMillis10s = currentMillis10s;
        return true;
    }
    return false;
}
bool Timer20s() {
    unsigned long currentMillis20s = millis();
    if (currentMillis20s - previousMillis20s >= 20000) {
        previousMillis20s = currentMillis20s;
        return true;
    }
    return false;
}
bool Timer30s() {
    unsigned long currentMillis30s = millis();
    if (currentMillis30s - previousMillis30s >= 30000) {
        previousMillis30s = currentMillis30s;
        return true;
    }
    return false;
}
bool Timer60s() {
    unsigned long currentMillis60s = millis();
    if (currentMillis60s - previousMillis60s >= 60000) {
        previousMillis60s = currentMillis60s;
        return true;
    }
    return false;
}
