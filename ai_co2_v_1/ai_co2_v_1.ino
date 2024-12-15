#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SensirionI2CScd4x.h>

// OLED Display Configuration
#define i2c_Address 0x3c // Typical eBay OLED's address
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // QT-PY / XIAO

const unsigned char smile_bit [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x07, 0xfe, 0x00, 0x0f, 0xff, 0x00, 0x1f, 
	0xff, 0x80, 0x1e, 0xf7, 0x80, 0x3d, 0x6b, 0xc0, 0x3d, 0x6b, 0xc0, 0x3d, 0x6b, 0xc0, 0x3f, 0xff, 
	0xc0, 0x3c, 0x03, 0xc0, 0x3d, 0xfb, 0xc0, 0x1e, 0x07, 0x80, 0x1f, 0x0f, 0x80, 0x0f, 0xff, 0x00, 
	0x07, 0xfe, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char skull_bit [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x07, 0xfc, 0x00, 0x0f, 0xfe, 0x00, 0x1f, 
	0xff, 0x00, 0x18, 0xe3, 0x00, 0x18, 0xe3, 0x00, 0x18, 0xe3, 0x00, 0x1f, 0xff, 0x00, 0x1f, 0x1f, 
	0x00, 0x1f, 0xff, 0x00, 0x0f, 0xfe, 0x00, 0x07, 0x5c, 0x00, 0x05, 0x54, 0x00, 0x05, 0x54, 0x00, 
	0x07, 0xfc, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


// Create display and SCD4x objects
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SensirionI2CScd4x scd4x;

// Utility function to print hex values
void printUint16Hex(uint16_t value) {
    Serial.print(value < 4096 ? "0" : "");
    Serial.print(value < 256 ? "0" : "");
    Serial.print(value < 16 ? "0" : "");
    Serial.print(value, HEX);
}

// Utility function to print serial number
void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
    Serial.print("Serial: 0x");
    printUint16Hex(serial0);
    printUint16Hex(serial1);
    printUint16Hex(serial2);
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    delay(250); // Wait for serial to initialize

    // Initialize Wire for I2C communication
    Wire.begin();

    // Initialize OLED Display
    display.begin(i2c_Address, true);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0,0);

    // Initialize SCD4x CO2 Sensor
    uint16_t error;
    char errorMessage[256];
    scd4x.begin(Wire);

    // Stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        display.println("Stop Measurement Error");
        display.display();
    }

    // Get and print serial number
    uint16_t serial0, serial1, serial2;
    error = scd4x.getSerialNumber(serial0, serial1, serial2);
    if (error) {
        display.println("Serial Num Error");
        display.display();
    } else {
        // Display serial number on OLED
        display.print("Serial: ");
        display.print(serial0, HEX);
        display.print(serial1, HEX);
        display.println(serial2, HEX);
        display.display();
        delay(1000);
    }

    // Start periodic measurement
    error = scd4x.startPeriodicMeasurement();
    if (error) {
        display.println("Start Measure Error");
        display.display();
    }

    display.clearDisplay();
    display.setCursor(0,0);
    display.println("SCD4x Initialized");
    display.display();
    delay(1000);
}

void loop() {
    uint16_t error;
    char errorMessage[256];
    delay(5000); // Wait 5 seconds between measurements
    display.clearDisplay();
    
    display.setCursor(0,0);

    // Check if data is ready
    bool isDataReady = false;
    error = scd4x.getDataReadyFlag(isDataReady);
    if (error) {
        display.println("Data Ready Error");
        display.display();
        return;
    }

    if (!isDataReady) {
        display.println("No data yet");
        display.display();
        return;
    }

    // Read measurement
    uint16_t co2 = 0;
    float temperature = 0.0f;
    float humidity = 0.0f;
    error = scd4x.readMeasurement(co2, temperature, humidity);

    
    if (error) {
        display.println("Measurement Error");
        display.display();
    } else if (co2 == 0) {
        display.println("Invalid sample");
        display.display();
    } else {
        
        // drawProgressBar(20000);
        drawCO2Scale(co2);
        
    }
}

void showCO2(int co2){
  display.setCursor(0,0);
  display.setTextWrap(true);
  display.setTextSize(2);
  display.print("CO");
  display.setTextSize(1);
  display.println(" ");
  display.println("2");
  display.setTextSize(3);
  display.print(co2);
  display.setTextSize(1);
  display.println(" ");
  display.setTextSize(2);
  display.println("ppm");
  display.display();

}

void drawProgressBar(int t){
  int interval = t/96;
  display.drawRect(16,24,96, 16, SH110X_WHITE);
  display.display();

  for (int i = 0; i<96;i++){
    int16_t x = 16 + i;
    display.fillRect(x,24,1,16, SH110X_WHITE);
    display.display();
    delay(interval);
    }

}

void drawCO2Scale(int co2){
  int x = 50;
  int y = 0;
  display.setCursor(x,y);
  display.setTextSize(2);
  display.print("CO");
  display.setTextSize(1);
  display.setCursor(x+24,y+9);
  display.print("2");
  if(co2<1000){
    display.setCursor(x-3,y+23);
  }else{
    display.setCursor(x-12,y+23);
  }
  display.setTextSize(2);
  display.print(co2);
  display.setCursor(x+50,y+30);
  display.setTextSize(1);
  display.println("ppm");

  display.drawBitmap(0,44,smile_bit , 20, 20, SH110X_WHITE );
  display.drawBitmap(108,44,skull_bit , 20, 20, SH110X_WHITE );
  display.drawRect(25,49, 78, 10, SH110X_WHITE);

  int tot = 25 + map(co2, 400, 5000, 0, 78); // tip of triangle

  display.drawTriangle(tot - 3 , 47 - 3 , tot , 47 , tot + 3, 47 - 3, SH110X_WHITE);
  display.fillRect(25, 49, tot - 25, 10, SH110X_WHITE);

  display.display();
}

