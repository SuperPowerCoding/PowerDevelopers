#include "buzzer.h"
#include <QDebug>

#include "melody.h"

Buzzer::Buzzer()
{
    if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
            qDebug() << "setup wiringPi failed !";

    }

    if(softToneCreate(BuzPin) == -1){
        qDebug() << "setup softTone failed !";
    }

    this->pollingPeriod = 1;

    this->curidx = 0;
    this->lastIdx = 0;
    this->size = 0;

    this->_exit = false;
    this->_enabled = true;

    qDebug() << "init buzzer success";
}

bool Buzzer::isEnabled()
{
    return this->_enabled;
}


void Buzzer::close()
{
    pinMode(BuzPin, INPUT);
}

void Buzzer::addNote(int tone, int beat)
{
    this->mutex.lock();

    this->lastIdx++;
    this->size++;

    if(this->lastIdx > BUZZER_BUF_SIZE - 1)
    {
        this->lastIdx = 0;
    }

    if(this->size > BUZZER_BUF_SIZE)
    {
        this->size = BUZZER_BUF_SIZE;
    }

    this->beatRingBuf[this->lastIdx] = 1000/beat;
    this->toneRingBuf[this->lastIdx] = tone;

    this->mutex.unlock();
}

void Buzzer::instantNote(int tone, int beat)
{
    this->mutex.lock();

    this->beatRingBuf[this->curidx] = beat;
    this->toneRingBuf[this->curidx] = tone;

    this->mutex.unlock();
}

void Buzzer::getNote(int * tone, int * beat)
{
    this->mutex.lock();

    if(this->size == 0)
    {
        *tone = REST;
        *beat = 1000;
        return;
    }

    *beat = this->beatRingBuf[this->curidx];
    *tone = this->toneRingBuf[this->curidx];

    this->curidx++;
    this->size--;

    if(this->curidx == this->lastIdx)
    {
        this->curidx = this->lastIdx;
    }
    else if(this->curidx > BUZZER_BUF_SIZE - 1)
    {
        this->curidx = 0;
    }


    if(this->size < 0 )
    {
        this->size = 0;
    }

    this->mutex.unlock();
}

void Buzzer::addMelody(int *tones, int *beats, int size, bool cut)
{
    for(int i = 0 ; i < size ; i++)
    {
        this->addNote(tones[i],beats[i]);
        if(cut)
        {
            this->addNote(REST,500);
        }
    }
}

int Buzzer::getSize()
{
    int size;
    this->mutex.lock();

    size = this->size;

    this->mutex.unlock();

    return size;
}


void Buzzer::playFinMelody()
{
    /*
    this->addNote(tones[5][C],8);
    this->addNote(REST,16);
    this->addNote(tones[5][E],8);
    this->addNote(REST,16);
    this->addNote(tones[5][G],8);
    this->addNote(REST,16);
    this->addNote(tones[6][C],8);
    this->addNote(REST,16);
    */
}

void Buzzer::playGetCoinMelody()
{
    this->addMelody(getCoin[0],getCoin[1],sizeof(getCoin[0])/sizeof(int),false);
    this->addNote(REST,16);
}

void Buzzer::playBonusUp()
{
    this->addMelody(bonusUp[0],bonusUp[1],sizeof(bonusUp[0])/sizeof(int),false);
    this->addNote(REST,16);
}

void Buzzer::playBubbleBubble()
{
    for(unsigned int i = 0 ; i < sizeof(bubbleBubble)/(sizeof(bubbleBubble[0])) ; i++)
    {
        this->addMelody(bubbleBubble[i][0],bubbleBubble[i][1],sizeof(bubbleBubble[i][0])/sizeof(int),false);
    }
    this->addNote(REST,16);
}

void Buzzer::playWrongMelody()
{
    /*
    this->addNote(tones[5][A],8);
    this->addNote(REST,16);
    this->addNote(tones[5][A],2);
    this->addNote(REST,16);
    */
}

void Buzzer::playCaptureMelody()
{
    this->addNote(tones[4][G],32);
    this->addNote(REST,32);
}

void Buzzer::playCaptureResultOKMelody()
{
    /*
    this->addNote(tones[5][G],32);
    this->addNote(REST,32);
    this->addNote(tones[5][G],32);
    this->addNote(REST,32);
    */
}

void Buzzer::playGetStartMelody()
{
    for(unsigned int i = 0 ; i < sizeof(getStar)/(sizeof(getStar[0])) ; i++)
    {
        this->addMelody(getStar[i][0],getStar[i][1],sizeof(getStar[i][0])/sizeof(int),true);
    }
    this->addNote(REST,16);
}

Buzzer::~Buzzer()
{
    this->close();
}

void Buzzer::run()
{
    int size;
    int tone;
    int beat;
/*
    this->playGetCoinMelody();
    this->playGetCoinMelody();
    this->playGetCoinMelody();
    this->addNote(REST,4);

    this->playBonusUp();
    this->addNote(REST,4);

    this->playGetStartMelody();
    this->playGetStartMelody();
    this->addNote(REST,4);

    this->playWrongMelody();
    this->addNote(REST,4);

    this->playWrongMelody();
    this->addNote(REST,4);

    this->playBubbleBubble();
*/
    /*
    this->playGetCoinMelody();
    this->addNote(REST,1);
    this->addNote(REST,1);

    this->playBonusUp();
    this->addNote(REST,4);
*/
    while(!this->_exit)
    {
        size = this->getSize();
        if(size > 0 )
        {
            this->getNote(&tone,&beat);
            if(tone == REST)
            {
                softToneWrite(BuzPin, 0);
            }
            else
            {
                softToneWrite(BuzPin, tone);
            }

            // qDebug() << "buzzer:" << tone << "," << beat;

            msleep(beat);
        }
        else
        {
            softToneWrite(BuzPin, 0);
            msleep(this->pollingPeriod);
        }
    }

    this->close();
}




