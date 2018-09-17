/* * * * * * * * * * * * * * * * * * * *
 * include  standard header
 * * * * * * * * * * * * * * * * * * * */
#include <QDebug>
#include <qevent.h>

/* * * * * * * * * * * * * * * * * * * *
 * include  coustomized header
 * * * * * * * * * * * * * * * * * * * */
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Camera/camera.h"
#include "config.h"



/*****************************************
 *
 *  constructor and destructor
 *
*******************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // set titleless window
    // this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::Popup);


    // this->setWindowFlags(Qt::Popup);
    res = new Resource();


    // init camera
    this->camTh = new Camera(D_CAMERA_POLLING_MSEC, D_CAMEARA_CAPTURE_WIDTH, D_CAMEARA_CAPTURE_HEIGHT);
    connect(this->camTh, SIGNAL(captureImg()), this, SLOT(streamImg()));
    this->camTh->start();

    // init nework
    this->netTh = new Network(D_NETWORK_SLEEP_MSEC,res);

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

    for(int i = 1 ; i <= 5 ; i++)
    {
        QString s = "M";
        s.append(QString::number(i));
        ui->factorycb->addItem(s);
    }

    for(int i = 1 ; i <= 3 ; i++)
    {
        QString s = "F";
        s.append(QString::number(i));
        ui->factorycb->addItem(s);
    }

    // setting
    ui->tabWidget->setCurrentIndex(0);

    this->capturedImg[0] = ui->img0;
    this->capturedImg[1] = ui->img1;
    this->capturedImg[2] = ui->img2;
    this->capturedImg[3] = ui->img3;
    this->capturedImg[4] = ui->img4;

    this->curIdx = 0;
    for(int i = 0; i < D_UI_NUMBER_OF_LOWER_UI_IMGS; i++)
    {
        this->index[i] = 0;
    }


/*  test code for image read
    res->pushData(NULL,0,1);
    int index;
    qDebug() << "1";
    cv::Mat img = res->getImgAndIdx(0,&index);
    qDebug() << "2";
    cv::cvtColor(img, img, CV_BGR2RGB);
    qDebug() << "3";
    cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH*4/7, D_CAMERA_DISPLAYED_HEIGHT*4/7), 0, 0, CV_INTER_LINEAR);
    ui->preCapturedImg->resize(img.cols, img.rows);
    ui->preCapturedImg->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
    */

    updateLowerUI(0);
}

MainWindow::~MainWindow()
{
    qDebug() << "delete ui";    
    delete ui;
}


/*****************************************
 *
 *  event method
 *
*******************************************/
void MainWindow::getRawImg()
{
    int size;
	uchar * data;

    data = this->camTh->getCapturedRawImg(&size);
	
    this->netTh->setRawImgData(data, size, res->getImgIdx(this->curIdx));

    emit updateRawImgFin();

}

void MainWindow::streamImg()
{
    Mat img = this->camTh->getCapturedImg();

    cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH, D_CAMERA_DISPLAYED_HEIGHT), 0, 0, CV_INTER_LINEAR);
    cv::cvtColor(img, img, CV_BGR2RGB);

    QPixmap qimg = QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888));
    QIcon ButtonIcon(qimg);
    ui->streamingImg->resize(img.cols, img.rows);
    ui->streamingImg->setIconSize(qimg.rect().size());
    ui->streamingImg->setIcon(ButtonIcon);

}

void MainWindow::on_exitButton_clicked()
{
    this->camTh->exit();
    this->close();
}


void MainWindow::updateIPResult()
{
	// show result data
    qDebug() << "result";
    this->drawImg(true,this->netTh->ipResult.x,this->netTh->ipResult.y,this->netTh->ipResult.result);
}


void MainWindow::updateResource()
{
    this->curIdx = 0;
    this->updateLowerUI(this->curIdx);
}



void MainWindow::on_streamingImg_clicked()
{
    this->buzzerTh->playCaptureMelody();
    this->getRawImg();
    this->drawImg(false,0,0,true);
}

void MainWindow::on_externalButton_pressed()
{
    this->buzzerTh->playCaptureMelody();

    this->getRawImg();
    this->drawImg(false,0,0,true);
}

void MainWindow::on_matchRateSlider_sliderMoved(int position)
{
    QString s = QString::number(position);
    s.append("%");
    ui->currentRate->setText(s);

    this->setImgMatchRate();
}


void MainWindow::on_ResetButton_clicked()
{
    this->curIdx = 0;
}

void MainWindow::on_leftButton_clicked()
{
    if(resourceFin == false)    return;

    this->viewIdx -= D_UI_NUMBER_OF_LOWER_UI_IMGS;
    if(viewIdx < 0 )
    {
        viewIdx = 0;
    }
    updateLowerUI(viewIdx);
}

void MainWindow::on_rightButton_clicked()
{
    if(resourceFin == false)    return;

    this->viewIdx += D_UI_NUMBER_OF_LOWER_UI_IMGS;
    if(viewIdx > this->res->getSize() - D_UI_NUMBER_OF_LOWER_UI_IMGS)
    {
        viewIdx = this->res->getSize() - D_UI_NUMBER_OF_LOWER_UI_IMGS;
        if(viewIdx < 0 ) viewIdx = 0;
    }

    qDebug() << viewIdx;

    updateLowerUI(viewIdx);
}


