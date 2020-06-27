/*

  Crypt Game Prototype

  Escape New Haven
  Justin C Kirk
  2020

*/

//Set to 0 to stop printing
#define DEBUG 1

//Used for reed switch or button inputs
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

//Create object
Adafruit_VS1053_FilePlayer audioPlayer = Adafruit_VS1053_FilePlayer(BOARD_RESET, BOARD_CS, BOARD_XDCS, BOARD_DREQ, BOARD_SDCS);

int playFileFlag = -1;
char* trackNames[] = {"test.mp3", "play.mp3", "thunder.mp3", "scary.mp3", "beast.mp3", "END.mp3"};
int trackIndex = -1;

//Inputs

//Puzzle 1
const int lightBarSwitchPin = 14;
Bounce lightBarSwitch = Bounce();
const int motionSensorPin = 15;
//Bounce motionSensor = Bounce();

const int filmCabinetReedPin = 39;
Bounce filmCabinetReed = Bounce();

//Puzzle 2
const int torch1ReedPin = 37;
Bounce torch1Reed = Bounce();
const int torch2ReedPin = 36;
Bounce torch2Reed = Bounce();
const int torch3ReedPin = 35;
Bounce torch3Reed = Bounce();
const int torch4ReedPin = 34;
Bounce torch4Reed = Bounce();
const int torch5ReedPin = 33;
Bounce torch5Reed = Bounce();
const int coffinReedPin = 32;
Bounce coffinReed = Bounce();

//Puzzle 3
const int candleReedPin  = 31;
Bounce candleReed = Bounce();
const int skullReedPin  = 30;
Bounce skullReed = Bounce();
const int demon1ReedPin  = 29;
Bounce demon1Reed = Bounce();
const int demon2ReedPin  = 28;
Bounce demon2Reed = Bounce();
const int demon3ReedPin  = 27;
Bounce demon3Reed = Bounce();

//Puzzle 4
const int touchSensorPin = 0;  //analog 0

//General
const int puzzle1OverridePin = 26;
Bounce puzzle1Override = Bounce();
const int puzzle2OverridePin = 25;
Bounce puzzle2Override = Bounce();
const int puzzle3OverridePin = 24;
Bounce puzzle3Override = Bounce();
const int puzzle4OverridePin = 23;
Bounce puzzle4Override = Bounce();

const int resetSwitchPin = 22;
Bounce resetSwitch = Bounce();

const int inputPins[] = {lightBarSwitchPin , motionSensorPin, filmCabinetReedPin ,
                         torch1ReedPin , torch2ReedPin , torch3ReedPin , torch4ReedPin , torch5ReedPin , coffinReedPin,
                         candleReedPin , skullReedPin , demon1ReedPin , demon2ReedPin , demon3ReedPin ,
                         puzzle1OverridePin, puzzle2OverridePin, puzzle3OverridePin, puzzle4OverridePin, resetSwitchPin
                        };

const int inputCount = sizeof(inputPins) / 2;



//Outputs

//Puzzle 1
const int lightBarLED = 12;
const int lightningLED = 11;
const int filmCabinetMagLock = 4;

//Puzzle 2
const int torchLED = 10;
const int coffinUVLED = 9;
const int coffinMagLock = 3;

//Puzzle 3
const int candleLED = 8;
const int skullLED = 7;
const int mirrorLED = 6;

//Puzzle 4


const int leds[] = {lightBarLED, lightningLED, torchLED, coffinUVLED, candleLED, skullLED, mirrorLED};
const int ledCount = sizeof(leds) / 2;


//Game Variables

//Puzzle 1
boolean lightsOff = false;
boolean standingStill = false;
int standStillTime = 0;
boolean filmCabinetOpen = false;
boolean puzzle1Conditions[] = {lightsOff, standingStill, filmCabinetOpen };
boolean puzzle1Complete = false;

//Puzzle 2
boolean torch1Placed = false;
boolean torch2Placed = false;
boolean torch3Placed = false;
boolean torch4Placed = false;
boolean torch5Placed = false;
boolean coffinOpen = false;
int coffinOpenCounter = 0;
unsigned long torchCurrent = 0;
unsigned long torchPrevious = 0;
const long torchInterval = 100;
int torchIncrement = 5;
int torchLEDVal = 0;

boolean torchesPlaced = false;
boolean puzzle2Conditions[] = {torch1Placed, torch2Placed, torch3Placed, torch4Placed, torch5Placed, coffinOpen};
boolean puzzle2Complete = false;

