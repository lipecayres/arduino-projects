// Felipe Simoes Cayres
// Apr 23, 2023

#include <LiquidCrystal.h>

// Variables

#define RGBRedPin 9    // The red RGB LED is connected pin 9 of the Arduino.
#define RGBGreenPin 13 // The green RGB LED is connected pin 13 of the Arduino.
#define RGBBluePin 10  // The blue RGB LED is connected pin 10 of the Arduino.
#define button1Pin 2   // The SW1 button is connect to pin 2 of the Arduino.
#define button2Pin 3   // The SW2 button is connect to pin 3 of the Arduino.

LiquidCrystal lcd(12, 11, 7, 6, 5, 4); // These pin numbers are hard coded in on the serial backpack board.

volatile int state = 0;  // Variable to store button1 state
volatile int state2 = 0; // Variable to store button2 state

// Array of questions
String questions[5]{"1+1 = 2?",
                    "Sun is cold?",
                    "Arduino is nice?"};

int answers[3] = {9, 9, 9};      // Array to save answers
int rightAnswers[3] = {1, 0, 1}; // Array to save  right answers
int yesOrNo[2];                  // Array to save yes/no quantity
int iteration;                   // Variable to track iteraction
int resetButton;                 // Variable to reset quiz

void setup()
{

    lcd.begin(16, 2); // Initialize the LCD.
    Serial.begin(9600);

    attachInterrupt(digitalPinToInterrupt(button1Pin), clickButton1, RISING); // Setup button1 pin as an input pin.
    attachInterrupt(digitalPinToInterrupt(button2Pin), clickButton2, RISING); // Setup button2 pin as an input pin.
}

void loop()
{

    resetButton = 0;

    // Display questions stored in an array
    for (int i = 0; i < 3; i++)
    {
        iteration = i;

        while (answers[i] == 9)
        {
            displayQuestion(questions[i]);
        }

        // condition to turn on led
        if (answers[i] == rightAnswers[i])
        { // Right answer - green
            turnOnLEDByRGB(0, 255, 0);
            delay(500);
            turnOffLED();
        }
        else
        {
            turnOnLEDByRGB(255, 0, 0); // Wrong answer - red
            delay(500);
            turnOffLED();
        }
    }

    // Getting answers

    totalAnswers();

    // Display yes/no quantity - restart message

    resetButton = 0;
    iteration = 0;

    while (resetButton <= 2)
    {
        if (answers[2] != 9)
        {

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Answer's Summary");
            lcd.setCursor(0, 1);
            lcd.print("Yes: " + (String)yesOrNo[0] + " | " + "No: " + (String)yesOrNo[1]); // Print at cursor Location
            delay(2000);

            lcd.clear();
            lcd.setCursor(0, 0);            // goto first column and first line (0,0)
            lcd.print("Want to restart?");  // Print at cursor Location
            lcd.setCursor(0, 1);            // goto first column and first line (0,0)
            lcd.print("Press 3x Yes btn "); // Print at cursor Location
            delay(2000);
        }
    }

    // Restart function

    if (resetButton == 3)
    {
        for (int i = 0; i < sizeof(answers); i++)
        {
            answers[i] = 9;

            if (i < 2)
            {
                yesOrNo[i] = 0;
            }
        }

        iteration = 0;

        lcd.clear();
        lcd.setCursor(0, 0);          // goto first column and first line (0,0)
        lcd.print("Restarting");      // Print at cursor Location
        lcd.setCursor(0, 1);          // goto first column and first line (0,0)
        lcd.print("Please wait... "); // Print at cursor Location
        delay(2000);
    }
}

// Functions

// Display question on screen
void displayQuestion(String question)
{
    String displayUp;
    String displayDown;
    int questionSize = question.length();

    for (int i = 0; i < questionSize; i++)
    {
        if (i < 16)
        {
            displayUp += question[i];
        }
        else
        {
            displayDown += question[i];
        }
    }

    lcd.clear();
    lcd.setCursor(0, 0);    // goto first column and first line (0,0)
    lcd.print(displayUp);   // Print at cursor Location
    lcd.setCursor(0, 1);    // goto first column and first line (0,0)
    lcd.print(displayDown); // Print at cursor Location
    delay(500);
}

// Get total answers (yes/no)
void totalAnswers()
{
    for (int i = 0; i < 3; i++)
    {
        if (answers[i] == 1)
        {
            yesOrNo[0] += 1;
        }
        else if (answers[i] == 0)
        {
            yesOrNo[1] += 1;
        }
    }
}

// Change button 1
void clickButton1()
{
    answers[iteration] = 1;
    resetButton += 1;
}

// Change button 2
void clickButton2()
{
    answers[iteration] = 0;
}

// Turn on leb by RBG code
void turnOnLEDByRGB(int red, int green, int blue)
{
    analogWrite(RGBRedPin, red);     // Turn on RED
    analogWrite(RGBGreenPin, green); // Turn on GREEN
    analogWrite(RGBBluePin, blue);   // Turn on BLUE
}

// Turn off LED
void turnOffLED()
{
    analogWrite(RGBRedPin, LOW);   // Turn off RED
    analogWrite(RGBBluePin, LOW);  // Turn off BLUE
    analogWrite(RGBGreenPin, LOW); // Turn off GREEN
}