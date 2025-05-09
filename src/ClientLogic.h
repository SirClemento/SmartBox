#ifndef CLIENT_LOGIC_H
#define CLIENT_LOGIC_H

#include <Arduino.h>

void connectToServerAP();
bool requestAndGetData(String* s1, String* s2, String* s3);

#endif