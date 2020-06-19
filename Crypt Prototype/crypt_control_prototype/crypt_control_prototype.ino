/*

  Crypt Game Prototype

  Escape New Haven
  Justin C Kirk
  2020

*/

//Libraries for audio player
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// SPI pins
// Make sure to use hardware SPI pins.
// See http://arduino.cc/en/Reference/SPI

const int BOARD_CLK = 52;
const int BOARD_MISO = 51;
const int BOARD_MOSI = 50;
const int BOARD_RESET = 49;
const int BOARD_CS = 48;
const int BOARD_XDCS = 47;
const int BOARD_SDCS = 46;
//Use a hardware inerrupt pin for DREQ
// See https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
const int BOARD_DREQ = 21;
Adafruit_VS1053_FilePlayer audioPlayer = Adafruit_VS1053_FilePlayer(BOARD_RESET, BOARD_CS, BOARD_XDCS, BOARD_DREQ, BOARD_SDCS);

//Library to handle button inputs
#include <Bounce2.h>

//Set to 0 to stop printing
#define PRINT_DEBUG 1

// GPIOs

//Puzzle 1
const int lightBarSwitch = 14;
const int motionSensorPin = 15;
const int filmCabinetReed = 39;

const int lightBarLED = 12;
const int lightningLED = 11;

//Puzzle 2
const int torch1Reed = 37;
const int torch2Reed = 36;
const int torch3Reed = 35;
const int torch4Reed = 34;
const int torch5Reed = 33;
const int coffinReed = 32;

const int torchLED = 10;
const int coffinUVLED = 9;

//Puzzle 3
const int candleReed = 31;
const int skullReed = 30;
const int demon1Reed = 29;
const int demon2Reed = 28;
const int demon3Reed = 27;

const int candleLED = 8;
const int skullLED = 7;
const int mirrorLED = 6;

//Puzzle 4
const int touchSensorPin = 0;  //analog 0

//General
const int puzzle1OverridePin = 26;
const int puzzle2OverridePin = 25;
const int puzzle3OverridePin = 24;
const int puzzle4OverridePin = 23;

boolean puzzleOverriden = false;

const int resetSwitchPin = 22;

const int inputPins[] = {lightBarSwitch, motionSensorPin, filmCabinetReed,
                         torch1Reed, torch1Reed, torch1Reed, torch1Reed, torch1Reed, coffinReed,
                         candleReed, skullReed, demon1Reed, demon2Reed, demon3Reed,
                         puzzle1OverridePin, puzzle2OverridePin, puzzle3OverridePin, puzzle4OverridePin, resetSwitchPin
                        };

const int inputCount = sizeof(inputPins) / 2;
//int inputCount = 19;

const int leds[] = {lightBarLED, lightningLED, torchLED, coffinUVLED, candleLED, skullLED, mirrorLED};
const int ledCount = sizeof(leds) / 2;




//Game Variables


//Puzzle 1 Vals
boolean lightsOff = false;
boolean standingStill = false;
const int standStillTime = 0;
boolean filmCabinetOpen = false;
boolean puzzle1Conditions[] = {lightsOff, standingStill, filmCabinetOpen };
boolean puzzle1Complete = false;

//Puzzle 2 Vals
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

//Puzzle 3 Vals
boolean candlePlaced = false;
boolean skullPlaced = false;
boolean demon1Placed = false;
boolean demon2Placed = false;
boolean demon3Placed = false;
boolean demonsPlaced = false;
boolean puzzle3Complete = false;

//Puzzle 4 Vals
const int touchThreshold = 500;
boolean isTouching = false;
boolean puzzle4Complete = false;


//Game Vals
unsigned long cryptCurrent = 0;
unsigned long cryptPrevious = 0;
const long cryptInterval = 100;

int cryptState = -1;
boolean updateCrypt = false;

//game states
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
  delay(1000);

  //Setup Audio Player

  if (!audioPlayer.begin()) {
    Serial.println(F("Cannot find audio player.  Check pins."));
    while (1);
  }
  Serial.println(F("Audio Player Found!"));

  //Play Test Tone
  audioPlayer.sineTest(0x44, 500);

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD read error.  Is card inserted?"));
    while (1);
  }
  Serial.println("SD Card Found!");

  //List SD Card Contents
  printDirectory(SD.open("/"), 0);

  // Set volume for left, right channels.
  // Lower numbers == Louder volume!
  audioPlayer.setVolume(20, 20);





  for (int i = 0; i < inputCount; i++) {
    pinMode(inputPins[i], INPUT);
  }

  Serial.println("Setup LEDs");
  Serial.println(ledCount);

  for (int i = 0; i < ledCount; i++) {
    pinMode(leds[i], OUTPUT);
    Serial.print(i);

    //analogWrite(leds[i], 0);
  }

  Serial.println();
  delay(1000);

  Serial.println("Crypt Control");

  delay(1000);

  getCryptState(resetState);

}


