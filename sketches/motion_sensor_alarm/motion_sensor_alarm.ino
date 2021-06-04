int motionSensorPin = 2;
int ledPin = 11;
int buttonPin = 12;
boolean soundAlarm = true;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(motionSensorPin, INPUT);
 

}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(motionSensorPin) == HIGH && soundAlarm == false){ 
    //also include in or away code
      soundAlarm = true;
      SoundAlarm();
  }
 

}

void SoundAlarm()
{
  //sound alarmand change light here
  while (soundAlarm == true){
    digitalWrite(ledPin, !digitalRead(ledPin));
    
    //sound alarm code here
    
    if (digitalRead(buttonPin) == HIGH){
      soundAlarm = false;
      return;
    }
    delay(1000);
    
  }
}
