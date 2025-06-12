#include <Arduino.h>
#include <Wire.h>
#include <I2C.h>
#include <timer.h>


byte* foundI2CAddresses = nullptr;
int foundDeviceCount = 0;
int errorCount;
unsigned long previousMillis = 0;
//int numberslave;
bool ledstatus = false;
bool I2C_failure;
bool displaystatus = false;
static bool measurementStarted = false;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool checkI2C_Address(int address) {
    Wire.beginTransmission(address);
    return (Wire.endTransmission() == 0);
}

bool checkI2C(int numberslave) {
    Serial.println("Scanning I2C bus...");
    int foundslaves = 0;
        for (byte addr = 0x01; addr < 0x7F; addr++) {     
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.printf("Device found at 0x%02X\n", addr);
            delay(10);
            foundslaves++;
        }  
}
    Serial.printf("%i\n", foundslaves);
    Serial.println("Scan complete.");  
    if(foundslaves == numberslave){
        Serial.println("Scan sucessfull.");
//        turnOn_led(Status_Led);
        setI2C_status(false); 
        I2C_failure = false;
        return true;  
    }
    
Serial.println("Scan failed");
//setI2C_status(true);
//devicenotfound();
return false;
}

String ScanI2C() {
    Serial.println("Scanning I2C bus...");
   // Zeiger auf das dynamische Array
    foundDeviceCount = 0;
    // Wenn das Array nicht existiert, initialisieren wir es
    if (foundI2CAddresses == nullptr) {
        foundI2CAddresses = (byte*)malloc(sizeof(byte) * 128); // Maximale Anzahl an Adressen: 128
        if (foundI2CAddresses == nullptr) {
            Serial.println("Memory allocation failed!");
            return "Memory allocation failed!";
        }
    }

    for (byte addr = 0x01; addr < 0x7F; addr++) {  
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            // Gerät gefunden
            //Serial.printf("Device found at 0x%02X\n", addr);

            // Füge die Adresse zum dynamischen Array hinzu
            foundI2CAddresses[foundDeviceCount] = addr;
            foundDeviceCount++;
            // Überprüfen, ob das Array voll ist
            if (foundDeviceCount >= 128) {
                Serial.println("Array full, stopping scan.");
                break;
            }
            delay(10);
        }
    }
    String output = "";
    for (int i = 0; i < foundDeviceCount; i++) {
        output += "0x";
        if (foundI2CAddresses[i] < 0x10) output += "0"; // führende Null
        output += String(foundI2CAddresses[i], HEX);
        if (i < foundDeviceCount - 1) output += ",  ";
    }
    return output;
}

void printFoundI2CAddresses() {

    Serial.println("Found I2C devices:");
    Serial.printf("%d devices found\n", foundDeviceCount);
    for (int i = 0; i < foundDeviceCount; i++) {
        Serial.printf("0x%02X\n", foundI2CAddresses[i]);
    }
}

void freeI2CAddresses() {
    if (foundI2CAddresses != nullptr) {
        free(foundI2CAddresses);
        foundI2CAddresses = nullptr;
        foundDeviceCount = 0;
    }
}

void devicenotfound(){
    Serial.println("LED_EIN/AUS");    
    setI2C_status(true);
    toggle_led_time(Status_Led);
    //writeRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT, 0b00001100);
}

void toggle_led_time(int Led_Pin) {
    if(I2C_failure == true){
        Serial.println("toogle Led True");   
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            ledstatus = !ledstatus;
            uint8_t currentState = readRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT);
            if (ledstatus) {
                currentState |= (1 << Led_Pin); 
            } else {
                currentState &= ~(1 << Led_Pin); 
            }
            writeRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT, currentState);
        }
    } else {
        Serial.println("toogle led false");   
        //writeRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT, 0b00001101);
    }
}

void toggleLedPin(int Led_Pin) {
    uint8_t currentState = readRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT);
    ledstatus = !ledstatus; // Toggle the LED status

    if (ledstatus) {
        currentState |= (1 << Led_Pin); // Turn LED on
    } else {
        currentState &= ~(1 << Led_Pin); // Turn LED off
    }

    writeRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT, currentState);

    // Verify if the LED state was successfully updated
    uint8_t verifyState = readRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT);
    if (((verifyState & (1 << Led_Pin)) != 0) != ledstatus) {
        Serial.println("Failed to toggle LED state.");
    } else {
        Serial.println("LED state toggled successfully.");
    }
}
void turnOn_led(int Led_Pin) {
    Serial.println("LED ON");
    uint8_t currentState = readRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT);
    if(lowActive == true){
        currentState &= ~(1 << Led_Pin); //für low aktiv
    } else {
        currentState |= (1 << Led_Pin); //für high aktiv
    }
    writeRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT, currentState);
}

