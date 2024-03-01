#ifndef Plant_h
#define Plant_h

#include "Arduino.h"

class Plant {
  
  private:
    int number;
    String name;
    int mode;
  public:
    Plant(int num, int RELAY_PIN, int MOISTURE_PIN);
    int RELAY_PIN;
    int MOISTURE_PIN;
    int timeToWaterInS;
    int percentMoistureToReach;
    void setName(String nam);
    String getName();
    bool isWateredByT;
    void waterPlant();
    void waterPlanrByP();
    void setModeTimer(int d, int h, int m);
    void setModeMoisture(int m);
    void setModeHybrid(int p, int d, int h, int m);
    void setModeBoth(int p, int d, int h, int m);
    void setModeSchedule(int d, int h, int m);
    void setWaterByP(int p);
    void setWaterByT(int t);
    String getMode();
  
    

    
};


#endif