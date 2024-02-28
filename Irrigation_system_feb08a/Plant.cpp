#include "Plant.h"

Plant::Plant(int num) {
  number = num;
  name = "";
  isWaterByT = true;
}

void Plant::setName(String nam){
  name = nam;
}

String Plant::getName(){
  return name;
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