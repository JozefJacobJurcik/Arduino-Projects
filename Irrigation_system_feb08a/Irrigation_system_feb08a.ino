#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/6f040ed3-399f-402e-a56d-b46155b23073 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  String message;
  bool led;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include "Menu.h"
#include "Plant.h"
#include "time.h"
#include "TimeLib.h"
#include "sntp.h"
#include "tuple"


const int LED_PIN = 14;
const int M1_PIN = 34;
const int M2_PIN = 35;
const int SWITCH_PIN = 32;
const int R1_PIN = 25;
const int R2_PIN = 27;
 

//------------------------------------------ temp
int temp = 20;
//------------------------------------------

//objects
Plant p1 = Plant(1 , R1_PIN , M2_PIN);
Plant p2 = Plant(2 , R2_PIN , M2_PIN);

Menu menu = Menu(0);

time_t timeoutAfterWatering;
time_t timeOfLastMessageChange;


void setup() {
  
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  
  pinMode(R1_PIN, OUTPUT);
  pinMode(R2_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
}

void loop() {
  ArduinoCloud.update();
  
  //update time at 12 am
  if ((hour()==0) && (0 < minute() < 3)){
    updateTime();
  }

  delay(200);
  
  if (ArduinoCloud.connected()){

    if (( getCustomTimeNow() - timeOfLastMessageChange) < 600){
      ArduinoCloud.update();
      delay(200);
    } else {
      doIfNotMessaging();
      delay(60000); //1min
    }
  }
    
}

void doIfNotMessaging(){


  //chcek bedtime
  auto bedtime = menu.getBedtime(); 
  bool isItBedtime = ((std::get<0>(bedtime)*60 + std::get<1>(bedtime)) < (hour()*60 + minute())) && ((hour()*60 + minute()) < (std::get<2>(bedtime)*60 + std::get<3>(bedtime)));
    
  if (!(isItBedtime || (getCustomTimeNow() < timeoutAfterWatering))){
      
    if (checkEnoughWater()){
      checkAlarmWaterSetNextAllPlants();
    } else {
      p1.setError("not enough water in tank");
      p2.setError("not enough water in tank");
      digitalWrite(LED_PIN, HIGH);
    }
  }

}

void checkAlarmWaterSetNextAllPlants(){
  bool wasWatered = false;

  if (p1.getAlarmIsTripped()){
    p1.waterPlant();
    p1.setAlarm();
    wasWatered = true;
  }

  if (p2.getAlarmIsTripped()){
    p2.waterPlant();
    p2.setAlarm();
    wasWatered = true;
  }
  
  if (p1.checkError() || p2.checkError()){
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  if (wasWatered){ 
    timeoutAfterWatering = getCustomTimeNow() + 600 ; // 10 min from now
    wasWatered = false;
  }
}

/*
  Since Led is READ_WRITE variable, onLedChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onLedChange()  {
  // Add your code here to act upon Led change
  if(led){
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
}

/*
  Since Message is READ_WRITE variable, onMessageChange() is
  executed every time a new value is received from IoT Cloud.
*/

void onMessageChange()  {

  timeOfLastMessageChange = getCustomTimeNow();

  if (menu.getReplyReadyStatus()){
    message = menu.getReply() ;
  } else {
    menu.check(message);
  }

} 


void switchLed(bool switchOn){
  if(switchOn){
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void updateTime(){
  time_t unixTime = ArduinoCloud.getLocalTime(); // Unix timestamp
  if (unixTime != 0){
    setTime(unixTime);
  } else {
    message = "Its either 1970 or something went wrong and time could not be updated";
    digitalWrite(LED_PIN, HIGH);
  } 
}

bool checkEnoughWater(){
  return (digitalRead(SWITCH_PIN)== HIGH);
}

time_t getCustomTimeNow(){
  return ArduinoCloud.getLocalTime();
}

