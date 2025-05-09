#include <Arduino.h>
#include "main.h"
#include "timer.h"
#include "ServerLogic.h"

#include "Wire.h"
#include "Adafruit_GFX.h"
float temperature = 0.0; // Variable to store temperature
float humidity = 0.0;    // Variable to store humidity
uint16_t co2 = 0.0;       // Variable to store CO2
float co2_perent = 0.0; // Variable to store CO2 percentage




void setup() {    
    Serial.begin(115200); 
    Wire.begin(SDA_PIN, SCL_PIN);
    numberslave = 3;
    checkI2C();
    // Setze IO0 und IO1 als Ausgang, IO2 und IO3 als Eingang
    writeRegister(PCA9536_I2C_ADDRESS, PCA9536_CONFIGURATION, 0b00001100);
    // Setze IO2 und IO3 auf Pull-Up
    writeRegister(PCA9536_I2C_ADDRESS, PCA9536_POLARITY_INVERSION, 0x00);
    // SCD40 initialisieren und Messung starten
    if (scd40_start_measurement()) {
        Serial.println("Messung gestartet!");
    } else {
        Serial.println("Fehler beim Starten der Messung.");
    }
 // Erstellen Sie eine Instanz der ServerLogic-Klasse

    serverBegin(); // Starten Sie den Server
    checkButton(Boot_Button); // Überprüfen Sie den Status des Buttons an Pin 2
    if(checkI2C_Address(SCREEN_ADDRESS) == true){
        Serial.println("Display found.");
        setDisplayStatus(true); // Setzen Sie den Status des Displays auf aktiv
        setupDisplay(); // Initialisieren Sie das Display
    Serial.println("Setup complete.");
    delay(1000);
    
}
}

void loop() {
    
    delay(20);
    
    checkButton(Boot_Button); // Überprüfen Sie den Status des Boot-Buttons

    if(Timer500ms()){
        //Serial.println("50ms Timer triggered.");
         if(checkButton(reserv_Button)){
        toggleLedPin(Wifi_Led);
        }
    }
    
   /* if(Timer2s()){
        /*if(checkI2C_Address(SHT31_ADDRESS) == true){
        Serial.println("2s Timer triggered.");
        readSHT31(&temperature, &humidity);    
        String row1 = "Temperatur " + String(temperature, 2);  // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        String row2 = "Humidity " + String(humidity, 2); // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        String row3 = "CO2";
        if(displaystatus){  
        printDisplay(row1, row2, row3);// Geben Sie den Status auf dem Display aus
        }

        sendStringsToClient(row1, row2, row3); // Senden Sie die Strings an den Client
        serverHandle(); // Verarbeiten Sie die Serveranfragen

        //sendData(String(temperature, 2),String(humidity, 2) , "Humidity"); // Senden Sie die Daten über ESP-NOW
    //}
    }*/

    /*if(Timer10s()){
        Serial.println("10s Timer triggered.");  

        ScanI2C(); // Rufen Sie die ScanI2C-Funktion auf   
        printFoundI2CAddresses(); // Geben Sie die gefundenen I2C-Adressen aus
        freeI2CAddresses(); // Geben Sie den Speicher für die I2C-Adressen wieder frei
     }
*/
    if(Timer1s()){   
        //Serial.println("1s Timer triggered.");  
        //printf("I2C status: %d\n", getI2C_status()); 
        if(getI2C_status()){
        toggleLedPin(Status_Led);  // Blinken Sie die LED an Pin 0
    }
       
    }

    if(Timer5s()){
        Serial.println("5s Timer triggered.");
        if(checkI2C_Address(SCD40_ADDRESS) == true){
        if (scd40_loop_measurement(&co2, &temperature, &humidity)) {
        co2_perent = getCO2Percent(co2); // Berechnen Sie den CO2-Gehalt    
        // Messwerte erfolgreich abgerufen

        String row1 = "Temperatur " + String(temperature, 2);  // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        String row2 = "Humidity " + String(humidity, 2); // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        String row3 = "CO2 " + String(co2_perent, 2); // Ruft die ScanI2C-Funktion auf und speichert den Rückgabewert (String)
        if(displaystatus){  
        printDisplay(row1, row2, row3);// Geben Sie den Status auf dem Display aus
        }

        sendStringsToClient(row1, row2, row3); // Senden Sie die Strings an den Client
        serverHandle(); // Verarbeiten Sie die Serveranfragen
        delay(2000);  // Pause zwischen Messungen
    } else {
        Serial.println("Fehler beim Abrufen der Messwerte.");
        delay(1000);  // Wiederhole nach 1 Sekunde
    }
    }
}
       
}