void turnOff_led(int Led_Pin) {
    Serial.println("LED OFF");
    uint8_t currentState = readRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT);
    if(lowActive == true){
        currentState |= (1 << Led_Pin); //für low aktiv
    } else {
        currentState &= ~(1 << Led_Pin); //für high aktiv
    }


    writeRegister(PCA9536_I2C_ADDRESS, PCA9536_OUTPUT_PORT, currentState);
}

bool getI2C_status(){
    return I2C_failure;
 }

void setI2C_status(bool status){
    Serial.println("I2C status set to: " + String(status));
    I2C_failure = status;
}

void writeRegister(int I2C_ADDRESS, uint8_t reg, uint8_t value ) {
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
    //Wire.endTransmission();
    if (Wire.endTransmission() != 0) {
        Serial.println("Transmission failed.");
        devicenotfound();
    }
   // Serial.printf("Write to reg 0x%02X = 0x%02X (Error: %u)\n", reg, value, error);
   // Serial.printf("endTransmission: %u\n", error);
  }

  uint8_t readRegister(int I2C_ADDRESS, uint8_t reg) {
    if (errorCount >= maxErrorrounds) {
        Serial.println("Max errors reached, stopping execution.");
        while (1);
    }
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
        Serial.printf("Error reading reg 0x%02X\n", reg);
        errorCount++;
        return 0xFF;
    }
    Wire.requestFrom(I2C_ADDRESS, 1);
    return Wire.available() ? Wire.read() : 0xFF;
}

bool checkButton(uint8_t buttonPin){
    uint8_t state = readRegister(PCA9536_I2C_ADDRESS, PCA9536_INPUT_PORT);
    if(!(state & (1 << buttonPin))){
        Serial.printf("Button %d is pressed\n", buttonPin);

        return true; 
        // Hier können Sie den Code hinzufügen, der ausgeführt werden soll, wenn der Button gedrückt wird
    } else {
        return false;
        // Prüfen, ob das entsprechende Bit LOW ist
    }  
}

void setupDisplay() {
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
    display.setTextSize(1);  
    display.setTextColor(SSD1306_WHITE);    
    display.setCursor(0, 0);
    display.println("Guten Tag");
    display.display(); 
}

void setDisplayStatus(bool status) {
    displaystatus = status;
}

void printDisplay(String row1, String row2, String row3, String row4) {
    display.clearDisplay();
    display.setTextSize(1);      // Textgröße
    display.setTextColor(SSD1306_WHITE); // Textfarbe
    
    // Zeile 1
    display.setCursor(0, 0);     // Position (X=0, Y=0)
    display.println(row1);
  
    // Zeile 2
    display.setCursor(0, 10);    // Position (X=0, Y=10)
    display.println(row2);
  
    // Zeile 3
    display.setCursor(0, 20);    // Position (X=0, Y=20)
    display.println(row3);
  
    // Zeile 4 (optional)
    if (!row4.isEmpty()) {
        display.setCursor(0, 30); // Position (X=0, Y=30)
        display.println(row4);
    }
    display.display();  // Alles anzeigen
}

bool readSHT31(float* temperature, float* humidity) {

    // 1. Sende 16-bit Messbefehl (zwei 8-Bit Schritte)
    //Wire.beginTransmission(SHT31_ADDRESS);
    writeRegister(SHT31_ADDRESS, (MEASURE_CMD >> 8) & 0xFF, MEASURE_CMD & 0xFF);
    if (Wire.endTransmission() != 0) {
        Serial.println("SHT31: Transmission failed.");
        devicenotfound();
        return false;
    }
    delay(20); // Warten auf Messung

    // 2. Lese 6 Bytes aus: Temp(2)+CRC + RH(2)+CRC
    Wire.requestFrom((uint8_t)SHT31_ADDRESS, (uint8_t)6);
    if (Wire.available() != 6) {
        Serial.println("SHT31: Not enough bytes received.");
        devicenotfound();
        return false;
    }

    uint8_t temp_msb = Wire.read();
    uint8_t temp_lsb = Wire.read();
    uint8_t temp_crc = Wire.read();

    uint8_t hum_msb = Wire.read();
    uint8_t hum_lsb = Wire.read();
    uint8_t hum_crc = Wire.read();
    // CRC-Prüfung
    if (temp_crc != crc8(temp_msb, temp_lsb)) {
        Serial.println("SHT31: Temperature CRC error.");
        devicenotfound();
        return false;
    }
    if (hum_crc != crc8(hum_msb, hum_lsb)) {
        Serial.println("SHT31: Humidity CRC error.");
        devicenotfound();
        return false;
    }
    // 3. Berechne Temperatur und Luftfeuchtigkeit
    uint16_t raw_temp = (temp_msb << 8) | temp_lsb;
    uint16_t raw_hum  = (hum_msb << 8) | hum_lsb;

    *temperature = -45.0 + 175.0 * ((float)raw_temp / 65535.0);
    *humidity    = 100.0 * ((float)raw_hum / 65535.0);
    Serial.printf("Raw Temp: %u, Raw Hum: %u\n", raw_temp, raw_hum);
    Serial.printf("SHT31: %.2f °C, %.2f %%RH\n", *temperature, *humidity);

    return true;
}

