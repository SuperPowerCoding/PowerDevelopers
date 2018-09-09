#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Camera/camera.h"
#include "config.h"

#include <qevent.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // set titleless window
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::Popup);

    //this->setWindowFlags(Qt::Popup);
    res = new Resource();


    // init camera
    this->camTh = new Camera(D_CAMERA_POLLING_MSEC, D_CAMEARA_CAPTURE_WIDTH, D_CAMEARA_CAPTURE_HEIGHT);
    connect(this->camTh, SIGNAL(captureImg()), this, SLOT(streamImg()));
    this->camTh->start();

    // init nework
    this->netTh = new Network(D_NETWORK_SLEEP_MSEC,res);
    // connect(this->netTh, SIGNAL(updateRawImg()), this, SLOT(getRawImg()));
    connect(this, SIGNAL(updateRawImgFin()), this->netTh, SLOT(sendRawImgData()));
	connect(this->netTh, SIGNAL(imgProcessFin()), this, SLOT(updateIPResult()));
    connect(this->netTh, SIGNAL(resourceUpdateFin()), this, SLOT(updateResource()));
    this->netTh->start();

    // buzzer init
    this->buzzerTh = new Buzzer();
    this->buzzerTh->start();

    // key init
    this->keyTh = new Key();
    connect(this->keyTh, SIGNAL(keyPressed()), this, SLOT(on_externalButton_pressed()));
    this->keyTh->start();

    ui->setupUi(this);

    for(int i = 0 ; i <= 255 ; i++)
    {
        QString s = QString::number(i);

        ui->ipcb1->addItem(s);
        ui->ipcb2->addItem(s);
        ui->ipcb3->addItem(s);
        ui->ipcb4->addItem(s);
    }

    ui->portcb->addItem("8080");

    ui->factorycb->addItem("main 8");

}

MainWindow::~MainWindow()
{
    qDebug() << "delete ui";    
    delete ui;
}

void MainWindow::getRawImg()
{
    int size;
	uchar * data;
	
	data = this->camTh->getCapturedRawImg(&size);  	
	
    this->netTh->setRawImgData(data, size, res->getimgIdx(this->curIdx));

	
    emit updateRawImgFin();
}

void MainWindow::streamImg()
{
    Mat img = this->camTh->getCapturedImg();

    cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH, D_CAMERA_DISPLAYED_HEIGHT), 0, 0, CV_INTER_LINEAR);
    cv::cvtColor(img, img, CV_BGR2RGB);

    ui->realtimeImg->resize(img.cols, img.rows);
    ui->realtimeImg->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
}

void MainWindow::on_exitButton_clicked()
{
    this->camTh->exit();
    this->close();
}


void MainWindow::updateIPResult()
{
	// show result data
    this->drawImg(0,this->netTh->ipResult.x,this->netTh->ipResult.y,this->netTh->ipResult.result,false);
}


void MainWindow::updateResource()
{
    this->curIdx = 0;   //
    this->maxIdx = res->getSize();
    resourceFin = true;

    for(int i = 0 ; i < MAX_CAPURES - 1 ;i++)
    {
        img[i+1] = res->getData(i, (index+i));
    }

    // update image
    drawImg(-1,0,0,false,false);

}



void MainWindow::drawImg(int idx,int x, int y, bool result, bool capture)
{

    static int cnt = 1;
    static int size[][2] =
    {
        {D_CAMERA_DISPLAYED_WIDTH*4/7, D_CAMERA_DISPLAYED_HEIGHT*4/7},
        {D_CAMERA_DISPLAYED_WIDTH*3/11, D_CAMERA_DISPLAYED_HEIGHT*3/11},
        {D_CAMERA_DISPLAYED_WIDTH*3/11, D_CAMERA_DISPLAYED_HEIGHT*3/11},
        {D_CAMERA_DISPLAYED_WIDTH*3/11, D_CAMERA_DISPLAYED_HEIGHT*3/11},
        {D_CAMERA_DISPLAYED_WIDTH*3/11, D_CAMERA_DISPLAYED_HEIGHT*3/11},
    };
    bool shift = false;

    static QLabel * lb[MAX_CAPURES] =
    {
        ui->capturedImg1, ui->capturedImg2, ui->capturedImg3, ui->capturedImg4, ui->capturedImg5,
    };

    if(capture == true)
	{
		img[0] = this->camTh->getCapturedImg();
		cv::cvtColor(img[0], img[0], CV_BGR2RGB);
	}
    else
    {
        if(0 <= idx && idx < MAX_CAPURES)
        {
            if(result == true)
            {
                this->buzzerTh->playBonusUp();
                shift = true;


                cv::rectangle(img[idx], Point(0,0), Point(img[idx].cols-5, img[idx].rows), Scalar(0,255,0), 10);


                this->curIdx++;
                QString s = QString::number(this->curIdx+1);
                ui->curStep->setText(s);
            }
            else
            {
                this->buzzerTh->playWrongMelody();
                cv::rectangle(img[idx], Point(0,0), Point(img[idx].cols-5, img[idx].rows), Scalar(255,0,0), 10);
            }
        }
    }

    for(int i = 0 ; i < cnt ; i++)
    {
        cv::resize(img[i], img[i], Size(size[i][0], size[i][1]), 0, 0, CV_INTER_LINEAR);
        lb[i]->resize(img[i].cols, img[i].rows);
        lb[i]->setPixmap(QPixmap::fromImage(QImage(img[i].data, img[i].cols, img[i].rows, img[i].step, QImage::Format_RGB888)));
    }

    if(shift)
	{
		for(int i = MAX_CAPURES - 1; i > 0 ;i--)
		{
			img[i] = img[i - 1].clone();
		}
		if(++cnt > (MAX_CAPURES-1)) cnt = MAX_CAPURES;
	}
}

void MainWindow::on_captureButton_clicked()
{
    this->getRawImg();
    this->drawImg(-1,0,0, false, true);

}

void MainWindow::on_externalButton_pressed()
{
    this->buzzerTh->playGetCoinMelody();

    this->getRawImg();
    this->drawImg(-1,0,0, false, true);
}

void MainWindow::on_matchRateSlider_sliderMoved(int position)
{
    QString s = QString::number(position);
    s.append("%");
    ui->currentRate->setText(s);
}


void MainWindow::on_ResetButton_clicked()
{
    this->curIdx = 0;
}

void MainWindow::on_leftButton_clicked()
{
    if(resourceFin == false)    return;

    this->viewIdx--;
    if(viewIdx < 0 )
    {
        viewIdx = 0;
    }
    updateLowerUI();
}

void MainWindow::on_rightButton_clicked()
{
    if(resourceFin == false)    return;

    this->viewIdx++;
    if(viewIdx > MAX_CAPURES - 1)
    {
        viewIdx = MAX_CAPURES - 1;
    }
    updateLowerUI();
}

void MainWindow::updateLowerUI()
{
    int index;
    for(int i = 0 ; i < MAX_CAPURES - 1 ;i++)
    {
        img[i+1] = res->getData(i+viewIdx, &index);
    }

    // update image
    drawImg(-1,0,0,false,false);
}
