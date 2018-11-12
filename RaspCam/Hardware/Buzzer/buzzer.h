#ifndef BUZZER_H
#define BUZZER_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include <wiringPi.h>
#include <softTone.h>

#include "tones.h"

#define BuzPin    0
#define BUZZER_BUF_SIZE 1024


class Buzzer : public QThread
{
    Q_OBJECT

public:
    Buzzer();
    ~Buzzer();

    void addNote(int tone, int beat);
    void instantNote(int tone, int beat);
    void addMelody(int * tones, int * beats, int size, bool cut);

    void playGetCoinMelody();
    void playBonusUp();
    void playBubbleBubble();
    void playWrongMelody();
    void playCaptureMelody();
    void playFinMelody();

    void playCaptureResultOKMelody();

    void playGetStartMelody();

    bool isEnabled();


private:
    unsigned int pollingPeriod;
    bool _exit;
    bool _enabled;
    void run();

    // ringbuf
    int toneRingBuf[BUZZER_BUF_SIZE];
    int beatRingBuf[BUZZER_BUF_SIZE];
    int lastIdx;
    int curidx;
    int size;

    QMutex mutex;


    void getNote(int * tone, int * beat);
    int getSize();

    void close();

};


#endif // BUZZER_H
