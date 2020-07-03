/*

  Crypt Game Prototype

  Escape New Haven
  Justin C Kirk
  2020

*/

//Set to 0 to stop printing
#define DEBUG 1

//Bounce library is used to handle inputs
//It has a few helper functions and features including handling debouncing
//https://github.com/thomasfredericks/Bounce2

#include <Bounce2.h>


// Audio Setup
//Libraries for audio player
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// SPI pins
// Make sure to use hardware SPI pins.
// See http://arduino.cc/en/Reference/SPI

const int BOARD_CLK = 52;
const int BOARD_MISO = 50;
const int BOARD_MOSI = 51;

//Board Pins and SD Card Pins
const int BOARD_RESET = 49;
const int BOARD_CS = 48;
const int BOARD_XDCS = 47;
const int BOARD_SDCS = 46;

//Use a hardware inerrupt pin for DREQ
//This allows two tracks to be played at the same time
// See https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
const int BOARD_DREQ = 21;

//Create audio player object
Adafruit_VS1053_FilePlayer audioPlayer = Adafruit_VS1053_FilePlayer(BOARD_RESET, BOARD_CS, BOARD_XDCS, BOARD_DREQ, BOARD_SDCS);

//cue a song to play
int playFileFlag = -1;
//Track names are limited to 8 characters
char* trackNames[] = {"test.mp3", "play.mp3", "thunder.mp3", "scary.mp3", "beast.mp3", "END.mp3"};
int trackIndex = -1;



//Inputs

//Puzzle 1
const int LIGHT_BAR_SWITCH_PIN = 14;
Bounce lightBarSwitch = Bounce();

const int MOTION_SENSOR_PIN = 15;
Bounce motionSensor = Bounce();

const int FILM_CABINET_REED_PIN = 39;
Bounce filmCabinetReed = Bounce();

//Puzzle 2
const int TORCH_1_REED_PIN = 37;
Bounce torch1Reed = Bounce();
const int TORCH_2_REED_PIN = 36;
Bounce torch2Reed = Bounce();
const int TORCH_3_REED_PIN = 35;
Bounce torch3Reed = Bounce();
const int TORCH_4_REED_PIN = 34;
Bounce torch4Reed = Bounce();
const int TORCH_5_REED_PIN = 33;
Bounce torch5Reed = Bounce();
const int COFFIN_REED_PIN = 32;
Bounce coffinReed = Bounce();

//Puzzle 3
const int CANDLE_REED_PIN  = 31;
Bounce candleReed = Bounce();
const int SKULL_REED_PIN  = 30;
Bounce skullReed = Bounce();
const int DEMON_1_REED_PIN  = 29;
Bounce demon1Reed = Bounce();
const int DEMON_2_REED_PIN  = 28;
Bounce demon2Reed = Bounce();
const int DEMON_3_REED_PIN  = 27;
Bounce demon3Reed = Bounce();

//Puzzle 4
const int TOUCH_SENSOR_PIN = 0;  //analog 0

//General
const int PUZZLE_1_OVERRIDE_PIN = 26;
Bounce puzzle1Override = Bounce();
const int PUZZLE_2_OVERRIDE_PIN = 25;
Bounce puzzle2Override = Bounce();
const int PUZZLE_3_OVERRIDE_PIN = 24;
Bounce puzzle3Override = Bounce();
const int PUZZLE_4_OVERRIDE_PIN = 23;
Bounce puzzle4Override = Bounce();

const int RESET_SWITCH_PIN = 22;
Bounce resetSwitch = Bounce();


//Outputs

//Puzzle 1
const int LIGHT_BAR_LED = 12;
const int LIGHTNING_LED = 11;
const int CRYPT_DOOR_MAGLOCK = 4;

//Puzzle 2
const int TORCH_LED = 10;
const int COFFIN_UVLED = 9;

//Puzzle 3
const int CANDLE_LED = 8;
const int SKULL_LED = 7;
const int MIRROR_LED = 6;

//Puzzle 4
//no outputs


//Game Variables

//Puzzle 1
//const int STAND_STILL_TIME = 10000;
int standStillTime = 0;

