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
char* trackNames[] = {"TEST.MP3", "PLAY.MP3", "THUNDER.MP3", "SCARY.MP3", "BEAST.MP3"};
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

//Puzzle 2
const int torchLED = 10;
const int coffinUVLED = 9;

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
const int touchThreshold = 500;
boolean isTouching = false;
boolean puzzle4Complete = false;


//General
unsigned long cryptCurrent = 0;
unsigned long cryptPrevious = 0;
const long cryptInterval = 100;

int cryptState = -1;
boolean updateCrypt = false;
int puzzle1State = -1;

//Game States
const int puzzle1 = 1;
const int puzzle1A = 10;
const int puzzle1B = 11;

const int puzzle2 = 2;
const int puzzle3 = 3;
const int puzzle4 = 4;
const int resetState = 0;
const int winnerState = 5;

void getCryptState(const int i) {

  updateCrypt = true;
  Serial.print("Getting State: ");
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


  audioPlayer.begin();
  audioPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
  SD.begin(BOARD_SDCS);    // initialise the SD card

  if (!audioPlayer.begin()) {
    Serial.println("Cannot find audio player.  Check pins.");
  }

  //File names seem to be restricted to 8 characters
  if (!SD.begin(BOARD_SDCS)) {
    Serial.println("SD read error.  Is card inserted?");
    //Turn on SD card error led?
  }

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


void loop() {

  //Check Reset State
  if (getResetState() == 1) {
    getCryptState(resetState);
  }

  puzzle1Override.update();
  if (puzzle1Override.read() == 1) {
    puzzle1Complete = true;
  }
  puzzle2Override.update();
  if (puzzle2Override.read() == 1) {
    puzzle2Complete = true;
  }
  puzzle3Override.update();
  if (puzzle3Override.read() == 1) {
    puzzle3Complete = true;
  }
  puzzle4Override.update();
  if (puzzle4Override.read() == 1) {
    puzzle4Complete = true;
  }



  //    MAIN GAME


  switch (cryptState) {

    case resetState:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println("RESET STATE");
        }

        //Reset Puzzle 1
        analogWrite(lightningLED, 0);
        puzzle1Complete = false;
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
      // Serial.println("Open all maglocks");
      // delay(5000);
      //close all mag locks
      //  Serial.println("Close all maglocks");

      //delay(1000);
      //Start game
      getCryptState(puzzle1);

      break;

    case puzzle1:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println("Puzzle 1");
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

            lightBarSwitch.update();
            filmCabinetReed.update();

            if (lightBarSwitch.read() == 1) {
              lightsOff = true;
              analogWrite(lightBarLED, 0);
            }
            if (lightBarSwitch.read() == 0) {
              lightsOff = false;
              analogWrite(lightBarLED, 200);
            }

            if (filmCabinetReed.fell()) {
              filmCabinetOpen = false;
            }
            if (filmCabinetReed.rose()) {
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
                  Serial.print("Lights Off: ");
                  Serial.print(lightsOff);
                  Serial.print(" ");
                  Serial.print("Standing Still: ");
                  Serial.print(standingStill);
                  Serial.print(" ");
                  Serial.print("Film Cabinet Open: ");
                  Serial.print(filmCabinetOpen);
                  Serial.print(" ");
                  Serial.print("Puzzle Sum: ");
                  Serial.println(puzzle1Sum);
                }
              } else if (puzzle1Sum == 3) {

                standStillTime += 1;
                if (standStillTime >= 100) {
                  puzzle1Complete = true;
                  audioPlayer.stopPlaying();
                  Serial.println("Puzzle 1 complete");
                  // playFileFlag = 0;
                  //puzzle1State = puzzle1B;
                } else if (standStillTime < 100) {
                  audioPlayer.setVolume(standStillTime, standStillTime);
                }

                if (DEBUG) {
                  Serial.print("Standing Stil For : ");
                  Serial.println(standStillTime);
                }
              }

              if (puzzle1Complete == true) {
                playFileFlag = 1;
                puzzle1State = puzzle1B;
              }

            }//end timer loop
          }//end music playing loop
          break;

        case puzzle1B:

          trackIndex = 2;
          audioPlayer.setVolume(0, 0);
          if (playFileFlag == 1) {
            audioPlayer.startPlayingFile(trackNames[trackIndex]);
            playFileFlag -= 1;
          }


          if (audioPlayer.playingMusic) {
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
            // Serial.println("Song Stop");
          }
          break;
      }

      break;

    case puzzle2:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println("Puzzle 2");
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

        lightBarSwitch.update();
        torch1Reed.update();
        torch2Reed.update();
        torch3Reed.update();
        torch4Reed.update();
        torch5Reed.update();
        coffinReed.update();

        if (lightBarSwitch.read() == 1) {
          lightsOff = true;
          analogWrite(lightBarLED, 0);
        }
        if (lightBarSwitch.read() == 0) {
          lightsOff = false;
          analogWrite(lightBarLED, 200);
        }

        if (torch1Reed.fell()) {
          torch1Placed = true;
        } 
        if (torch1Reed.rose()) {
          torch1Placed = false;
        }

        if (torch2Reed.fell()) {
          torch2Placed = true;
        }
        if (torch2Reed.rose()) {
          torch2Placed = false;
        }

        if (torch3Reed.fell()) {
          torch3Placed = true;
        } 
        if (torch3Reed.rose()) {
          torch3Placed = false;
        }

        if (torch4Reed.fell()) {
          torch4Placed = true;
        } 
        if (torch4Reed.rose()) {
          torch4Placed = false;
        }

        if (torch5Reed.fell()) {
          torch5Placed = true;
        } 
        if (torch5Reed.rose()) {
          torch5Placed = false;
        }

        if (coffinReed.fell()) {
          coffinOpen = true;
          coffinOpenCounter += 1;
        }
        if (coffinReed.rose()) {
          coffinOpen = false;
          coffinOpenCounter += 1;
        }

        int torchValSum = torch1Placed + torch2Placed + torch3Placed + torch4Placed + torch5Placed;


        cryptCurrent = millis();
        if (cryptCurrent - cryptPrevious >= cryptInterval) {

          if (torchValSum < 5) {
            if (DEBUG) {
              Serial.print("Torch 1: ");
              Serial.print(torch1Placed);
              Serial.print(" ");
              Serial.print("Torch 2: ");
              Serial.print(torch2Placed);
              Serial.print(" ");
              Serial.print("Torch 3: ");
              Serial.print(torch3Placed);
              Serial.print(" ");
              Serial.print("Torch 4: ");
              Serial.print(torch4Placed);
              Serial.print(" ");
              Serial.print("Torch 5: ");
              Serial.print(torch5Placed);
              Serial.print(" ");
              Serial.print("Coffin Open: ");
              Serial.print(coffinOpen);
              Serial.print(" ");
              Serial.print("Torch Sum: ");
              Serial.println(torchValSum);
            }

            //LEDs off if torches are not all placed
            //Pulse TorchLED

            analogWrite(torchLED, 0);
            analogWrite(coffinUVLED, 0);

            //analogWrite(coffinUVLED, 0);
          } else if (torchValSum == 5) {
            if (DEBUG) {
              Serial.println("Torches Placed");

            }

            puzzle2Complete = true;
          }

          if (puzzle2Complete == true) {
            torchLEDVal += torchIncrement;
            if (torchLEDVal >= 255 || torchLEDVal <= 0) {
              torchIncrement = -torchIncrement;
            }
            analogWrite(torchLED, torchLEDVal);

            //Count times coffin has been opened.
            //If coffin lid opens once then is closed, the LED turns on
            if (coffinOpenCounter % 2 == 0) {
              analogWrite(coffinUVLED, 255);
              getCryptState(puzzle3);
            } else {
              analogWrite(coffinUVLED, 0);
            }

          }

          cryptPrevious = cryptCurrent;
        }
      }
      break;

    case puzzle3:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println("Puzzle 3");
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
        lightBarSwitch.update();
        candleReed.update();
        skullReed.update();
        demon1Reed.update();
        demon2Reed.update();
        demon3Reed.update();

        if (lightBarSwitch.read() == 1) {
          lightsOff = true;
          analogWrite(lightBarLED, 0);
        }
        if (lightBarSwitch.read() == 0) {
          lightsOff = false;
          analogWrite(lightBarLED, 200);
        }

        if (candleReed.fell()) {
          candlePlaced = true;
        } else if (candleReed.rose()) {
          candlePlaced = false;
        }

        if (skullReed.fell()) {
          torch2Placed = true;
        } else if (skullReed.rose()) {
          torch2Placed = false;
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


          int puzzle3Sum = candlePlaced + skullPlaced + demon1Placed + demon2Placed + demon3Placed;

          if (puzzle3Sum < 5) {

            if (DEBUG) {
              Serial.print("Candle: ");
              Serial.print(candlePlaced);
              Serial.print(" ");
              Serial.print("Skull: ");
              Serial.print(skullPlaced);
              Serial.print(" ");
              Serial.print("Demon 1: ");
              Serial.print(demon1Placed);
              Serial.print(" ");
              Serial.print("Demon 2: ");
              Serial.print(demon2Placed);
              Serial.print(" ");
              Serial.print("Demon 3: ");
              Serial.print(demon3Placed);
              Serial.print(" ");
              Serial.print("Puzzle 3 Sum: ");
              Serial.println(puzzle3Sum);
            }

            analogWrite(mirrorLED, 0);

          } else if (puzzle3Sum == 5) {
            puzzle3Complete = true;
          }

          if (puzzle3Complete == true) {
            if (DEBUG) {
              Serial.println("!!!!!THE BEAST!!!!!");
            }
            analogWrite(lightBarLED, 0);
            analogWrite(mirrorLED, 200);
            getCryptState(puzzle4);
          }

          cryptPrevious = cryptCurrent;
        }
      }
      break;

    case puzzle4:
      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println("Puzzle 4");
        }
        puzzle4Complete = false;
        updateCrypt = false;
      }

      cryptCurrent = millis();
      if (cryptCurrent - cryptPrevious >= cryptInterval) {

        int touchVal = analogRead(touchSensorPin);
        if (touchVal > touchThreshold) {
          if (DEBUG) {
            Serial.print("Touch Val: ");
            Serial.println(touchVal);

          }
        } else if (touchVal < touchThreshold) {
          puzzle4Complete = true;
        }

        int puzzle4OverrideVal = digitalRead(puzzle4OverridePin);

        if (puzzle4OverrideVal == 1) {
          puzzle4Complete = true;
        }

        if (puzzle4Complete == true) {
          if (DEBUG ) {
            Serial.println("ESCAPED THE GAME!!");
          }
          delay(2000);
          getCryptState(winnerState);
        }
        cryptPrevious = cryptCurrent;
      }
      break;

    case winnerState:
      if (DEBUG) {
        Serial.println("Winner State");
        Serial.println("Congrats!");
      }
      //Play some audio
      //Flash some lights
      //Open final door
      delay(2000);
      getCryptState(resetState);

      break;

  }

}


int getResetState() {
  int resetVal = digitalRead(resetSwitchPin);
  return resetVal;
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
