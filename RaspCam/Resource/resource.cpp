#include <QDebug>
#include <QTime>

#include "resource.h"
#include "../config.h"

using namespace std;

Resource::Resource()
{
    blank = cv::Mat(200,200,CV_8UC3, cv::Scalar(255, 255, 255));
}

Resource::~Resource()
{
    this->clear();
}

void Resource::clear()
{
    this->mutex.lock();


    //qDeleteAll(this->imgs.begin(), this->imgs.end());
    this->imgs.clear();

    //qDeleteAll(this->indexs.begin(), this->indexs.end());
    this->indexs.clear();
    this->accuracies.clear();
    this->accChanged.clear();

    this->mutex.unlock();


}

void Resource::pushData(char * data, int size, int index, int accuracy)
{
    //std::vector<uchar> jpgbytes; // from your db
    //std::vector<char> data(buffer, buffer + size);
    //std::vector<uchar> jpgbytes(data, data + size); // from your db
    QTime time;

    //cv::Mat img = cv::imdecode(jpgbytes);
    cv::Mat img;

    time.start();
    if( data == NULL )
    {

        QString s = IMAGE_BASE_FOLDER_PATH;
        s.append(QString::number(index));
        s.append(".jpg");
        std::string utf8_text = s.toUtf8().constData();
        img = cv::imread(utf8_text);
        if(img.empty())
        {
            qDebug() << "fail to read file :" <<s;
            return;
        }
        cv::cvtColor(img, img, CV_BGR2RGB);

    }
    else
    {
        img = cv::imdecode(cv::Mat(1, size, CV_8UC1, data), CV_LOAD_IMAGE_UNCHANGED);        
    }

    this->mutex.lock();

    this->imgs.append(img);
    this->indexs.append(index);
    this->accChanged.append(false);
    this->accuracies.append(accuracy);

    this->mutex.unlock();

    int elapsed = time.elapsed();

    cout << "pushData : "<<elapsed<<endl;
}

int Resource::getSize()
{
    int size;
    this->mutex.lock();

    size = this->imgs.size();
    this->mutex.unlock();

    return size;

}
void Resource::updateImg(int idx,cv::Mat img)
{
    this->mutex.lock();

    this->imgs.removeAt(idx);
    this->imgs.insert(idx,img);

    this->mutex.unlock();
}

cv::Mat Resource::getClearImg()
{
    return blank;
}

cv::Mat Resource::getImgAndIdx(int idx, int * index)
{
    cv::Mat img;

    this->mutex.lock();

    if( idx >= imgs.size() || idx < 0)
    {
        img = blank;
        *index = 0;
    }
    else
    {
        img = this->imgs.at(idx);
        *index = this->indexs.at(idx);
    }

    this->mutex.unlock();

    return img;

}

int Resource::getIndexOf(int idx)
{
    int index = 0;

    this->mutex.lock();

    index = this->indexs.indexOf(idx);


    this->mutex.unlock();

    return index;
}

void Resource::setImg(int idx, cv::Mat img)
{
    this->mutex.lock();

    if(this->indexs.size() > idx )
    {
        this->imgs.removeAt(idx);
        this->imgs.insert(idx,img);
    }


    this->mutex.unlock();
}

int Resource::getImgIdx(int idx)
{
    int index;

    // idx = idx - 1;
    if(idx < 0 ) return 0;

    this->mutex.lock();

    if(this->indexs.size() < idx )
    {
        index = 0;
    }
    else
    {
        index = this->indexs.at(idx);
    }

    this->mutex.unlock();

    return index;
}

int Resource::getImgAccuracy(int idx)
{
    int accuracy;

    if(idx < 0) return 0;

    this->mutex.lock();

    if(this->accuracies.size() < idx)
    {
        accuracy = 0;
    }
    else
    {
        accuracy = this->accuracies.at(idx);
    }

    this->mutex.unlock();

    return accuracy;
}

void Resource::setImgAccuracy(int idx, int val)
{
    if(idx < 0 ) return ;

    this->mutex.lock();

    if(this->accuracies.size() > idx )
    {
        this->accuracies[idx] = val;
        this->accChanged[idx] = true;
    }

    this->mutex.unlock();
}

int Resource::getAccChangedSize(void)
{
    return this->accChanged.size();
}

bool Resource::getAccChangedFlag(int idx)
{
    return this->accChanged.at(idx);
}
