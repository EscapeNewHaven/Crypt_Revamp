/*
Jhonatan M. Nagasako
Hand-in-Hand
Crypt
Two silver pannels are bridge to connect a circuit which unlocks the escape
door and plays music
 */


const int hand = 2;   // active low
const int music = 13; // active low
const int door = 8;   // active low


void setup() {
  
  pinMode(hand, INPUT_PULLUP); // store bought circuit
  // Store bought circuit from this vendor
  // http://www.easternvoltageresearch.com/bodylight10.html
  // used the negative side of output terminal
  
  pinMode(music, OUTPUT);
  pinMode(door,OUTPUT);

  digitalWrite(music,HIGH);
  digitalWrite(door,HIGH);

}

void loop() {

  // Take a sample once every 10ms for 250ms, they all have to be low (strong debounce)
  int cleanread = 0;
  for(int i = 0; i < 25; i++){
    cleanread |= digitalRead(hand);
    delay(10);
  }

  if(!cleanread){
    digitalWrite(music,LOW);
    digitalWrite(door,LOW);
    delay(30000);
    digitalWrite(music,HIGH);
    digitalWrite(door,HIGH);
  }
  
}
