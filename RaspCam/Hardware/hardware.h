#ifndef ___HARDWARE____H__
#define ___HARDWARE____H__

// HW ports
/*********************************************
*   Port Map
**********************************************/
/*
    Please do not add enum.
*/
typedef enum _RaspCam_Port_Map
{
    BuzPin = 0, //wpi0 = 0,    
    wpi1,
    KeyPin,     // wpi2,
    wpi3,
    VibMot,     // wpi4,

    wpi5,
    wpi6,
    wpi7,
    reserved_for_i2c_SDA,  // wpi8, // please do not use this port. It is reserved for i2c SDA.
    reserved_for_i2c_SCL,  // wpi9, // please do not use this port. It is reserved for i2c SCL.

    wpi10,
    wpi11,
    wpi12,
    wpi13,
    wpi14,

    wpi15,
    wpi16,
    wpi17,
    wpi18,
    wpi19,

    wpi20,
    wpi21,
    wpi22,
    LedGreen,   // wpi23,
    LedYellow,  // wpi24,

    LedRed,     // wpi25,
    wpi26,
    wpi27,
    wpi28,
    wpi29,

    wpi30,
    wpi31,
    wpiLast = wpi31,    

}RaspCam_Port_Map;


/*********************************************
*   Buzzer module
**********************************************/
// 1. module enable
#define BUZZER_MODULE_ENALBE    0

// 2. polling period
/* 
    Must do not change polling period.    
    It is just for task sleep to operate safely.
*/
#define BUZZER_POLLING_PERIOD_MS    1

/*********************************************
*   key module
**********************************************/
// 1. module enable
#define KEY_MODULE_ENABLE           1
#define LED_MODULE_ENALBE           0

// 2. polling period
#define KEY_POLLING_PERIOD_MS   1

/*********************************************
*   [laser sensor module]
*
*    Because reference source codes are not made using wiring pi library,
*    laser sensor module does not use wiring pi.
*    But laser sensor communicate using i2c protocol. 
*    So do not use port 8, 9 (phys 3,5) for gpio. ( They are used for i2c )
**********************************************/
// 1. module enable
#define LASER_SENSOR_MODULE_ENABLE     1

// 2. polling period
/* 
    Must do not change polling period.    
    It is just for task sleep to operate safely.
*/
#define LASER_SENSOR_POLLING_PERIOD_MS  10

#define LASER_SENSOR_SLEEP_MS_AT_FAILED  2000

/*********************************************
*   Vib motor module
**********************************************/
// 1. module enable
#define VIB_MOTOR_MODULE_ENALBE     1

// 2. polling period
/* 
    Must do not change polling period.    
    It is just for task sleep to operate safely.
*/
#define VIB_MOTOR_POLLING_PERIOD_MS 1






// output HW operating flag
#define VIB_MOT_OPERATING_FLAG  0x00000001
#define BUZZER_OPERATING_FLAG  0x00000002



#endif