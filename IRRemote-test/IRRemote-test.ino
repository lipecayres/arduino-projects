// 200548559 - Felipe Cayres
// Mar 20, 2023

#include <IRremote.hpp>
#include <LiquidCrystal.h>

// LED definitions
LiquidCrystal lcd(11, 10, 7, 6, 5, 4);

// Button name

String btnNames[21] = {"Power", "Vol+", "FN/ST", "Backward", "Play/Pause",
                       "Forward", "Arrow down", "Vol-", "Arrow up",
                       "Num 0", "EQ", "ST/REPT", "Num 1", "Num 2",
                       "Num 3", "Num 4", "Num 5", "Num 6", "Num 7",
                       "Num 8", "Num 9"};

void setup()
{
    // Settings
    lcd.begin(16, 2);
    Serial.begin(9600);
    IrReceiver.begin(12, ENABLE_LED_FEEDBACK);

    // First message when power system
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Click any btn");
    lcd.setCursor(0, 1);
    lcd.print("    to test!");
}

void loop()
{

    if (IrReceiver.decode())
    {
        Serial.println(IrReceiver.decodedIRData.decodedRawData); // Print "old" raw data

        IrReceiver.resume(); // Enable receiving of the next value

        switch (IrReceiver.decodedIRData.decodedRawData)
        {
        case 4278238976: // power
            updateLCD(btnNames[0]);
            break;
        case 4261527296: // vol+
            updateLCD(btnNames[1]);
            break;
        case 4244815616: // func/Stop
            updateLCD(btnNames[2]);
            break;
        case 4211392256: // backward
            updateLCD(btnNames[3]);
            break;
        case 4194680576: // play/pause
            updateLCD(btnNames[4]);
            break;
        case 4177968896: // forward
            updateLCD(btnNames[5]);
            break;
        case 4144545536: // down
            updateLCD(btnNames[6]);
            break;
        case 4127833856: // vol-
            updateLCD(btnNames[7]);
            break;
        case 4111122176: // up
            updateLCD(btnNames[8]);
            break;
        case 4077698816: // number 0
            updateLCD(btnNames[9]);
            break;
        case 4060987136: // EQ
            updateLCD(btnNames[10]);
            break;
        case 4044275456: // ST/REPT
            updateLCD(btnNames[11]);
            break;
        case 4010852096: // number 1
            updateLCD(btnNames[12]);
            break;
        case 3994140416: // number 2
            updateLCD(btnNames[13]);
            break;
        case 3977428736: // number 3
            updateLCD(btnNames[14]);
            break;
        case 3944005376: // number 4
            updateLCD(btnNames[15]);
            break;
        case 3927293696: // number 5
            updateLCD(btnNames[16]);
            break;
        case 3910582016: // number 6
            updateLCD(btnNames[17]);
            break;
        case 3877158656: // number 7
            updateLCD(btnNames[18]);
            break;
        case 3860446976: // number 8
            updateLCD(btnNames[19]);
            break;
        case 3843735296: // number 9
            updateLCD(btnNames[20]);
            break;
        default:
            updateLCD("Error - restart");
            break;
        }
    }
}

void updateLCD(String displayDown)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Button name:");
    lcd.setCursor(0, 1);
    lcd.print(displayDown);
    delay(500);
}