void loop() {

  //  //Check Reset State
  if (getResetState() == 1) {
    getCryptState(resetState);
  }


  switch (cryptState) {

    case resetState:

      if (updateCrypt == true) {
        Serial.println("RESET STATE");

        //Reset all variables

        //Reset Puzzle 1
        puzzle1Complete = false;
        lightsOff = false;
        standingStill = false;
        filmCabinetOpen = false;
        analogWrite(lightningLED, 0);

        //Reset Puzzle 2
        puzzle2Complete = false;
        analogWrite(torchLED, 0);
        analogWrite(coffinUVLED, 0);
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


        //Open all maglocks
        delay(2000);
        //close all maglocks
        Serial.println("START");

        updateCrypt = false;
      }

      //Start game
      // getCryptState(puzzle4);

      //Test functions
      //puzzleInputTest();
      //ledTest(0);
      //audioPlayerTest();

      break;

    case puzzle1:

      if (updateCrypt == true) {
        Serial.println(">Puzzle 1");
        delay(1000);
        digitalWrite(lightBarLED, LOW);
        digitalWrite(lightningLED, LOW);
        puzzle1Complete = false;
        lightsOff = false;
        standingStill = false;
        filmCabinetOpen = false;


        updateCrypt = false;
      }
      /*
           //Loop play music

                 audioPlayer.playFullFile("playMusic.wav");
                 if (audioPlayer.stopped) {
                   audioPlayer.playFullFile("playMusic.wav");
                 }
      */

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

        int motionVal = digitalRead(motionSensorPin);
        if (motionVal == 0) {
          standingStill = true;
        } else {
          standingStill = false;
          standStillTime = 0;
        }

        int filmCabinetVal = digitalRead(filmCabinetReed);
        if (filmCabinetVal == 1) {
          filmCabinetOpen = !filmCabinetOpen;
        }

        int puzzle1Sum = lightsOff + standingStill + filmCabinetOpen;
        int puzzle1OverrideVal = digitalRead(puzzle1OverridePin);
        // Serial.println(puzzle1OverrideVal);

        if (puzzle1OverrideVal == 1) {
          puzzle1Complete = true;
        }


        if (puzzle1Sum < 3) {
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
        } else if (puzzle1Sum == 3) {
          standStillTime += 1;
          Serial.print("Standing Stil For : ");
          Serial.println(standStillTime);
          if (standStillTime >= 100) {
            puzzle1Complete = true;
          }
        }


        if (puzzle1Complete == true) {
          Serial.println("PUZZLE COMPLETE!");
          Serial.println("GAME MUSIC DIMS");
          audioPlayer.stopPlaying();
          delay(1000);
          audioPlayer.startPlayingFile("thunderClap.wav");
          //Need to remove the delays from the lightning flash

          Serial.println("LIGHTNING FLASH!!");
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

          Serial.println("SCARY MUSIC PLAYS");
          if (audioPlayer.stopped()) {
            audioPlayer.startPlayingFile("scaryMusic.wav");
          }
          Serial.println("DOOR TO PUZZLE 2 OPENS");
          delay(2000);
          getCryptState(puzzle2);
        }
        cryptPrevious = cryptCurrent;
      }

      break;

    case puzzle2:

      if (updateCrypt == true) {
        Serial.println(">Puzzle 2");
        delay(1000);
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
      if (audioPlayer.stopped()) {
        audioPlayer.startPlayingFile("scaryMusic.wav");
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


        int torch1Val = digitalRead(torch1Reed);
        if (torch1Val == 1) {
          torch1Placed = !torch1Placed;
        }

        int torch2Val = digitalRead(torch2Reed);
        if (torch2Val == 1) {
          torch2Placed = !torch2Placed;
        }

        int torch3Val = digitalRead(torch3Reed);
        if (torch3Val == 1) {
          torch3Placed = !torch3Placed;
        }

        int torch4Val = digitalRead(torch4Reed);
        if (torch4Val == 1) {
          torch4Placed = !torch4Placed;
        }

        int torch5Val = digitalRead(torch5Reed);
        if (torch5Val == 1) {
          torch5Placed = !torch5Placed;
        }

        int coffinVal = digitalRead(coffinReed);
        if (coffinVal == 1) {
          coffinOpen = !coffinOpen;
        }

        int torchValSum = torch1Placed + torch2Placed + torch3Placed + torch4Placed + torch5Placed;
        int puzzle2OverrideVal = digitalRead(puzzle2OverridePin);

        if (puzzle2OverrideVal == 1) {
          puzzle2Complete = true;
        }

        if (torchValSum < 5) {

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
          //LEDs off if torches are not all placed
          analogWrite(torchLED, 0);
          analogWrite(coffinUVLED, 0);
        } else if (torchValSum == 5) {
          Serial.println("Torches Placed");
          puzzle2Complete = true;
          Serial.println("Puzzle Complete");
        }

        if (puzzle2Complete == true) {
          Serial.println("PUZZLE COMPLETE!");
          analogWrite(torchLED, 200);
          //activate coffin knocker
          // 3 times every 10 seconds
          Serial.println("KNOCK KNOCK");

          //In real game, coffin led will turn on only after the coffin has been opened once
          analogWrite(coffinUVLED, 200);

          delay(2000);
          getCryptState(puzzle3);
        }

        cryptPrevious = cryptCurrent;
      }

      break;

    case puzzle3:

      if (updateCrypt == true) {
        Serial.println(">Puzzle 3");
        delay(1000);
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
        }

        if (candlePlaced) {
          analogWrite(candleLED, 200);
        } else {
          analogWrite(candleLED, 0);
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
          analogWrite(mirrorLED, 0);

        } else if (puzzle3Sum == 5) {
          puzzle3Complete = true;
        }

        if (puzzle3Complete == true) {
          //Play audio track?
          //Dim lights?
          Serial.println("!!!!!THE BEAST!!!!!");
          analogWrite(mirrorLED, 200);
          delay(2000);
          getCryptState(puzzle4);
        }

        cryptPrevious = cryptCurrent;
      }

      break;

    case puzzle4:
      if (updateCrypt == true) {
        Serial.println(">Puzzle 4");
        delay(1000);
        puzzle4Complete = false;
        updateCrypt = false;
      }

      cryptCurrent = millis();
      if (cryptCurrent - cryptPrevious >= cryptInterval) {

        int touchVal = analogRead(touchSensorPin);

        Serial.print("Touch Val: ");
        Serial.println(touchVal);

        if (touchVal > touchThreshold) {
          Serial.print("Touch Val: ");
          Serial.println(touchVal);
        } else if (touchVal < touchThreshold) {
          puzzle4Complete = true;
        }

        int puzzle4OverrideVal = digitalRead(puzzle4OverridePin);
        if (puzzle4OverrideVal == 1) {
          puzzle4Complete = true;
        }

        if (puzzle4Complete == true) {
          Serial.println("ESCAPED THE GAME!!");
          delay(2000);
          getCryptState(winnerState);
        }
        cryptPrevious = cryptCurrent;
      }
      break;

    case winnerState:
      Serial.println(">Winner State");
      Serial.println("Congrats!");
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

  for (int i = 0; i < ledCount; i++) {
    analogWrite(leds[i], val);
  }

}

