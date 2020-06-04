
/*
  Crypt Game Prototype

  Escape New Haven
  Justin C Kirk
  2020

  Proof of concept code for testing inputs and outputs.

  This version uses an Arduino Nano.

  Two MCP3008 ADC chips are used to extend the number of inputs.

  SoftPWM library is used to increase the number of pwm pins available.

  In future prototypes, an Arduino Mega will be used and the ADC chips as well as SoftPWM library
  will not be needed.


*/


#include <MCP3XXX.h>
MCP3008 adc1, adc2;
#include <SoftPWM.h>

//Two ADC chips are used for prototyping.
//These two chips can read 16 inputs using 4 GPIO pins
//This array stores values from 16 inputs.
//The first 13 entries are button inputs
//Entry 14 is the conductance sensor
//The last two are left unused
int ADC_ARRAY [16] = {};

//gpio
int motionSensor = 4;  //dressing room motion sensor
int lightSwitchPin  = 2;

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

void getCryptState(int i) {
  cryptState = i;
}

void setup() {

  Serial.begin(115200);

  adc1.begin(12, 11, 10, 9);
  adc2.begin(8, 7, 6, 5);

  SoftPWMBegin();
  SoftPWMSet(lightbarLED, 0);
  SoftPWMSet(torchLED, 0);
  SoftPWMSet(candleLED, 0);
  SoftPWMSet(skullLED, 0);
  SoftPWMSet(mirrorLED, 0);

  pinMode(motionSensor, INPUT_PULLUP);
  pinMode(lightSwitchPin, INPUT_PULLUP);

}

void loop() {

  cryptCurrent = millis();
  if (cryptCurrent - cryptPrevious >= cryptInterval) {

    getADC();
    //printADC();                                       // WMS - I assume these should be un-commented back in?
                                                        // Also, we'll need some kind of Reset Handler that can
    //getPuzzle1Vals();                                 // be triggered from any game state.
    //getPuzzle2Vals();
    //getPuzzle3Vals();
    //getPuzzle4Vals();

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

  if (ADC_ARRAY[0] == 1023) {                             // WMS - It looks like whenever the cabinet is open,
    filmCabinetOpen = !filmCabinetOpen;                   // it rapidly toggles the variable on and off?
  }                                                       // Very likely that I just don't know how the ADC works.
                                                          
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
  if (puzzle1Sum == 3) {                              // WMS - must stay true for 10 continuous seconds
    Serial.println("SCARY MUSIC PLAYS");
    Serial.println("LIGHTNING FLASHES");              // LED strip missing
    //delay(2000);

  }

}

void getPuzzle2Vals() {

  if (ADC_ARRAY[1] == 1023) {                   // WMS - looks like these are all "latching" (or...."toggling");
    torch1Placed = !torch1Placed;               // should be momentary, so if you remove one, it is no longer
  }                                             // "solved."
  if (ADC_ARRAY[2] == 1023) {                   // Also, should still listen for lightswitch
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

  if (puzzle2Sum == 5) {                          // WMS - I'd like a few seconds between completing the torches
    SoftPWMSet(torchLED, 200);                    // and the coffin knocks.
    Serial.print("COFFIN KNOCKS");
    if (coffinOpen == true) {
      Serial.print("\tCOFFIN OPEN");              // WMS - Coffin UV light strip not modeled?
    }
  } else {
    SoftPWMSet(torchLED, 0);
  }


}

void getPuzzle3Vals() {

  if (ADC_ARRAY[7] == 1023) {             // WMS - same comment about toggle vs. momentary
    candlePlaced = !candlePlaced;         // Also, should still listen for lightswitch IFF sum < 5
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
    //Demon LED ON                      // WMS - This if/else isn't necessary, but doesn't really hurt anything
  } else {
    //Demon LED OFF
  }

  int puzzle3sum = candlePlaced + skullPlaced + demon1Placed + demon2Placed + demon3Placed;

  Serial.print("\tPuzzle 3 Sum:");
  Serial.println(puzzle3sum);
    
  if (puzzle3sum == 5) {                // WMS - turn lightbar off, signal audio cues, kill coffin UV
    SoftPWMSet(mirrorLED, 200);
    Serial.println("THE BEAST!!!!!");
  } else {
    SoftPWMSet(mirrorLED, 0);
  }

}

void getPuzzle4Vals() {

  Serial.println(ADC_ARRAY[13]);

  if (ADC_ARRAY[13] >= touchThreshold) {          // WMS - dim sconces, trigger audio, release maglock
    Serial.print("TOUCHING");
    Serial.println("\tESCAPED THE GAME!");
  }

}

void getADC() {                                     // WMS - any reason you're using analogRead()
                                                    // instead of digitalRead()?
  for (size_t i = 0; i < adc1.numChannels(); ++i)
  {
    ADC_ARRAY[i] = adc1.analogRead(i);
  }

  for (size_t j = 0; j < adc2.numChannels(); ++j)
  {
    ADC_ARRAY[8 + j] = adc2.analogRead(j);
  }

}

void printADC() {

  for (int i = 0; i < 16; i++) {
    Serial.print(ADC_ARRAY[i]);
    if (i == 15) {
      Serial.println();
    } else {
      Serial.print("\t");
    }
  }

  /*
    for (int i = 0; i < 8; i++) {
      Serial.print(adc_1_vals[i]);
      if (i == 7) {
        Serial.print(" : ");
        for (int j = 0; j < 8; j++) {
          Serial.print(adc_2_vals[j]);
          if (j == 7) {
            Serial.println();
          } else {
            Serial.print(" : ");
          }
        }
      } else {
        Serial.print(" : ");
      }
    }
  */

}
