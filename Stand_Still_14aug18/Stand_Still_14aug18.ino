/*
Edited by Jeremy Medow 14aug18
Added test of cabient open (NO switch to ground)
Consolidated rest of code

 */
const int motion1 = 6;
const int motion2 = 7;
const int photo = A3;
const int cabinet = 5;    // NC switch to ground (connection breaks when cabinet open)

int has_opened = 0;

int lightSig = 8;
int magLock= 9;
long t_elapsed = 0;
long t_start = millis();
int threshold = 800; //how dark it has to be to be dark (out of 1023)


void setup() {
  //configure pins as an inputs and enable the internal pull-up resistor
  pinMode(motion1, INPUT_PULLUP);
  pinMode(motion2, INPUT_PULLUP);
  pinMode(photo, INPUT_PULLUP);
  pinMode(cabinet, INPUT_PULLUP);
  // configure outputs and set their initial state
  pinMode(lightSig, OUTPUT);
  pinMode(magLock, OUTPUT);
  digitalWrite(lightSig, LOW);
  digitalWrite(magLock, HIGH);
}

void loop() {

  if(has_opened){
  
    if(analogRead(photo) <= threshold) { //if lights are on (analog, DIY thresh)
      t_elapsed = 0;               //reset counter
      t_start = millis();          //reset start time
      delay(5);
    }
    else{
      t_elapsed = millis() - t_start; //count
      if(digitalRead(motion1) == HIGH && digitalRead(motion2) == HIGH && t_elapsed >= 10000) {
        winCondition();
      }
      else{
        delay(5);
      }
    }

  }
  else {  // Check cabinet opened
    has_opened = 1;
    for(int i = 0; i < 20; i++){  // 20 readings over 1/2 second, all cabinet open
      has_opened &= digitalRead(cabinet);
      delay(25);
    }
    t_elapsed = 0;
    t_start = millis();   // So opening the cabinet doesn't immediately trigger it
  }
}

void winCondition(){
  digitalWrite(lightSig, HIGH); //LEDs on
  digitalWrite(magLock, LOW);   //maglock open
  delay(250);
  digitalWrite(lightSig, LOW); // turn of lights, editted by Jhonatan Nagasako jan 4, 2016
  while(1);
}

