#ifndef KEY_H
#define KEY_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include <wiringPi.h>
#include "../hardware.h"


class Key : public QThread
{
    Q_OBJECT

public:
    /***************************
    * constructor & destructor
    ****************************/
    Key();

    bool isEnabled();
    void setEnable(bool en);

    /***************************
    * led control method
    ****************************/
    void setRed(bool onoff);
    void setGreen(bool onoff);
    void setYellow(bool onoff);
    void setLeds(bool red, bool green, bool yellow);

private:
    unsigned int pollingPeriod;
    bool _enabled;
    void run();

    void close();

signals:
    /* key pressed event */
    void keyPressed();
};



#endif // KEY_H
