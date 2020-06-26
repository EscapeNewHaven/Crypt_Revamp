

/*


      if (puzzle1Complete == false) {
        //Loop play music
        audioPlayer.startPlayingFile("play.mp3");

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

            int motionVal = digitalRead(motionSensorPin);
            if (motionVal == 0) {
              standingStill = true;
            } else {
              standingStill = false;
              standStillTime = 0;
            }



            int puzzle1Sum = lightsOff + standingStill + filmCabinetOpen;

            int puzzle1OverrideVal = digitalRead(puzzle1OverridePin);
            if (puzzle1OverrideVal == 1) {
              puzzle1Complete = true;
            }

            if (puzzle1Sum < 3) {
              audioPlayer.setVolume(20, 20);
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
              } else if (standStillTime < 100) {
                audioPlayer.setVolume(standStillTime, standStillTime);
              }

              if (DEBUG) {
                Serial.print("Standing Stil For : ");
                Serial.println(standStillTime);
              }
            }

            cryptPrevious = cryptCurrent;
          }
        }

      } else if (puzzle1Complete == true) {

        audioPlayer.stopPlaying();

        if (DEBUG) {
          Serial.println("PUZZLE COMPLETE!");
          Serial.println("LIGHTNING FLASH!!");
        }

        audioPlayer.setVolume(20, 20);
        audioPlayer.startPlayingFile("thunder.mp3");
        //  while (audioPlayer.playingMusic) {

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
        //  }

        if (audioPlayer.stopped()) {
          getCryptState(puzzle2);
        }
      }








 */




















































/*

  //Puzzle 1
  //Lightbar Switch
  Serial.print(inputs[0].read());
  Serial.print(" : ");
  //Motion Sensor
  Serial.print(inputs[1].read());
  Serial.print(" : ");
  //Film Cabinet Reed
  Serial.print(inputs[2].read());
  Serial.println();
  /*
    //Puzzle 2
    //Torch 1
    Serial.print(inputs[3].read());
    Serial.print(" : ");
    //Torch 2
    Serial.print(inputs[4].read());
    Serial.print(" : ");
    //Torch 3
    Serial.print(inputs[5].read());
    Serial.print(" : ");
    Serial.print(inputs[6].read());
    Serial.print(" : ");
    //Torch 4
    Serial.print(inputs[7].read());
    Serial.print(" : ");
    //Torch 5
    Serial.print(inputs[8].read());
    Serial.print(" : ");
    //Coffin UV
    Serial.print(inputs[9].read());
    Serial.println();

    //Puzzle 3
    //Candle
    Serial.print(inputs[10].read());
    Serial.print(" : ");
    //Skull
    Serial.print(inputs[11].read());
    Serial.print(" : ");
    //Demon 1
    Serial.print(inputs[12].read());
    Serial.print(" : ");
    //Demon 2
    Serial.print(inputs[13].read());
    Serial.print(" : ");
    //Demon 3
    Serial.print(inputs[14].read());
    Serial.print(" : ");
  //General Inputs
    //Puzzle 1 Override
    Serial.print(inputs[15].read());
    Serial.print(" : ");
    //Puzzle 2 Override
    Serial.print(inputs[16].read());
    Serial.print(" : ");
    //Puzzle 3 Override
    Serial.print(inputs[17].read());
    Serial.print(" : ");
    //Puzzle 4 Override
    Serial.print(inputs[18].read());
    Serial.print(" : ");
    //Game Reset
    Serial.print(inputs[19].read());
    Serial.println();

     //Update state of all inputs
  for (int i = 0; i < inputCount; i++) {
    inputs[i].update();
  }
          if (inputs[0].read() == 1) {
            lightsOff = true;
            analogWrite(lightBarLED, 0);
          } else if (inputs[0].read() == 0) {
            lightsOff = false;
            analogWrite(lightBarLED, 255);
          }
          //Loop play music
          //audioPlayer.playFullFile("test.wav");

          //if (audioPlayer.stopped()) {
          //  audioPlayer.playFullFile("test.wav");
          // }

  //Puzzle 1
  //Lightbar Switch
  Serial.print(inputs[0].read());
  Serial.print(" : ");
  //Motion Sensor
  Serial.print(inputs[1].read());
  Serial.print(" : ");
  //Film Cabinet Reed
  Serial.print(inputs[2].read());
  Serial.println();

  //Puzzle 2
  //Torch 1
  Serial.print(inputs[3].read());
  Serial.print(" : ");
  //Torch 2
  Serial.print(inputs[4].read());
  Serial.print(" : ");
  //Torch 3
  Serial.print(inputs[5].read());
  Serial.print(" : ");
  Serial.print(inputs[6].read());
  Serial.print(" : ");
  //Torch 4
  Serial.print(inputs[7].read());
  Serial.print(" : ");
  //Torch 5
  Serial.print(inputs[8].read());
  Serial.print(" : ");
  //Coffin UV
  Serial.print(inputs[9].read());
  Serial.println();

  //Puzzle 3
  //Candle
  Serial.print(inputs[10].read());
  Serial.print(" : ");
  //Skull
  Serial.print(inputs[11].read());
  Serial.print(" : ");
  //Demon 1
  Serial.print(inputs[12].read());
  Serial.print(" : ");
  //Demon 2
  Serial.print(inputs[13].read());
  Serial.print(" : ");
  //Demon 3
  Serial.print(inputs[14].read());
  Serial.print(" : ");

  //General Inputs
  //Puzzle 1 Override
  Serial.print(inputs[15].read());
  Serial.print(" : ");
  //Puzzle 2 Override
  Serial.print(inputs[16].read());
  Serial.print(" : ");
  //Puzzle 3 Override
  Serial.print(inputs[17].read());
  Serial.print(" : ");
  //Puzzle 4 Override
  Serial.print(inputs[18].read());
  Serial.print(" : ");
  //Game Reset
  Serial.print(inputs[19].read());
  Serial.println();























          //Puzzle lights off
          digitalWrite(lightningLED, LOW);
          digitalWrite(torchLED, LOW);
          digitalWrite(coffinUVLED, LOW);
          digitalWrite(candleLED, LOW);
          digitalWrite(skullLED, LOW);
          digitalWrite(mirrorLED, LOW);

          //Reset all puzzles
          puzzle1Complete = false;
          puzzle2Complete = false;
          puzzle3Complete = false;
          puzzle4Complete = false;







        Serial.println(updateCrypt);
        /*
          
*/



