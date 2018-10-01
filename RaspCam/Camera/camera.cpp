#include <QDebug>

#include "camera.h"
#include "../config.h"

Camera::Camera(unsigned int msecPollingPeriod, unsigned int width, unsigned int height)
{
    this->_exit = false;
    this->pollingPeriod = msecPollingPeriod;
    this->_enabled = false;
    // this->capturedImg = NULL;

    initCamera(width, height);

    qDebug() << "init Camera Thread Compelete : " << this->isEnable();

}

void Camera::setCameraSize(unsigned int width, unsigned int height)
{
    this->cam.set ( CV_CAP_PROP_FORMAT, CV_8UC3 );
    this->cam.set ( CV_CAP_PROP_FRAME_WIDTH, width);
    this->cam.set ( CV_CAP_PROP_FRAME_HEIGHT, height);
}

bool Camera::initCamera(unsigned int width, unsigned int height)
{
    // do not need to init
    if(this->cam.isOpened()){
        return true;
    }

    this->setCameraSize(width, height);
    if( !this->cam.open())
    {
        return false;
    }
    return true;
}

void Camera::close()
{
    if(this->cam.isOpened())
    {
        this->cam.release();
    }
}

bool Camera::isEnable()
{
    return this->cam.isOpened();
}

void Camera::setPollingPeriod(unsigned int msec)
{
    this->pollingPeriod = msec;
}

void Camera::updateImg(Mat img)
{
    captuerImgMutex.lock();
    this->capturedImg = img;
    captuerImgMutex.unlock();
}

cv::Mat Camera::getCapturedImg()
{
    Mat img;
    // oImg.copyTo(cImg5);
    captuerImgMutex.lock();
    img = this->capturedImg;
    captuerImgMutex.unlock();

    return img;
}



uchar * Camera::Mat2RawData(Mat image, int * size)
{
	uchar * data;
    
    std::vector<int> param(2);
	std::vector<uchar> buff;
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = 80;//default(95) 0-100
    cv::imencode(".jpg", image, buff, param);
	
	//
	*size = buff.size();
	
	data = new uchar[*size];
	
	std::copy(buff.begin(), buff.end(), data);
	
	return data;
}


uchar * Camera::getCapturedRawImg(int * size)
{
    Mat image;
	uchar * data;
	
    captuerImgMutex.lock();
    image = this->capturedImg;

    std::vector<int> param(2);
	std::vector<uchar> buff;
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = 80;//default(95) 0-100
    cv::imencode(".jpg", image, buff, param);
	
	
	//
	*size = buff.size();
	
	data = new uchar[*size];
	
	std::copy(buff.begin(), buff.end(), data);
	
    captuerImgMutex.unlock();
	
	return data;
}


void Camera::enableStreaming(bool enable)
{
    this->_enabled = enable;
}

void Camera::run()
{
    cv::Mat img;
	
	cam.grab();
	cam.retrieve(img);
	this->updateImg(img);
	
    while(!this->_exit)
    {
        // qDebug() << "Cam Th";
        if(_enabled == true)
        {
            cam.grab();
            cam.retrieve(img);

            // cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH, D_CAMERA_DISPLAYED_HEIGHT), 0, 0, CV_INTER_LINEAR);

            // image update
            this->updateImg(img);
            emit captureImg();   // stream img
        }

        msleep(this->pollingPeriod);
    }

    qDebug() << "release camera resource..";

    this->close();
}

void Camera::exit()
{
    this->_exit = true;
}
