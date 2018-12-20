#ifndef ___VIB___MOTOR____H____
#define ___VIB___MOTOR____H____


#include <QMutex>
#include <QThread>
#include <QObject>

#include <wiringPi.h>
#include "../hardware.h"

class VibMotor : public QThread
{
    Q_OBJECT

public:
    VibMotor();
    ~VibMotor();

    void ngVibrate();
    void okVibrate();

private:
    unsigned int pollingPeriod;
    bool _enabled;

    
    QList<int> vibNTimes;
    QList<int> vibTime;
    
    void run();
    void close();
    
    bool hardwareInit();
    void vibrate(int ms, int nTimes);

    QMutex mutex;
};

#endif