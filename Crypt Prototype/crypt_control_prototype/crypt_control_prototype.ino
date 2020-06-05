
/*
  Crypt Game Prototype

  Escape New Haven
  Justin C Kirk
  2020

*/





// GPIOs

//Puzzle 1
int lightBarSwitch = 39;
int motionSensorPin = 38;
int filbCabinet = 37;
int lightbarLED = 8;
int lightingLED = 7;

//Puzzle 2
int torch1Reed = 36;
int torch2Reed = 35;
int torch3Reed = 34;
int torch4Reed = 33;
int torch5Reed = 32;
int coffinReed = 31;

int torchLED = 12;
int coffinLED;

//Puzzle 3
int candleReed = 30;
int skullReed = 29;
int demon1Reed = 28;
int demon2Reed = 27;
int demon3Reed = 26;
int candleLED = 11;
int skullLED = 10;
int mirrorLED = 9;

//Puzzle 4
int touchSensorPin = 0;  //analog 0

//General
int puzzle1Override = 25;
int puzzle2Override = 24;
int puzzle3Override = 23;
int puzzle4Override = 22;
int resetSwitch = 21;

int inputCount = 19;
int inputPins[inputCount] = {};
int outputCount = 6;
int outputPins[outputCount] = {};


//Puzzle 1 Vals
boolean lightsOff = false;
boolean standingStill = false;
boolean filmCabinetOpen = false;
boolean puzzle1Conditions[] = {lightsOff, standingStill, filmCabinetOpen };
int lightbarLED = 15;

//Puzzle 2 Vals
boolean torch1Placed = false;
boolean torch2Placed = false;
boolean torch3Placed = false;
boolean torch4Placed = false;
boolean torch5Placed = false;
boolean coffinOpen = false;
boolean torchesPlaced = false;
boolean puzzle2Conditions[] = {torch1Placed, torch2Placed, torch3Placed, torch4Placed, torch5Placed, coffinOpen};
int torchLED = 16;

//Puzzle 3 Vals
boolean candlePlaced = false;
boolean skullPlaced = false;
boolean demon1Placed = false;
boolean demon2Placed = false;
boolean demon3Placed = false;
int candleLED = 17;
int skullLED = 18;
int mirrorLED = 19;

//Puzzle 4 Vals
int touchVal = 0;
int touchThreshold = 2;
boolean isTouching = false;

unsigned long cryptCurrent = 0;
unsigned long cryptPrevious = 0;
const long cryptInterval = 10;

int cryptState = 0;
boolean updateCrypt = false;

int puzzle1 = 1;
int puzzle2 = 2;
int puzzle3 = 3;
int puzzle4 = 4;


void getCryptState(int i) {
  updateCrypt = true;
  cryptState = i;
}

void setup() {

  Serial.begin(115200);

  for (int i = 0; i < inputCount; i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
  }

  for (int i = 0; i < outputCount; i++) {
    pinMode(outputPins[i], OUTPUT);
  }
}

void loop() {

  cryptCurrent = millis();
  if (cryptCurrent - cryptPrevious >= cryptInterval) {

    getResetState() ;

    switch (cryptState) {
      case puzzle1:
        if (updateCrypt == true) {
          //check stuff
          updateCrypt = false;
        }
        //getPuzzle1Vals();
        //check reset state
        //check puzzle sensors
        //check if puzzle is sovled
        //if solved, go to next state
        break;

      case puzzle2:
        if (updateCrypt == true) {
          //check stuff
          updateCrypt = false;
        }
        //getPuzzle2Vals();
        //check reset state
        //check puzzle sensors
        //check if puzzle is sovled
        //if solved, go to next state
        break;

      case puzzle3:
        if (updateCrypt == true) {
          //check stuff
          updateCrypt = false;
        }
        //getPuzzle3Vals();
        //check reset state
        //check puzzle sensors
        //check if puzzle is sovled
        //if solved, go to next state
        break;

      case puzzle4:
        if (updateCrypt == true) {
          //check stuff
          updateCrypt = false;
        }
        //getPuzzle4Vals();
        //check reset state
        //check puzzle sensors
        //check if puzzle is sovled
        //if solved, go to next state
        break;

    }
    cryptPrevious = cryptCurrent;
  }
}

