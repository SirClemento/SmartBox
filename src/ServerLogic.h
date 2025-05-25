#ifndef SERVER_LOGIC_H
#define SERVER_LOGIC_H

#include <Arduino.h>



void serverBegin();
void serverHandle();
void sendStringsToClient(const String& s1, const String& s2, const String& s3, const String& s4);

#endif