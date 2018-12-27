#ifndef BUZZER_H
#define BUZZER_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include <wiringPi.h>
#include <softTone.h>

#include "tones.h"
#include "../hardware.h"

#define BUZZER_BUF_SIZE 1024


class Buzzer : public QThread
{
    Q_OBJECT

public:
    /*****************************************************
    * constructor, destuctor, hardware init method
    ******************************************************/
    Buzzer();
    ~Buzzer();
    bool buzzerHwInit(RaspCam_Port_Map buzerPin);

    /*****************************************************
    * note method
    ******************************************************/    
    void addNote(int tone, int beat);               // add a note to tail.
    void instantNote(int tone, int beat);           // add a note to head.
    void addMelody(int * tones, int * beats, int size, bool cut);   // add melody to tail


    /*****************************************************
    * play saved melody method
    ******************************************************/
    /* super mario melody */ 
    void playGetCoinMelody();
    void playBonusUp();
    void playGetStartMelody();

    /* bubble bubble melody */
    void playBubbleBubble();

    /* process ok/ng  melody */
    void playWrongMelody();
    void playCaptureMelody();
    void playFinMelody();
    void playCaptureResultOKMelody();
    
    
    bool isEnabled();
    void setEnable(bool en);

private:
    /*****************************************************
    * basic thread 
    ******************************************************/
    unsigned int pollingPeriod;    
    bool _enabled;
    void run();

    /*****************************************************
    * ring buffer
    ******************************************************/
    int toneRingBuf[BUZZER_BUF_SIZE];
    int beatRingBuf[BUZZER_BUF_SIZE];
    int lastIdx;
    int curidx;
    int size;
    QMutex mutex;

    /*****************************************************
    * get
    ******************************************************/
    void getNote(int * tone, int * beat);
    int getSize();

    void close();

signals:    
    void buzzFinished();

};


#endif // BUZZER_H
