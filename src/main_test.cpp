#include <Arduino.h>
#include "main.h"
#include "timer.h"
#include "ServerLogic.h"

#include "Wire.h"
#include "Adafruit_GFX.h"
float temperature = 0.0; // Variable to store temperature
float humidity = 0.0;    // Variable to store humidity




void setup() {    
    Serial.begin(115200); 
    Wire.begin(SDA_PIN, SCL_PIN);
    numberslave = 2;
    checkI2C();
    // Setze IO0 und IO1 als Ausgang, IO2 und IO3 als Eingang
    writeRegister(PCA9536_I2C_ADDRESS, PCA9536_CONFIGURATION, 0b00001100);
    // Setze IO2 und IO3 auf Pull-Up
    writeRegister(PCA9536_I2C_ADDRESS, PCA9536_POLARITY_INVERSION, 0x00);
    
 // Erstellen Sie eine Instanz der ServerLogic-Klasse

    serverBegin(); // Starten Sie den Server
    checkButton(Boot_Button); // Überprüfen Sie den Status des Buttons an Pin 2
    if(checkI2C_Address(SCREEN_ADDRESS) == true){
        Serial.println("Display found.");
        setDisplayStatus(true); // Setzen Sie den Status des Displays auf aktiv
        setupDisplay(); // Initialisieren Sie das Display
    Serial.println("Setup complete.");
}
}

void loop() {
    
    delay(20);
    
    checkButton(Boot_Button); // Überprüfen Sie den Status des Boot-Buttons

    if(Timer2s()){
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


    }
    if(Timer10s()){
        Serial.println("10s Timer triggered.");  

        ScanI2C(); // Rufen Sie die ScanI2C-Funktion auf   
        printFoundI2CAddresses(); // Geben Sie die gefundenen I2C-Adressen aus
        freeI2CAddresses(); // Geben Sie den Speicher für die I2C-Adressen wieder frei
     }

    if(Timer1s()){   
        Serial.println("1s Timer triggered.");   
        if(getI2C_status()){
        toggleLedPin(Status_Led);  // Blinken Sie die LED an Pin 0
    }
        if(checkButton(reserv_Button)){
        toggleLedPin(Wifi_Led);
    }        

    }
        
}