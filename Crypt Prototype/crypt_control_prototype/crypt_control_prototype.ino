/*

  Crypt Game Prototype

  Escape New Haven
  Justin C Kirk
  2020

*/

//Set to 0 to stop printing
#define PRINT_DEBUG 1

// GPIOs

//Puzzle 1
int lightBarSwitch = 20;
int motionSensorPin = 19;
int filmCabinetReed = 37;

int lightBarLED = 8;
int lightningLED = 6;

//Puzzle 2
int torch1Reed = 36;
int torch2Reed = 35;
int torch3Reed = 34;
int torch4Reed = 33;
int torch5Reed = 32;
int coffinReed = 31;

int torchLED = 12;
int coffinUVLED = 7;

//Puzzle 3
int candleReed = 30;
int skullReed = 29; //Had trouble with gpio#29
int demon1Reed = 28;
int demon2Reed = 27;
int demon3Reed = 26;

int candleLED = 11;
int skullLED = 10;
int mirrorLED = 9;

//Puzzle 4
int touchSensorPin = 0;  //analog 0

//General
int puzzle1OverridePin = 25;
int puzzle2OverridePin = 24;
int puzzle3OverridePin = 23;
int puzzle4OverridePin = 22;

boolean puzzleOverriden = false;

int resetSwitchPin = 21;

int inputPins[] = {lightBarSwitch, motionSensorPin, filmCabinetReed,
                   torch1Reed, torch1Reed, torch1Reed, torch1Reed, torch1Reed, coffinReed,
                   candleReed, skullReed, demon1Reed, demon2Reed, demon3Reed,
                   puzzle1OverridePin, puzzle2OverridePin, puzzle3OverridePin, puzzle4OverridePin, resetSwitchPin
                  };

int inputCount = sizeof(inputPins) / 2;
//int inputCount = 19;

int leds[] = {lightBarLED, lightningLED, torchLED, coffinUVLED, candleLED, skullLED, mirrorLED};
int ledCount = sizeof(leds) / 2;




//Game Variables


//Puzzle 1 Vals
boolean lightsOff = false;
boolean standingStill = false;
int standStillTime = 0;
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
int touchThreshold = 2;
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

  for (int i = 0; i < inputCount; i++) {
    pinMode(inputPins[i], INPUT);
  }

  for (int i = 0; i < ledCount; i++) {
    pinMode(leds[i], OUTPUT);
    analogWrite(leds[i], 0);
  }

  delay(1000);

  Serial.println("Crypt Control");

  delay(1000);

  getCryptState(resetState);

}


void loop() {

  //Test Inputs
  //puzzleInputTest();

  //Check Reset State
  if (getResetState() == 1) {
    getCryptState(resetState);
  }


  switch (cryptState) {

    case resetState:
      if (updateCrypt == true) {
        Serial.println("RESET STATE");
        //All LEDs off
        for (int i = 0; i < ledCount; i++) {
          analogWrite(leds[i], 0);
        }

        updateCrypt = false;
      }

      //Open all maglocks
      // delay(1000);
      //close all maglocks

      //Start game
      delay(2000);
      getCryptState(puzzle1);

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
          delay(1000);
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

        //Light switch in dressing room should work as normal
        int lightBarVal = digitalRead(lightBarSwitch);
        if (lightBarVal == 0) {
          analogWrite(lightBarLED, 0);
        } else {
          analogWrite(lightBarLED, 200);
        }

        int touchVal = analogRead(touchSensorPin);

        if (touchVal < touchThreshold) {
          Serial.print("Touch Val: ");
          Serial.println(touchVal);
        } else if (touchVal > touchThreshold) {
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





void puzzleInputTest() {

  //int inputVal = digitalRead(24);
  //Serial.println(inputVal);

  /*
    Serial.print(digitalRead(21));
    Serial.print(" : ");
    Serial.print(digitalRead(22));
    Serial.print(" : ");
    Serial.print(digitalRead(23));
    Serial.print(" : ");
    Serial.print(digitalRead(24));
    Serial.print(" : ");
    Serial.print(digitalRead(25));
    Serial.print(" : ");
    Serial.print(digitalRead(26));
    Serial.print(" : ");
    Serial.print(digitalRead(27));
    Serial.print(" : ");
    Serial.print(digitalRead(28));
    Serial.print(" : ");
    Serial.print(digitalRead(29));
    Serial.print(" : ");
    Serial.print(digitalRead(30));
    Serial.print(" : ");
    Serial.print(digitalRead(31));
    Serial.print(" : ");
    Serial.print(digitalRead(32));
    Serial.print(" : ");
    Serial.print(digitalRead(33));
    Serial.print(" : ");
    Serial.print(digitalRead(34));
    Serial.print(" : ");
    Serial.print(digitalRead(35));
    Serial.print(" : ");
    Serial.print(digitalRead(36));
    Serial.print(" : ");
    Serial.print(digitalRead(37));
    Serial.println();
    Serial.print(digitalRead(20));
    Serial.print(" : ");
    Serial.print(digitalRead(19));
    Serial.println();

  */

  for (int i = 0; i <= inputCount; i++) {
    int inputVal = digitalRead(inputPins[i]);
    if (i == inputCount - 1) {
      Serial.println();
    } else {
      Serial.print(inputVal);
      Serial.print(" : ");
    }
  }

}
