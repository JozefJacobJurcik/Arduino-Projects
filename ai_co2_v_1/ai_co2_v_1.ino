#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SensirionI2CScd4x.h>
#include <EC11.hpp>


EC11 encoder;

EC11Event e;

// OLED Display Configuration
#define i2c_Address 0x3c // Typical eBay OLED's address
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // QT-PY / XIAO

const uint8_t encoderPinA = 4;
const uint8_t encoderPinB = 16;
const uint8_t encoderButton = 17;
const uint8_t VocPin = 25;
// pin 34 vcc for voc


static int encoder_value = 0;
int encoder_value_old = 0;
uint16_t error = 0;
int co2_old = 0;
float temperature_old = 0;
float humidity_old = 0;
bool buttonState = false;

const float max_volts = 3.3;
const float max_analog_steps = 4095;

void pinDidChange() {
  encoder.checkPins(digitalRead(encoderPinA), digitalRead(encoderPinB));
}

void prepare() {
  attachInterrupt(encoderPinA, pinDidChange, CHANGE);
  attachInterrupt(encoderPinB, pinDidChange, CHANGE);
}

// Create display and SCD4x objects
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SensirionI2CScd4x scd4x;

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

const unsigned char fire_1_bit [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x30, 0x00, 0x00, 0x60, 0x00, 0x00, 
	0xe0, 0x00, 0x01, 0xf0, 0x00, 0x01, 0xf8, 0x00, 0x03, 0xfc, 0x00, 0x07, 0xbe, 0x00, 0x07, 0xbf, 
	0x00, 0x07, 0x3f, 0x00, 0x07, 0x1f, 0x00, 0x05, 0x0f, 0x00, 0x06, 0x07, 0x00, 0x06, 0x07, 0x00, 
	0x03, 0x06, 0x00, 0x01, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char fire_2_bit [] PROGMEM = {
	0x00, 0x40, 0x00, 0x00, 0x80, 0x00, 0x01, 0x80, 0x00, 0x01, 0x80, 0x00, 0x01, 0xc0, 0x00, 0x04, 
	0xc4, 0x00, 0x06, 0xe4, 0x00, 0x02, 0x76, 0x00, 0x03, 0xfe, 0x00, 0x03, 0x7e, 0x00, 0x07, 0x3e, 
	0x00, 0x07, 0x1f, 0x00, 0x0f, 0x0f, 0x00, 0x0f, 0x8f, 0x00, 0x0c, 0x8f, 0x00, 0x04, 0x06, 0x00, 
	0x06, 0x04, 0x00, 0x03, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char fire_3_bit [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x00, 0x60, 0x00, 0x00, 0xf0, 0x00, 0x01, 
	0xf8, 0x00, 0x03, 0xf8, 0x00, 0x07, 0xfc, 0x00, 0x07, 0xbe, 0x00, 0x0f, 0xde, 0x00, 0x0f, 0xcf, 
	0x00, 0x0e, 0xe7, 0x00, 0x0c, 0xc7, 0x00, 0x0c, 0x47, 0x00, 0x0c, 0x07, 0x00, 0x04, 0x0e, 0x00, 
	0x02, 0x1c, 0x00, 0x01, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void encoderCheck(){

      if (encoder.read(&e)) {

        if (e.type == EC11Event::StepCW) {
          // Clock-wise.
          encoder_value += e.count;
          
        } else {
          // Counter clock-wise.
          encoder_value -= e.count;
        }
      }
}

void drawVOC(){
  display.setTextSize(1);
  display.setCursor(18,0);
  display.print("Volatile Organic");
  display.setCursor(40,10);
  display.print("Compounds");
  //display.drawFastVLine(64,0,64, SH110X_WHITE);
  
  uint8_t x = 48;
  uint8_t y = 51;
  display.setCursor(x+11,48);
  display.print("OK");
  display.drawTriangle( x, y, x-6, y +4, x-6, y-4, SH110X_WHITE);

  display.display();
}

void drawExitQuestion(){
  display.setCursor(35,0);
  display.setTextSize(2);
  display.print("Exit?");
  display.display();
}

void drawVOCmeasurment(int voc){ // VOC needs to be normalized to a 0-128 range
  display.setTextSize(1);
  display.setCursor(18,0);
  display.print("Volatile Organic");
  display.setCursor(40,10);
  display.print("Compounds");
  display.drawCircle(64, 128, 105, SH110X_WHITE);
  int y = int(-sqrt(sq(105)-sq(voc-64))+128);
  display.drawTriangle(62,63,66,63,voc,y,SH110X_WHITE);
  //display.drawFastVLine(64,128,-104,SH110X_WHITE);
  display.setCursor(64,20);
  display.print(".");
  display.setCursor(64,24);
  display.print(".");
  display.setCursor(64,28);
  display.print(".");

  display.setCursor(0,56);
  display.print("Good");
  display.setCursor(110,56);
  display.print("Bad");

  display.display();
}


void drawTemp(float temp){

  display.setTextSize(1);
  display.setCursor(34,0);
  display.print("Approximate");
  display.setTextSize(2);
  display.setCursor(43,12);
  display.print("TEMP");
  display.setCursor(35,42);
  display.print(temp);
  display.print(" C");
  display.drawRect(101,42, 4,4, SH110X_WHITE);

  display.display();
}

void drawHumidity(float hum){
  
  display.setTextSize(1);
  display.setCursor(34,0);
  display.print("Approximate");
  display.setTextSize(2);
  display.setCursor(20,12);
  display.print("HUMIDITY");
  display.setCursor(31,42);
  display.print(hum);
  display.print("%");

  display.display();
}

void drawYesNo(bool yes){
  
  uint8_t x = 46;
  uint8_t y = 0;
  if(yes){
    y = 33;
  } else {
    y = 51;
  }

  display.setTextSize(1);
  display.setCursor(x+8,30);
  display.print("YES");
  display.setCursor(x+11,48);
  display.print("NO");

  display.drawTriangle( x, y, x-6, y +4, x-6, y-4, SH110X_WHITE);

  display.display();


}

void drawQuestion(){
  display.setCursor(5,0);
  display.setTextSize(1);
  display.print("The sensor requires heating up. Continue?");
  display.display();
}

void drawLoading(int t){
  
  uint8_t x = 54;
  uint8_t y = 20;
  int interval = t/96;
  uint8_t current_bitmap = 1;
  uint8_t counter = 0;
  uint8_t counter_max = 10;

  for (int i = 0; i<96;i++){
    if (current_bitmap == 1 && counter == 0){
      display.clearDisplay();
      display.drawBitmap(x,y,fire_1_bit,20,20,SH110X_WHITE);
      counter++;
    } else if (current_bitmap == 2 && counter == 0){
      display.clearDisplay();
      display.drawBitmap(x,y,fire_2_bit,20,20,SH110X_WHITE);
      counter++;
    } else if (current_bitmap == 3 && counter == 0){
      display.clearDisplay();
      display.drawBitmap(x,y,fire_3_bit,20,20,SH110X_WHITE);
      counter++;
    } else if (current_bitmap == 3 && counter > counter_max) {
      current_bitmap = 1;
      counter = 0;
    } else if(current_bitmap != 3 && counter > counter_max){
      current_bitmap++;
      counter = 0;
    }else{
      counter++;
    }
    display.drawTriangle(64,y -10,42,y+20,86,y+20,SH110X_WHITE);
    // display.drawCircle(64, y +7, 14, SH110X_WHITE);
    display.drawRect(16,44,96, 16, SH110X_WHITE);
    display.setCursor(5,0);
    display.setTextSize(1);
    display.print("Sensor is heating up");

    int16_t x = 16 + i;
    display.fillRect(16,44,i+1,16, SH110X_WHITE);
    display.display();
    delay(interval);
    }
}


void drawCO2warning(int co2){
  uint8_t x = 0;
  uint8_t y = 0;
  display.setCursor(x,y);
  display.setTextSize(2);
  display.print("CO");
  display.setTextSize(1);
  display.setCursor(x+24,y+9);
  display.print("2");
  display.setCursor(x+36,y+4);
  display.print(": ");
  display.print(co2);
  display.print(" ppm");

  display.setTextSize(1);
  display.setCursor(x+5,y+20);
  if(co2<500){
    display.print("malo");
  }else{
    display.print("vela");
  }

  display.display();
}

void drawCO2Scale(int co2){
  uint8_t x = 50;
  uint8_t y = 0;
  display.setCursor(x,y);
  display.setTextSize(2);
  display.print("CO");
  display.setTextSize(1);
  display.setCursor(x+24,y+9);
  display.print("2");
  display.setTextSize(2);
  if(co2<1000){
    display.setCursor(x-3,y+23);
  }else{
    display.setCursor(x-12,y+23);
  }
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

void measuringVOC(){
  buttonState = true;
  delay(500);

  while(buttonState){
    display.clearDisplay();
    drawVOCmeasurment(50); // todo feed real values
    delay(100);
    buttonState = digitalRead(encoderButton);
  }

}

void theVOC(){
  
  display.clearDisplay();
  drawQuestion();
  drawYesNo(false);
  delay(500); // so that we dont take the previous press
  buttonState = true;

  while(buttonState){
    encoderCheck();
    display.clearDisplay();
    drawQuestion();
    drawYesNo(encoder_value % 2 == 0);
    
    encoder_value_old = encoder_value;
    while(encoder_value_old == encoder_value){
      buttonState = digitalRead(encoderButton);
      if(!buttonState){
        break;
      }
      delay(100);
      encoderCheck();
    }

  }

  if(encoder_value % 2 == 0){
      drawLoading(20000); // todo change 
      bool continueWhile = true;
      while(continueWhile){
        measuringVOC();
        
        //pressed button
        display.clearDisplay();
        drawExitQuestion();
        drawYesNo(false);

        delay(500);
        buttonState = true;

        while(buttonState){
          encoderCheck();
          display.clearDisplay();
          drawExitQuestion();
          drawYesNo(encoder_value % 2 == 0);

          encoder_value_old = encoder_value;
          while(encoder_value_old == encoder_value){
            buttonState = digitalRead(encoderButton);
            if(!buttonState){
              break;
            }
            delay(100);
            encoderCheck();
          }
        }

        if(encoder_value % 2 == 0){
          continueWhile = false;
        } 
      
      }

  }

}

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
    Serial.begin(115200); // TODO remove
    delay(200); // Wait for serial to initialize TODO remove

    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);
    pinMode(encoderButton, INPUT);
    pinMode(VocPin, INPUT);
    
    prepare();

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
}


void loop() {
    if (error) {
        display.println("Measurement Error");
        display.display();
        delay(1000);
    }

    if (encoder_value == 4){
      for(uint8_t i = 0; i<20;i++){
        buttonState = digitalRead(encoderButton);
        if(buttonState == LOW){
          theVOC(); // todo refresh rate flickers
          delay(500);
          encoder_value = 4;
        }
        delay(10);
      }
    } else {
      delay(200);
    }

    encoderCheck();
    display.clearDisplay();
    display.setCursor(0,0);
    
    switch(encoder_value) {
      case 0:          
        drawCO2Scale(co2_old);
      break;

      case 1:         
        drawCO2warning(co2_old);
      break;

      case 2:
        drawTemp(temperature_old);
      break;

      case 3:
        drawHumidity(humidity_old);
      break;

      case 4:         
        drawVOC();
      break;

      default:
        drawYesNo(false); // TODO
    }

      
      /* 
      if(encoder_value == 1){
        drawCO2Scale(co2_old);
      } else if (encoder_value == 2){
        drawCO2warning(co2_old);
      } else if (encoder_value == 3){
        drawTemp(temperature_old);
      } else if (encoder_value == 4){
        drawHumidity(humidity_old);
      } else {
        drawYesNo(false);
      }
      */
        
        
    
    char errorMessage[256];
    bool isDataReady = false;
    error = scd4x.getDataReadyFlag(isDataReady);
    if (error) {
        display.println("Data Ready Error");
        display.display();
        return;
    }

    if (!isDataReady) {
        return;
    }

    // Read measurement
    uint16_t co2 = 0;
    float temperature = 0.0f;
    float humidity = 0.0f;
    error = scd4x.readMeasurement(co2, temperature, humidity);

    co2_old = co2;
    temperature_old = temperature; // calibration function TODO
    humidity_old = humidity;

}



