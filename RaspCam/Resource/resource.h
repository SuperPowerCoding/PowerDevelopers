#ifndef RESOURCE_CPP
#define RESOURCE_CPP

#include <QMutex>
#include <QThread>
#include <QObject>


#include <raspicam/raspicam_cv.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

class Resource
{
    // Q_OBJECT

public:
    /*******************************
     *  constructor & destructor     
     *******************************/
    Resource();
    ~Resource();

    // push data to resource
    void pushData(char * data, int size, int index, int accuracy);

    // data all clear    
    void clear();
    
    // get image and index
    cv::Mat getImgAndIdx(int idx, int * index);
    
    // set image at idx
    void setImg(int idx, cv::Mat img);

    // get image index at idx
    int getImgIdx(int idx);

    // get image accuracy at idx
    int getImgAccuracy(int idx);

    // set image accracy at idx
    void setImgAccuracy(int idx, int val);

    // duplicated
    // void updateImg(int idx,cv::Mat img);

    // get total resource size
    int getSize();

    // find list index that have image idx
    int getIndexOf(int idx);

    int getAccChangedSize(void);
    bool getAccChangedFlag(int idx);
    int getStepNum(int idx);
    uchar getIsFinal(int idx);

    cv::Mat getClearImg();


private:


    QList<int> indexs;
    QList<int> accuracies;
    QList<bool> accChanged;
    QList<cv::Mat> imgs;


    QMutex mutex;


    cv::Mat blank;
};



#endif // RESOURCE_CPP