/*
  else {
  if (puzzle1Sum == 3 ) {
    standStillTime += 1;
    Serial.print("StandStill Time: ");
    Serial.println(standStillTime);
    if (standStillTime > 10) {
      puzzle1Complete = true;
    }
  } else {
    standStillTime = 0;
    Serial.println();
  }
  }
*/

/*


*/


/*

     
        if (puzzle1OverrideVal == 1) {
          puzzleOverriden = !puzzleOverriden;
          Serial.println(puzzleOverriden);
          Serial.println();
          Serial.println("LIGHTNING FLASHES!");
          Serial.println("PUZZLE 1 COMPLETE!");
         // delay(1000);
          getCryptState(puzzle2);
        }


        if (puzzle1Complete == true) {
          //flash lightning led
          Serial.println();
          Serial.println("LIGHTNING FLASHES!");
          Serial.println("PUZZLE 1 COMPLETE!");
          delay(1000);
          getCryptState(puzzle3);
         // puzzle1Complete = false;//?
        }

*/
/*
    cryptCounter += 1;
    Serial.print("Crypt State: ");
    Serial.println(cryptCounter);
    getCryptState(cryptCounter);

    if (cryptCounter > 4) {
      cryptCounter = 0;
    }



    //For denbu
int cryptCounter = 0;




  //check puzzle overrides
  // Serial.print(cryptState);
  // Serial.print("@");
  // Serial.println(millis());

*/


/*

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

void loop() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;


  // turns on the LED every four button pushes by checking the modulo of the
  // button push counter. the modulo function gives you the remainder of the
  // division of two numbers:
  if (buttonPushCounter % 4 == 0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

}
*/
