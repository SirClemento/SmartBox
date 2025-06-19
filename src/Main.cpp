#include <Arduino.h>
#include "I2C.h"
#include "timer.h"
#include "ServerLogic.h"
#include "ClientLogic.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
float temperature = 0.0; // Variable to store temperature
float humidity = 0.0;    // Variable to store humidity
uint16_t co2 = 0.0;       // Variable to store CO2
float co2_perent = 0.0; // Variable to store CO2 percentage
bool DisplayBoxModus = false;
int numberslave = 3; // Anzahl der erwarteten I2C-Slaves
String receivedRow1;
String receivedRow2;
String receivedRow3;
String receivedRow4;


void DisplayBox();
void SmartBox();

void setup() {    
    Serial.begin(115200); 
    Wire.begin(SDA_PIN, SCL_PIN);

    if(checkI2C_Address(SCREEN_ADDRESS) == true && checkI2C(1) == true){
            Serial.println("DisplayBox Modus");
            DisplayBoxModus = true; // Setze den Modus auf DisplayBox
            setupDisplay(); // Initialisiere das Display

            connectToServerAP();
        if(checkI2C_Address(SCREEN_ADDRESS) == true){
            Serial.println("Display found.");
            setDisplayStatus(true); // Setzen Sie den Status des Displays auf aktiv
            setupDisplay(); // Initialisieren Sie das Display
        Serial.println("Setup complete.");
    }
    } else {
    Serial.println("SmartBox Modus");
        numberslave = 3;
        DisplayBoxModus = false; // Setze den Modus auf normalen Betrieb
        if(checkI2C(numberslave) == true){
            turnOn_led(Status_Led); // Schalte die Status-LED ein
        } else {
            devicenotfound(); // Rufe die Funktion auf, wenn ein Gerät nicht gefunden wird
        }
        if(checkI2C_Address(PCA9536_I2C_ADDRESS) == true){
                // Setze IO0 und IO1 als Ausgang, IO2 und IO3 als Eingang
        writeRegister(PCA9536_I2C_ADDRESS, PCA9536_CONFIGURATION, 0b00001100);
        // Setze IO2 und IO3 auf Pull-Up
        writeRegister(PCA9536_I2C_ADDRESS, PCA9536_POLARITY_INVERSION, 0x00);

        checkButton(Boot_Button); // Überprüfen Sie den Status des Buttons an Pin 2
        if(checkI2C_Address(SCREEN_ADDRESS) == true){
            Serial.println("Onboard Display found.");
            setDisplayStatus(true); // Setzen Sie den Status des Displays auf aktiv
            setupDisplay(); // Initialisieren Sie das Display
        // SCD40 initialisieren und Messung starten
    } 
    if(checkI2C_Address(SCD40_ADDRESS) == true){
    if (scd40_start_measurement()) {
        Serial.println("start measure!");
    } else {
        Serial.println("failed to measure");
    }
}
}
    serverBegin(); // Starten Sie den Server
    Serial.println("Setup complete.");
    delay(1000);
    
}
}

void loop() {
    if(DisplayBoxModus){
        DisplayBox(); // Rufen Sie die Funktion für das DisplayBox-Modul auf
    }
    else {
        SmartBox(); // Rufen Sie die Funktion für das SmartBox-Modul auf
    }
}


void DisplayBox(){
while(true){
    delay(100);
    if(Timer2s()) { 
        Serial.println("5s Timer"); // Timer für 5 Sekunden
       requestAndGetData(&receivedRow1, &receivedRow2, &receivedRow3, &receivedRow4); //  um Daten vom Server zu erhalten
        Serial.println("row1: " + receivedRow1); 
        Serial.println("row2: " + receivedRow2); 
        Serial.println("row3: " + receivedRow3); 
        Serial.println("row4: " + receivedRow4);
        if(displaystatus){  
            String row1 = receivedRow1;  // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
            String row2 = receivedRow2; // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
            String row3 = receivedRow3; // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
            String row4 = receivedRow4; // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
            printDisplay(row1, row2, row3, row4);// Geben Sie den Status auf dem Display aus
            }
    }
}

}

void SmartBox(){
    while(true){
    delay(20);

    if(Timer500ms()){
        //Serial.println("50ms Timer triggered.");
         if(checkButton(reserv_Button)){
        toggleLedPin(Wifi_Led);
        }
    }
    
   if(Timer2s()){
        if(checkI2C_Address(SHT31_ADDRESS) == true){
        Serial.println("2s Timer triggered.");
        readSHT31(&temperature, &humidity);    
        String row1 = "Temperatur " + String(temperature, 2);  // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        String row2 = "Humidity " + String(humidity, 2); // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        String row3 = "CO2";
        String row4 = "data by SmartBox";
        if(displaystatus){  
        printDisplay(row1, row2, row3, row4);// Geben Sie den Status auf dem Display aus
        }

        sendStringsToClient(row1, row2, row3, row4); // Senden Sie die Strings an den Client
        serverHandle(); // Verarbeiten Sie die Serveranfragen
    }
    }
    if(Timer1s()){   
        //Serial.println("1s Timer triggered.");  
        //printf("I2C status: %d\n", getI2C_status()); 
        if(getI2C_status()){
        toggleLedPin(Status_Led);  // Blinken Sie die LED an Pin 0
    }
       
    }
 /*   if(Timer5s()){
        Serial.println("5s Timer triggered.");
        if(checkI2C_Address(SCD40_ADDRESS) == true){
        if (scd40_loop_measurement(&co2, &temperature, &humidity)) {
        co2_perent = getCO2Percent(co2); // Berechnen Sie den CO2-Gehalt    
        // Messwerte erfolgreich abgerufen

        String row1 = "Temperatur " + String(temperature, 2);  // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        String row2 = "Humidity " + String(humidity, 2); // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        String row3 = "CO2 " + String(co2_perent, 2); // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        String row4 = "data by SmartBox"; // Zusätzliche Zeile für SmartBox
        if(displaystatus){  
        printDisplay(row1, row2, row3, row4);// Geben Sie den Status auf dem Display aus
        }

        sendStringsToClient(row1, row2, row3); // Senden Sie die Strings an den Client
        serverHandle(); // Verarbeiten Sie die Serveranfragen
        delay(2000);  // Pause zwischen Messungen
    } else {
        Serial.println("Fehler beim Abrufen der Messwerte.");
        delay(1000);  // Wiederhole nach 1 Sekunde
    }
    }
}*/
    }
}