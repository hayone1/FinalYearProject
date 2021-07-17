#include <Wire.h>
#define SLAVE_ADDRESS 0x08

#include <EEPROM.h>
#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal.h>
#include <Servo.h>



Servo MyServo;  //current servo
int servoPin = 12;



int redLedPin = 9;
int insideDoorButtonPin = 13;
boolean LastDoorState = LOW;  //sarts as shut

int outdoorLightsPin = 11;

int contactSensorPin = 10;
//int insideDoorButtonPin = 0;

int pos = 0;  //initial position of servo
int lastDoorState;  //to track the state of the door

//lcd declearations
LiquidCrystal lcd(A4, A5, A3, A2, A1, A0);
//const int rs = A4, en = A5, d4 = A3, d5 = A2, d6 = A1, d7 = A0;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//serial declearations
byte address = 0x0; //you may use this instead of the define
char DevModeArg[2]; //received from rpi to run commands format: {ModeModeArguement}
int dev;
int mode;
int arg;
//char* arg;
//char *receivedData;
int receivedData;

byte passCodeStatus = 0x0;  //default state
//1-read door sensor, 
//2-read servo Position,
//3-set light2,
//4-respond to passcode

//index 3: 0 -(off/LOW) 1 -(on/HIGH) 3-TOGGLE
//if 4| 0-wrong password 1-rightpassword


//keypad declearations
const byte rows = 4;
const byte cols = 4;
byte rowPins[rows] = {4,3,2,1}; //this may be incorrect
byte colPins[cols] = {5,6,7,8};

//define the symbols on the buttons of the keypads
char key[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//initialize an instance of class NewKeypad
Keypad myKeyPad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);
char defaultPassword[4] = {'0', '1','2','3'};
char currentPassword[4]; //value will be gotten from the cloud
char codeInput[4];  // Variable to store current user's pressed password
int currentKeyCount = 0;
char currentKeyPressed; //stores the keys as the user presses
int currentposition = 0;  //the position of the cursor

//PLEASE INCREASE TIMEOUT TIME
unsigned long timeout_millis = 10000;   //3 secs timeout to wait for password authentication
unsigned long current_millis;
bool count_timeout = false; //flag to signify wheather timeout should be counted or not




void setup() {
    
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("im working");
  pinMode(redLedPin, OUTPUT);
  //pinMode(greenLedPin, OUTPUT);
  //pinMode(solenoidLockPin, OUTPUT);
  
  //!!!!!!NOTE NOTE NOTE!!!!!!
  //the readings on these pins were inverted for some reason on my Arduino
  //please check the code where the pins are digitallyread to ensure they work right with your arduino
  pinMode(contactSensorPin, INPUT_PULLUP);
  pinMode(insideDoorButtonPin, INPUT);

  MyServo.attach(servoPin); //Declare servoPin for servo
  MyServo.write(0); // Set initial position at 0 degrees

  lcd.begin(16, 2);
  if (currentposition == 0){
    lcd.print("Password Access:");
    Serial.println("password Access:");
    lcd.setCursor(0,1); //Move coursor to second Line
  }
  lcd.cursor(); //turn on the cursor

  //digitalWrite(greenLedPin, LOW); //TURN OFF
  digitalWrite(redLedPin, HIGH);  //TURN ON
  currentKeyCount = 0;

    //get curent password from the cloud here
    // currentPassword = defaultPassword;  //set to default password for now

  count_timeout = false;
  current_millis = millis();    //set to current elapsed time

}


