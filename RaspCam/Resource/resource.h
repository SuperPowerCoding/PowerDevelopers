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
    Resource();
    ~Resource();

    void clear();


    void pushData(char * data, int size, int index, int accuracy);

    cv::Mat getImgAndIdx(int idx, int * index);
    void setImg(int idx, cv::Mat img);

    int getImgIdx(int idx);
    int getImgAccuracy(int idx);
    void setImgAccuracy(int idx, int val);
    void updateImg(int idx,cv::Mat img);

    int getSize();
    int getIndexOf(int idx);

    int getAccChangedSize(void);
    bool getAccChangedFlag(int idx);

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
