#ifndef KEY_H
#define KEY_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include <wiringPi.h>


#define KeyPin  2
#define LedGreen    23
#define LedYellow   24
#define LedRed	    25

class Key : public QThread
{
    Q_OBJECT

public:
    Key();

    bool isEnabled();
    void setRed(bool onoff);
    void setGreen(bool onoff);
    void setYellow(bool onoff);
    void setLeds(bool red, bool green, bool yellow);

private:
    unsigned int pollingPeriod;
    bool _exit;
    bool _enabled;
    void run();

    void close();

signals:
    void keyPressed();
};



#endif // KEY_H
