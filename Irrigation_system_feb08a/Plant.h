#ifndef Plant_h
#define Plant_h

#include "Arduino.h"

class Plant {
  
  private:
    int number;
    String name;
    int mode;
  public:
    Plant(int num);
    void setName(String nam);
    String getName();
    bool isWaterByT;
    void setModeTimer(int d, int h, int m);
    void setModeMoisture(int m);
    void setModeHybrid(int p, int d, int h, int m);
    void setModeBoth(int p, int d, int h, int m);
    void setWaterByP(int p);
    void setWaterByT(int t);
    String getMode();
  
    

    
};


#endif