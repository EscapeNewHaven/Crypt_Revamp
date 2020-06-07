/*


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
