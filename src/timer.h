//Timer
#ifndef TIMER_h
#define TIMER_h

extern unsigned long previousMillis50ms;
extern unsigned long previousMillis500ms;
extern unsigned long previousMillis1s;
extern unsigned long previousMillis2s;
extern unsigned long previousMillis5s;
extern unsigned long previousMillis10s;
extern unsigned long previousMillis20s;
extern unsigned long previousMillis30s;
extern unsigned long previousMillis60s;

const long interval50ms = 50;    // 50 Millisekunden
const long interval500ms = 500;  // 500 Millisekunden
const long interval1s = 1000;    // 1000 Millisekunden (1 Sekunde)
const long interval2s = 2000;    // 2000 Millisekunden (2 Sekunden)
const long interval5s = 5000;    // 5000 Millisekunden (5 Sekunden)
const long interval10s = 10000;  // 10000 Millisekunden (10 Sekunden)
const long interval20s = 20000;  // 20000 Millisekunden (20 Sekunden)
const long interval30s = 30000;  // 30000 Millisekunden (30 Sekunden)
const long interval60s = 60000;  // 60000 Millisekunden (60 Sekunden)

bool Timer50ms(); //Timer für 50ms
bool Timer500ms(); //Timer für 500ms
bool Timer1s(); //Timer für 1s
bool Timer10s(); //Timer für 10s
bool Timer2s(); //Timer für 2s
bool Timer5s(); //Timer für 5s
bool Timer10s(); //Timer für 10s
bool Timer20s(); //Timer für 20s
bool Timer30s(); //Timer für 30s
bool Timer60s(); //Timer für 60s

#endif