boolean bLightsOff = false;
boolean bStandingStill = false;
boolean bFilmCabinetOpen = false;
boolean bPuzzle1Complete = false;
boolean bLightningFlash  = false;


//Puzzle 2
boolean bTorch1Placed = false;
boolean bTorch2Placed = false;
boolean bTorch3Placed = false;
boolean bTorch4Placed = false;
boolean bTorch5Placed = false;
boolean bCoffinOpen = false;
boolean bCoffinOpenFirstTime = false;

unsigned long torchCurrent = 0;
unsigned long torchPrevious = 0;
const long TORCH_INTERVAL = 100;
int torchIncrement = 5;
int torchLEDVal = 0;

boolean bTorchesPlaced = false;
boolean bPuzzle2Complete = false;

//Puzzle 3
boolean bCandlePlaced = false;
boolean bSkullPlaced = false;
boolean bDemon1Placed = false;
boolean bDemon2Placed = false;
boolean bDemon3Placed = false;
boolean bPuzzle3Complete = false;

//Puzzle 4
//Will need to adjust vakue for use with touch sensor
const int TOUCH_THRESHOLD = 500;
boolean bIsTouching = false;
boolean bPuzzle4Complete = false;


//General

//Time in millis button needs to be held to trigger override
const int PUZZLE_OVERRIDE_INTERVAL = 10000;
//Time in millis button needs to be held to trigger reset
const int RESET_SWITCH_INTERVAL = 10000;

unsigned long cryptCurrent = 0;
unsigned long cryptPrevious = 0;
const long CRYPT_INTERVAL = 100;

int cryptState = -1;
boolean bUpdateCrypt = false;
int puzzle1State = -1;
int puzzle3State = -1;

//Game States
//Puzzle 1 and 3 need sub states to handle the different audio tracks being played
const int PUZZLE_1 = 1;
const int PUZZLE_1_A = 10;
const int PUZZLE_1_B = 11;
const int PUZZLE_2 = 2;
const int PUZZLE_3 = 3;
const int PUZZLE_3_A = 30;
const int PUZZLE_3_B = 31;
const int PUZZLE_4 = 4;
const int RESET_STATE = 0;
const int WINNER_STATE = 5;

void getCryptState(const int i) {

  bUpdateCrypt = true;
  Serial.print(F("Getting State: "));
  Serial.println(i);
  cryptState = i;

}

