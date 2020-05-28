// Oct 23: added feedBuffer to main loop, added softReset to every new track play
// lowered softReset delay to 10ms (in library)


#include <Adafruit_VS1053.h>
#include <CANtroller.h>
#include <avr/wdt.h>
//#define ECHO

#ifdef ECHO
  #define echo(x) Serial.print(x)
  #define echoln(x) Serial.println(x)
#else
  #define echo(x)
  #define echoln(x)
#endif

const int UNIQUEPINS[]={A0,A1,A2,A3,A4,A5};   // Pins to poll for unique address

CANtroller AudioSlave(DEFAULT_CS_PIN);                           // Name it what you like

#define BASEFILENAME "/track000.ogg"

// Audio player definitions

#define VS_RST  8      // VS1053 reset pin (output)
#define VS_CS   7     // VS1053 chip select pin (output)
#define VS_DCS  5      // VS1053 Data/command select pin (output)
#define SD_CS   6         // SD Card chip select pin
#define VS_DREQ 3       // Helpful to have an interrupt pin, 2 is taken by CAN shield

Adafruit_VS1053_FilePlayer audio = 
  Adafruit_VS1053_FilePlayer(VS_RST, VS_CS, VS_DCS, VS_DREQ, SD_CS);

// The meat of the player

#define MINVOL  200
#define MAXVOL  1
uint8_t targetVolume = 40;    // in VS1053 terms, so 254 is quietest, 0 is loudest
uint8_t currentVolume = 50;
#define RESETVOLUME 30
#define VOLDELAY  100          // Time to go up/down one notch in volume
#define VOLSTEP 2
unsigned long lastVolChange = 0;
uint8_t forceVol = 0;
bool volFlag = false;

uint8_t trackNum = 0;
uint8_t playStatus = 0;   // As below
#define STOPPED 0
#define PLAYING 1
#define PAUSED  2
bool looping = 0;
#define STOPDELAY 50    // Delay between stopping and old file and starting a new
uint16_t tracksPlayed = 0;

#define STATUSDELAY 200   // How often to poll for play status
unsigned long lastStatusUpdate = 0;

uint16_t commander = 0; // The id of whoever sent the play command (for sending COMPLETE)

#define INT_PIN 2

/////
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
 
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
/////

void setup() {

  wdt_disable();

  #ifdef ECHO
    Serial.begin(115200);
  #endif

  if (!audio.begin()) { // initialise the music player
    echoln(F("Couldn't find VS1053, do you have the right pins defined?"));
    delay(1000);
  }
  echoln(F("VS1053 found"));
  
   if (!SD.begin(SD_CS)) {
    echoln(F("SD failed, or not present"));
    delay(1000);
  }

  audio.useInterrupt(VS1053_FILEPLAYER_PIN_INT);

  pinMode(VS_CS,OUTPUT);
  pinMode(SD_CS,OUTPUT);
  pinMode(INT_PIN,INPUT_PULLUP);

  pinMode(VS_DREQ,INPUT);
  digitalWrite(VS_DREQ,LOW);

  AudioSlave.initCAN(SLAVE,16+pollPins(UNIQUEPINS,ELEMENTS(UNIQUEPINS)));    // Pass type (master/slave, etc), unique id

  wdt_enable(WDTO_4S);

}

void loop() {

  audio.feedBuffer();

  if(!digitalRead(INT_PIN)){      // Only when there's something to do
    cli();
    if(AudioSlave.msgAvailable())
      handleMSG();
    sei();
  }

  updateAll(); // Smoothly changes volume (if necessary), updates playing status, restarts looped tracks, etc.

  wdt_reset();  
  
}

