#include "laserSensor.h"

#define VERSION_REQUIRED_MAJOR 1
#define VERSION_REQUIRED_MINOR 0
#define VERSION_REQUIRED_BUILD 1


#define MAX_RANGE_DISTANCE  1000

#define MIN_DETECT_DISTANCE  75

#define MAX_DETECT_DISTANCE  90


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


void LaserSensor::close()
{
    this->_enabled = false;
}


LaserSensor::LaserSensor()
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    VL53L0X_DeviceInfo_t DeviceInfo;

#if LASER_SENSOR_MODULE_ENABLE == 0
    _enabled = false;
    return ;
#else
    _enabled = true;
#endif

    pollingPeriod = LASER_SENSOR_POLLING_PERIOD_MS;


    pMyDevice = &MyDevice;
    pVersion   = &Version;

    // Initialize Comms
    pMyDevice->I2cDevAddr      = 0x29;

    pMyDevice->fd = VL53L0X_i2c_init("/dev/i2c-1", pMyDevice->I2cDevAddr); //choose between i2c-0 and i2c-1; On the raspberry pi zero, i2c-1 are pins 2 and 3
    if (MyDevice.fd<0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        printf ("Failed to init\n");
    }

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
}

void LaserSensor::run()
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;
    int i;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    uint32_t filteredVal;

    int32_t offsetMicroMeter;

    if(_enabled == false)
    {
        return;
    }

     if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_StaticInit\n");
        Status = VL53L0X_StaticInit(pMyDevice); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_PerformRefSpadManagement\n");
        Status = VL53L0X_PerformRefSpadManagement(pMyDevice,
        		&refSpadCount, &isApertureSpads); // Device Initialization
        printf ("refSpadCount = %d, isApertureSpads = %d\n", refSpadCount, isApertureSpads);
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_PerformRefCalibration\n");
        Status = VL53L0X_PerformRefCalibration(pMyDevice,
        		&VhvSettings, &PhaseCal); // Device Initialization
        printf ("VhvSettings = %d, PhaseCal = %d\n", VhvSettings, PhaseCal);
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {

        // no need to do this when we use VL53L0X_PerformSingleRangingMeasurement
        printf ("Call of VL53L0X_SetDeviceMode\n");
        Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
        print_pal_error(Status);
    }

    if (Status == VL53L0X_ERROR_NONE) {
        Status = VL53L0X_SetLimitCheckValue(pMyDevice,        
        VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
        (FixPoint1616_t)(0.25*65536));
    }
    if (Status == VL53L0X_ERROR_NONE) {
        Status = VL53L0X_SetLimitCheckValue(pMyDevice,
        VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
        (FixPoint1616_t)(18*65536));
    }
    if (Status == VL53L0X_ERROR_NONE) {
        Status =
        VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,
        200000);
    }


    if(Status == VL53L0X_ERROR_NONE)
    {
        uint32_t measurement;
        uint32_t no_of_measurements = 5000;

        uint16_t* pResults = (uint16_t*)malloc(sizeof(uint16_t) * no_of_measurements);

        while(_enabled)
        {
            Status = VL53L0X_PerformSingleRangingMeasurement(pMyDevice,
            		&RangingMeasurementData);

            // print_pal_error(Status);
            // print_range_status(&RangingMeasurementData);

           
            if (Status != VL53L0X_ERROR_NONE) break;

            if( (RangingMeasurementData.RangeStatus == 0 || RangingMeasurementData.RangeMilliMeter) && RangingMeasurementData.RangeMilliMeter < MAX_RANGE_DISTANCE)
            {
                filteredVal = medianFilter(RangingMeasurementData.RangeMilliMeter);

                if(MIN_DETECT_DISTANCE <= filteredVal && filteredVal <= MAX_DETECT_DISTANCE )
                {
                    // wait for stability.
                    msleep(50);
                    // call interrupt signal
                    emit approachingDetected();
                }
                // printf("Measured distance: %i , %i\n", RangingMeasurementData.RangeMilliMeter,filteredVal);
            }

            // have to add sleep function to operate safely.
            msleep(pollingPeriod);
        }
    }

    VL53L0X_i2c_close();

}