void MainWindow::drawImg(bool draw, int x, int y, bool result)
{
    cv::Mat img = this->camTh->getCapturedImg();
    cv::cvtColor(img, img, CV_BGR2RGB);

    cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH*4/7, D_CAMERA_DISPLAYED_HEIGHT*4/7), 0, 0, CV_INTER_LINEAR);
    ui->preCapturedImg->resize(img.cols, img.rows);

    if(draw == true)
    {
        if(result == true)
        {
            this->buzzerTh->playCaptureResultOKMelody();
            cv::rectangle(img, Point(0,0), Point(img.cols-5, img.rows), Scalar(0,255,0), 10);

            // update img
            res->setImg(this->curIdx,img);
            this->curIdx++;

            QString s = QString::number(this->curIdx);
            ui->curStep->setText(s);
            updateLowerUI(this->curIdx);
        }
        else
        {
            this->buzzerTh->playWrongMelody();
            cv::rectangle(img, Point(0,0), Point(img.cols-5, img.rows), Scalar(255,0,0), 10);
        }
    }

    ui->preCapturedImg->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
}
void MainWindow::updateLowerUI(int indexStart)
{
    int idx;
    cv::Mat img;

    for(int i = 0 ; i < D_UI_NUMBER_OF_LOWER_UI_IMGS; i++)
    {
        img = res->getImgAndIdx(i + indexStart, &idx);
        index[i] = idx;

        cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH*3/5, D_CAMERA_DISPLAYED_HEIGHT*3/5), 0, 0, CV_INTER_LINEAR);
        cv::cvtColor(img, img, CV_BGR2RGB);

        QPixmap qimg = QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888));
        QIcon ButtonIcon(qimg);
        capturedImg[i]->resize(img.cols, img.rows);
        capturedImg[i]->setIconSize(qimg.rect().size());
        capturedImg[i]->setIcon(ButtonIcon);
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    // qDebug() << index;
    bool camEnable = index == 1 ? true : false;

    this->camTh->enableStreaming(camEnable);

}

void MainWindow::setIpAddress() // test ok
{
    static QComboBox * order[] =
    {
        ui->ipcb1, ui->ipcb2, ui->ipcb3, ui->ipcb4
    };

    if(this->netTh == NULL) return;

    IP.clear();

    for(int i = 0 ; i < 4 ; i++)
    {
        IP.append(order[i]->currentText());
        if(i != 3)
        {
            IP.append(".");
        }
    }

    // qDebug() << IP;

    tempQs = IP.toLatin1();

    netTh->setServerIpAddress(tempQs.data());

}

void MainWindow::setPort()  // test ok
{
    if(this->netTh == NULL) return;

    int port = this->ui->portcb->currentText().toInt();

    // qDebug() << port;

    netTh->setPort(port);
}

void MainWindow::setImgMatchRate()  // test ok
{
    if(this->netTh == NULL) return;
    int pos = this->ui->matchRateSlider->sliderPosition();

    // qDebug() << pos;

    netTh->setImgRate(pos);
}

void MainWindow::setProcess()   // test ok
{
    if(this->netTh == NULL) return;

    process.clear();

    process.append(ui->factorycb->currentText());

    // qDebug() << process;

    tempQs = process.toLatin1();

    netTh->setProcess(tempQs.data());
}



void MainWindow::on_factorycb_currentTextChanged(const QString &arg1)
{
    // qDebug() << arg1;
    this->setProcess();
}

/*
 *  Network setting changed event
 */
void MainWindow::on_ipcb1_currentIndexChanged(const QString &arg1)
{
    this->setIpAddress();
}

void MainWindow::on_ipcb2_currentIndexChanged(const QString &arg1)
{
    this->setIpAddress();
}

void MainWindow::on_ipcb3_currentIndexChanged(const QString &arg1)
{
    this->setIpAddress();
}

void MainWindow::on_ipcb4_currentIndexChanged(const QString &arg1)
{
    this->setIpAddress();
}

void MainWindow::on_portcb_currentIndexChanged(const QString &arg1)
{
    this->setPort();
}


/*
 *      lower img click event
 */

void MainWindow::imgClickEvent(int idx)
{
    if( index[idx] > 0 )
    {
        this->curIdx = index[idx];
        QString s = QString::number(this->curIdx);
        ui->curStep->setText(s);
        updateLowerUI(this->curIdx);
    }
}

void MainWindow::on_img0_clicked()
{
    imgClickEvent(0);
}

void MainWindow::on_img1_clicked()
{
    imgClickEvent(1);
}

void MainWindow::on_img2_clicked()
{
    imgClickEvent(2);
}

void MainWindow::on_img3_clicked()
{
    imgClickEvent(3);
}

void MainWindow::on_img4_clicked()
{
    imgClickEvent(4);
}
