
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

const int buz_pin = 7 ;
const int motion_pin = 9 ;

//LOW level trigger relays
const int lamp_pin = 3 ;
const int pump_pin = 5;

unsigned long current_millis = 0;
unsigned long pump_trigger = 0;
unsigned long buz_trigger_1 = 0;
unsigned long buz_trigger_2 = 0;
unsigned long lamp_wait_trigger = 0;
unsigned long lamp_clean_trigger = 0;

// 5 min 
const long pump_interval = 5000; //300000

// 5d 
const long buz_interval_1 = 600000; // 432000000

// 7h 
const long buz_interval_2 = 120000; //25200000

// 1h 
const long lamp_wait_interval = 30000; //3600000

// 10 min
const long lamp_clean_interval = 10000 ; //600000

int pump_state = 0;
int motion_state = 0;
int lamp_state = 0;
int lamp_waited = 0;
int buz_1_triggered = 0;


void setup() {
  
  pinMode(motion_pin, INPUT);
  pinMode(lamp_pin, OUTPUT);
  pinMode(pump_pin, OUTPUT);

  // turn off relays
  digitalWrite(lamp_pin, HIGH);
  delay(1000);
  digitalWrite(pump_pin, HIGH);
  

  current_millis = millis();

  lamp_wait_trigger = current_millis + lamp_wait_interval;
  buz_trigger_1 = buz_interval_1;
  buz_trigger_2 = buz_interval_1 + buz_interval_1;

  delay(5000);

  
  
}

void loop(){
  // get time
  current_millis = millis();

  motion_state = digitalRead(motion_pin);

  

  if (motion_state == 1){
    //testing  
    tone(buz_pin, 100, 200);
    
    pump_trigger = current_millis + pump_interval;

    if (lamp_state == 1){
      lamp_set_state(0);
    }

    if (pump_state == 0){
      pump_set_state(1);
    }
  }

  if (pump_trigger < current_millis && pump_state == 1 && lamp_state == 0) {
    
    // turn off the pump
    pump_set_state(0);

  }

  if (lamp_wait_trigger < current_millis){
    lamp_waited = 1;
  }

  // start clean
  if (pump_state == 0 && lamp_waited == 1 && lamp_state == 0){
    
    lamp_clean_trigger = current_millis + lamp_clean_interval;
    pump_set_state(1);
    lamp_set_state(1);
  }

  // end uninterrupted clean
  if (lamp_clean_trigger < current_millis && lamp_state == 1){
    
    lamp_set_state(0);
    pump_set_state(1);
    lamp_waited = 0;
    lamp_wait_trigger = current_millis + lamp_wait_interval;
  }

  if (buz_trigger_1 < current_millis && buz_1_triggered == 0){
    buz_1_triggered = 1;

    buz_trigger_2 = current_millis + buz_interval_2;
    play_melody(buz_pin);
  }

  if (buz_1_triggered == 1 && buz_trigger_2 < current_millis){
    buz_trigger_2 = current_millis + buz_interval_2;
    play_melody(buz_pin);
  }

}

void pump_set_state(int state_in){
  
  if (state_in == 1){
    //pump on
    digitalWrite(pump_pin, LOW);
    pump_state = 1;
  } else {
    //lamp off
    digitalWrite(pump_pin, HIGH);
    pump_state = 0;
  }

}

void lamp_set_state(int state_in){
  if (state_in == 1){
    //lamp on
    digitalWrite(lamp_pin, LOW);
    lamp_state = 1;
  } else {
    //lamp off
    digitalWrite(lamp_pin, HIGH);
    lamp_state = 0;
  }
}

void play_melody(int buzzer_pin){
  
  int tempo = 200;
  int buzzer = buzzer_pin;
  int melody[] = {
  
  
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
  NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  
  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,


};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);

}
}