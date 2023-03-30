// Felipe Simoes Cayres
// Mar 30, 2023

#define blueLED 13
#define redLED 12
#define btn 2

int fastDelay = 150, slowDelay = 500, blinkPosition;

void setup()
{
    pinMode(blueLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(btn), clickBtn, RISING); // Setup button1 pin as an input pin.
}

void loop()
{
    if (blinkPosition == 0)
    {
        blinkBlueLED(3);
        blinkRedLED(3);
    }
    else if (blinkPosition == 1)
    {
        blinkTwoLeds(3);
    }
}

// blink blue LED
void blinkBlueLED(int times)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(blueLED, HIGH);
        delay(fastDelay);
        digitalWrite(blueLED, LOW);
        delay(fastDelay);
    }
}

// blink red LED
void blinkRedLED(int times)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(redLED, HIGH);
        delay(fastDelay);
        digitalWrite(redLED, LOW);
        delay(fastDelay);
    }
}

// blink both leds
void blinkTwoLeds(int times)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(redLED, HIGH);
        digitalWrite(blueLED, HIGH);
        delay(fastDelay);
        digitalWrite(redLED, LOW);
        digitalWrite(blueLED, LOW);
        delay(fastDelay);
    }
    delay(500);
}

// click button
void clickBtn()
{
    blinkPosition++;
    if (blinkPosition > 1)
    {
        blinkPosition = 0;
    }
}