//Puzzle 3
boolean candlePlaced = false;
boolean skullPlaced = false;
boolean demon1Placed = false;
boolean demon2Placed = false;
boolean demon3Placed = false;
boolean demonsPlaced = false;
boolean puzzle3Complete = false;

//Puzzle 4

//Will need to adjust this for use with touch sensor
const int touchThreshold = 4;
boolean isTouching = false;
boolean puzzle4Complete = false;


//General
unsigned long cryptCurrent = 0;
unsigned long cryptPrevious = 0;
const long cryptInterval = 100;

int cryptState = -1;
boolean updateCrypt = false;
int puzzle1State = -1;
int puzzle3State = -1;

//Game States
//Puzzle 1 and 3 need sub states to handle the different audio tracks being played
const int puzzle1 = 1;
const int puzzle1A = 10;
const int puzzle1B = 11;
const int puzzle2 = 2;
const int puzzle3 = 3;
const int puzzle3A = 30;
const int puzzle3B = 31;
const int puzzle4 = 4;
const int resetState = 0;
const int winnerState = 5;

void getCryptState(const int i) {

  updateCrypt = true;
  Serial.print(F("Getting State: "));
  Serial.println(i);
  cryptState = i;

}

void setup() {

  Serial.begin(115200);

  //Setup Inputs
  //Puzzle 1
  //lightBarSwitch.attach(lightBarSwitchPin, INPUT_PULLUP);
  lightBarSwitch.attach(lightBarSwitchPin);
  lightBarSwitch.interval(25);
  filmCabinetReed.attach(filmCabinetReedPin, INPUT_PULLUP);
  filmCabinetReed.interval(25);

  //Puzzle 2
  torch1Reed.attach(torch1ReedPin);
  torch1Reed.interval(25);
  torch2Reed.attach(torch2ReedPin);
  torch2Reed.interval(25);
  torch3Reed.attach(torch3ReedPin);
  torch3Reed.interval(25);
  torch4Reed.attach(torch4ReedPin);
  torch4Reed.interval(25);
  torch5Reed.attach(torch5ReedPin);
  torch5Reed.interval(25);
  coffinReed.attach(coffinReedPin);
  coffinReed.interval(25);

  //Puzzle 3
  candleReed.attach(candleReedPin);
  candleReed.interval(25);
  skullReed.attach(skullReedPin);
  skullReed.interval(25);
  demon1Reed.attach(demon1ReedPin);
  demon1Reed.interval(25);
  demon2Reed.attach(demon2ReedPin);
  demon2Reed.interval(25);
  demon3Reed.attach(demon3ReedPin);
  demon3Reed.interval(25);

  //General
  puzzle1Override.attach(puzzle1OverridePin);
  puzzle1Override.interval(25);
  puzzle2Override.attach(puzzle2OverridePin);
  puzzle2Override.interval(25);
  puzzle3Override.attach(puzzle3OverridePin);
  puzzle3Override.interval(25);
  puzzle4Override.attach(puzzle4OverridePin);
  puzzle4Override.interval(25);
  resetSwitch.attach(resetSwitchPin);
  resetSwitch.interval(25);

  //Setup Outputs
  for (int i = 0; i < ledCount; i++) {
    pinMode(leds[i], OUTPUT);
    analogWrite(leds[i], 0);
  }

  pinMode(filmCabinetMagLock, OUTPUT);
  pinMode(coffinMagLock, OUTPUT);

  audioPlayer.begin();
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
  audioPlayer.playFullFile("test.mp3");

  if (audioPlayer.stopped()) {
    getCryptState(resetState);
  }
}

void checkResetSwitch() {
  //Check Reset State
  resetSwitch.update();
  if (resetSwitch.read() == 1) {
    audioPlayer.stopPlaying();
    getCryptState(resetState);
  }
}

void checkPuzzleOverrides() {

  puzzle1Override.update();
  if (puzzle1Override.rose()) {
    if (DEBUG) {
      Serial.println(F("Puzzle 1 Override"));
    }
    //audioPlayer.stopPlaying();
    puzzle1Complete = true;
  }

  puzzle2Override.update();
  if (puzzle2Override.rose()) {
    if (DEBUG) {
      Serial.println(F("Puzzle 2 Override"));
    }
    //audioPlayer.stopPlaying();
    puzzle2Complete = true;
  }

  puzzle3Override.update();
  if (puzzle3Override.rose()) {
    if (DEBUG) {
      Serial.println(F("Puzzle 3 Override"));
    }
    //audioPlayer.stopPlaying();
    puzzle3Complete = true;
  }

  puzzle4Override.update();
  if (puzzle4Override.rose()) {
    if (DEBUG) {
      Serial.println(F("Puzzle 4 Override"));
    }
    //audioPlayer.stopPlaying();
    puzzle4Complete = true;
  }

}

