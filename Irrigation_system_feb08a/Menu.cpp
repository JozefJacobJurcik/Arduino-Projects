#include "Menu.h"
#include "Plant.h"
#include "TimeLib.h"
#include "tuple"

extern void onMessageChange();
extern void updateTime();
extern void switchLed(bool switchOn);
extern bool checkEnoughWater();
extern Plant p1;
extern Plant p2;
int d,h,m,p,t,x,w,hh,mm,numValues;


Menu::Menu(int stat) {
  status = stat;
  replyReady = false;
  selectedPlant = 0;
  bedtimeStartH = 0;
  bedtimeStartM = 0;
  bedtimeEndH = 0;
  bedtimeEndM = 0;
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




// default status = 0 - there probably is a better way but i dont think it would be practical to rewrite it
void Menu::check(String userIn) {


  switch (status) {

    // default menu
    case 0:
      printAnswer("Menu type:\n'settings' to show additional settings\n'volume' to set how much water each plant should get\n'mode' to switch between modes\n'now' to water a plant manually right now\n'rename' to rename the plants");
      //todo errors
      status = 1;
      d = 0;
      h = 0;
      m = 0;
      p = 0;
      t = 0;
      x = 0;
      w = 0;
      hh = 0;
      mm = 0;
      numValues = 0;
      selectedPlant = 0;
    break;

    // reply to 1st input after menu
    case 1:

      // because switch with a String doesnt work in c++ for some reason and im not bothered to do it with vectors
      if (userIn == "settings") {
        printAnswer("Type:\n'time' to set the current time\n'calibrate' to calibrate the moisture sensors\n'bedtime' to set a do-not-water time period\n'check' to check if there is enough water\n'error' to see the error message\nOr type 'back' to go back.");
        status = 6; 

      }else if (userIn == "volume") {
        reply = "Change amount of water for '";
        reply += p1.getName();
        reply += "' or '";
        reply += p2.getName();
        reply += "' ? or type 'back' to go back.";
        printAnswer(reply);
        status = 5;

      } else if (userIn == "mode") {
        reply = "Change mode for '";
        reply += p1.getName();
        reply += "' or '";
        reply += p2.getName();
        reply += "' ? or type 'back' to go back.";
        printAnswer(reply);
        status = 2;

      } else if (userIn == "now") {
        reply = "Water '";
        reply += p1.getName();
        reply += "' or '";
        reply += p2.getName();
        reply += "' right now? or type 'back' to go back.";
        printAnswer(reply);
        status = 3;

      } else if (userIn == "rename") {
        printAnswer("Type 1 or 2 depending on what plant you want to rename. Or type 'back' to go back.");
        status = 4;

      } else {
        status = 0;
        onMessageChange();

      }
    break;

    // reply after selecting mode
    case 2:
      if (userIn == p1.getName()){
        selectedPlant = 1;
        printAnswer("Water when:\ntimer runs out - 'timer'\nmoisture is reached - 'moisture'\ntimer runs out after a moisture is reached - 'hybrid'\nwhat comes first misture or timer - 'both'\nevery x days at a specified time - 'schedule' \n'back' to go back");
        status = 21;


      }else if (userIn == p2.getName()){
        selectedPlant = 2;
        printAnswer("Water when:\ntimer runs out - 'timer'\nmoisture is reached - 'moisture'\ntimer runs out after a moisture is reached - 'hybrid'\nwhat comes first misture or timer - 'both'\n'back' to go back");
        status = 21;

      }else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again... or type 'back'");
      }

    break;
    
    // reply after selecting what plant and what mode
    case 21:

      if (userIn == "timer"){
        printAnswer("How long between watterings? \n Type the time in this format: {number of days}d{hours}h{minutes}m \n e.g. : 2d15h30m ");
        status = 211;        

      }else if (userIn == "moisture"){
        printAnswer("At what moisture % should watering occur? \n Type a number from 1 to 100 :");
        status = 212;

      }else if (userIn == "hybrid"){
        printAnswer("At what moisture % should a timer for how much time start? \n Type it in this format:\n{moisture % - from 1 to 100}p{number of days}d{hours}h{minutes}m \n e.g. : 75p0d2h30m");
        status = 213;
        
      }else if (userIn == "both"){
        printAnswer("Set a timer and moisture treshold, whatever comes first will water the plant\nType it in this format:\n{moisture % - from 1 to 100}p{number of days}d{hours}h{minutes}m \n e.g. : 35p0d2h30m");
        status = 214;
        
      }else if (userIn == "schedule"){
        printAnswer("Set what hour and every how many days the plant should be watered. \nType it in this format:\{every how many days}d{at this hour}h{minutes}m \n e.g. every 5 days at 15:30:\n 5d15h30m ");
        status = 215;
        
      }else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again... or type 'back'");
      }
    break;

    //setting timer
    case 211:
      
      
      numValues = sscanf(userIn.c_str(), "%dd%dh%dm", &d, &h, &m);
      
      if (numValues == 3){
        if (selectedPlant == 1){
          p1.setModeTimer(d,h,m);

        } else if (selectedPlant == 2){
          p2.setModeTimer(d,h,m);

        } else {
          printAnswer(" error in 211, exiting...type anything to go back to menu");
          status = 0;
          
        }
      
      reply = "Timer was set to Days: ";
      reply += String(d);
      reply += " Hours: ";
      reply += String(h);
      reply += " Minutes: ";
      reply += String(m);
      printAnswer(reply);
      status = 0;

      } else {
        printAnswer("wrong format could not set timer, exiting...type anything to go back to menu");
        status = 0;
        
      }
    break;

    // setting moisture
    case 212:
      
      p = userIn.toInt();

      if (0 < p < 101){
        if (selectedPlant == 1){
          p1.setModeMoisture(p);

        } else if (selectedPlant == 2){
          p2.setModeMoisture(p);

        } else {
          printAnswer(" error in 212, exiting...type anything to go back to menu");
          status = 0;
          
        }

      reply = "Moisture treshold is set to ";
      reply += String(p);
      reply += " %";
      printAnswer(reply);
      status = 0;

      } else {
        printAnswer("wrong format could not set moisture, exiting...type anything to go back to menu");
        status = 0;
        
      }

    break;

    //setting hybrid
    case 213:
      numValues = sscanf(userIn.c_str(), "%dp%dd%dh%dm", &p, &d, &h, &m);

      if (numValues == 4 && (0 < p < 101) ){
        if (selectedPlant == 1){
          p1.setModeHybrid(p,d,h,m);

        } else if (selectedPlant == 2){
          p2.setModeHybrid(p,d,h,m);

        } else {
          printAnswer(" error in 213, exiting...type anything to go back to menu");
          status = 0;
          
        }
      
      reply = "After moisture in the pot reaches ";
      reply += String(p);
      reply += " % a timer of Days: ";
      reply += String(d);
      reply += " Hours: ";
      reply += String(h);
      reply += " Minutes: ";
      reply += String(m);
      reply += " will start, after which the plant will be watered.";
      printAnswer(reply);
      status = 0;

      } else {
        printAnswer("wrong format could not set hybrid mode, exiting...type anything to go back to menu");
        status = 0;
        
      }
      
    break;

    case 214:
    numValues = sscanf(userIn.c_str(), "%dp%dd%dh%dm", &p, &d, &h, &m);

      if (numValues == 4 && (0 < p < 101) ){
        if (selectedPlant == 1){
          p1.setModeBoth(p,d,h,m);

        } else if (selectedPlant == 2){
          p2.setModeBoth(p,d,h,m);

        } else {
          printAnswer(" error in 214, exiting...type anything to go back to menu");
          status = 0;
          
        }
      
      reply = "After moisture in the pot reaches ";
      reply += String(p);
      reply += " % plant will be watered or if the timer of ";
      reply += String(d);
      reply += " Hours: ";
      reply += String(h);
      reply += " Minutes: ";
      reply += String(m);
      reply += " runs out.";
      printAnswer(reply);
      status = 0;

      } else {
        printAnswer("wrong format could not set both mode, exiting...type anything to go back to menu");
        status = 0;
        
      }

    break;
      

    case 215:
      numValues = sscanf(userIn.c_str(), "%dd%dh%dm", &d, &h, &m);
      
      if ( (numValues == 3 ) && (d>0) ){
        if (selectedPlant == 1){
          p1.setModeSchedule(d,h,m);

        } else if (selectedPlant == 2){
          p2.setModeSchedule(d,h,m);

        } else {
          printAnswer(" error in 215, exiting..type anything to go back to menu");
          status = 0;
          
        }
      
      reply = "Plant will be watered every ";
      reply += String(d);
      reply += " days at ";
      reply += String(h);
      reply += ":";
      reply += String(m);
      printAnswer(reply);
      status = 0;

      } else {
        printAnswer("wrong format could not set schedule, exiting...type anything to go back to menu");
        status = 0;
        
      }
    break;

    //reply after selecting to water now and selecting a plant
    case 3:
      if (userIn == p1.getName()){
        p1.waterPlant();
        printAnswer("watering... press anything to go back");
        status = 0; 

      }else if (userIn == p2.getName()){
        p2.waterPlant();
        printAnswer("watering... press anything to go back");
        status = 0;

      } else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again... or type 'back'");
      }
    break; 

    case 4:
      if (userIn == "1"){
        printAnswer("Name plant 1:");
        status = 41; 

      }else if (userIn == "2"){
        printAnswer("Name plant 2:");
        status = 42;

      } else if (userIn == "back"){
        status = 0;
        onMessageChange();

      } else {
        printAnswer("Hmm...try again... or type 'back' ");
      }
    break;
    
    // rename plant 1
    case 41:
      if (userIn == "back" || userIn == p2.getName()){
        printAnswer("invalid name, exiting...type anything to go back to menu");
        status = 0;
        
      } else {
        p1.setName(userIn);
        reply = "Name of plant 1 was changed to : ";
        reply += userIn;
        reply += " ...type anything to go back to menu"; 
        printAnswer(reply);
        status = 0;
        
      }
    break;

    //rename plant 2
    case 42:
      if (userIn == "back" || userIn == p1.getName()){
        printAnswer("invalid name, exiting... type anything to go back to menu");
        status = 0;
        
      } else {
        p2.setName(userIn);
        reply = "Name of plant 2 was changed to : ";
        reply += userIn;
        reply += " ...type anything to go back to menu";
        printAnswer(reply);
        status = 0;
        
      }
    break;

    case 5:
      
      if (userIn == p1.getName()){
        selectedPlant = 1;
        printAnswer("For watering to a specified moisture level type 'moisure' and for setting a fixed time a pump should run type 'time'");
        status = 51; 

      }else if (userIn == p2.getName()){
        selectedPlant = 2;
        printAnswer("For watering to a specified moisture level type 'moisure' and for setting a fixed time a pump should run type 'time' or type back.");
        status = 51;

      } else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again... or type 'back' ");
      }
    break;

    case 51:
      if (userIn == "moisture"){
        printAnswer("The pump should be running for x seconds then wait w seconds and repeat thet until p (in %) moisture is reached.\nSet x,w,p in this format {p}p{x}x{w}w\ne.g : 85p5x300w\n meaning: water for 5s wait 5 min and repeat until 85%");
        status = 511;

      } else if (userIn == "time"){
        printAnswer("How long should the pump run in seconds?");
        status = 512;

      } else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again... or type 'back'");
      }

    break;

    case 511:
      numValues = sscanf(userIn.c_str(), "%dp%dx%dw", &p, &x, &w);
      
      if ( (numValues == 3 ) && (0<p<101) && (x<121) ){
        if (selectedPlant == 1){
          p1.isWateredByT = false;
          p1.setWaterByP(p,x,w);

        } else if (selectedPlant == 2){
          p2.isWateredByT = false;
          p2.setWaterByP(p,x,w);

        } else {
          printAnswer(" error in 511, exiting..type anything to go back to menu");
          status = 0;
          
        }

      } else {
        printAnswer("wrong format could not set volume by moisture, type anything to go back to menu.");
        status = 0;
        
      }

    break;

    case 512:
      
      t = userIn.toInt();

      if (0 < t < 61){ 
        if (selectedPlant == 1) {
          p1.isWateredByT = true;
          p1.setWaterByT(t);
        } else if (selectedPlant == 2) {
          p2.isWateredByT = true;
          p2.setWaterByT(t);
        } else {
          printAnswer("error in 512, type anything to go back to menu...");
          status = 0;
          
        }
      } else {
        printAnswer("wrong format could not set volume by time, type anything to go back to menu...");
        status = 0;
        
      }
    break;
    
    case 6:
      if (userIn == "time"){
        updateTime();
        delay(1000);
        reply = "Time was set to: ";
        reply += String(hour());
        reply += ":" ;
        reply += String(minute());
        reply += " ... type anything to go back tomenu" ;
        printAnswer(reply);
        status = 0;
        

      }else if (userIn == "calibrate"){
        reply = "Do you want to calibrate the sensor of ";
        reply += p1.getName();
        reply += " or ";
        reply += p2.getName();
        reply += " ?";
        printAnswer(reply);
        
        status = 61;

      }else if (userIn == "check"){
        
        if(checkEnoughWater()){
          printAnswer("Yes, there is enough water, for now...type anything to go back to menu...");
        }else{
          printAnswer("NO! there is not enough water... Plants wont be watered! Type anything to go back to menu.");
        }
        status = 0;

      }else if (userIn == "bedtime"){
        printAnswer("Define bedtime in 24h format:\ne.g. : 00:30 - 10:25 would be 0h30m10hh25mm");
        status = 62;

      }else if (userIn == "error"){
        
        if (p1.checkError() || p2.checkError()){

          reply = p1.getErrorMessageAndReset();
          reply += " and ";
          reply += p2.getErrorMessageAndReset();
          reply += "\nerrors were reset, type anytning to go back to menu";
          printAnswer(reply);
          status = 0;

        } else {
          printAnswer("no errors, type anytning to go back to menu");
          status = 0;
        }

      }else if (userIn == "back"){
        status = 0;
        onMessageChange();

      } else {
        printAnswer("Hmm...try again... or type 'back'");
      }
    break;

    case 61:
      if (userIn == p1.getName()){
        selectedPlant = 1;
        printAnswer("Hold the sensor in open air and type 'ok' or 'back' to cancel");
        status = 611;

      }else if (userIn == p2.getName()){
        selectedPlant = 2;
        printAnswer("Hold the sensor in open air and type 'ok' (It takes ~5s to take a reading) or 'back' to cancel");
        status = 611;

      } else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again... or type 'back'");
      }
    break;

    case 611:
      if (userIn == "ok"){
        if (selectedPlant == 1){
          p1.setSensorAir();          

        } else if (selectedPlant == 2){
          p2.setSensorAir();

        } else{
          printAnswer("error in 611, type anything to go back to menu...");
          status = 0;

        }
        printAnswer("Put the sensor into Water and type 'ok' (It takes ~5s to take a reading) or 'back' to cancel");
        status = 6111;

      } else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again... type 'ok' or type 'back'");
      }
    break;

    case 6111:
      if (userIn == "ok"){
        if (selectedPlant == 1){
          p1.setSensorWater();          

        } else if (selectedPlant == 2){
          p2.setSensorWater();

        } else{
          printAnswer("error in 6111, type anything to go back to menu...");
          status = 0;

        }
        printAnswer("Sensor set! ... type anything to go back to menu.");
        status = 0;

      } else if (userIn == "back"){
        status = 0;
        onMessageChange();
      } else {
        printAnswer("Hmm...try again... type 'ok' or type 'back'");
      }
    break;

    case 62:
      numValues = sscanf(userIn.c_str(), "%dh%dm%dhh%dmm", &h, &m, &hh, &mm);
      
      if ((numValues == 4) && (0 <= h < 24) && (0 <= hh < 24) && (0 <= m < 61) && (0 <= mm < 61)){
        bedtimeStartH = h;
        bedtimeStartM = m;
        bedtimeEndH = hh;
        bedtimeEndM = mm;
      } else {
        printAnswer("wrong format could not set bedtime, type anything to go back to menu.");
        status = 0;
        
      }
    break;



     
    // if something went wrong
    default:
      printAnswer("default case status ... something went wrong press anything to go back to menu");
      status = 0;
    break;
  }

  

  
}

std::tuple<int, int, int, int> Menu::getBedtime(){
  return std::make_tuple(bedtimeStartH, bedtimeStartM, bedtimeEndH, bedtimeEndM);
}


