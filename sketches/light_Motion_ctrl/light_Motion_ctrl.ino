#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

#include <Wire.h>

#include <HID.h>

//#define SLAVE_ADDRESS 0x09


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
//Underworld melody
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
//Underwolrd tempo
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};








#define DHTTYPE    DHT11  
#define melodyPin 3
#define TemperaturePin A2
int DHT11Pin = 4;
int RelayPin = 7;
int motionSensorPin = A3;
boolean motionSensorState = LOW;  //start out as low
int calibrationTime = 30; //the time when the sensor outputs a low impulse
// bool motionState = LOW; //start with no motion
int alarmToggleButtonPin = 2;
int alarmForceStopButtonPin = 3;
bool alarmToggleState = false;  //by default, the alarm is off
bool alarmToggleGetready = false;  
//bcs its a button, pressing button gets ready, releasing toggles
int buzzerPin = 5;
int blueledPin = 11;
int redledPin = 9;
int greenledPin = 10;
int adhocledPin = 13;


//int lightSwitchButton1State = 0;  //the state of the above button
boolean soundAlarm = false;
int song = 0;

char DevModeArg[2]; //received from rpi to run commands format: {DeviceModeArguement}
int dev;
int mode;
int arg;
int receivedData;

bool awayMode = true;
byte Response[5]; //unused
//byte ok = 14;
//1-read sensor, 
//2-read servo Position,
//3-set light2,
//4-respond to passcode

//index 2: 0 -{off/LOW) 1 -(on/HIGH) 3-TOGGLE
//0-read device state-- not used
//1- read temperature sensor 
//2- read humidity sensor
//3- read motion sensor 
//4- set light state
//5- toggle alarm state
//6- set home and away mode


DHT_Unified dht(DHT11Pin, DHTTYPE);   //initialize the dht attributes
sensors_event_t event;