void setup() {

  Serial.begin(115200);

  //Setup Inputs
  //attach() creates the Bounce object and sets up the pin as an input
  //interval() sets the debounce time in millis

  //Puzzle 1
  lightBarSwitch.attach(LIGHT_BAR_SWITCH_PIN, INPUT_PULLUP);
  lightBarSwitch.interval(25);
  motionSensor.attach(MOTION_SENSOR_PIN, INPUT_PULLUP);
  motionSensor.interval(25);
  filmCabinetReed.attach(FILM_CABINET_REED_PIN, INPUT_PULLUP);
  filmCabinetReed.interval(25);

  //Puzzle 2
  torch1Reed.attach(TORCH_1_REED_PIN, INPUT_PULLUP);
  torch1Reed.interval(25);
  torch2Reed.attach(TORCH_2_REED_PIN, INPUT_PULLUP);
  torch2Reed.interval(25);
  torch3Reed.attach(TORCH_3_REED_PIN, INPUT_PULLUP);
  torch3Reed.interval(25);
  torch4Reed.attach(TORCH_4_REED_PIN, INPUT_PULLUP);
  torch4Reed.interval(25);
  torch5Reed.attach(TORCH_5_REED_PIN, INPUT_PULLUP);
  torch5Reed.interval(25);
  coffinReed.attach(COFFIN_REED_PIN, INPUT_PULLUP);
  coffinReed.interval(25);

  //Puzzle 3
  candleReed.attach(CANDLE_REED_PIN, INPUT_PULLUP);
  candleReed.interval(25);
  skullReed.attach(SKULL_REED_PIN, INPUT_PULLUP);
  skullReed.interval(25);
  demon1Reed.attach(DEMON_1_REED_PIN, INPUT_PULLUP);
  demon1Reed.interval(25);
  demon2Reed.attach(DEMON_2_REED_PIN, INPUT_PULLUP);
  demon2Reed.interval(25);
  demon3Reed.attach(DEMON_3_REED_PIN, INPUT_PULLUP);
  demon3Reed.interval(25);

  //General
  puzzle1Override.attach(PUZZLE_1_OVERRIDE_PIN, INPUT_PULLUP);
  puzzle1Override.interval(25);
  puzzle2Override.attach(PUZZLE_2_OVERRIDE_PIN, INPUT_PULLUP);
  puzzle2Override.interval(25);
  puzzle3Override.attach(PUZZLE_3_OVERRIDE_PIN, INPUT_PULLUP);
  puzzle3Override.interval(25);
  puzzle4Override.attach(PUZZLE_4_OVERRIDE_PIN, INPUT_PULLUP);
  puzzle4Override.interval(25);
  resetSwitch.attach(RESET_SWITCH_PIN);
  resetSwitch.interval(25);

  //Setup Outputs
  //For Arduino Mega, pins 2-13 are all PWM enabled
  pinMode(LIGHT_BAR_LED, OUTPUT);
  pinMode(LIGHTNING_LED, OUTPUT);
  pinMode(CRYPT_DOOR_MAGLOCK, OUTPUT);
  pinMode(TORCH_LED, OUTPUT);
  pinMode(COFFIN_UVLED, OUTPUT);
  pinMode(CANDLE_LED, OUTPUT);
  pinMode(SKULL_LED, OUTPUT);
  pinMode(MIRROR_LED, OUTPUT);
  delay(1000);

  audioPlayer.begin();
  //Enable the player to use a hardware interrupt pin
  audioPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
  SD.begin(BOARD_SDCS);    // initialise the SD card

  if (!audioPlayer.begin()) {
    Serial.println(F("Cannot find audio player.  Check pins."));
  }

  if (!SD.begin(BOARD_SDCS)) {
    Serial.println(F("SD read error.  Is card inserted?"));
    //Turn on SD card error led?
  }

  //File names are restricted to 8 characters
  //List SD Card Contents
  printDirectory(SD.open("/"), 0);
  audioPlayer.setVolume(0, 0);
  //Play test file to confirm audio player is working
  trackIndex = 0;
  audioPlayer.playFullFile(trackNames[trackIndex]);
  //audioPlayer.playFullFile("test.mp3");

  if (audioPlayer.stopped()) {
    getCryptState(RESET_STATE);
  }

}


void checkResetSwitch(int holdDuration) {

  resetSwitch.update();

  if (resetSwitch.read() == 1) {
    if (resetSwitch.duration() > holdDuration) {
      audioPlayer.stopPlaying();
      getCryptState(RESET_STATE);
    }
  }

}
void checkPuzzleOverrides(int holdDuration) {

  puzzle1Override.update();
  puzzle2Override.update();
  puzzle3Override.update();
  puzzle4Override.update();

  if (puzzle1Override.read() == 1) {
    if (DEBUG) {
      Serial.print("Puzzle 1 Override: ");
      Serial.println(puzzle1Override.duration());
    }
    if (puzzle1Override.duration() > holdDuration) {
      bPuzzle1Complete = true;
    }
  }

  if (puzzle2Override.read() == 1) {
    if (DEBUG) {
      Serial.print("Puzzle 2 Override: ");
      Serial.println(puzzle2Override.duration());
    }
    if (puzzle2Override.duration() > holdDuration) {
      bPuzzle2Complete = true;
    }
  }

  if (puzzle3Override.read() == 1) {
    if (DEBUG) {
      Serial.print("Puzzle 3 Override: ");
      Serial.println(puzzle3Override.duration());
    }
    if (puzzle3Override.duration() > holdDuration) {
      bPuzzle3Complete = true;
    }
  }

  if (puzzle4Override.read() == 1) {
    if (DEBUG) {
      Serial.print("Puzzle 4 Override: ");
      Serial.println(puzzle4Override.duration());
    }
    if (puzzle4Override.duration() > holdDuration) {
      bPuzzle4Complete = true;
    }
  }


}

