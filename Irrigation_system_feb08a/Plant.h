#ifndef Plant_h
#define Plant_h

#include "Arduino.h"

class Plant {
  
  private:
    int number;
    String name;
    int mode;
    bool moistureAlarmIsTripped;
    bool alarmWasAlreadySetOnce;
    time_t firstAlarmForSchedule;
    int percentMoistureToWater;
    int timerInS;
    time_t alarmTime;
    String errorMessage;
    bool isError;
    int waterByPPumpTimeInS;
    int waterByPWaitTimeInS;
  public:
    Plant(int num, int RELAY_PIN, int MOISTURE_PIN);
    int RELAY_PIN;
    int MOISTURE_PIN;
    int wateringTimeInS;
    int percentMoistureToReach;
    void setName(String nam);
    String getName();
    bool isWateredByT;
    void waterPlant();
    void waterPlantByP(int p);
    void waterPlantByT(int t);
    void setAlarm();
    bool getAlarmIsTripped();
    void setModeTimer(int d, int h, int m);
    void setModeMoisture(int m);
    void setModeHybrid(int p, int d, int h, int m);
    void setModeBoth(int p, int d, int h, int m);
    void setModeSchedule(int d, int h, int m);
    void setWaterByP(int p, int x, int w);
    void setWaterByT(int t);
    int getMoistureFromSensor();
    String getMode();
    void setError(String eMessage);
    String getErrorMessageAndReset();
    bool checkError();

  
    

    
};


#endif