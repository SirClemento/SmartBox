#ifndef main_h
#define main_h

#include <Arduino.h>
#include <Wire.h>


// I2C-Adressen
#define PCA9536_I2C_ADDRESS 0x41 // I2C->IO
#define SCREEN_ADDRESS 0x3C        //LCD 
#define SHT31_ADDRESS 0x44      //SHT31 Temperature and Humidity Sensor
#define SCD40_ADDRESS 0x62   //SCD40 CO2 Sensor

//SSD_1306 I2C Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
extern bool displaystatus; //true = display ist aktiv, false = display ist inaktiv

//low oder high aktive Led am Pca9536
const bool lowActive = true; //true = low active, false = high active

//SHT31

#define MEASURE_CMD 0x2400 // Befehl zum Messen von Temperatur und Luftfeuchtigkeit

// PCA9536 Register und Pinärcodes für Output
#define PCA9536_INPUT_PORT 0x00
#define PCA9536_OUTPUT_PORT 0x01
#define PCA9536_POLARITY_INVERSION 0x02
#define PCA9536_CONFIGURATION 0x03
#define IoOff 0x00
#define Io0on 0x01
#define Io1on 0x02
#define Io2on 0x04
#define Io3on 0x08
#define Io4on 0x10
#define Io0off  ~Io0on  // 0xFE
#define Io1off  ~Io1on  // 0xFD
#define Io2off  ~Io2on  // 0xFB
#define Io3off  ~Io3on  // 0xF7
#define Io4off  ~Io4on  // 0xEF

//SCD 40 Befehle
#define SCD40_CMD_START_MEASUREMENT   0x21B1
#define SCD40_CMD_READ_MEASUREMENT    0xEC05
#define SCD40_CMD_STOP_MEASUREMENT    0x3F86
#define SCD40_CMD_SET_MEASUREMENT_INTERVAL 0x460D
#define SCD40_CMD_SET_AUTO_SELF_CALIBRATION 0x241D
#define SCD40_CMD_SET_FORCED_RECALIBRATION 0xE000
#define SCD40_CMD_GET_DATA_READY 0xE4B8

//ausgänge am IO Expander PCA9536
#define Status_Led 0
#define Wifi_Led 1
#define Boot_Button 2
#define reserv_Button 3

// I2C-Pins für den ESP8266 
#define SDA_PIN D1
#define SCL_PIN D2

//count wie viele Geräte gefunden werden sollen, zb durchgänge
extern int numberslave; //const int numberslave = 2; // wie viele I2C Geräte gefunden werden sollen, zb durchgänge
extern int errorCount;
const int maxErrorrounds = 3;

//für LED Blink
extern unsigned long previousMillis;
extern bool ledstatus;
extern bool I2C_failure;
const long interval = 1000;  // 1 Sekunde

//für I2C Scan
extern byte* foundI2CAddresses;   // Dynamisches Array für die I2C-Adressen speichert alle gefundenen I2C Adressen



void devicenotfound(); //aufrufen wenn slave nicht gefunden wird
//void toggle_led_I2C(int Led_Pin); //lässt led blinken bei fehlern
void toggle_led_time(int Led_Pin); //lässt die LED blinken
void toggleLedPin(int Led_Pin); //schaltet die LED ein oder aus
void turnOn_led(int Led_Pin); //schaltet die LED ein
void turnOff_led(int Led_Pin); //schaltet die LED aus
bool checkI2C_Address(int I2C_ADDRESS); //prüft ob die I2C Adresse vorhanden ist
bool checkI2C(int numberslave); //überprüft beim starten ob alle I2C gefunden werden, überprüft nur nach anzahl und nicht nach Adresse
String ScanI2C(); //scannt alle I2C Adressen und gibt sie aus
void printFoundI2CAddresses(); //gibt die gefundenen I2C Adressen aus
void freeI2CAddresses(); //gibt den Speicher für die I2C Adressen wieder frei
bool getI2C_status(); //gibt den Status des I2C zurück
void setI2C_status(bool status); //setzt den Status des I2C
void writeRegister(int I2C_ADDRESS, uint8_t reg, uint8_t value); // schreibt auf registern des IOExpanders PCA9536 raus auf verschiedenen adressen
uint8_t readRegister(int I2C_ADDRESS, uint8_t reg); //inbut->liest von registern des IOExpanders PCA9536 raus auf verschiedenen adressen
bool checkButton(uint8_t buttonPin); //prüft ob der Button gedrückt wurde und gibt den Status zurück
void setupDisplay(); //setzt die I2C Adresse des Displays
void setDisplayStatus(bool status); //setzt den Status des Displays
void printDisplay(String row1, String row2, String row3, String row4); //gibt die I2C Adresse des Displays aus
bool readSHT31(float* temperature, float* humidity); //liest die Temperatur und die Luftfeuchtigkeit vom SHT31 Sensor aus
uint8_t crc8(uint8_t msb, uint8_t lsb); //CRC8 Berechnung für den SHT31 Sensor

//co2
bool scd40_init(void);
bool scd40_start_measurement(void);
bool scd40_stop_measurement(void);
bool scd40_read_measurement(uint16_t* co2, float* temperature, float* humidity);
bool scd40_data_ready(void);
uint8_t scd40_get_status(void);
uint8_t calculateCRC(uint8_t* data, uint8_t length);
bool scd40_data_ready();
bool scd40_loop_measurement(uint16_t* co2, float* temperature, float* humidity);
float getCO2Percent(uint16_t co2);
#endif
