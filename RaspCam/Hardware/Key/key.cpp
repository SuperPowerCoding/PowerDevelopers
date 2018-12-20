#include "key.h"
#include <QDebug>

Key::Key()
{
    
    this->pollingPeriod = KEY_POLLING_PERIOD_MS;

#if KEY_MODULE_ENABLE == 0
    this->_enabled = false;
    qDebug() << "key module disabled";
    return;
#endif
    
    this->_enabled = true;

    if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
            qDebug() << "setup wiringPi failed!! at Key";
            return ;
    }

    pinMode(KeyPin, INPUT);

#if LED_MODULE_ENALBE
    pinMode(LedRed, OUTPUT);
    pinMode(LedGreen, OUTPUT);
    pinMode(LedYellow, OUTPUT);

    digitalWrite(LedRed, HIGH);
    digitalWrite(LedGreen, HIGH);
    digitalWrite(LedYellow, HIGH);
#endif
    qDebug() << "key module enabled";
}

void Key::setEnable(bool en)
{
    this->_enabled = en;
}


void Key::run()
{
    int count = 0;
    int port;
    bool longKey = false;

    while(this->_enabled)
    {
        port = digitalRead(KeyPin);

        // detect high
        if(port == 1)
        {
            // start chattering
            count++;

            // if the key is pressed for 150 ms
            if(count > 150) 
            {
                // event
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
#if LED_MODULE_ENALBE
    if(onoff)
    {
        digitalWrite(LedRed, LOW);
    }
    else
    {
        digitalWrite(LedRed, HIGH);
    }
#endif
}

void Key::setGreen(bool onoff)
{
#if LED_MODULE_ENALBE    
    if(onoff)
    {
        digitalWrite(LedGreen, LOW);
    }
    else
    {
        digitalWrite(LedGreen, HIGH);
    }
#endif
}

void Key::setYellow(bool onoff)
{
#if LED_MODULE_ENALBE
    if(onoff)
    {
        digitalWrite(LedYellow, LOW);
    }
    else
    {
        digitalWrite(LedYellow, HIGH);
    }
#endif
}

void Key::setLeds(bool red, bool green, bool yellow)
{
#if LED_MODULE_ENALBE
    this->setRed(red);
    this->setGreen(green);
    this->setYellow(yellow);
#endif
}
