#ifndef LASERSENSOR_H
#define LASERSENSOR_H

#include <QMutex>
#include <QThread>
#include <QObject>
#include "../hardware.h"


extern "C"
{
#include "inc/vl53l0x_api.h"
#include "inc/vl53l0x_platform.h"
}

class LaserSensor : public QThread
{
    Q_OBJECT

public:
    LaserSensor();
    bool isEnabled();
    

    void sleep(int ms);

    int getCurDistance();    

    void stopMeasure();
    void startMeasure();

    void clearInterruptFlag();

    void setMinDistance(int dist);
    void setMaxDistance(int dist);

    int getMinDistance();
    int getMaxDistance();

private:
    unsigned int pollingPeriod;    
    bool _enabled;
    void run();

    bool hardwareInit();
    bool interruptFlag;
    bool farDistanceRecon = false;

    int sleepMs;

    bool _measure;
    QMutex measureMutex;

    int minDistance;
    int maxDistance;
    QMutex minMaxMutex;

    VL53L0X_Dev_t MyDevice;
    VL53L0X_Dev_t *pMyDevice;
    VL53L0X_Version_t Version;
    VL53L0X_Version_t *pVersion;

    uint32_t medianFilter(uint32_t cur);
    void insertionSort(uint32_t * arr, uint32_t size);

    void print_range_status(VL53L0X_RangingMeasurementData_t* pRangingMeasurementData);
    void print_pal_error(VL53L0X_Error Status);
    VL53L0X_Error WaitStopCompleted(VL53L0X_DEV Dev);
    VL53L0X_Error WaitMeasurementDataReady(VL53L0X_DEV Dev);

    QMutex mutex;
    int distance;   // ms

    void setCurDistance(int dist);
    void resetDevice();

signals:
    void approachingDetected();
    void requestDistanceUpdate();
};




#endif // LASERSENSOR_H