void loop() { //LOOP  LOOP  LOOP  LOOPO
  // put your main code here, to run repeatedly:
    CheckContactSensor();   //check door state
  
  if (digitalRead(insideDoorButtonPin) != LastDoorState){  
    //when indoor button is pressed and new door state doesnt match last dor state
    Serial.println("I am toggled");
    LastDoorState = digitalRead(insideDoorButtonPin);
    //doorToggledfromIn = true;
    ToggleDoor();
  }
  //else if (digitalRead(insideDoorButtonPin) == HIGH && doorToggledfromIn == true){
  //  Serial.println("I am low");
  //  doorToggledfromIn = false;
  //}


  ReadSerial();
  

  currentKeyPressed = myKeyPad.getKey(); //to catch keys pressed by the user
  //if (currentKeyPressed){ //this is implemented in UpdateDisplay
    
  //}
  if (count_timeout != true) {  //if timeout isnt meant to be counting yet
    UpdateDisplay();  //update the display when a key is pressed and request password verification when user presses 4 keys
    
    return;
  }
  if (count_timeout == true) {  //that is when user is dont imputting pin then check if serial is available and read from serial
      
      if ((millis() - current_millis > timeout_millis)) {   //or if timeout is passed, simply jump back to lock the system
          //(signify)lock door and return
          lcd.clear();
          lcd.setCursor(0, 0);    //set to top
          lcd.print("Verificaion error"); //clear what is there
          lcd.setCursor(0, 1);    //set to top
          
          lcd.print("Check Connection"); //clear what is there
          delay(500);
          Lock();
          return;
      }
      //if (passCodeStatus == 0x0) {  //if password is not confirmed
        //  //just wait 
          //Serial.println("just waiting");
          //return;   //return and keep counting
      //}
      

      if (!(millis() - current_millis > timeout_millis)) { //if serial is available and timeout hasnt reached, note the !
          MatchPassword(passCodeStatus); //check on the cloud if the password is correct
          return;   //return if its available or unavailable or if password matched or not
      }
  }
  Serial.println("im working");

  
}
  void ReadSerial()
  {
      if (Serial.available()) {
      delay(10);
      //Serial.readBytesUntil('\n', receivedData, 2); //read the received stream with asize of 3 bytes
       //Serial.readBytes(receivedData, 2); //read the received stream with asize of 3 bytes
       receivedData = Serial.parseInt();
       //snprintf(DevModeArg, sizeof(DevModeArg), "%d", receivedData);  //convert to char array
       sprintf(DevModeArg, "%d", receivedData); //convert received data to char array
       
      //receivedData.toCharArray(receivedChar, strlen(receivedData)); //get character array from received string
      Serial.print("\nSerial is available: ");
      Serial.println(DevModeArg);
      dev = DevModeArg[0] - '0';
      Serial.println(("dev is %s",dev));
      mode = DevModeArg[1] - '0';   //C automatically converts it
      Serial.println(("mode is %s",mode));
      //arg = receivedData[2] - '0';
      //char* receivedarg;
      //argCopy(receivedarg, receivedData, 2);    //copy the last two numbers
      //arg = receivedarg;    //convert it to integer

      switch (dev)  //switch in first byte
      {
          case 1:   //read doorsensor
              CheckContactSensor();
              if (lastDoorState == HIGH) { Serial.println("mydoorsensor;property2:True");}  //true means opened
              else { Serial.println("mydoorsensor;property2:False");} //dooris closed
              break;
          case 2:   //read servo position
              Serial.println(("mydoorcontroller;property2:&d",MyServo.read()));  //send door state to user
              break;
          case 3: //set light
          Serial.println("light control accepted");
              switch (mode) {    //what does it wasnt to set the device to
                  case 0:
                      TurnOffLight();
                      break;
                  case 1:
                      TurnOnLight();
                      break;
                  case 2:
                      ToggleoutDoorLight();
                      break;
                  default:
                      break;
              }
              break;
          case 4:   //respond to passCode verification request, or just control door password is checked in loop
                Serial.println("evaluating pass");
                delay(500);
              switch (mode) {
              case 0:   //wrong password will lead to door lock
                  passCodeStatus = 0x8; //will lead to default switch case
                  Serial.println("wrong pass");
                  Lock();
                  //count_timeout = false; //this allows bypassing of UpdateDisplay
                  break;
              case 1:
                  passCodeStatus = 0x1; //correct password
                  Serial.println("correct pass");
                  Unlock();
                  //count_timeout = true; //this allows bypassing of UpdateDisplay
                  break;
              default:
                  passCodeStatus = 0x0;
                  Serial.println("default pass result");
                  break;
              }
              break;

          default:
              break;
      }

  }
  }

void CheckContactSensor()
{
    if (digitalRead(contactSensorPin) == HIGH) { //if contact sensor is triggered
        //Lock();
        //Serial.println("Door is open");
        lastDoorState = HIGH;
    }
    else {
        lastDoorState = LOW;   //door is open
        //Serial.println("Door is CLOSED");
    }
}


void Lock()  //lock the servo and update the display (cant also be triggered by contact sensor)
{
    //show that door is locked
    //clear anyhing on that line first
    lcd.setCursor(0,1);
    lcd.print("                ");

    lcd.setCursor(0, 1);
    lcd.print("Door Locked     ");

    //wait then ask for password again
    delay(500);
    lcd.clear(); //Clear screen content
    lcd.setCursor(0,0);
    lcd.print("Password Access:");
    delay(500);

    lcd.setCursor(0,1); //set the ursor to second line where pin will be imputed

    memset(codeInput, 0, sizeof defaultPassword); //clear this array so user can re-input password
    currentKeyCount = 0;
    currentposition = 0;

    //use light to indicate that passcide is needed
    //digitalWrite(greenLedPin, LOW); //TURN OFF
    digitalWrite(redLedPin, HIGH);  //TURN ON

    if (MyServo.read() != 0) {  //if the servo isnt in locked state
    MyServo.write(0); // Set position at 0 degrees to  lock the door
    //lastDoorState = LOW;    //consider using the contact sensor for this
    }
    passCodeStatus = 0x0;
    count_timeout = false; //change state


}


