#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMutex>
#include <QThread>
#include <QObject>

// basic module headers
#include "Camera/camera.h"
#include "Network/network.h"
#include "Resource/resource.h"

/*
    optional module headers
    To turn off module, set enable define 1 to 0 in "Hardware/hardware.h"
    Extra codes are not required when enable define chnaged.
*/ 
#include "Hardware/Buzzer/buzzer.h"
#include "Hardware/Key/key.h"
#include "Hardware/LaserSensor/laserSensor.h"
#include "Hardware/VibrationMotor/vibMotor.h"


namespace Ui {
class MainWindow;
}


#define MAX_CAPURES  5
#define ERR_NONE                0
#define ERR_ITEM_REMAINED       1
#define ERR_ALREADY_REGISTERED  2
#define ERR_PRE_SEQ_FAIL        3
#define ERR_UNKNOWN              4


#define MAX_RETRY_NUM       1


enum CaptureStatus
{
    NOT_CAPTURED_YET = 0,
    CAPTURED_FROM_BUTTON,
    CAPTURED_FROM_SENSOR,
    CAPTURED_STATUS_OVERFLOW
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:


    void updateResource();
    /**************************
    *   network events
    ***************************/    
    void updateIPResult();

    /**************************
    *   image events
    ***************************/    
    // image stream
    void streamImg();
    void getRawImg();

    /**************************
    *   external Hardware module events
    ***************************/    
    // external button
    void on_externalButton_pressed();    


    /**************************
    *   ui events
    ***************************/
    void on_matchRateSlider_sliderMoved(int position);
	    
    /* buttons events */
    void on_ResetButton_clicked();
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void on_exitButton_clicked();   
    
    // capture!!
    void on_streamingImg_clicked();

    // captured from distance sensor
    void in_camera_focus_distance();
    void updateDistance();

    // lower ui clicked
    void on_img0_clicked();
    void on_img1_clicked();
    void on_img2_clicked();
    void on_img3_clicked();
    void on_img4_clicked();


    /* setting events */    
    // tab changed
    void on_tabWidget_currentChanged(int index);

    // factory process changed
    void on_factorycb_currentTextChanged(const QString &arg1);

    // ip changed
    void on_ipcb1_currentIndexChanged(const QString &arg1);
    void on_ipcb2_currentIndexChanged(const QString &arg1);
    void on_ipcb3_currentIndexChanged(const QString &arg1);
    void on_ipcb4_currentIndexChanged(const QString &arg1);

    // port changed
    void on_portcb_currentIndexChanged(const QString &arg1);

    // cell info
    void on_cellinfocb_currentTextChanged(const QString &arg1);

    void on_updateButton_clicked();


    // check output hw finished
    void on_buzzer_finished();
    void on_vib_motor_finished();

private:

    Ui::MainWindow *ui;
    /*********************************
    *   modules
    **********************************/
    // basic module
    Resource * res;                // resource
    Camera * camTh = NULL;          // camera module
    Network * netTh= NULL;          // network module
    
    // for optional 
    // To turn off module, set enable define value 1 to 0 in hardware.h
    Buzzer * buzzerTh= NULL;        // buzzer module for sound.
    Key * keyTh = NULL;             // key module for button & leds
    LaserSensor * laserTh = NULL;   // laser distance sensor for automatic camera shutter.
    VibMotor * vibTh = NULL;        // vibration motor module for vibaration

    // camera status
    CaptureStatus curCapturedStatus = CaptureStatus::NOT_CAPTURED_YET;
    QMutex statusMutex;

   /*********************************
    *   modules
    **********************************/
	cv::Mat preCapturedMatImg;
    bool waitForResponse;           // preventing duplicated capture.
    bool resourceFin;               // flag for updating resource finished
    
    /*********************************
    *   lower ui
    **********************************/
    // lower image
    QPushButton * capturedImg[D_UI_NUMBER_OF_LOWER_UI_IMGS];    
    int index[D_UI_NUMBER_OF_LOWER_UI_IMGS];                    // real index of image ex) 201, 203 ....
    int curIdx;                                                 // current index
    // int maxIdx;
    int viewIdx;

    /*********************************
    *  distance sensor
    **********************************/    
    int distanceSensor_retryCnt;
    QMutex distMutex;

    void increaseTrialCnt();
    int getTrialCnt();
    void clearTrialCnt();
    

    /*********************************
    *   set method
    **********************************/
    void setIpAddress();
    QString IP;
    QByteArray tempQs; //temp data for QString -> char*

    void setProcess();
    QString process;

    void setCellInfo();
    QString cellInfo;

    void setPort();
    void setImgMatchRate();
    /*********************************
    *   draw image method
    **********************************/
    void updateCurIdx(int idx);

    void drawImg(bool draw, int x, int y,int matchRate, bool result, uchar errcode);

    void updateLowerUI(int startIdx);

    void imgClickEvent(int idx);
    

    /*
    *   capture Status
    */
   bool setCaptureStatus(CaptureStatus status);
   bool canWeCaptureNow();
   

   int outputHWoperating;
   bool isOutputHWOperating();
   void outputHWsetOperatingFlag(int flag, bool onOff);

   void checkOutputHWFinished();

signals :
    void updateRawImgFin();
    void updateDbReq();
};

#endif // MAINWINDOW_H
