#include "Menu.h"

extern void onMessageChange();

Menu::Menu(int stat) {
  status = stat;
  replyReady = false;
}

void Menu::printAnswer(String rep){
      replyReady = true;
      reply = rep;
      onMessageChange();
      replyReady = false;
}

bool Menu::getReplyReadyStatus(){
  return replyReady;
}

String Menu::getReply(){
  return reply;
}

void Menu::check(String userIn){

  
  switch(status){
        
    case 0:
      printAnswer(userIn +" 0");
      status++;
      break;

    case 1:
      printAnswer(userIn +" 1");
      status++;
      break;

    default:
      printAnswer("length:" + userIn.length());
      break;

      
  }
}


