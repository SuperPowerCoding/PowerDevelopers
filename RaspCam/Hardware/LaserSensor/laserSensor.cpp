#include "laserSensor.h"
#include <unistd.h>

#define VERSION_REQUIRED_MAJOR 1
#define VERSION_REQUIRED_MINOR 0
#define VERSION_REQUIRED_BUILD 1

// max range
#define MAX_RANGE_DISTANCE  1000

// Min, Max distance
#define MIN_DETECT_DISTANCE  95
#define MAX_DETECT_DISTANCE  110


void LaserSensor::insertionSort(uint32_t * arr, uint32_t size)
{
    for(int i = 1 ; i < size; i++)
    {       
        int key = arr[i];
        int j = i-1;
        for(; j >=0 && arr[j] > key ; j--)
        {
            arr[j+1] = arr[j];
        }
        arr[j+1] = key;
    }
}

#define MEDIAN_FILTER_SIZE  17

uint32_t LaserSensor::medianFilter(uint32_t cur)
{
    static uint32_t storedData[MEDIAN_FILTER_SIZE];    
    static uint32_t idx = 0;
    uint32_t i;
    uint32_t mid;
    uint32_t arrangedData[MEDIAN_FILTER_SIZE];

    if( idx < MEDIAN_FILTER_SIZE)
    {
        storedData[idx++] = cur;        
    }
    else
    {
        for(i = 0; i < MEDIAN_FILTER_SIZE; i++)
        {
            arrangedData[i] = storedData[i];
        }

        insertionSort(arrangedData, MEDIAN_FILTER_SIZE);

        // shift data
        for(i = 0; i < MEDIAN_FILTER_SIZE - 1 ; i++)
        {
            storedData[i] = storedData[i+1];
        }
        storedData[MEDIAN_FILTER_SIZE - 1] = cur;

        cur = arrangedData[MEDIAN_FILTER_SIZE/2];
    }

    return cur;
}

bool LaserSensor::hardwareInit()
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    
    VL53L0X_Version_t                   Version;
    VL53L0X_Version_t                  *pVersion   = &Version;
    VL53L0X_DeviceInfo_t                DeviceInfo;
    

    pMyDevice = &MyDevice;

    int32_t status_int;

    printf ("VL53L0X PAL Continuous Ranging example\n\n");

    // Initialize Comms
    pMyDevice->I2cDevAddr      = 0x29;

    pMyDevice->fd = VL53L0X_i2c_init("/dev/i2c-1", pMyDevice->I2cDevAddr); //choose between i2c-0 and i2c-1; On the raspberry pi zero, i2c-1 are pins 2 and 3
    if (MyDevice.fd<0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        printf ("Failed to init\n");
    }

    printf ("i2c init sucess : %d\n", pMyDevice->fd);

    /*
     *  Get the version of the VL53L0X API running in the firmware
     */

    if(Status == VL53L0X_ERROR_NONE)
    {
        status_int = VL53L0X_GetVersion(pVersion);
        if (status_int != 0)
            Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    /*
     *  Verify the version of the VL53L0X API running in the firmrware
     */

    if(Status == VL53L0X_ERROR_NONE)
    {
        if( pVersion->major != VERSION_REQUIRED_MAJOR ||
            pVersion->minor != VERSION_REQUIRED_MINOR ||
            pVersion->build != VERSION_REQUIRED_BUILD )
        {
            printf("VL53L0X API Version Error: Your firmware has %d.%d.%d (revision %d). This example requires %d.%d.%d.\n",
                pVersion->major, pVersion->minor, pVersion->build, pVersion->revision,
                VERSION_REQUIRED_MAJOR, VERSION_REQUIRED_MINOR, VERSION_REQUIRED_BUILD);
        }
    }

    // End of implementation specific
    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_DataInit\n");
        Status = VL53L0X_DataInit(&MyDevice); // Data initialization
        print_pal_error(Status);
    }
    
    if(Status == VL53L0X_ERROR_NONE)
    {
        Status = VL53L0X_GetDeviceInfo(&MyDevice, &DeviceInfo);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf("VL53L0X_GetDeviceInfo:\n");
        printf("Device Name : %s\n", DeviceInfo.Name);
        printf("Device Type : %s\n", DeviceInfo.Type);
        printf("Device ID : %s\n", DeviceInfo.ProductId);
        printf("ProductRevisionMajor : %d\n", DeviceInfo.ProductRevisionMajor);
        printf("ProductRevisionMinor : %d\n", DeviceInfo.ProductRevisionMinor);

        if ((DeviceInfo.ProductRevisionMinor != 1) && (DeviceInfo.ProductRevisionMinor != 1)) {
        	printf("Error expected cut 1.1 but found cut %d.%d\n",
        			DeviceInfo.ProductRevisionMajor, DeviceInfo.ProductRevisionMinor);
        	Status = VL53L0X_ERROR_NOT_SUPPORTED;
        }
    }

    print_pal_error(Status);

    if(Status == VL53L0X_ERROR_NONE)
    {
        return true;
    }

    return false;
}

