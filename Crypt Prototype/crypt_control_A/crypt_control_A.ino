/*
  Control Script for Crypt Game

  Escape New Haven
  Justin C Kirk
  2020

*/

//library for audio player
//file for lightning
// file for non scary music
// file for scary music


//Button or Switch?
#define RESET_PIN 0

//SPST Switch
#define LIGHT_SWITCH 0

//PIR Sensor
#define MOTION_SENSOR 0

//Reed Switches
#define FILM_REED 0
#define COFFIN_REED 0
#define COFFIN_ROCKER 0

//Hall Effect Sensors
#define TORCH_1_HALL 0
#define TORCH_2_HALL 0
#define TORCH_3_HALL 0
#define TORCH_4_HALL 0
#define TORCH_5_HALL 0
#define CANDLE_1_HALL 0
#define CANDLE_2_HALL 0
#define CANDLE_3_HALL 0
#define SKULL_HALL 0
#define DEMON_1_HALL 0
#define DEMON_2_HALL 0
#define DEMON_3_HALL 0

//Conductive Sensor
#define TOUCH_SENSOR 0

//Maglock
#define CRYPT_DOOR 0

#define TORCH_1_LED 0
#define TORCH_2_LED 0
#define TORCH_3_LED 0
#define TORCH_4_LED 0
#define TORCH_5_LED 0

#define CANDLE_1_LED 0
#define CANDLE_2_LED 0
#define CANDLE_3_LED 0
#define SKULL_LED 0


//Timers
unsigned long sensorCurrent = 0;
unsigned long sensorPrevious = 0;
const int sensorInterval = 10;

int resetVal = 0;

//First Puzzle Vals
boolean lightSwitchOff = false;
boolean filmCabinetClosed = false;
int motionVal = 0;
boolean stoodStill = false;

//Second Puzzle Vals
int torch1Val = 0;
int torch2Val = 0;
int torch3Val = 0;
int torch4Val = 0;
int torch5Val = 0;
int coffinReedVal = 0;
int coffinRockerVal = 0;

unsigned long torchLedCurrent = 0;
unsigned long torchLedPrevious = 0;
const int torchLedInterval = 10;

int torchVals[] = {torch1Val, torch2Val, torch3Val, torch4Val, torch5Val,};
int torchLedBrightness = 0;
const int torchLedBrightMax = 255;


//Third Puzzle Vals

int candleLedBrightness = 0;
int skullLedBrightness = 0;

int candle1Val = 0;
int candle2Val = 0;
int candle3Val = 0;
int skullVal = 0;

int medicineCabinetVals[] = {candle1Val, candle2Val, candle3Val, skullVal};

int demon1Val = 0;
int demon2Val = 0;
int demon3Val = 0;

int demonVals[] = {demon1Val, demon2Val, demon3Val};

//Fourth Puzzle Vals
int touchVal = 0;
boolean playersTouching = false;


//States of game
const int FIRST_PUZZLE = 1;
const int SECOND_PUZZLE = 2;
const int THIRD_PUZZLE = 3;
const int FOURTH_PUZZLE = 4;
const int RESET_PUZZLE = 0;

int cryptState = 0;
boolean updateCrypt = false;

void getCryptState(int i ) {
  updateCrypt = true;
  //do other stuff
  cryptState = i;
}


void setup() {

  Serial.begin(115200);

  //Setup output pins

  //Setup input pins

  //setup audio player
  
  //set light vals
  //reset all sensor vals

  delay(5000);

  //Set state to FIRST_PUZZLE
  getCryptState(FIRST_PUZZLE);

}


void loop() {

  //set timers

  switch (cryptState) {

    case FIRST_PUZZLE:

      if (updateCrypt == true) {
        //update sensors
        //update lights
        updateCrypt = false;
      }

      //play non scary music

      //poll reset
      //if reset val is true, get reset puzzle state

      //poll light switch sensor
      //poll pir sensor
      //poll film cabinet sensor

      if (lightSwitchOff == true && stoodStill == true && filmCabinetOpen == true) {
        //fade non scary music
        //flash leds
        //play lightning crash
        //play scary music
        //open door
        //reset leds
        //reset sensor states
        //get second puzzle state
        getCryptState(SECOND_PUZZLE);
      }
      break;


    //May break this up into two parts, SECOND_PUZZLE_A and SECOND_PUZZLE_B
    //SECOND_PUZZLE_A for torches
    //SECOND_PUZZLE_B for coffin

    case SECOND_PUZZLE:

      if (updateCrypt == true) {
        //update sensors
        //update lights
        //knock count = 0
        updateCrypt = false;
      }

      //loop scary music

      //poll reset
      //if reset val is true, get reset puzzle state

      //poll torch sensors

      //all 5 torches need to be placed
      if (torch1Val == 1 ) {

        //while knockCount <= 1
        //knock coffin
        //knockCount +=1

        //unlock coffin
        //pulse torch leds
        //turn on coffin UV but dimmed

      }

      //poll coffin sensor
      //if coffin door opens once then closed:
      //coffin UV full brightness
      //then:
      getCryptState(THIRD_PUZZLE);

      break;

    case THIRD_PUZZLE:
      if (updateCrypt == true) {
        //update sensors
        //update lights
        updateCrypt = false;
      }

      //loop scary music

      //poll reset
      //if reset val is true, get reset puzzle state

      //poll candle sensors
      //poll skull sensor
      //poll demon positions

      //if candle/skull condition met, open box for demons?
      //if demon condition met, mirror led ON
      //get fourth puzzle state

      getCryptState(FOURTH_PUZZLE);
      break;

    case FOURTH_PUZZLE:
      if (updateCrypt == true) {
        //update sensors
        //play scary music
        updateCrypt = false;
      }

      //poll reset
      //if reset val is true, get reset puzzle state

      //loop scary music
      //poll touch sensors

      if (playersTouching == true) {
        //Open final door
        //play winner audio
        getCryptState(RESET_PUZZLE);
      }

      break;

    case RESET_PUZZLE:
      if (updateCrypt == true) {
        //reset all sensor vals
        //turn off all leds
        //turn on room lights
        //open all maglocks
        delay(5000);
        //close all mag locks
        //stop all music
        updateCrypt = false;
      }

      //get first puzzle state
      getCryptState(FIRST_PUZZLE);

      break;
  }





}
