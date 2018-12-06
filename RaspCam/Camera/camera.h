#ifndef CAMERA_H
#define CAMERA_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include <raspicam/raspicam_cv.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include "config.h"

using namespace cv;


class Camera : public QThread
{
    Q_OBJECT

public:
    /* constructor */
    Camera(unsigned int msecPollingPeriod, unsigned int width, unsigned int height);


    bool initCamera(unsigned int width, unsigned int height);

    /* set method */
    void setPollingPeriod(unsigned int msec);
    void setCameraSize(unsigned int width, unsigned int height);

    /* get method */
    cv::Mat getCapturedImg();
    uchar * getCapturedRawImg(int * size);


    void exit();                    // When you call this function, the camera thread will be dead.
    bool isEnable();
    
    /* enable camera */
    void enableStreaming(bool enable);

    /* img process */
    uchar * Mat2RawData(Mat image, int * size); // open cv Mat to uchar converter

private:
    /* private members */
    // basic task members
    unsigned int pollingPeriod;     // task sleep ms    
    bool _exit;                     // if _exit = true -> thread will be dead
    bool _enabled;                  // enable streaming
    
    // camera
    raspicam::RaspiCam_Cv cam;      // control raspicam
    
    // image
    cv::Mat capturedImg;            // captured img
    QMutex captuerImgMutex;         // mutex for critical section for capturedImg

    // task 
    void run();

    void updateImg(cv::Mat img);
    void close();

    

signals:
    void captureImg();              // image capture event
};

#endif // CAMERA_H