void getPuzzle1Vals() {

  //Get Sesnor readings and update winning conditions
  if (digitalRead(motionSensor) == LOW) {
    standingStill = true;
  } else {
    standingStill = false;
  }

  if (ADC_ARRAY[0] == 1023) {
    filmCabinetOpen = !filmCabinetOpen;
  }

  if (digitalRead(lightSwitchPin) == LOW) {
    lightsOff = false;
    SoftPWMSet(lightbarLED, 200);

  } else {
    lightsOff = true;
    SoftPWMSet(lightbarLED, 0);
  }

  Serial.print(standingStill);
  Serial.print("\t");
  Serial.print(filmCabinetOpen); //film cabinet reed
  Serial.print("\t");
  Serial.print(digitalRead(lightSwitchPin));
  Serial.print("\t");
  Serial.print(lightsOff); //lightbar switch

  //Check amount of conditions met
  int puzzle1Sum = standingStill + filmCabinetOpen + lightsOff;

  Serial.print("\tPuzzle 1 Sum: ");
  Serial.println(puzzle1Sum);

  //If all conditions met, grant prize
  if (puzzle1Sum == 3) {
    Serial.println("SCARY MUSIC PLAYS");
    Serial.println("LIGHTNING FLASHES");
    //delay(2000);

  }

}

void getPuzzle2Vals() {

  if (ADC_ARRAY[1] == 1023) {
    torch1Placed = !torch1Placed;
  }
  if (ADC_ARRAY[2] == 1023) {
    torch2Placed = !torch2Placed;
  }
  if (ADC_ARRAY[3] == 1023) {
    torch3Placed = !torch3Placed;
  }
  if (ADC_ARRAY[4] == 1023) {
    torch4Placed = !torch4Placed;
  }
  if (ADC_ARRAY[5] == 1023) {
    torch5Placed = !torch5Placed;
  }

  if (ADC_ARRAY[6] == 1023) {
    coffinOpen = !coffinOpen;
  }

  Serial.print(torch1Placed);
  Serial.print("\t");
  Serial.print(torch2Placed);
  Serial.print("\t");
  Serial.print(torch3Placed);
  Serial.print("\t");
  Serial.print(torch4Placed);
  Serial.print("\t");
  Serial.print(torch5Placed);
  Serial.print("\t");
  Serial.print(coffinOpen);

  int puzzle2Sum = torch1Placed + torch2Placed + torch3Placed + torch4Placed + torch5Placed;

  Serial.print("\tPuzzle 2 Sum: ");
  Serial.println(puzzle2Sum);

  if (puzzle2Sum == 5) {
    SoftPWMSet(torchLED, 200);
    Serial.print("COFFIN KNOCKS");
    if (coffinOpen == true) {
      Serial.print("\tCOFFIN OPEN");
    }
  } else {
    SoftPWMSet(torchLED, 0);
  }
}

void getPuzzle3Vals() {

  if (ADC_ARRAY[7] == 1023) {
    candlePlaced = !candlePlaced;
  }
  if (ADC_ARRAY[8] == 1023) {
    skullPlaced = !skullPlaced;
  }
  if (ADC_ARRAY[9] == 1023) {
    demon1Placed = !demon1Placed;
  }
  if (ADC_ARRAY[10] == 1023) {
    demon2Placed = !demon2Placed;
  }
  if (ADC_ARRAY[11] == 1023) {
    demon3Placed = !demon3Placed;
  }

  Serial.print(candlePlaced);//candle
  Serial.print("\t");
  Serial.print(skullPlaced);//skull
  Serial.print("\t");
  Serial.print(demon1Placed);//demon 1
  Serial.print("\t");
  Serial.print(demon2Placed);//deomn 2
  Serial.print("\t");
  Serial.print(demon3Placed); //demon 3

  if (candlePlaced == true) {
    SoftPWMSet(candleLED, 200);
  } else {
    SoftPWMSet(candleLED, 0);
  }

  if (skullPlaced == true) {
    SoftPWMSet(skullLED, 200);
  } else {
    SoftPWMSet(skullLED, 0);
  }

  if (demon1Placed == true && demon2Placed == true && demon3Placed == true) {
    //Demon LED ON
  } else {
    //Demon LED OFF
  }

  int puzzle3sum = candlePlaced + skullPlaced + demon1Placed + demon2Placed + demon3Placed;

  Serial.print("\tPuzzle 3 Sum:");
  Serial.println(puzzle3sum);

  if (puzzle3sum == 5) {
    SoftPWMSet(mirrorLED, 200);
    Serial.println("THE BEAST!!!!!");
  } else {
    SoftPWMSet(mirrorLED, 0);
  }

}

void getPuzzle4Vals() {

  Serial.println(ADC_ARRAY[13]);

  if (ADC_ARRAY[13] >= touchThreshold) {
    Serial.print("TOUCHING");
    Serial.println("\tESCAPED THE GAME!");
  }

}