void updateAll(){

  if(volFlag){
    cli();
    if(forceVol){
      currentVolume = targetVolume;
      audio.setVolume(currentVolume,currentVolume);
      echoln("Forcing volume");
      forceVol = 0;
    } else {   
      if((millis()-lastVolChange)>VOLDELAY){
        lastVolChange = millis();
        if(currentVolume < targetVolume)
          currentVolume+=(currentVolume>(targetVolume-VOLSTEP))?(targetVolume-currentVolume):VOLSTEP;
        else
          currentVolume-=(currentVolume<(targetVolume+VOLSTEP))?(currentVolume-targetVolume):VOLSTEP;
        audio.setVolume(currentVolume,currentVolume);
      }
    }
    volFlag = (currentVolume!=targetVolume);
    sei();
  }
  
  if((millis()-lastStatusUpdate)>STATUSDELAY){      // Just in case something wacky happens, check no matter what you *think* is happening
    cli();
    lastStatusUpdate = millis();  
    if(audio.stopped()){
      if((playStatus == PLAYING) && (!looping) ){     // Nothing is playing but at last check it was, so send COMPLETE
        AudioSlave.msgSendForACK(commander,COMPLETE);
        echoln("Complete sent");
      }
      playStatus = STOPPED;
      if(looping)
        playTrack(trackNum);
    }
    else 
      playStatus = audio.paused()?PAUSED:PLAYING;
 //   echoln(playStatus);
    sei();
  }
  
}

void playTrack(uint8_t which){
  char trackname[15] = BASEFILENAME;

  trackname[6] = (which/100)%10 + '0';
  trackname[7] = (which/10)%10 + '0';
  trackname[8] = (which/1)%10 + '0';

  cleanStop();
  if(audio.startPlayingFile(trackname)){
    playStatus = PLAYING;
    trackNum = which;
    echo(F("Playing "));
    echoln(trackname);
//    echo("Tracks played: ");
//    echoln(tracksPlayed++);
//    echo("Free memory: ");
//    echoln(freeMemory());
  }
  else{
    playStatus = STOPPED;
    looping = 0;              // Without this line, sending an invalid "play" command would continue looping the last track
  }
}

void playPause(){
  if(playStatus == STOPPED)
    return;
  else{
    playStatus = audio.paused()?PLAYING:PAUSED;   // the NEW status after toggle
    audio.pausePlaying(playStatus == PAUSED);     // send true to pause it, false to resume
    echoln((playStatus == PLAYING)?"Playing":"Pausing");
  }
}

void cleanStop(){
  if(audio.paused()){
      audio.pausePlaying(false);
      delay(STOPDELAY);
  }
  if(!audio.stopped()){
    audio.stopPlaying();  
  }
  audio.softReset();        // Always softReset on a stop
}

void handleMSG(){
  uint8_t len = 0;
  uint8_t data[8];
  
  IDmsg command = AudioSlave.getCommand();

  switch (command.msgHigh){

    case ENABLE:
    case TURNON:
      AudioSlave.sndACK();
      len = AudioSlave.lastData(data);
      if(len==1){
        looping = command.msgLow;         // Needs to be before playTrack to handle incorrect track numbers correctly (i.e. stop playing anything)
        playTrack(data[0]);
        commander = command.sndID;        // On a valid play command, keep track of who sent it
      }
      else
        playPause();
      break;
      
    case TURNOFF:
      AudioSlave.sndACK();
      if(playStatus)
        cleanStop();
      playStatus = STOPPED;
      looping = 0;
      break;

    case LEVEL:
      AudioSlave.sndACK();
      len = AudioSlave.lastData(data);
      if(len==1){                // Only handle a correctly formed message
        targetVolume = map(data[0],0,255,MINVOL,MAXVOL);
        volFlag = true;
        echo(F("Setting volume to "));
        echoln(targetVolume);
        forceVol = command.msgLow;
      }
      break;

    case PAUSEALL:
      if(playStatus == PLAYING){
        audio.pausePlaying(true);
        playStatus = PAUSED;
      }
      break;
      
    case RESET:
      AudioSlave.sndACK();
    case RESETALL:
      if(playStatus)
        cleanStop();
      playStatus = STOPPED;
      looping = 0;
      targetVolume = RESETVOLUME;
      break;

    case STATUS:
      AudioSlave.statusRespond(playStatus);
      echo("S ");
      echoln(playStatus);
      break;
      
  }
}