void Unlock() {
  lcd.clear();
  lcd.print("Pass Accepted");
  MyServo.write(180); // open the door mechanically
  delay(4000);
  ////also use light to indicate
  //digitalWrite(greenLedPin, HIGH);  
  digitalWrite(redLedPin, LOW);

    //delay(500);
    lcd.clear(); //Clear screen content
    lcd.setCursor(0,0);
    lcd.print("Password Access:");
    delay(500);

    lcd.setCursor(0,1); //set the ursor to second line where pin will be imputed

  memset(codeInput, 0, sizeof defaultPassword); //clear this array so user can re-input password
  currentKeyCount = 0;
  currentposition = 0;
  //do other things relating with the cloud here
  //delay(500);
  //Lock(); //lock afterwards
  passCodeStatus = 0x0; //set back to default
  count_timeout = false; //change state to allow code read passkey
}

boolean UpdateDisplay()
{   //updates the lcd display with user input, returns true if user has inputted 4 letters for correctness checking
    if (!currentKeyPressed) {  //if the key isnt pressed
      
        if (currentKeyCount == 4 && strlen(codeInput) == 5) {  //if user inputs 4 digits. 5 there accounts for null terminating
            //set cursor to third row and tell user to hold on
            lcd.setCursor(0, 2);    
            lcd.print("checking Password...");

            if (count_timeout != true) {    //if timeout already isnt counting
                //char output[] = "passInput: %s",codeInput;
                Serial.write("passInput"); //send output to raspberry pi
                Serial.println(codeInput); //send output to raspberry pi
                current_millis = millis(); //set timeout counter to current milliseconds on device
                count_timeout = true; //allow timeout to start countnig
                return count_timeout;
            }
        }
        return false;
    }
    //if key is pressed
    if (currentKeyCount < 4 && currentKeyPressed != '#' && currentKeyPressed != '*') { 
      Serial.println("I got here"); 
        lcd.print(currentKeyPressed); //for now displays the key to test if its working
        Serial.println(currentKeyPressed);
        codeInput[currentKeyCount] = currentKeyPressed; //store the current key as part of the code
        currentKeyCount = currentKeyCount +1;  //reflect the increase in keycount
        Serial.print("code is");
        Serial.println((codeInput));
    }
    else if (currentKeyPressed == '#') { //# clears the imputted password
        Lock();
    }
    else if (currentKeyPressed == '*') { //* deletes an inputted password the imputted password
      //not sure hw to unprint will do this later
    }
    return false;
}

boolean MatchPassword (byte pass_code_state)
{
        switch (pass_code_state)
        {
        case 0x1:
            Unlock(); //unlock the door
            Serial.println("open up FBI");
            return true;    //allow unlock
            break;
        case 0x0:
            //Serial.println("\nawaiting password confirmation");
            break;
        case 0x8:    //eg 0x8
        Serial.print("hmmm password confirmation");
            lcd.setCursor(0, 0);    //set to top
            lcd.print("                  "); //clear what is there
            lcd.setCursor(0, 0);    //set to top
            lcd.print("Incorrect PassCode"); //signify incorrectness
            delay(1000);

            Lock(); //lock or maintain door lock state
            return false;   //otherwise return false
            break;
        }
  //for (int i=0;i<sizeof defaultPassword;i++){
  //  if (codeInput[i] != defaultPassword[i]){
  //      return false; //return false if any of the letters dont match
  //  }
  //}
    return (false);  //else jsut return false
}

void ToggleoutDoorLight()  //adjust outdoorlight based on time of day
{
    digitalWrite(outdoorLightsPin, !(digitalRead(outdoorLightsPin)));
}
void TurnOffLight() { digitalWrite(outdoorLightsPin, LOW); }
void TurnOnLight() { digitalWrite(outdoorLightsPin, HIGH); }
//in case I implement the code for time of day detections
//void ControlOutDoorLight(int hourofday, int hourthreshold)  //adjust outdoorlight based on time of day
//{
//    if (hourofday > hourthreshold) {
//        digitalWrite(outdoorLightsPin, HIGH);
//    }
//    else {
//        digitalWrite(outdoorLightsPin, LOW);
//    }
//}
void ToggleDoor()
{
  switch (digitalRead(contactSensorPin)){   
    case (LOW): //if the door is open
      Lock();
      break;
    default: //if the door is closed
      Unlock();
      break;
  }
}

void argCopy(char* dest, char* src, size_t n) {
    size_t len = 0;
    if (!dest || !src || (len = strlen(src)) < n)
        return;
    char* p = src + (len - n);
    strcpy(dest, p);
}
