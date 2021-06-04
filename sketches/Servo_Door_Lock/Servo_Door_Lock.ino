#include <EEPROM.h>
#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal.h>
#include <Servo.h>


Servo MyServo;  //current servo
int servoPin = 8;
int solenoidLockPin = 12;
int redLedPin = 9;
int greenLedPin = 13;
int outdoorLightsPin = 11;
int contactSensorPin = 10;
int insideDoorButtonPin = 0;

int pos = 0;  //initial position of servo
int lastDoorState;  //to track the state of the door

//lcd declearations
LiquidCrystal lcd(A4, A5, A3, A2, A1, A0);
const byte rows = 4;
const byte cols = 3;

//keypad declearations
byte rowPins[rows] = {4,3,2,1}; //this may be incorrect
byte colPins[cols] = {5,6,7};

//define the symbols on the buttons of the keypads
char key[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

//initialize an instance of class NewKeypad
Keypad myKeyPad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);
char defaultPassword[4] = {'0', '1','2','3'};
char currentPassword[4]; //value will be gotten from the cloud
char codeInput[4];  // Variable to store current user's pressed password
int currentKeyCount = 0;
char currentKeyPressed; //stores the keys as the user presses
int currentposition = 0;  //the position of the cursor




void setup() {
    
  // put your setup code here, to run once:
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(solenoidLockPin, OUTPUT);
  pinMode(contactSensorPin, INPUT);
  pinMode(insideDoorButtonPin, INPUT);

  MyServo.attach(servoPin); //Declare servoPin for servo
  MyServo.write(0); // Set initial position at 0 degrees

  lcd.begin(16, 2);
  if (currentposition == 0){
    lcd.print("Password Access:");
    lcd.setCursor(0,1); //Move coursor to second Line
  }
  lcd.cursor(); //turn on the cursor

  digitalWrite(greenLedPin, LOW); //TURN OFF
  digitalWrite(redLedPin, HIGH);  //TURN ON
  currentKeyCount = 0;

    //get curent password from the cloud here
    // currentPassword = defaultPassword;  //set to default password for now

  

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(contactSensorPin) == HIGH && digitalRead(solenoidLockPin) == HIGH){ //if contact sensor is triggered then lock door
    SolenoidLock();
  }
  if (digitalRead(insideDoorButtonPin) == HIGH){  
    //when button is pressed and new door state doesnt match last dor state
    ToggleDoor();
  }
  

  currentKeyPressed = myKeyPad.getKey(); //to catch keys pressed by the user
  if (!currentKeyPressed){  //if the key isnt pressed
    if (currentKeyCount == 4 && strlen(codeInput) == 5){  //if user inputs 4 digits. 5 there accounts for null terminating
      if (MatchPassword(codeInput)){  //if user inputs correct password
          Unlock();
      }
      else {  //if it is incorrect
        //lcd.setCursor(0,1); //Move coursor back to second Line
        lcd.clear();
        lcd.print(" Incorrect Passcode");
        // lcd.print(("difference is %d",defaultPassword));
        // lcd.print(("difference is %d",codeInput));
        
        delay(500);
        Lock();
  
      }
  }
    return;
  }
  if (currentKeyCount < 4 && currentKeyPressed != '#' && currentKeyPressed != '*'){
    lcd.print(currentKeyPressed); //for now displays the key to test if its working
    codeInput[currentKeyCount] = currentKeyPressed; //store the current key as part of the code
    currentKeyCount++;  //reflect the increase in keycount
  }
  else if (currentKeyPressed == '#'){ //# clears the imputted password
    Lock();
  }
  else if (currentKeyPressed == '*'){ //* deletes an inputted password the imputted password
    //not sure hw to unprint will do this later
  }

  
}



void Lock()  //lock the servo and update the display
{

lcd.setCursor(0,1);

lcd.print("Door Locked     ");

delay(500);

lcd.setCursor(0,1);

lcd.clear(); //Clear Password

lcd.setCursor(0,1);

currentKeyCount = 0;

digitalWrite(greenLedPin, LOW); //TURN OFF
digitalWrite(redLedPin, HIGH);  //TURN ON
MyServo.write(0); // Set initial position at 0 degrees

}

void SolenoidLock()
{
  currentposition = 0;

if (currentposition == 0){  //ask for the password again
    lcd.setCursor(0,0); //Move coursor to second Line
    lcd.print("Password Access:");
    lcd.setCursor(0,1); //Move coursor to second Line
  }
  digitalWrite(solenoidLockPin, LOW); //change the solenoid lock to its locked state
  lastDoorState = LOW;
}

void Unlock() {
  lcd.clear();
  lcd.print("Pass Accepted");
  digitalWrite(solenoidLockPin, HIGH);  //unlock the solenoid lock 
  delay(500);
  MyServo.write(90); // open the door mechanically
  delay(500);
  ////also use light to indicate
  digitalWrite(greenLedPin, HIGH);  
  digitalWrite(redLedPin, HIGH);

  memset(codeInput, 0, sizeof defaultPassword); //clear this array so user can re-input password
  delay(500);
  //Lock(); //lock afterwards
  //do other things relating with the cloud here
}

boolean MatchPassword (char _inputCode[4])
{
  for (int i=0;i<sizeof defaultPassword;i++){
    if (codeInput[i] != defaultPassword[i]){
        return false; //return false if any of the letters dont match
    }
  }
    return (true);  //else returntrue
}

void ControlOutDoorLight(int hourofday, int hourthreshold)  //adjust outdoorlight based on time of day
{
  if (hourofday >hourthreshold){
    digitalWrite(outdoorLightsPin, HIGH);
  }
  else {
    digitalWrite(outdoorLightsPin, LOW);
  }
}

void ToggleDoor()
{
  switch (digitalRead(contactSensorPin)){
    case (LOW): 
      Lock();
      break;
    default:
      Unlock();
      break;
  }
}