void loop() {

  //    MAIN GAME


  switch (cryptState) {

    case resetState:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("RESET STATE"));
        }

        //Reset Puzzle 1
        analogWrite(lightningLED, 0);
        puzzle1Complete = false;
        puzzle1State = -1;
        lightsOff = false;
        standingStill = false;
        filmCabinetOpen = false;

        //Reset Puzzle 2
        analogWrite(torchLED, 0);
        analogWrite(coffinUVLED, 0);
        puzzle2Complete = false;
        torch1Placed = false;
        torch2Placed = false;
        torch3Placed = false;
        torch4Placed = false;
        torch5Placed = false;
        coffinOpen = false;

        //Reset Puzzle 3
        analogWrite(candleLED, 0);
        analogWrite(skullLED, 0);
        analogWrite(mirrorLED, 0);
        puzzle3Complete = false;
        puzzle3State = -1;
        candlePlaced = false;
        skullPlaced = false;
        demon1Placed = false;
        demon2Placed = false;
        demon3Placed = false;

        //Reset Puzzle 4
        puzzle4Complete = false;

        updateCrypt = false;
      }
      //Test functions
      //puzzleInputTest();
      //ledTest(0);

      //open all MagLocks
      Serial.println(F("Open all maglocks"));
      digitalWrite(filmCabinetMagLock, LOW);
      digitalWrite(coffinMagLock, LOW);
      delay(5000);
      //close all mag locks
      Serial.println(F("Close all maglocks"));
      digitalWrite(filmCabinetMagLock, HIGH);
      digitalWrite(coffinMagLock, HIGH);

      delay(1000);
      //Start game
      getCryptState(puzzle1);

      break;

    case puzzle1:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Puzzle 1"));
        }
        digitalWrite(lightBarLED, LOW);
        digitalWrite(lightningLED, LOW);
        puzzle1Complete = false;
        lightsOff = false;
        standingStill = false;
        filmCabinetOpen = false;
        trackIndex = 1;
        puzzle1State = puzzle1A;
        playFileFlag = 1;
        updateCrypt = false;
      }

      switch (puzzle1State) {

        case puzzle1A:

          if (playFileFlag == 1 && puzzle1Complete == false) {
            audioPlayer.startPlayingFile(trackNames[trackIndex]);
            playFileFlag -= 1;
          }
          if (audioPlayer.stopped() && puzzle1Complete == false) {
            playFileFlag = 1;
          }

          if (audioPlayer.playingMusic) {

            checkResetSwitch();
            checkPuzzleOverrides();

            lightBarSwitch.update();
            filmCabinetReed.update();

            if (lightBarSwitch.read() == 1) {
              lightsOff = true;
              analogWrite(lightBarLED, 0);
            } else if (lightBarSwitch.read() == 0) {
              lightsOff = false;
              analogWrite(lightBarLED, 200);
            }

            if (filmCabinetReed.fell()) {
              filmCabinetOpen = false;
            } else if (filmCabinetReed.rose()) {
              filmCabinetOpen = true;
            }

            int motionVal = digitalRead(motionSensorPin);
            if (motionVal == 0) {
              standingStill = true;
            } else {
              standingStill = false;
              standStillTime = 0;
            }

            int puzzle1Sum = lightsOff + standingStill + filmCabinetOpen;

            cryptCurrent = millis();

            if (cryptCurrent - cryptPrevious > cryptInterval) {
              cryptPrevious = cryptCurrent;

              if (puzzle1Sum < 3) {
                standStillTime = 0;
                audioPlayer.setVolume(0, 0);
                if (DEBUG) {
                  Serial.print(F("Lights Off: "));
                  Serial.print(lightsOff);
                  Serial.print(F(" "));
                  Serial.print(F("Standing Still: "));
                  Serial.print(standingStill);
                  Serial.print(F(" "));
                  Serial.print(F("Film Cabinet Open: "));
                  Serial.print(filmCabinetOpen);
                  Serial.print(F(" "));
                  Serial.print(F("Sum: "));
                  Serial.println(puzzle1Sum);
                }
              } else if (puzzle1Sum == 3) {

                standStillTime += 1;
                if (standStillTime >= 100) {
                  puzzle1Complete = true;
                  audioPlayer.stopPlaying();
                  Serial.println(F("Puzzle 1 complete"));
                  // playFileFlag = 0;
                  //puzzle1State = puzzle1B;
                } else if (standStillTime < 100) {
                  audioPlayer.setVolume(standStillTime, standStillTime);
                }

                if (DEBUG) {
                  Serial.print(F("Standing Stil For : "));
                  Serial.println(standStillTime);
                }
              }

              if (puzzle1Complete == true) {
                Serial.println(F("Puzzle 1 Complete"));
                audioPlayer.stopPlaying();
                playFileFlag = 1;
                puzzle1State = puzzle1B;
              }

            }//end timer loop
          }//end music playing loop
          break;

        case puzzle1B:
          // Serial.println("thunder");
          trackIndex = 2;
          audioPlayer.setVolume(0, 0);

          if (playFileFlag == 1) {
            audioPlayer.startPlayingFile(trackNames[trackIndex]);
            playFileFlag -= 1;
          }

          if (audioPlayer.playingMusic) {

            checkResetSwitch();
            checkPuzzleOverrides();

            analogWrite(lightningLED, 200);
            delay(100);
            analogWrite(lightningLED, 0);
            delay(50);
            analogWrite(lightningLED, 200);
            delay(100);
            analogWrite(lightningLED, 0);
            delay(50);
            analogWrite(lightningLED, 200);
            delay(100);
            analogWrite(lightningLED, 0);
            delay(50);
          }

          if (audioPlayer.stopped()) {
            // playFileFlag = 1;
            getCryptState(puzzle2);
          }
          break;
      }

      break;

    case puzzle2:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Puzzle 2"));
        }
        puzzle2Complete = false;
        analogWrite(torchLED, 0);
        analogWrite(coffinUVLED, 0);
        torch1Placed = false;
        torch2Placed = false;
        torch3Placed = false;
        torch4Placed = false;
        torch5Placed = false;
        coffinOpen = false;
        coffinOpenCounter = 0;
        trackIndex = 3;
        updateCrypt = false;
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

        checkResetSwitch();
        //checkPuzzleOverrides();
        puzzle2Override.update();

        if (puzzle2Override.read() == 1) {
          Serial.println(F("Puzzle 2 Override"));
          audioPlayer.stopPlaying();
          puzzle2Complete = true;
          getCryptState(puzzle3);
        }

        lightBarSwitch.update();
        torch1Reed.update();
        torch2Reed.update();
        torch3Reed.update();
        torch4Reed.update();
        torch5Reed.update();

        if (lightBarSwitch.read() == 1) {
          lightsOff = true;
          analogWrite(lightBarLED, 0);
        } else if (lightBarSwitch.read() == 0) {
          lightsOff = false;
          analogWrite(lightBarLED, 200);
        }

        if (torch1Reed.fell()) {
          torch1Placed = true;
        } else if (torch1Reed.rose()) {
          torch1Placed = false;
        }

        if (torch2Reed.fell()) {
          torch2Placed = true;
        } else if (torch2Reed.rose()) {
          torch2Placed = false;
        }

        if (torch3Reed.fell()) {
          torch3Placed = true;
        } else if (torch3Reed.rose()) {
          torch3Placed = false;
        }

        if (torch4Reed.fell()) {
          torch4Placed = true;
        } else if (torch4Reed.rose()) {
          torch4Placed = false;
        }

        if (torch5Reed.fell()) {
          torch5Placed = true;
        } else if (torch5Reed.rose()) {
          torch5Placed = false;
        }

        int torchValSum = torch1Placed + torch2Placed + torch3Placed + torch4Placed + torch5Placed;

        cryptCurrent = millis();
        if (cryptCurrent - cryptPrevious >= cryptInterval) {

          if (torchValSum < 5) {
            if (DEBUG) {
              Serial.print(F("T1: "));
              Serial.print(torch1Placed);
              Serial.print(F(" "));
              Serial.print(F("T2: "));
              Serial.print(torch2Placed);
              Serial.print(F(" "));
              Serial.print(F("T3: "));
              Serial.print(torch3Placed);
              Serial.print(F(" "));
              Serial.print(F("T4: "));
              Serial.print(torch4Placed);
              Serial.print(F(" "));
              Serial.print(F("T5: "));
              Serial.print(torch5Placed);
              Serial.print(F(" "));
              // Serial.print("C: ");
              // Serial.print(coffinOpen);
              //  Serial.print(" ");
              Serial.print(F("T Sum: "));
              Serial.println(torchValSum);
            }

            analogWrite(torchLED, 0);
            analogWrite(coffinUVLED, 0);

          } else if (torchValSum == 5) {
            if (DEBUG) {
              Serial.println(F("Torches Placed"));
            }
            puzzle2Complete = true;

          }

          if (puzzle2Complete == true) {

            torchLEDVal += torchIncrement;
            if (torchLEDVal >= 255 || torchLEDVal <= 0) {
              torchIncrement = -torchIncrement;
            }
            analogWrite(torchLED, torchLEDVal);

            //Open mag lock
            //Double check state
            //digitalWrite(coffinMagLock, HIGH);

            coffinReed.update();
            //Count times coffin has been opened.
            if (coffinReed.fell()) {
              coffinOpen = true;
              coffinOpenCounter += 1;
            } else if (coffinReed.rose()) {
              coffinOpen = false;
              coffinOpenCounter += 1;
            }

            //If coffin lid opens once then is closed, the LED turns on
            if (coffinOpenCounter % 2 == 0 && coffinOpenCounter > 0) {

              analogWrite(coffinUVLED, 255);
              audioPlayer.stopPlaying();
              getCryptState(puzzle3);
            } else {
              analogWrite(coffinUVLED, 0);
            }

            if (DEBUG) {
              Serial.println(coffinOpenCounter);
            }


          }
          cryptPrevious = cryptCurrent;
        }
      }

      break;

    case puzzle3:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Puzzle 3"));
        }

        analogWrite(candleLED, 0);
        analogWrite(skullLED, 0);
        analogWrite(mirrorLED, 0);
        puzzle3Complete = false;
        candlePlaced = false;
        skullPlaced = false;
        demon1Placed = false;
        demon2Placed = false;
        demon3Placed = false;
        puzzle3State = puzzle3A;
        trackIndex = 3;
        updateCrypt = false;
      }


      switch (puzzle3State) {

        case puzzle3A:

          audioPlayer.setVolume(0, 0);
          if (playFileFlag == 1) {
            audioPlayer.startPlayingFile(trackNames[trackIndex]);
            playFileFlag -= 1;
          }
          if (audioPlayer.stopped() && puzzle3Complete == false) {
            playFileFlag = 1;
          }

          while (audioPlayer.playingMusic) {

            checkResetSwitch();
            checkPuzzleOverrides();

            lightBarSwitch.update();
            coffinReed.update();
            candleReed.update();
            skullReed.update();
            demon1Reed.update();
            demon2Reed.update();
            demon3Reed.update();

            //Keep lightbar switch active
            if (lightBarSwitch.read() == 1) {
              lightsOff = true;
              analogWrite(lightBarLED, 0);
            } else if (lightBarSwitch.read() == 0) {
              lightsOff = false;
              analogWrite(lightBarLED, 200);
            }

            //Keep coffin elements going
            if (coffinReed.fell()) {
              coffinOpen = true;
              coffinOpenCounter += 1;
            } else if (coffinReed.rose()) {
              coffinOpen = false;
              coffinOpenCounter += 1;
            }

            if (coffinOpenCounter % 2 == 0 && coffinOpenCounter > 0) {
              analogWrite(coffinUVLED, 255);
            } else {
              analogWrite(coffinUVLED, 0);
            }

            if (candleReed.fell()) {
              candlePlaced = true;
              analogWrite(candleLED, 255);
            } else if (candleReed.rose()) {
              candlePlaced = false;
              analogWrite(candleLED, 0);
            }

            if (skullReed.fell()) {
              skullPlaced = true;
              analogWrite(skullLED, 255);
            } else if (skullReed.rose()) {
              skullPlaced = false;
              analogWrite(skullLED, 0);

            }

            if (demon1Reed.fell()) {
              demon1Placed = true;
            } else if (demon1Reed.rose()) {
              demon1Placed = false;
            }

            if (demon2Reed.fell()) {
              demon2Placed = true;
            } else if (demon2Reed.rose()) {
              demon2Placed = false;
            }

            if (demon3Reed.fell()) {
              demon3Placed = true;
            } else if (demon3Reed.rose()) {
              demon3Placed = false;
            }

            cryptCurrent = millis();
            if (cryptCurrent - cryptPrevious >= cryptInterval) {

              //Keep pulsing the torches
              torchLEDVal += torchIncrement;
              if (torchLEDVal >= 255 || torchLEDVal <= 0) {
                torchIncrement = -torchIncrement;
              }
              analogWrite(torchLED, torchLEDVal);




              int puzzle3Sum = candlePlaced + skullPlaced + demon1Placed + demon2Placed + demon3Placed;

              if (puzzle3Sum < 5) {

                if (DEBUG) {
                  Serial.print(F("Candle: "));
                  Serial.print(candlePlaced);
                  Serial.print(F(" "));
                  Serial.print(F("Skull: "));
                  Serial.print(skullPlaced);
                  Serial.print(F(" "));
                  Serial.print(F("Demon 1: "));
                  Serial.print(demon1Placed);
                  Serial.print(F(" "));
                  Serial.print(F("Demon 2: "));
                  Serial.print(demon2Placed);
                  Serial.print(F(" "));
                  Serial.print(F("Demon 3: "));
                  Serial.print(demon3Placed);
                  Serial.print(F(" "));
                  Serial.print(F("Puzzle 3 Sum: "));
                  Serial.println(puzzle3Sum);
                }

                analogWrite(mirrorLED, 0);

              } else if (puzzle3Sum == 5) {
                puzzle3Complete = true;
              }

              if (puzzle3Complete == true) {
                if (DEBUG) {
                  Serial.println(F("!!!!!THE BEAST!!!!!"));
                }
                analogWrite(mirrorLED, 255);
                audioPlayer.stopPlaying();
                playFileFlag = 1;
                puzzle3State = puzzle3B;
              }
              cryptPrevious = cryptCurrent;
            }
          }

          break;

        case puzzle3B:
          trackIndex = 4;
          audioPlayer.setVolume(0, 0);

          if (playFileFlag == 1) {
            audioPlayer.startPlayingFile(trackNames[trackIndex]);
            playFileFlag -= 1;
          }

          if (audioPlayer.playingMusic) {
            checkResetSwitch();
            checkPuzzleOverrides();
            analogWrite(lightBarLED, 0);
            analogWrite(mirrorLED, 200);
          }
          if (audioPlayer.stopped()) {
            getCryptState(puzzle4);
          }
          break;
      }
      break;

    case puzzle4:
      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Puzzle 4"));
        }
        trackIndex = 3;//play scary music?
        puzzle4Complete = false;
        updateCrypt = false;
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

        checkResetSwitch();
        checkPuzzleOverrides();

        cryptCurrent = millis();
        if (cryptCurrent - cryptPrevious >= cryptInterval) {

          //Keep pulsing torch leds?
          //Keep coffin elements going?

          //May need to adjust threshold
          int touchVal = analogRead(touchSensorPin);
          if (touchVal < touchThreshold) {
            if (DEBUG) {
              Serial.print(F("Touch Val: "));
              Serial.println(touchVal);
            }
          } else if (touchVal > touchThreshold) {
            puzzle4Complete = true;
          }

          int puzzle4OverrideVal = digitalRead(puzzle4OverridePin);

          if (puzzle4OverrideVal == 1) {
            puzzle4Complete = true;
          }

          if (puzzle4Complete == true) {
            if (DEBUG ) {
              Serial.println(F("ESCAPED THE GAME!!"));
            }
            delay(2000);
            playFileFlag = 1;
            audioPlayer.stopPlaying();
            getCryptState(winnerState);
          }
          cryptPrevious = cryptCurrent;
        }

      }

      break;

    case winnerState:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println(F("Winner State"));
          Serial.println(F("Congrats!"));
        }
        trackIndex = 5;//play end music
        puzzle4Complete = false;
        updateCrypt = false;
      }


      audioPlayer.setVolume(0, 0);
      if (playFileFlag == 1) {
        audioPlayer.startPlayingFile(trackNames[trackIndex]);
        playFileFlag -= 1;
      }

      if (audioPlayer.stopped()) {
        //playFileFlag = 1;
        getCryptState(resetState);
      }

      while (audioPlayer.playingMusic) {
        checkResetSwitch();
      }

      break;

  }

}



void ledTest(int val) {

  analogWrite(lightBarLED, val);
  analogWrite(lightningLED, val);
  analogWrite(torchLED, val);
  analogWrite(coffinUVLED, val);
  analogWrite(candleLED, val);
  analogWrite(skullLED, val);
  analogWrite(mirrorLED, val);

}



void puzzleInputTest() {



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