void loop() {

  //    MAIN GAME

  audioPlayer.feedBuffer();

  switch (cryptState) {

    case RESET_STATE:

      if (bUpdateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("RESET STATE"));
        }

        //Reset Puzzle 1
        analogWrite(LIGHTNING_LED, 0);
        bPuzzle1Complete = false;
        puzzle1State = -1;
        bLightsOff = false;
        bStandingStill = false;
        bFilmCabinetOpen = false;

        //Reset Puzzle 2
        analogWrite(TORCH_LED, 0);
        analogWrite(COFFIN_UVLED, 0);
        bPuzzle2Complete = false;
        bTorch1Placed = false;
        bTorch2Placed = false;
        bTorch3Placed = false;
        bTorch4Placed = false;
        bTorch5Placed = false;
        bCoffinOpen = false;

        //Reset Puzzle 3
        analogWrite(CANDLE_LED, 0);
        analogWrite(SKULL_LED, 0);
        analogWrite(MIRROR_LED, 0);
        bPuzzle3Complete = false;
        puzzle3State = -1;
        bCandlePlaced = false;
        bSkullPlaced = false;
        bDemon1Placed = false;
        bDemon2Placed = false;
        bDemon3Placed = false;

        //Reset Puzzle 4
        bPuzzle4Complete = false;

        //open all MagLocks
        Serial.println(F("Open all maglocks"));
        digitalWrite(CRYPT_DOOR_MAGLOCK, LOW);
        delay(5000);
        //close all mag locks
        Serial.println(F("Close all maglocks"));
        digitalWrite(CRYPT_DOOR_MAGLOCK, HIGH);

        delay(1000);

        bUpdateCrypt = false;
      }



      //Test functions
      // checkResetSwitch(2000);
      // checkPuzzleOverrides(2000);

      //inputTest_puzzle1();
      //inputTest_puzzle2();
      // inputTest_puzzle3();
      inputTest_puzzle4();

      // ledTest(255);

      //Start game
      getCryptState(PUZZLE_1);

      break;

    case PUZZLE_1:

      if (bUpdateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Puzzle 1"));
        }
        digitalWrite(LIGHT_BAR_LED, LOW);
        digitalWrite(LIGHTNING_LED, LOW);
        bPuzzle1Complete = false;
        bLightsOff = false;
        bStandingStill = false;
        bFilmCabinetOpen = false;
        trackIndex = 1;
        puzzle1State = PUZZLE_1_A;
        playFileFlag = 1;
        bUpdateCrypt = false;
      }

      switch (puzzle1State) {

        case PUZZLE_1_A:

          if (playFileFlag == 1 && bPuzzle1Complete == false) {
            audioPlayer.startPlayingFile(trackNames[trackIndex]);
            playFileFlag -= 1;
          }

          if (audioPlayer.stopped() && bPuzzle1Complete == false) {
            playFileFlag = 1;
          }

          if (audioPlayer.playingMusic) {

            checkResetSwitch(RESET_SWITCH_INTERVAL);
            checkPuzzleOverrides(PUZZLE_OVERRIDE_INTERVAL);

            lightBarSwitch.update();
            filmCabinetReed.update();
            motionSensor.update();

            if (lightBarSwitch.read() == 1) {
              bLightsOff = true;
              analogWrite(LIGHT_BAR_LED, 0);
            } else if (lightBarSwitch.read() == 0) {
              bLightsOff = false;
              analogWrite(LIGHT_BAR_LED, 200);
            }

            if (filmCabinetReed.read() == 1) {
              bFilmCabinetOpen = true;
            }

            if (motionSensor.read() == 0) {
              bStandingStill = true;
            } else if (motionSensor.read() == 1) {
              bStandingStill = false;
            }

            int puzzle1Sum = bLightsOff + bStandingStill + bFilmCabinetOpen;

            cryptCurrent = millis();

            if (cryptCurrent - cryptPrevious > CRYPT_INTERVAL) {
              cryptPrevious = cryptCurrent;

              if (puzzle1Sum < 3) {
                standStillTime = 0;
                audioPlayer.setVolume(0, 0);
                if (DEBUG) {
                  Serial.print(F("Lights Off: "));
                  Serial.print(bLightsOff);
                  Serial.print(F(" "));
                  Serial.print(F("Standing Still: "));
                  Serial.print(bStandingStill);
                  Serial.print(F(" "));
                  Serial.print(F("Film Cabinet Open: "));
                  Serial.print(bFilmCabinetOpen);
                  Serial.print(F(" "));
                  Serial.print(F("Sum: "));
                  Serial.println(puzzle1Sum);
                }
              } else if (puzzle1Sum == 3) {

                standStillTime += 1;

                if (standStillTime >= 100) {
                  bPuzzle1Complete = true;
                  audioPlayer.stopPlaying();
                  Serial.println(F("Puzzle 1 complete"));

                } else if (standStillTime < 100) {
                  audioPlayer.setVolume(standStillTime, standStillTime);
                }

                if (DEBUG) {
                  Serial.print(F("Standing Stil For : "));
                  Serial.println(standStillTime);
                }
              }

              if (bPuzzle1Complete == true) {
                Serial.println(F("Puzzle 1 Complete"));
                audioPlayer.stopPlaying();
                playFileFlag = 1;
                puzzle1State = PUZZLE_1_B;
              }

            }//end timer loop
          }//end music playing loop
          break;

        //Thunder music state
        case PUZZLE_1_B:

          trackIndex = 2;
          audioPlayer.setVolume(0, 0);

          if (playFileFlag == 1) {
            audioPlayer.startPlayingFile(trackNames[trackIndex]);
            playFileFlag -= 1;
          }

          if (audioPlayer.playingMusic) {

            checkResetSwitch(RESET_SWITCH_INTERVAL);
            checkPuzzleOverrides(PUZZLE_OVERRIDE_INTERVAL);

            //Switches every CRYPT_INTERVAL
            bLightningFlash = !bLightningFlash;

            if (bLightningFlash == true) {
              analogWrite(LIGHTNING_LED, 200);
            } else if (bLightningFlash == false) {
              analogWrite(LIGHTNING_LED, 0);
            }

          }

          if (audioPlayer.stopped()) {
            //Open crypt door
            digitalWrite(CRYPT_DOOR_MAGLOCK, HIGH);
            getCryptState(PUZZLE_2);
          }
          break;
      }

      break;

    case PUZZLE_2:

      if (bUpdateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Puzzle 2"));
        }
        bPuzzle2Complete = false;
        analogWrite(TORCH_LED, 0);
        analogWrite(COFFIN_UVLED, 0);
        bTorch1Placed = false;
        bTorch2Placed = false;
        bTorch3Placed = false;
        bTorch4Placed = false;
        bTorch5Placed = false;
        bCoffinOpen = false;
        bCoffinOpenFirstTime = false;
        trackIndex = 3;
        bUpdateCrypt = false;
      }

      audioPlayer.setVolume(0, 0);

      if (playFileFlag == 1) {
        audioPlayer.startPlayingFile(trackNames[trackIndex]);
        playFileFlag -= 1;
      }
      if (audioPlayer.stopped()) {
        playFileFlag = 1;
      }

      while (audioPlayer.playingMusic) {


        checkResetSwitch(RESET_SWITCH_INTERVAL);
        checkPuzzleOverrides(PUZZLE_OVERRIDE_INTERVAL);

        //Keep control of lights active
        lightBarSwitch.update();
        if (lightBarSwitch.read() == 1) {
          analogWrite(LIGHT_BAR_LED, 0);
        } else if (lightBarSwitch.read() == 0) {
          analogWrite(LIGHT_BAR_LED, 200);
        }

        //Check coffin.  If opened once, active LED control
        coffinReed.update();
        if (coffinReed.read() == 1) {
          bCoffinOpen = true;
        }

        if (bCoffinOpen == true && coffinReed.read() == 0) {
          analogWrite(COFFIN_UVLED, 255);
        } else {
          analogWrite(COFFIN_UVLED, 0);
        }

        //Check torches
        torch1Reed.update();
        torch2Reed.update();
        torch3Reed.update();
        torch4Reed.update();
        torch5Reed.update();

        //Current setup is for use with buttons
        //Change the 1 to a 0 when using a reed switch
        if (torch1Reed.read() == 1) {
          bTorch1Placed = true;
        } else {
          bTorch1Placed = false;
        }

        if (torch2Reed.read() == 1) {
          bTorch2Placed = true;
        } else {
          bTorch2Placed = false;
        }

        if (torch3Reed.read() == 1) {
          bTorch3Placed = true;
        } else {
          bTorch3Placed = false;
        }

        if (torch4Reed.read() == 1) {
          bTorch4Placed = true;
        } else {
          bTorch4Placed = false;
        }

        if (torch5Reed.read() == 1) {
          bTorch5Placed = true;
        } else {
          bTorch5Placed = false;
        }

        int torchValSum = bTorch1Placed + bTorch2Placed + bTorch3Placed + bTorch4Placed + bTorch5Placed;

        cryptCurrent = millis();
        if (cryptCurrent - cryptPrevious >= CRYPT_INTERVAL) {

          if (torchValSum < 5) {
            if (DEBUG) {
              Serial.print(F("T1: "));
              Serial.print(bTorch1Placed);
              Serial.print(F(" "));
              Serial.print(F("T2: "));
              Serial.print(bTorch2Placed);
              Serial.print(F(" "));
              Serial.print(F("T3: "));
              Serial.print(bTorch3Placed);
              Serial.print(F(" "));
              Serial.print(F("T4: "));
              Serial.print(bTorch4Placed);
              Serial.print(F(" "));
              Serial.print(F("T5: "));
              Serial.print(bTorch5Placed);
              Serial.print(F(" "));
              Serial.print(F("T Sum: "));
              Serial.println(torchValSum);
            }

            analogWrite(TORCH_LED, 0);

          } else if (torchValSum == 5) {

            if (DEBUG) {
              Serial.println(F("Torches Placed"));
            }

            torchLEDVal += torchIncrement;
            if (torchLEDVal >= 255 || torchLEDVal <= 0) {
              torchIncrement = -torchIncrement;
            }
            analogWrite(TORCH_LED, torchLEDVal);

            bPuzzle2Complete = true;
          }

          if (bPuzzle2Complete == true) {
            audioPlayer.stopPlaying();
            getCryptState(PUZZLE_3);
          }
          cryptPrevious = cryptCurrent;
        }
      }

      break;

    case PUZZLE_3:

      if (bUpdateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Puzzle 3"));
        }

        analogWrite(CANDLE_LED, 0);
        analogWrite(SKULL_LED, 0);
        analogWrite(MIRROR_LED, 0);
        bPuzzle3Complete = false;
        bCandlePlaced = false;
        bSkullPlaced = false;
        bDemon1Placed = false;
        bDemon2Placed = false;
        bDemon3Placed = false;
        puzzle3State = PUZZLE_3_A;
        trackIndex = 3;
        bUpdateCrypt = false;
      }


      switch (puzzle3State) {

        case PUZZLE_3_A:

          audioPlayer.setVolume(0, 0);
          if (playFileFlag == 1) {
            audioPlayer.startPlayingFile(trackNames[trackIndex]);
            playFileFlag -= 1;
          }
          if (audioPlayer.stopped() && bPuzzle3Complete == false) {
            playFileFlag = 1;
          }

          while (audioPlayer.playingMusic) {

            checkResetSwitch(RESET_SWITCH_INTERVAL);
            checkPuzzleOverrides(PUZZLE_OVERRIDE_INTERVAL);


            //Keep lightbar switch active
            lightBarSwitch.update();
            if (lightBarSwitch.read() == 1) {
              analogWrite(LIGHT_BAR_LED, 0);
            } else if (lightBarSwitch.read() == 0) {
              analogWrite(LIGHT_BAR_LED, 200);
            }

            //Keep coffin elements going
            coffinReed.update();
            if (bCoffinOpen == true && coffinReed.read() == 0) {
              analogWrite(COFFIN_UVLED, 255);
            } else {
              analogWrite(COFFIN_UVLED, 0);
            }

            //Puzzle 3 elements
            candleReed.update();
            skullReed.update();
            demon1Reed.update();
            demon2Reed.update();
            demon3Reed.update();

            //Make sure to adjust value for reed switches
            if (candleReed.read() == 1) {
              bCandlePlaced = true;
              analogWrite(CANDLE_LED, 255);
            } else {
              bCandlePlaced = false;
              analogWrite(CANDLE_LED, 0);
            }

            if (skullReed.read() == 1) {
              bSkullPlaced = true;
              analogWrite(SKULL_LED, 255);
            } else {
              bSkullPlaced = false;
              analogWrite(SKULL_LED, 0);
            }

            if (demon1Reed.read() == 1) {
              bDemon1Placed = true;
            } else {
              bDemon1Placed = false;
            }

            if (demon2Reed.read() == 1) {
              bDemon2Placed = true;
            } else {
              bDemon2Placed = false;
            }

            if (demon3Reed.read() == 1) {
              bDemon3Placed = true;
            } else {
              bDemon3Placed = false;
            }

            cryptCurrent = millis();
            if (cryptCurrent - cryptPrevious >= CRYPT_INTERVAL) {

              //Keep pulsing the torches
              torchLEDVal += torchIncrement;
              if (torchLEDVal >= 255 || torchLEDVal <= 0) {
                torchIncrement = -torchIncrement;
              }
              analogWrite(TORCH_LED, torchLEDVal);


              int puzzle3Sum = bCandlePlaced + bSkullPlaced + bDemon1Placed + bDemon2Placed + bDemon3Placed;

              if (puzzle3Sum < 5) {

                if (DEBUG) {
                  Serial.print(F("Candle: "));
                  Serial.print(bCandlePlaced);
                  Serial.print(F(" "));
                  Serial.print(F("Skull: "));
                  Serial.print(bSkullPlaced);
                  Serial.print(F(" "));
                  Serial.print(F("Demon 1: "));
                  Serial.print(bDemon1Placed);
                  Serial.print(F(" "));
                  Serial.print(F("Demon 2: "));
                  Serial.print(bDemon2Placed);
                  Serial.print(F(" "));
                  Serial.print(F("Demon 3: "));
                  Serial.print(bDemon3Placed);
                  Serial.print(F(" "));
                  Serial.print(F("Puzzle 3 Sum: "));
                  Serial.println(puzzle3Sum);
                }

                analogWrite(MIRROR_LED, 0);

              } else if (puzzle3Sum == 5) {
                bPuzzle3Complete = true;
              }

              if (bPuzzle3Complete == true) {
                if (DEBUG) {
                  Serial.println(F("!!!!!THE BEAST!!!!!"));
                }
                analogWrite(MIRROR_LED, 255);
                audioPlayer.stopPlaying();
                playFileFlag = 1;
                puzzle3State = PUZZLE_3_B;
              }
              cryptPrevious = cryptCurrent;
            }
          }

          break;

        //Beast music state
        case PUZZLE_3_B:
          trackIndex = 4;
          audioPlayer.setVolume(0, 0);

          if (playFileFlag == 1) {
            audioPlayer.startPlayingFile(trackNames[trackIndex]);
            playFileFlag -= 1;
          }

          if (audioPlayer.playingMusic) {
            checkResetSwitch(RESET_SWITCH_INTERVAL);
            checkPuzzleOverrides(PUZZLE_OVERRIDE_INTERVAL);
            analogWrite(LIGHT_BAR_LED, 0);
            analogWrite(MIRROR_LED, 200);
          }
          if (audioPlayer.stopped()) {
            getCryptState(PUZZLE_4);
          }
          break;
      }
      break;

    case PUZZLE_4:
      if (bUpdateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Puzzle 4"));
        }
        trackIndex = 3;//play scary music?
        bPuzzle4Complete = false;
        bUpdateCrypt = false;
      }

      audioPlayer.setVolume(0, 0);
      if (playFileFlag == 1) {
        audioPlayer.startPlayingFile(trackNames[trackIndex]);
        playFileFlag -= 1;
      }
      if (audioPlayer.stopped()) {
        playFileFlag = 1;
      }

      while (audioPlayer.playingMusic) {

        checkResetSwitch(RESET_SWITCH_INTERVAL);
        checkPuzzleOverrides(PUZZLE_OVERRIDE_INTERVAL);

        //Keep coffin elements going
        coffinReed.update();
        if (bCoffinOpen == true && coffinReed.read() == 0) {
          analogWrite(COFFIN_UVLED, 255);
        } else {
          analogWrite(COFFIN_UVLED, 0);
        }


        cryptCurrent = millis();
        if (cryptCurrent - cryptPrevious >= CRYPT_INTERVAL) {

          //Keep pulsing the torches
          torchLEDVal += torchIncrement;
          if (torchLEDVal >= 255 || torchLEDVal <= 0) {
            torchIncrement = -torchIncrement;
          }
          analogWrite(TORCH_LED, torchLEDVal);

          int touchVal = analogRead(TOUCH_SENSOR_PIN);
          if (touchVal < TOUCH_THRESHOLD) {
            if (DEBUG) {
              Serial.print(F("Touch Val: "));
              Serial.println(touchVal);
            }
          } else if (touchVal > TOUCH_THRESHOLD) {
            bPuzzle4Complete = true;
          }

          if (bPuzzle4Complete == true) {
            if (DEBUG ) {
              Serial.println(F("ESCAPED THE GAME!!"));
            }
            delay(2000);
            playFileFlag = 1;
            audioPlayer.stopPlaying();
            getCryptState(WINNER_STATE);
          }
          cryptPrevious = cryptCurrent;
        }

      }

      break;

    case WINNER_STATE:

      if (bUpdateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Winner State"));
          Serial.println(F("Congrats!"));
        }
        trackIndex = 5;//play end music
        bPuzzle4Complete = false;
        bUpdateCrypt = false;
      }


      audioPlayer.setVolume(0, 0);
      if (playFileFlag == 1) {
        audioPlayer.startPlayingFile(trackNames[trackIndex]);
        playFileFlag -= 1;
      }

      if (audioPlayer.stopped()) {
        getCryptState(RESET_STATE);
      }

      break;

  }

}



