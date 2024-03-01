#include "Plant.h"


Plant::Plant(int num, int R_PIN, int M_PIN) {
  MOISTURE_PIN = R_PIN;
  RELAY_PIN = M_PIN;
  number = num;
  timeToWaterInS = 1; // so the default is enough for the user to know it works and isnt set correctly
  percentMoistureToReach = 50; // idk adequate default
  isWateredByT = true;
  name = "undefined";
}

void Plant::setName(String nam){
  name = nam;
}

String Plant::getName(){
  return name;
}

void Plant::waterPlant(){
  if (isWateredByT){
    digitalWrite(RELAY_PIN, HIGH);
    delay(timeToWaterInS*1000);
    digitalWrite(RELAY_PIN, LOW);
  } else {
    waterPlanrByP();
  }
}

void Plant::waterPlanrByP(){
  //todo
}

void Plant::setModeTimer(int d, int h, int m){
  //todo
}

void Plant::setModeMoisture(int m){
  //todo
}

void Plant::setModeHybrid(int p, int d, int h, int m){
  //todo
}

void Plant::setModeBoth(int p, int d, int h, int m){
  //todo
}

void Plant::setModeSchedule(int d, int h, int m){
  //todo
}

void Plant::setWaterByP(int p){
  //todo
}

void Plant::setWaterByT(int t){
  //todo
}

String Plant::getMode(){

  switch (mode){

    case 1: 
      return "Timer";
    
    case 2:
      return "Moisture";
    
    case 3:
      return "Hybrid";
    
    case 4:
      return "Both";
    
    default:
      return "Something went wrong - try setting it again";


  }

}