void setup() {
  // Set RelayPin as an output pin
  Serial.begin(9600);
  Serial.println("im working");
  
  pinMode(RelayPin, OUTPUT);
  pinMode(redledPin, OUTPUT);
  pinMode(blueledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(melodyPin, OUTPUT);
  pinMode(motionSensorPin, INPUT);
  pinMode(alarmToggleButtonPin, INPUT);
  pinMode(alarmForceStopButtonPin, INPUT);
  pinMode(greenledPin, OUTPUT);
  

  dht.begin();
  sensor_t sensor;
  dht.humidity().getSensor(&sensor);

  //randomSeed(analogRead(3)); //initialize random number generator

  //attachInterrupt(digitalPinToInterrupt(alarmForceStopButtonPin), StopAlarm, CHANGE); // trigger when button pressed, but not when released.

 
  soundAlarm = false;
  //wait for pir to calibrate
  Serial.print("calibrating sensor "); 
  for(int i = 0; i < calibrationTime; i++){ 
   Serial.print("."); 
   delay(1000); 
   } 
   //Serial.println(" done"); 
   Serial.println("SENSOR ACTIVE"); 
   delay(50);
}

void loop() {
  //I commented this away bcs the logic is handled by the RPI
  // if (awayMode == true && digitalRead(motionSensorPin) == HIGH && soundAlarm == false){ 
  //   //also include in or away code and door bypassed code
  //     soundAlarm = true;
  //     SoundAlarm();
  // }
  if (digitalRead(alarmToggleButtonPin) == LOW) {  //or if receive sound off from pi
      alarmToggleGetready = true;
      Serial.println("I am detected as presseD");
      //return;
  }
  else if (digitalRead(alarmToggleButtonPin) == HIGH && alarmToggleGetready == true) {  //or if receive sound off from pi
      soundAlarm = !soundAlarm;
      Serial.println("I am detected as released");
      SoundAlarm();
      alarmToggleGetready = false;  
      
      //to prevent a held down button from causing continuous trigger
      //return;
  }

  ReadSerial();
  
  digitalWrite(adhocledPin, !digitalRead(adhocledPin));
  digitalWrite(greenledPin, LOW);
  //this lcd changing helps me know the arduino is working
  delay(400);
  
  // Let's turn off the relay...
  //digitalWrite(RelayPin, HIGH);
  //delay(100);
}

void ToggleIndoorLight()
{
    digitalWrite(RelayPin, !digitalRead(RelayPin)); //write the opposite 
}
void TurnOffLight()
{
    digitalWrite(RelayPin, LOW); //write the opposite 
}
void TurnOnLight()
{
    digitalWrite(RelayPin, HIGH); //write the opposite 
}
void StopAlarm()
{
  soundAlarm = false;
}

void SoundAlarm()
{
    digitalWrite(redledPin, HIGH);  //start by putting on red led
    digitalWrite(greenledPin, LOW);  //start by putting on red led
    digitalWrite(blueledPin, LOW);
    //digitalWrite(adhocledPin, HIGH);
    
    if (soundAlarm != true) {
      //digitalWrite(buzzerPin, HIGH);
      digitalWrite(redledPin, LOW);
      digitalWrite(blueledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      Serial.println("sound alarm not true");
      return;
    }
  while (soundAlarm == true){
      //Blink red and blue Led
      if (digitalRead(redledPin)) {
          digitalWrite(redledPin, LOW);
          // digitalWrite(greenledPin, LOW);
          digitalWrite(blueledPin, HIGH);
      }
      else
      {
          digitalWrite(redledPin, HIGH);
          // digitalWrite(greenledPin, LOW);
          digitalWrite(blueledPin, LOW);
      }
    //digitalWrite(ledPin, !digitalRead(ledPin));
    
    //sound alarm code here 2 represents the song
    long _randSongNumber = random(1, 3);
    sing(2);    //hmmm will the method wait for it to finish?
    if (digitalRead(alarmForceStopButtonPin) == LOW){  //or if receive sound off from pi
      Serial.println("alarm force stop");
      soundAlarm = false;
      return;
    }
    ReadSerial();
    delay(1000);
    
    
  }
}

void ReadSerial()
{
  //first read motionsensor pin
  if (analogRead(motionSensorPin) > 50 && motionSensorState != HIGH){
    motionSensorState = HIGH;
    Serial.println("motion detected");
  }
  else if (analogRead(motionSensorPin) == LOW && motionSensorState != LOW){
    motionSensorState = LOW;
    Serial.println("motion not detected");
  }

  int val = analogRead(A3);  // read the input pin
  Serial.print("analog read: ");          // debug value
  Serial.print(val);          // debug value
  Serial.print(" sensor state: ");          // debug value
  Serial.println(motionSensorState); 
  
    if (Serial.available()) {
      Serial.println("starting to read serial");
      delay(10);
      receivedData = Serial.parseInt();
      sprintf(DevModeArg, "%d", receivedData); //convert received data to char array
      Serial.print("\nSerial is available: ");
      Serial.println(DevModeArg);
      dev = DevModeArg[0] - '0';
      Serial.println(("dev is %s",dev));
      mode = DevModeArg[1] - '0';   //C automatically converts it
      Serial.println(("mode is %s",mode));
      switch (dev)  //respond to received code
      {
        case 1: { //read temp sensor
          float tempcel = ReadLM35();
          //char sign = (tempcel < 0) ? '-' : '+';
          Serial.print("mytemperaturesensor;property2:");
          Serial.println(tempcel);
          // Serial.println(("mytemperaturesensor;property2:%c%g",sign,tempcel));
          break; }
        case 2: { //read humidity sensor
          int temphum = ReadHumidity();
          Serial.print("myhumiditysensor;property2:");
          Serial.println(temphum);
          break; }
        case 3: { //read motion sensor
          //bool tempmtn = ReadMotion();
          //true, false string must be capitalized to work
          const char *state = analogRead(motionSensorPin) > 50 ? "True" : "False";
          Serial.print("mymotionsensor;property2:");
          //Serial.print(analogRead(motionSensorPin));
          //Serial.println(motionSensorState);
          Serial.println(state);
        break; }
        case 4: { //set light state and extension box
          switch (mode){
            case 0: {
              TurnOffLight();
              break; }
            case 1: {
              TurnOnLight();
              break; }
            default: {  //case 3 or otherwise
              ToggleIndoorLight();
              Serial.println("toggle light");
              
              break; }
          }
          break; }  //break for parent case 4
        case 5: { //togle alarm
          if (soundAlarm == false){
            Serial.println("toggle sound alarm");
            soundAlarm = true;
            SoundAlarm();
          }
          else {soundAlarm = false;}
        break; }  
        case 6: { //set home and away
          switch (mode){
            case 0: {
              awayMode = false;
              break; }
            case 1: {
              awayMode = true;
              break; }
            default: {  //case 3 or otherwise, just toggle
              Serial.println("toggle away mode");
              awayMode = !awayMode;
              break; }
          }
        break; }  //break for parent case 4
        default: {
            break; }
      }
      digitalWrite(12, !digitalRead(12)); //to indicate that action has been performed
  
  }
}

float ReadLM35() //read the lm35 temperature
{
    int val;
    val = analogRead(TemperaturePin);
    if (isnan(val)) {   //for error
        val = -1;
        return val;
    }
    float TempCel = (val / 1024.0) * 500; // Getting the celsius value from 10 bit analog value
    
    return TempCel ;
}

int ReadHumidity()
{
  delay(500);  //dht needs a delay of 1500 secs actually
    dht.humidity().getEvent(&event);  //event is defined above
    int val = event.relative_humidity;
    if (isnan(val)) {   //for error
        val = -1;
        return val;
    }
    return val;
}

boolean ReadMotion()
{
  return digitalRead(motionSensorPin);
  // if (motionState != val){  
  //     motionState = val;
  //     return motionState;
  // }
}









void sing(long s) {
  // iterate over the notes of the melody:
  song = s;
  if (song == 2) {
    Serial.println(" 'Underworld Theme'");
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / underworld_tempo[thisNote];

      buzz(buzzerPin, underworld_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      if (digitalRead(alarmForceStopButtonPin) == LOW) {  //or if receive sound off from pi
        Serial.println("alarm force stop");
          soundAlarm = false;
          return;   //end the sound
      }
      ReadSerial();
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(buzzerPin, 0, noteDuration);

    }

  } else {

    Serial.println(" 'Mario Theme'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];
      //tone(buzzerPin, melody[thisNote], noteDuration);
      //buzz(buzzerPin, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(buzzerPin, 0, noteDuration);

    }
  }
}

void buzz(int targetPin, long frequency, long length) {
  //digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
      if (digitalRead(alarmForceStopButtonPin) == LOW) {  //or if receive sound off from pi
          soundAlarm = false;
          Serial.println("alarm force stop");
          return;   //end the sound
      }
      ReadSerial();
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(targetPin, LOW);

}
