/* Crypt Torch Puzzle
 *  
 *  When all the magnets read LOW,
 *  Light up all the LEDs.
 */

// variables for pin assignment
int LEDPin = 3;
int magPins[] = {2};
int brightness = 0;

void setup() {
  for (int i=0; i<sizeof(magPins); i++) {
    pinMode(magPins[i], INPUT_PULLUP);
  }
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
//  Serial.begin(9600);
}

void loop() {
//  Serial.print(digitalRead(circuitPin));
//  Serial.println(digitalRead(doorPin));
  int fail = 0;
  for (int i=0; i<sizeof(magPins); i++) {
    if (digitalRead(magPins[i]) == HIGH) {
      fail = 1;
      delay(5);
    }
  }
  if (fail == 0) {
    for (int j=0; j<250; j++) {
      brightness = 250 - abs(2*(j-125));
      analogWrite(LEDPin, brightness);
      delay(10);
    }
  }
  else {
    digitalWrite(LEDPin, LOW);
  }
  delay(10);
}
