#include "Menu.h"

extern void onMessageChange();
extern void switchLed(bool switchOn);

Menu::Menu(int stat) {
  status = stat;
  replyReady = false;
}

void Menu::printAnswer(String rep) {
  replyReady = true;
  reply = rep;
  onMessageChange();
  replyReady = false;
}

bool Menu::getReplyReadyStatus() {
  return replyReady;
}

String Menu::getReply() {
  return reply;
}
// default status = 0
void Menu::check(String userIn) {


  switch (status) {

    // default menu
    case 0:
      printAnswer("Menu: \n (1): led on \n (2): led off \n (3) back to menu");
      status = 1;
      break;

    // reply to 1st input
    case 1:

      if (userIn == "1") {
        printAnswer("switching led on");
        switchLed(true);

      } else if (userIn == "2") {
        printAnswer("switching led off");
        switchLed(false);

      } else if (userIn == "3") {
        printAnswer("");
        status = 0;
        onMessageChange();

      } else {
        printAnswer("wtf");
      }
      break;

    // if something went wrong or clear messages
    default:
      printAnswer("default");
      break;
  }

  

  
}

