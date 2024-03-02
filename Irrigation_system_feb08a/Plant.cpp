#include "Plant.h"
#include "TimeLib.h"
extern time_t getCustomTimeNow();

Plant::Plant(int num, int R_PIN, int M_PIN) {
  MOISTURE_PIN = R_PIN;
  RELAY_PIN = M_PIN;
  number = num;
  wateringTimeInS = 1; // so the default is enough for the user to know it works and isnt set correctly
  percentMoistureToReach = 50; // idk adequate default
  isWateredByT = true;
  name = "undefined";
  moistureAlarmIsTripped = false; 
  alarmWasAlreadySetOnce = false;
  firstAlarmForSchedule = 0;
  timerInS = 60;
  alarmTime = 0; // unix time when the alarm is tripped
}

void Plant::setName(String nam){
  name = nam;
}

String Plant::getName(){
  return name;
}

bool Plant::getAlarmIsTripped(){
  
  switch(mode){

    //timer
    case 1:  
        return (alarmTime < getCustomTimeNow());
    break;

    // moisture
    case 2:
      return (getMoistureFromSensor() < percentMoistureToReach);
    break;

    // hybrid
    case 3:
      
      if (!moistureAlarmIsTripped){
        moistureAlarmIsTripped = (getMoistureFromSensor() < percentMoistureToReach);
        return false;

      } else if (!alarmWasAlreadySetOnce){
        setAlarm();
        alarmWasAlreadySetOnce = true;
        return false;

      } else {
        if (alarmTime < getCustomTimeNow()){
          alarmWasAlreadySetOnce = false;
          moistureAlarmIsTripped = false;
          return true;
        
        } else { 
          return false; 
        }
      }
    break;

    // both
    case 4:
      return ((getMoistureFromSensor() < percentMoistureToReach) || (alarmTime < getCustomTimeNow()));
    break;

    // schedule
    case 5:
      
      if (!alarmWasAlreadySetOnce){
        
        if (firstAlarmForSchedule < getCustomTimeNow()){
          alarmWasAlreadySetOnce = true;
          return true;
        } else { 
          return false;
        }

      } else {
        return (alarmTime < getCustomTimeNow());
      }

    break;

    default:
      return true; // idk probably better than return nothing and get an error - potential bug
    break;



  }
}

void Plant::setAlarm(){
  
  if (mode != 2){
  time_t timeNow= getCustomTimeNow();
  alarmTime = timeNow + timerInS;
  }

}

void Plant::waterPlant(){
  if (isWateredByT){
    digitalWrite(RELAY_PIN, HIGH);
    delay(wateringTimeInS*1000);
    digitalWrite(RELAY_PIN, LOW);
  } else {
    waterPlanrByP();
  }
}

void Plant::waterPlanrByP(){
  //todo
}

void Plant::setModeTimer(int d, int h, int m){
  mode = 1;
  timerInS = m*60 + h*3600 + d*86400;
  setAlarm();
}

void Plant::setModeMoisture(int p){
  mode = 2;
  percentMoistureToReach = p;

}

void Plant::setModeHybrid(int p, int d, int h, int m){
  mode = 3;
  percentMoistureToReach = p;
  timerInS = m*60 + h*3600 + d*86400;
  alarmWasAlreadySetOnce = false;
  moistureAlarmIsTripped = false;
  setAlarm();
}

void Plant::setModeBoth(int p, int d, int h, int m){
  mode = 4;
  percentMoistureToReach = p;
  timerInS = m*60 + h*3600 + d*86400;
  setAlarm();

}

void Plant::setModeSchedule(int d, int h, int m){
  mode = 5;
  timerInS = d*86400;
  
  time_t timeNow= getCustomTimeNow();
  firstAlarmForSchedule = timeNow + (((m*60 + h*3600) - (minute()*60 + hour()*3600)) % 86400) ; // seconds to the next specified time
  alarmWasAlreadySetOnce = false;

}

void Plant::setWaterByP(int p){
  //todo
}

void Plant::setWaterByT(int t){
  //todo
}

int Plant::getMoistureFromSensor(){
  //todo has to  return a number from 0 to 100
}

String Plant::getMode(){

  switch (mode){

    case 1: 
      return "Timer";
    break;

    case 2:
      return "Moisture";
    break;  
    
    case 3:
      return "Hybrid";
    break;  
    
    case 4:
      return "Both";
    break;

    case 5:
      return "Schedule";
    break;  
    
    default:
      return "Something went wrong - try setting it again"; 
    break;

  }

}