uint8_t crc8(uint8_t msb, uint8_t lsb) {
    uint8_t crc = 0xFF;
    uint8_t data[2] = { msb, lsb };

    for (int i = 0; i < 2; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}


uint8_t calculateCRC(uint8_t *data, size_t len) {
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t bit = 8; bit > 0; bit--) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

// Hilfsfunktion, um den Status zu überprüfen
bool scd40_data_ready() {
    
    writeRegister(SCD40_ADDRESS, (SCD40_CMD_GET_DATA_READY >> 8) & 0xFF, SCD40_CMD_GET_DATA_READY & 0xFF);
    delay(5);
    Wire.requestFrom(SCD40_ADDRESS, (uint8_t)3);
    if (Wire.available() != 3) return false;

    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    Wire.read(); // CRC byte

    uint16_t status = (msb << 8) | lsb;
    return (status & 0x07FF) != 0; // Prüfen, ob Messwerte bereit sind
}

// Startet die Messung
bool scd40_start_measurement() {
    writeRegister(SCD40_ADDRESS, (SCD40_CMD_START_MEASUREMENT >> 8) & 0xFF, SCD40_CMD_START_MEASUREMENT & 0xFF);
    delay(10);  // Warten, um die Messung zu starten
    return true;
}

// Stoppt die Messung
bool scd40_stop_measurement() {
    writeRegister(SCD40_ADDRESS, (SCD40_CMD_STOP_MEASUREMENT >> 8) & 0xFF, SCD40_CMD_STOP_MEASUREMENT & 0xFF);
    delay(10);
    return true;
}

// Liest die Messwerte: CO2, Temperatur, Luftfeuchtigkeit
bool scd40_read_measurement(uint16_t* co2, float* temperature, float* humidity) {
    writeRegister(SCD40_ADDRESS, (SCD40_CMD_READ_MEASUREMENT >> 8) & 0xFF, SCD40_CMD_READ_MEASUREMENT & 0xFF);
    delay(5);

    Wire.requestFrom(SCD40_ADDRESS, (uint8_t)9);  // 9 Bytes: CO2, Temp, Humidity + CRC
    if (Wire.available() != 9) {
        Serial.println("SCD40: Fehler beim Abrufen der Daten.");
        return false;
    }

    // CO2
    uint8_t co2_msb = Wire.read();
    uint8_t co2_lsb = Wire.read();
    Wire.read();  // CRC überspringen

    // Temperatur
    uint8_t t_msb = Wire.read();
    uint8_t t_lsb = Wire.read();
    Wire.read();  // CRC überspringen

    // Luftfeuchtigkeit
    uint8_t h_msb = Wire.read();
    uint8_t h_lsb = Wire.read();
    Wire.read();  // CRC überspringen

    *co2 = (co2_msb << 8) | co2_lsb;
    uint16_t raw_t = (t_msb << 8) | t_lsb;
    uint16_t raw_h = (h_msb << 8) | h_lsb;

    *temperature = -45.0 + 175.0 * ((float)raw_t / 65535.0);  // Umrechnung auf Temperatur in °C
    *humidity = 100.0 * ((float)raw_h / 65535.0);  // Umrechnung auf Luftfeuchtigkeit in %

    // CO2 in Prozent berechnen

    Serial.printf("SCD40: CO2: %u ppm, Temp: %.2f°C, Humidity: %.2f%%\n", *co2, *temperature, *humidity);

    return true;
}

// Hauptmessloop, der sicherstellt, dass die Daten bereit sind
bool scd40_loop_measurement(uint16_t* co2, float* temperature, float* humidity) {
    if (!scd40_data_ready()) {
        //Serial.println("SCD40: Messdaten noch nicht bereit.");
        return false;
    }

    return scd40_read_measurement(co2, temperature, humidity);
}

float getCO2Percent(uint16_t co2) {

    return (float)co2 / 1000.0;  // Umrechnung auf CO2 in Prozent
}

