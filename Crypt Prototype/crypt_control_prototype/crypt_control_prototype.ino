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


char* trackNames[] = {"TEST.MP3", "PLAY.MP3", "THUNDER.MP3", "SCARY.MP3", "BEAST.MP3"};
int trackIndex = -1;

//Inputs

//Puzzle 1
const int lightBarSwitch = 14;
const int motionSensorPin = 15;
const int filmCabinetReed = 39;

//Puzzle 2
const int torch1Reed = 37;
const int torch2Reed = 36;
const int torch3Reed = 35;
const int torch4Reed = 34;
const int torch5Reed = 33;
const int coffinReed = 32;

//Puzzle 3
const int candleReed = 31;
const int skullReed = 30;
const int demon1Reed = 29;
const int demon2Reed = 28;
const int demon3Reed = 27;

//Puzzle 4
const int touchSensorPin = 0;  //analog 0

//General
const int puzzle1OverridePin = 26;
const int puzzle2OverridePin = 25;
const int puzzle3OverridePin = 24;
const int puzzle4OverridePin = 23;

const int resetSwitchPin = 22;

const int inputPins[] = {lightBarSwitch, motionSensorPin, filmCabinetReed,
                         torch1Reed, torch1Reed, torch1Reed, torch1Reed, torch1Reed, coffinReed,
                         candleReed, skullReed, demon1Reed, demon2Reed, demon3Reed,
                         puzzle1OverridePin, puzzle2OverridePin, puzzle3OverridePin, puzzle4OverridePin, resetSwitchPin
                        };

const int inputCount = sizeof(inputPins) / 2;

//Used the Bounce2 library for better control of inputs.
//The library is was made to work with buttons but really any input
//  which gives a high or low state will work just as well.
//Bounce * inputs = new Bounce[inputCount];


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

//Game States
const int puzzle1 = 1;
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
  for (int i = 0; i < inputCount; i++) {
    pinMode(inputPins[i], INPUT);
  }
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
  //audioPlayer.playFullFile(trackNames[trackIndex]);

  // if (audioPlayer.stopped()) {
  getCryptState(resetState);
  // }


}