void ledTest(int val) {
  analogWrite(LIGHT_BAR_LED, val);
  analogWrite(LIGHTNING_LED, val);
  analogWrite(TORCH_LED, val);
  analogWrite(COFFIN_UVLED, val);
  analogWrite(CANDLE_LED, val);
  analogWrite(SKULL_LED, val);
  analogWrite(MIRROR_LED, val);
}

void inputTest_puzzle1() {

  lightBarSwitch.update();
  filmCabinetReed.update();
  motionSensor.update();

  Serial.print("LightBar: ");
  Serial.print(lightBarSwitch.read());
  Serial.print("Film Cab Reed: ");
  Serial.print(filmCabinetReed.read());
  Serial.print("Motion: ");
  Serial.println(motionSensor.read());

}
void inputTest_puzzle2() {

  torch1Reed.update();
  torch2Reed.update();
  torch3Reed.update();
  torch4Reed.update();
  torch5Reed.update();
  coffinReed.update();

  Serial.print("Torch 1: ");
  Serial.print(torch1Reed.read());
  Serial.print("Torch 2: ");
  Serial.print(torch2Reed.read());
  Serial.print("Torch 3: ");
  Serial.print(torch3Reed.read());
  Serial.print("Torch 4: ");
  Serial.print(torch4Reed.read());
  Serial.print("Torch 5: ");
  Serial.print(torch5Reed.read());
  Serial.print("Coffin: ");
  Serial.println(coffinReed.read());

}
void inputTest_puzzle3() {


  candleReed.update();
  skullReed.update();
  demon1Reed.update();
  demon2Reed.update();
  demon3Reed.update();

  Serial.print("Candle: ");
  Serial.print(candleReed.read());
  Serial.print("Skull: ");
  Serial.print(skullReed.read());
  Serial.print("Demon 1: ");
  Serial.print(demon1Reed.read());
  Serial.print("Demon 2: ");
  Serial.print(demon2Reed.read());
  Serial.print("Demon 3: ");
  Serial.print(demon3Reed.read());

}


void inputTest_puzzle4() {

  Serial.print("Touch Sensor: ");
  Serial.println(analogRead(TOUCH_SENSOR_PIN));

}



void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      //Serial.println("**nomorefiles**");
      break;
    }

    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }

    Serial.print(entry.name());

    if (entry.isDirectory()) {
      Serial.println();
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
