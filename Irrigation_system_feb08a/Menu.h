#ifndef Menu_h
#define Menu_h


#include "Arduino.h"

class Menu {
  private:
    int bedtimeStartH;
    int bedtimeStartM;
    int bedtimeEndH;
    int bedtimeEndM;
  public:
    Menu(int stat);
    void printAnswer(String rep);
    void check(String userIn);
    bool getReplyReadyStatus();
    String getReply();
    std::tuple<int, int, int, int> getBedtime();
    int status;
    bool replyReady;
    String reply;
    int selectedPlant;
    
    
};

#endif