LaserSensor::LaserSensor()
{
#if LASER_SENSOR_MODULE_ENABLE == 0
    printf ("laser sensor disabled\n\n");
    _enabled = false;
    return ;
#endif

    _enabled = this->hardwareInit();

    if( _enabled == false)
    {
        printf("Failed to init laserSensor Hardware!!\n");        
    }

    sleepMs = 0;
    _measure = false;
    pollingPeriod = LASER_SENSOR_POLLING_PERIOD_MS;
}


void LaserSensor::print_pal_error(VL53L0X_Error Status){
    char buf[VL53L0X_MAX_STRING_LENGTH];
    VL53L0X_GetPalErrorString(Status, buf);
    printf("API Status: %i : %s\n", Status, buf);
}

void LaserSensor::print_range_status(VL53L0X_RangingMeasurementData_t* pRangingMeasurementData){
    char buf[VL53L0X_MAX_STRING_LENGTH];
    uint8_t RangeStatus;

    /*
     * New Range Status: data is valid when pRangingMeasurementData->RangeStatus = 0
     */

    RangeStatus = pRangingMeasurementData->RangeStatus;

    VL53L0X_GetRangeStatusString(RangeStatus, buf);
    printf("Range Status: %i : %s\n", RangeStatus, buf);

}


VL53L0X_Error LaserSensor::WaitMeasurementDataReady(VL53L0X_DEV Dev) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t NewDatReady=0;
    uint32_t LoopNb;

    // Wait until it finished
    // use timeout to avoid deadlock
    if (Status == VL53L0X_ERROR_NONE) {
        LoopNb = 0;
        do {
            Status = VL53L0X_GetMeasurementDataReady(Dev, &NewDatReady);
            if ((NewDatReady == 0x01) || Status != VL53L0X_ERROR_NONE) {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(Dev);
        } while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) {
            Status = VL53L0X_ERROR_TIME_OUT;
        }
    }

    return Status;
}

VL53L0X_Error LaserSensor::WaitStopCompleted(VL53L0X_DEV Dev) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t StopCompleted=0;
    uint32_t LoopNb;

    // Wait until it finished
    // use timeout to avoid deadlock
    if (Status == VL53L0X_ERROR_NONE) {
        LoopNb = 0;
        do {
            Status = VL53L0X_GetStopCompletedStatus(Dev, &StopCompleted);
            if ((StopCompleted == 0x00) || Status != VL53L0X_ERROR_NONE) {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(Dev);
        } while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) {
            Status = VL53L0X_ERROR_TIME_OUT;
        }
	
    }

    return Status;
}

void LaserSensor::sleep(int ms)
{
    sleepMs = ms;
}

int LaserSensor::getCurDistance()
{
    int temp;

    mutex.lock();

    temp = distance;

    mutex.unlock();

    return temp;
}

void LaserSensor::setCurDistance(int dist)
{
    mutex.lock();
    distance = dist;
    mutex.unlock();
}

void LaserSensor::stopMeasure()
{
    measureMutex.lock();
    _measure = false;
    measureMutex.unlock();
}
void LaserSensor::startMeasure()
{
    measureMutex.lock();
    _measure = true;
    measureMutex.unlock();
}


void LaserSensor::resetDevice()
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;   
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    
    close(pMyDevice->fd);

    hardwareInit();

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_StaticInit\n");
        Status = VL53L0X_StaticInit(pMyDevice); // Device Initialization
        // StaticInit will set interrupt by default
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_PerformRefCalibration\n");
        Status = VL53L0X_PerformRefCalibration(pMyDevice,
        		&VhvSettings, &PhaseCal); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_PerformRefSpadManagement\n");
        Status = VL53L0X_PerformRefSpadManagement(pMyDevice,
        		&refSpadCount, &isApertureSpads); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {

        printf ("Call of VL53L0X_SetDeviceMode\n");
        Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING); // Setup in single ranging mode
        print_pal_error(Status);
    }
    
    if(Status == VL53L0X_ERROR_NONE)
    {
		printf ("Call of VL53L0X_StartMeasurement\n");
		Status = VL53L0X_StartMeasurement(pMyDevice);
		print_pal_error(Status);
    }
}

