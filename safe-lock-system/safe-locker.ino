#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define buzz 11     // piezo
#define servo 10    // servo
#define redLed 12   // red led
#define greenLed 13 // green led

// lcd settings
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// servo settings
Servo myservo;
int pos = 0;

// keypad settings
const byte ROWS = 4;
const byte COLS = 4;

char hexakey[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(hexakey), rowPins, colPins, ROWS, COLS);

// passwords and users
String password[5] = {"AD00", "1020", "3040", "5060", "7080"};
String users[5] = {"admin", "A0012", "B0005", "C0037", "D0010"};
String inputKey = "";

// variables
int position, attempts = 3, validEntry, usernamePosition;

void setup()
{
  // buzzer
  pinMode(buzz, OUTPUT);
  // serial monitor
  Serial.begin(9600);
  // servo
  myservo.attach(servo);
  // lcd connection
  lcd.begin(16, 2);
  // initial screen
  displayDefaultMessage();
}

void loop()
{

  // Step 1
  // get complete attempt password from user --------------
  //
  
  // get entry of keyboard
  char key = keypad.getKey();

  // verify if available
  if (key != NO_KEY && attempts > 0)
  {
    // default message keeps on after any attempt
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  SAFE  LOCKER  ");
    lcd.setCursor(6, 1);

    // display * (password simbol) to each pressed key
    for (int i = 0; i <= position; i++)
    {
      lcd.print("*");
      playKeySound(1);
    }
    // saving pressed btn and going to next position
    inputKey += key;
    position++;
  }

  // Step 2
  // check password (wait for 4 pressed btn's) --------------
  //

  while (position >= 4 && attempts > 0)
  {

    // check each stored password once
    for (int j = 0; j < 5; j++)
    {
      for (int i = 0; i < 4; i++)
      {

        // char check
        if (inputKey[i] == password[j][i])
        {

          // add 1 to each valid char on password
          validEntry++;

          // track username at this time
          usernamePosition = j;
        } else {
        	i=4;
        }

        // check if password is valid
        if (validEntry == 4)
        {
          // stop loop
          i = 100;
        }

        // if password is invalid after check
        else if (validEntry != 4 && i == 3)
        {
          // new attempt (back to zero)
          validEntry = 0;
        }
      }
    }

    // Step 3
    // return message after check password --------------
    //

    // password valid
    if (validEntry == 4)
    {
      // unlock device
      Serial.println("System OPEN");
      digitalWrite(greenLed, HIGH);
      unlockdoor();
      digitalWrite(greenLed, LOW);

      // reset variables to next attempt
      position = 0;
      validEntry = 0;
      inputKey = "";
    }
    // password not valid
    else
    {
      Serial.println("Password INVALID");
      // failed message
      attempts--;
      digitalWrite(redLed, HIGH);

      failedAttempt();
      digitalWrite(redLed, LOW);
      // reset variables to next attempt
      position = 0;
      validEntry = 0;
      inputKey = "";
    }
	
    // Step 4
    // check attempts limit --------------
    //
    
    if (attempts <= 0)
    {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("LOCKED FOR");
      lcd.setCursor(0, 1);

      // Display countdown from 9 to 0
      for (int i = 9; i >= 0; i--)
      {
        lcd.setCursor(7, 1);
        lcd.print(i);
        lcd.print("s");
        delay(1000);
      }

      attempts = 3;
    }    

    // password default message
    displayDefaultMessage();
  }
}

// functions:

// unlock device
void unlockdoor()
{
  // welcome message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     WELCOME    ");
  lcd.setCursor(6, 1);
  lcd.print(users[usernamePosition]);

  playKeySound(3);

  // servo position to open device
  for (pos = 90; pos >= 0; pos -= 5)
  {
    myservo.write(pos);
    delay(5);
  }

  // time device is open until close again
  delay(5000);

  lockDevice();

  // servo position to close device
  for (pos = 0; pos <= 90; pos += 5)
  {
    myservo.write(pos);
    delay(15);

    position = 0;

    lcd.clear();
  }

  // final lock message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     LOCKED     ");
  delay(1000);
}

// failed attempt to unlock device
void failedAttempt()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" WRONG PASSWORD ");
  lcd.setCursor(0, 1);
  lcd.print("   Attempts:");
  lcd.setCursor(13, 1);
  lcd.print(attempts);

  // 3 times sound
  playKeySound(3);

  delay(500);
}

// default message on LCD
void displayDefaultMessage()
{
  lcd.setCursor(0, 0);
  lcd.println("  SAFE  LOCKER  ");
  lcd.setCursor(0, 1);
  lcd.println("    PASSWORD    ");
}

// play sound to key press
void playKeySound(int ocurrences)
{
  for (int i = 0; i < ocurrences; i++)
  {
    digitalWrite(buzz, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    delay(100);
  }
}

// lock device after some time opened
void lockDevice()
{
  // regressive counter lock message (upper line)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("   LOCKING IN   ");

  // counter lock time -> 5sec (lower line)
  for (int i = 5; i > 0; i--)
  {
    lcd.setCursor(7, 1);
    lcd.print(i);
    playKeySound(1);
    delay(1000);
  }
  // play wrong attempt sound (4 times key press)
  playKeySound(4);

  // re-locking message (upper line)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   RE-LOCKING   ");

  // display "dot" counter and key sound (lower line)
  for (int i = 0; i < 4; i++)
  {
    lcd.setCursor(6 + i, 1);
    lcd.print(".");
    delay(200);
    digitalWrite(buzz, LOW);
  }
  Serial.println("System CLOSED");
}
