#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

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

void drawExitQuestion(){
  display.setCursor(35,0);
  display.setTextSize(2);
  display.print("Exit?");
  display.display();
}

void drawVOC(int voc){ // VOC needs to be normalized to a 0-128 range
  display.setTextSize(1);
  display.setCursor(20,0);
  display.print("Volatile Organic");
  display.setCursor(42,10);
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
  
  int x = 46;
  int y = 0;
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
  
  int x = 54;
  int y = 20;
  int interval = t/96;
  int current_bitmap = 1;
  int counter = 0;
  int counter_max = 10;

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
  int x = 0;
  int y = 0;
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
  int x = 50;
  int y = 0;
  display.setCursor(x,y);
  display.setTextSize(2);
  display.print("CO");
  display.setTextSize(1);
  display.setCursor(x+24,y+9);
  display.print("2");
  display.setCursor(x-12,y+23);
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

void setup()   {

  Serial.begin(9600);

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
 
  display.display();
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  /*
  display.drawBitmap(0,44,smile_bit , 20, 20, SH110X_WHITE );
  display.drawBitmap(108,44,skull_bit , 20, 20, SH110X_WHITE );
  display.drawRect(25,49, 78, 10, SH110X_WHITE);
  display.display();
  */
  
  drawExitQuestion();
  drawYesNo(true);
  
  
  
  
  for(int i = 0; i<128; i++){
    display.clearDisplay();
    drawVOC(i);
    delay(500);
  }

  
  display.clearDisplay();
  drawLoading(400);
  delay(2000);

  display.clearDisplay();
  drawLoading(1000);
  delay(2000);
  display.clearDisplay();
  drawLoading(2000);
  delay(2000);
  display.clearDisplay();
  drawLoading(4000);
  delay(2000);
  
  drawQuestion();
  drawYesNo(false);
  delay(5000);

  display.clearDisplay();
  drawQuestion();
  drawYesNo(true);
  delay(5000);

  display.clearDisplay();
  drawQuestion();
  drawYesNo(false);
  

}


void loop() {
  
}