void LaserSensor::clearInterruptFlag()
{
    interruptFlag = false;
    printf("laser interrupt flag cleared\n");
}

void LaserSensor::setMinDistance(int dist)
{
    minMaxMutex.lock();
    minDistance = dist;
    minMaxMutex.unlock();
}
void LaserSensor::setMaxDistance(int dist)
{
    minMaxMutex.lock();
    maxDistance = dist;
    minMaxMutex.unlock();
}

int LaserSensor::getMinDistance()
{
    int temp;
    minMaxMutex.lock();
    temp = minDistance;
    minMaxMutex.unlock();

    return temp;
}

int LaserSensor::getMaxDistance()
{
    int temp;
    minMaxMutex.lock();
    temp = maxDistance;
    minMaxMutex.unlock();

    return temp;
}


void LaserSensor::run()
{
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;
    VL53L0X_RangingMeasurementData_t   *pRangingMeasurementData    = &RangingMeasurementData;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;

    uint32_t filteredVal;

    if(_enabled == false)
    {
        return;
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_StaticInit\n");
        Status = VL53L0X_StaticInit(pMyDevice); // Device Initialization
        // StaticInit will set interrupt by default
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_PerformRefCalibration\n");
        Status = VL53L0X_PerformRefCalibration(pMyDevice,
        		&VhvSettings, &PhaseCal); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_PerformRefSpadManagement\n");
        Status = VL53L0X_PerformRefSpadManagement(pMyDevice,
        		&refSpadCount, &isApertureSpads); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {

        printf ("Call of VL53L0X_SetDeviceMode\n");
        Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING); // Setup in single ranging mode
        print_pal_error(Status);
    }
    
    if(Status == VL53L0X_ERROR_NONE)
    {
		printf ("Call of VL53L0X_StartMeasurement\n");
		Status = VL53L0X_StartMeasurement(pMyDevice);
		print_pal_error(Status);
    }


    setMinDistance(MIN_DETECT_DISTANCE);
    setMaxDistance(MAX_DETECT_DISTANCE);

    if(Status == VL53L0X_ERROR_NONE)
    {
        
        while(_enabled)
        {
            if(sleepMs)
            {
                printf("max tryial reached sleep :%d\n", sleepMs);
                msleep(sleepMs);
                sleepMs = 0;
            }

            if(_measure == true)
            {
                Status = WaitMeasurementDataReady(pMyDevice);

                if(Status == VL53L0X_ERROR_NONE)
                {
                    Status = VL53L0X_GetRangingMeasurementData(pMyDevice, pRangingMeasurementData);
                    
                    filteredVal = medianFilter(pRangingMeasurementData->RangeMilliMeter);

                    // printf("Laser :%d, %d\n",pRangingMeasurementData->RangeMilliMeter, filteredVal);

                    if(interruptFlag == false)
                    {
                        if(getMinDistance() <= filteredVal && filteredVal <= getMaxDistance() )
                        {
                            printf("Laser : detect object (%d)\n",filteredVal);
                            // wait for stability.
                            msleep(100);
                            // call interrupt signal
                            interruptFlag = true;
                            emit approachingDetected();
                        }
                    }
                    
                    setCurDistance(filteredVal);                
                    emit requestDistanceUpdate();

                    // Clear the interrupt
                    VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
                    // VL53L0X_PollingDelay(pMyDevice);
                } else {
                    // break;
                    setCurDistance(-1);
                    emit requestDistanceUpdate();
                    
                    printf("An error occuerd :%d\n",Status);
                    print_pal_error(Status);
                    // VL53L0X_ResetDevice(pMyDevice);
                    
                    resetDevice();
                }
            }
            // have to add sleep function to operate safely.
            msleep(pollingPeriod);
        }
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_StopMeasurement\n");
        Status = VL53L0X_StopMeasurement(pMyDevice);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Wait Stop to be competed\n");
        Status = WaitStopCompleted(pMyDevice);
    }

    if(Status == VL53L0X_ERROR_NONE)
	Status = VL53L0X_ClearInterruptMask(pMyDevice,
		VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);

    VL53L0X_i2c_close();
}