void puzzleInputTest() {

  //int inputVal = digitalRead(24);
  //Serial.println(inputVal);

  //Buttons
  /*
    Serial.print(digitalRead(39));
    Serial.print(" : ");
    Serial.print(digitalRead(37));
    Serial.print(" : ");
    Serial.print(digitalRead(36));
    Serial.print(" : ");
    Serial.print(digitalRead(35));
    Serial.print(" : ");
    Serial.print(digitalRead(34));
    Serial.print(" : ");
    Serial.print(digitalRead(33));
    Serial.print(" : ");
    Serial.print(digitalRead(32));
    Serial.print(" : ");
    Serial.print(digitalRead(31));
    Serial.print(" : ");
    Serial.print(digitalRead(30));
    Serial.print(" : ");
    Serial.print(digitalRead(29));
    Serial.print(" : ");
    Serial.print(digitalRead(28));
    Serial.print(" : ");
    Serial.print(digitalRead(27));
    Serial.print(" : ");
    Serial.print(digitalRead(26));
    Serial.print(" : ");
    Serial.print(digitalRead(25));
    Serial.print(" : ");
    Serial.print(digitalRead(24));
    Serial.print(" : ");
    Serial.print(digitalRead(23));
    Serial.print(" : ");
    Serial.print(digitalRead(22));
    Serial.println();
  */


  //Switch
  Serial.print(digitalRead(14));
  Serial.print(" : ");
  //Motion
  Serial.print(digitalRead(15));
  Serial.print(" : ");
  //Touch
  Serial.print(analogRead(0));
  Serial.println();


  /*
    for (int i = 0; i <= inputCount; i++) {
      int inputVal = digitalRead(inputPins[i]);
      if (i == inputCount - 1) {
        Serial.println();
      } else {
        Serial.print(inputVal);
        Serial.print(" : ");
      }
    }
  */

}
