#ifndef Menu_h
#define Menu_h

#include "Arduino.h"

class Menu {
  
  public:
    Menu(int stat);
    void printAnswer(String rep);
    void check(String userIn);
    bool getReplyReadyStatus();
    String getReply();
    int status;
    bool replyReady;
    String reply;
    int selectedPlant;
    
    
};

#endif