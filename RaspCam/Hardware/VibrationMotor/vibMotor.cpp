#include <QDebug>

#include "vibMotor.h"
#include "../hardware.h"

/*
*   constructor
*/
VibMotor::VibMotor()
{
#if VIB_MOTOR_MODULE_ENALBE == 0
    this->_enabled = false;
    return;
#endif
    this->_enabled = hardwareInit();
    this->pollingPeriod = VIB_MOTOR_POLLING_PERIOD_MS;
}

VibMotor::~VibMotor()
{

}

bool VibMotor::hardwareInit()
{
    if (wiringPiSetup () == -1) return false;
    
    pinMode (VibMot, OUTPUT);
    digitalWrite (VibMot, 0);
    
    return true;
}

void VibMotor::vibrate(int ms, int nTimes)
{
    mutex.lock();
    this->vibTime.append(ms);
    this->vibNTimes.append(nTimes);
    mutex.unlock();
}

void VibMotor::ngVibrate()
{
    vibrate(250,3);
}


void VibMotor::run()
{
    int nTimes;
    int time;

    while(_enabled)
    {
        mutex.lock();
        if(vibNTimes.size() > 0 )
        {
            nTimes = vibNTimes.at(0);
            time = vibTime.at(0);
            mutex.unlock();

            vibNTimes.removeAt(0);
            vibTime.removeAt(0);
        }
        else
        {
            
            msleep(this->pollingPeriod);            
        }
        mutex.unlock();

        while( nTimes > 0)
        {
            digitalWrite (VibMot, 1);
            msleep(time);
            digitalWrite (VibMot, 0);
            usleep(150000);
            nTimes--;
        }
    }
}