void loop() {

  //Check Reset State
  if (getResetState() == 1) {
    getCryptState(resetState);
  }



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
      //delay(100);
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
        updateCrypt = false;
      }

      //audioPlayer.startPlayingFile(trackNames[trackIndex]);

      while (audioPlayer.playingMusic) {

        cryptCurrent = millis();

        if (cryptCurrent - cryptPrevious >= cryptInterval) {

          int lightBarVal = digitalRead(lightBarSwitch);
          if (lightBarVal == 0) {
            lightsOff = true;
            analogWrite(lightBarLED, 0);
          } else {
            lightsOff = false;
            analogWrite(lightBarLED, 200);
          }

          int filmCabinetVal = digitalRead(filmCabinetReed);
          Serial.println(filmCabinetVal);
          if (filmCabinetVal == 1) {
            filmCabinetOpen = true;
          } else {
            filmCabinetOpen = false;
          }

          if (filmCabinetOpen == true) {
            trackIndex = 2;
            Serial.println(trackIndex);
            audioPlayer.stopPlaying();
            //audioPlayer.startPlayingFile("thunder.mp3");
          }

          cryptPrevious = cryptCurrent;
        }

      }



      //

      //  if (audioPlayer.stopped()) {
      //   getCryptState(puzzle2);
      //   }

      break;

    case puzzle2:

      if (updateCrypt == true) {
        if (DEBUG) {
          Serial.println("Puzzle 2");
        }
        puzzle2Complete = false;
        //reset torches
        //reset coffin
        analogWrite(torchLED, 0);
        analogWrite(coffinUVLED, 0);
        torch1Placed = false;
        torch2Placed = false;
        torch3Placed = false;
        torch4Placed = false;
        torch5Placed = false;
        coffinOpen = false;
        updateCrypt = false;
      }

      //Loop scary music
      audioPlayer.startPlayingFile("scary.mp3");

      while (audioPlayer.playingMusic) {



        cryptCurrent = millis();
        if (cryptCurrent - cryptPrevious >= cryptInterval) {

          //Light switch in dressing room should work as normal
          int lightBarVal = digitalRead(lightBarSwitch);
          if (lightBarVal == 0) {
            analogWrite(lightBarLED, 0);
          } else {
            analogWrite(lightBarLED, 200);
          }


          int torch1Val = digitalRead(torch1Reed);
          if (torch1Val == 1) {
            torch1Placed = true;
          } else {
            torch1Placed = false;
          }

          int torch2Val = digitalRead(torch2Reed);
          if (torch2Val == 1) {
            torch2Placed = true;
          } else {
            torch2Placed = false;
          }

          int torch3Val = digitalRead(torch3Reed);
          if (torch3Val == 1) {
            torch3Placed = true;
          } else {
            torch3Placed = false;
          }

          int torch4Val = digitalRead(torch4Reed);
          if (torch4Val == 1) {
            torch4Placed = true;
          } else {
            torch4Placed = false;
          }

          int torch5Val = digitalRead(torch5Reed);
          if (torch5Val == 1) {
            torch5Placed = true;
          } else {
            torch5Placed = false;
          }

          int coffinVal = digitalRead(coffinReed);
          if (coffinVal == 1) {
            coffinOpen = true;
          } else {
            coffinOpen = false;
          }

          int torchValSum = torch1Placed + torch2Placed + torch3Placed + torch4Placed + torch5Placed;
          int puzzle2OverrideVal = digitalRead(puzzle2OverridePin);

          if (puzzle2OverrideVal == 1) {
            puzzle2Complete = true;
          }

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
          } else if (torchValSum == 5) {
            if (DEBUG) {
              Serial.println("Torches Placed");
              Serial.println("Puzzle Complete");
            }
            puzzle2Complete = true;
          }

          if (puzzle2Complete == true) {
            analogWrite(torchLED, 200);
            //activate coffin knocker
            // 3 times every 10 seconds

            //Count times coffin has been opened.
            //If coffin lid opens once then is closed, the LED turns on
            analogWrite(coffinUVLED, 200);

            //Once coffin lid opens then closes, get next puzzle
            getCryptState(puzzle3);
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

      cryptCurrent = millis();
      if (cryptCurrent - cryptPrevious >= cryptInterval) {

        //Light switch in dressing room should work as normal
        int lightBarVal = digitalRead(lightBarSwitch);
        if (lightBarVal == 0) {
          analogWrite(lightBarLED, 0);
        } else {
          analogWrite(lightBarLED, 200);
        }

        int candleVal = digitalRead(candleReed);
        if (candleVal == 1) {
          candlePlaced = !candlePlaced;
          if (candlePlaced == true) {
            analogWrite(candleLED, 200);
          } else {
            analogWrite(candleLED, 0);
          }
        }

        int skullVal = digitalRead(skullReed);
        if (skullVal == 1) {
          skullPlaced = !skullPlaced;
        }

        if (skullPlaced) {
          analogWrite(skullLED, 200);
        } else {
          analogWrite(skullLED, 0);
        }

        int demon1Val = digitalRead(demon1Reed);
        if (demon1Val == 1) {
          demon1Placed = !demon1Placed;
        }

        int demon2Val = digitalRead(demon2Reed);
        if (demon2Val == 1) {
          demon2Placed = !demon2Placed;
        }

        int demon3Val = digitalRead(demon3Reed);
        if (demon3Val == 1) {
          demon3Placed = !demon3Placed;
        }

        int puzzle3Sum = candlePlaced + skullPlaced + demon1Placed + demon2Placed + demon3Placed;

        int puzzle3OverrideVal = digitalRead(puzzle3OverridePin);
        if (puzzle3OverrideVal == 1) {
          puzzle3Complete = true;
        }

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



  /*
    //May need to break this up into a few lines
    Serial.print(digitalRead(filmCabinetReed));
    Serial.print(" : ");
    Serial.print(digitalRead(torch1Reed));
    Serial.print(" : ");
    Serial.print(digitalRead(torch2Reed));
    Serial.print(" : ");
    Serial.print(digitalRead(torch3Reed));
    Serial.print(" : ");
    Serial.print(digitalRead(torch4Reed));
    Serial.print(" : ");
    Serial.print(digitalRead(torch5Reed));
    Serial.print(" : ");
    Serial.print(digitalRead(coffinReed));
    Serial.print(" : ");
    Serial.print(digitalRead(candleReed));
    Serial.print(" : ");
    Serial.print(digitalRead(skullReed));
    Serial.print(" : ");
    Serial.print(digitalRead(demon1Reed));
    Serial.print(" : ");
    Serial.print(digitalRead(demon2Reed));
    Serial.print(" : ");
    Serial.print(digitalRead(demon3Reed));
    Serial.print(" : ");
    Serial.print(digitalRead(touchSensorPin));
    Serial.print(" : ");
    Serial.print(digitalRead(puzzle1OverridePin));
    Serial.print(" : ");
    Serial.print(digitalRead(puzzle2OverridePin));
    Serial.print(" : ");
    Serial.print(digitalRead(puzzle3OverridePin));
    Serial.print(" : ");
    Serial.print(digitalRead(puzzle4OverridePin));
    Serial.print(" : ");
    Serial.print(digitalRead(resetSwitchPin));
    Serial.println();

    Serial.print(digitalRead(lightBarSwitch));
    Serial.print(" : ");
    Serial.print(digitalRead(motionSensorPin));
    Serial.print(" : ");
    Serial.print(analogRead(touchSensorPin));
    Serial.println();
  */

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
