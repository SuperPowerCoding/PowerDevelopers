#include "key.h"


Key::Key()
{
    this->_enabled = true;
    this->_exit = false;
    this->pollingPeriod = 1;

    pinMode(KeyPin, INPUT);
    pinMode(LedRed, OUTPUT);
    pinMode(LedGreen, OUTPUT);
    pinMode(LedYellow, OUTPUT);

    digitalWrite(LedRed, HIGH);
    digitalWrite(LedGreen, HIGH);
    digitalWrite(LedYellow, HIGH);
}




void Key::run()
{
    int count = 0;
    int port;
    bool longKey = false;

    while(!this->_exit)
    {
        port = digitalRead(KeyPin);

        if(port == 1)
        {
            count++;
            if(count > 150)
            {
                emit keyPressed();
                longKey = true;
            }

            if(longKey == true)
            {
                count = 0;
            }
        }
        else
        {
            count = 0;
            longKey = false;
        }

        msleep(this->pollingPeriod);
    }
}

void Key::setRed(bool onoff)
{
    if(onoff)
    {
        digitalWrite(LedRed, LOW);
    }
    else
    {
        digitalWrite(LedRed, HIGH);
    }
}

void Key::setGreen(bool onoff)
{
    if(onoff)
    {
        digitalWrite(LedGreen, LOW);
    }
    else
    {
        digitalWrite(LedGreen, HIGH);
    }
}

void Key::setYellow(bool onoff)
{
    if(onoff)
    {
        digitalWrite(LedYellow, LOW);
    }
    else
    {
        digitalWrite(LedYellow, HIGH);
    }
}

void Key::setLeds(bool red, bool green, bool yellow)
{
    this->setRed(red);
    this->setGreen(green);
    this->setYellow(yellow);
}
