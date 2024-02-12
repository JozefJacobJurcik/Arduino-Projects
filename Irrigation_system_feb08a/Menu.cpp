#include "Menu.h"

extern void onMessageChange();
extern void switchLed(bool switchOn);
extern String getCurrentTime();

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
      printAnswer("Menu: \n Type \"time\" to update and display current time. \n Type \"mode\" to switch between timer mode, moisture mode and hybrid \n \"now\" to water a plant manually right now");
      status = 1;
      break;

    // reply to 1st input after menu
    case 1:

      // because switch with a String doesnt work in c++ for some reason and im not bothered to do it with vectors
      if (userIn == "time") {
       
        printAnswer("Updating... Current time: " + getCurrentTime());
        status = 0;

      } else if (userIn == "mode") {
        
        //printAnswer("Type timer to set time interval for watering, Type moisture to water when a specific moisture is reached, Type hybrid to set a timer for watering after a specific moisture is reached, Type both to water after whatever comes first: time or moisture treshold, or type back to go back.");
        printAnswer("Type timer to set time interval for watering, Type moisture"); //todo prilis dlhe to hore asi
        status = 2;


      } else if (userIn == "now") {
        printAnswer("Type 1 or 2 depending on what plant you want to water. Or type \"back\" to go back.");
        status = 3;

      } else {
        printAnswer("wtf");
      }
      break;

    // reply after selecting mode
    case 2:

      if (userIn == "timer"){

      }else if (userIn == "moisture"){

      }else if (userIn == "hybrid"){
        
      }else if (userIn == "both"){
        
      }else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again...");
      }
      break;

    //reply after selecting to water now and selecting a plant
    case 3:
      if (userIn == "1"){
        printAnswer("How much ? #todo");
        status = 31; 

      }else if (userIn == "2"){
        printAnswer("How much ? #todo");
        status = 32;

      } else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again...");
      }
    break; 

    // water plant 1 userIn amount right now
    case 31:
      status = 0; // todo

    break;

    // water plant 2 userIn amount right now
    case 32:
      status = 0;//todo

    break;  
    // if something went wrong
    default:
      printAnswer("default");
      break;
  }

  

  
}

