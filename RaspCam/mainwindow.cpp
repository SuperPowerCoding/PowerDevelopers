/* * * * * * * * * * * * * * * * * * * *
 * include  standard header
 * * * * * * * * * * * * * * * * * * * */
#include <QDebug>
#include <qevent.h>
#include <QTime>

/* * * * * * * * * * * * * * * * * * * *
 * include  coustomized header
 * * * * * * * * * * * * * * * * * * * */
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Camera/camera.h"
#include "config.h"

using namespace std;

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
    

    /******************************************
    *   basic module initialization
    ******************************************/
    res = new Resource();
    /* init camera */
    this->camTh = new Camera(D_CAMERA_POLLING_MSEC, D_CAMEARA_CAPTURE_WIDTH, D_CAMEARA_CAPTURE_HEIGHT);
    connect(this->camTh, SIGNAL(captureImg()), this, SLOT(streamImg()));
    this->camTh->start();

    /* init nework */
    this->netTh = new Network(D_NETWORK_SLEEP_MSEC,res);

    connect(this, SIGNAL(updateRawImgFin()), this->netTh, SLOT(sendRawImgData()));
	connect(this->netTh, SIGNAL(imgProcessFin()), this, SLOT(updateIPResult()));
    connect(this->netTh, SIGNAL(resourceUpdateFin()), this, SLOT(updateResource()));
    connect(this, SIGNAL(updateDbReq()), this->netTh, SLOT(updateDbAccuracies()));
    this->netTh->start();

    /******************************************
    *   optional module initialization
    ******************************************/
    /* buzzer init */
    this->buzzerTh = new Buzzer();
    this->buzzerTh->start();

    /* key init */
    this->keyTh = new Key();
    // connect key pressed signal.
    connect(this->keyTh, SIGNAL(keyPressed()), this, SLOT(on_externalButton_pressed()));
    this->keyTh->start();

    /* vibmotor init */
    this->vibTh = new VibMotor();
    this->vibTh->start();

    /* laser sensor init */
    this->laserTh = new LaserSensor();
    distanceSensor_retryCnt = 0;
    // connect laser sensor
    connect(this->laserTh, SIGNAL(approachingDetected()), this, SLOT(on_externalButton_pressed()));
    this->laserTh->start();


    /******************************************
    *   UI initialization
    ******************************************/
    /* setup ui */
    ui->setupUi(this);

	// ip combo box
    for(int i = 0 ; i <= 255 ; i++)
    {
        QString s = QString::number(i);

        ui->ipcb1->addItem(s);
        ui->ipcb2->addItem(s);
        ui->ipcb3->addItem(s);
        ui->ipcb4->addItem(s);
    }

    // defulat server ip : 192.168.1.102
    ui->ipcb1->setCurrentIndex(ui->ipcb1->findText("192"));
    ui->ipcb2->setCurrentIndex(ui->ipcb2->findText("168"));
    ui->ipcb3->setCurrentIndex(ui->ipcb3->findText("1"));
    ui->ipcb4->setCurrentIndex(ui->ipcb4->findText("102"));
	
    ui->portcb->addItem("5001");

	// factory process
    for(int i = 1 ; i <= 5 ; i++)
    {
        QString s = "M";
        s.append(QString::number(i));
        ui->factorycb->addItem(s);
    }
	
    //ui->factorycb->addItem("FC");

    for(int i = 1 ; i <= 8 ; i++)
    {
        QString s = "F";
        s.append(QString::number(i));
        ui->factorycb->addItem(s);
    }
	
	ui->factorycb->addItem("FC");
	
    ui->factorycb->setCurrentIndex(ui->factorycb->findText("F8"));

    ui->cellinfocb->addItem("501");
    ui->cellinfocb->addItem("502");
    ui->cellinfocb->setCurrentIndex(ui->cellinfocb->findText("501"));
	
    // setting
    ui->tabWidget->setCurrentIndex(0);

    this->capturedImg[0] = ui->img0;
    this->capturedImg[1] = ui->img1;
    this->capturedImg[2] = ui->img2;
    this->capturedImg[3] = ui->img3;
    this->capturedImg[4] = ui->img4;

    this->curIdx = -1;
    for(int i = 0; i < D_UI_NUMBER_OF_LOWER_UI_IMGS; i++)
    {
        this->index[i] = 0;
        this->capturedImg[i]->resize(64,66);
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

    // test code for push data
/*
	res->pushData(NULL,0,11);
	res->pushData(NULL,0,12);
	res->pushData(NULL,0,13);
	res->pushData(NULL,0,14);
	res->pushData(NULL,0,15);
*/
    waitForResponse = false;

    // update ui
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
    QTime time;
    int size;
	uchar * data;
	
    time.start();
	Mat img = this->camTh->getCapturedImg().clone();
	
	this->preCapturedMatImg.release();
	this->preCapturedMatImg = img;
	
	data = this->camTh->Mat2RawData(this->preCapturedMatImg, &size);
    //data = this->camTh->getCapturedRawImg(&size);
	//this->preCapturedMatImg = this->camTh->getCapturedImg();
	
	qDebug() << "[ui->net]index : " << this->curIdx << "," << res->getImgIdx(this->curIdx);
	
    this->netTh->setRawImgData(data, size, res->getImgIdx(this->curIdx), res->getImgAccuracy(this->curIdx), res->getStepNum(this->curIdx), res->getIsFinal(this->curIdx));

    emit updateRawImgFin();
    cout<<"[TIME] getRawImg sec : "<<time.second()<<" ms : "<<time.msec()<<endl;


    cout<<"[TIME] getRawImg : "<<time.elapsed()<<endl;
}

void MainWindow::streamImg()
{    
    Mat img = this->camTh->getCapturedImg();//clone();

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
    this->keyTh->setLeds(false, false, false);
    this->close();
}


void MainWindow::updateIPResult()
{
	// show result data
    qDebug() << "result";
    waitForResponse = false;
    this->drawImg(true,this->netTh->ipResult.x,this->netTh->ipResult.y,this->netTh->ipResult.matchRate,this->netTh->ipResult.result, this->netTh->ipResult.err_code);
}


void MainWindow::updateResource()
{
    this->curIdx = -1;
    this->updateLowerUI(this->curIdx);
}



void MainWindow::on_streamingImg_clicked()
{
    cout<<"streaming IMG Clicked!!!"<<endl;

    if (waitForResponse == false && ui->tabWidget->currentIndex() == 1)
    {
        this->buzzerTh->playCaptureMelody();
        this->getRawImg();
        this->drawImg(false,0,0,0,true,0);
        waitForResponse = true;
        this->keyTh->setLeds(false, false, true);
    }

    /*
    this->drawImg(true,0,0,true);
    waitForResponse = true;
    */
}

void MainWindow::in_camera_focus_distance()
{
    cout<<"in_camera_focus_distance"<<endl;
    
    if (waitForResponse == false && ui->tabWidget->currentIndex() == 1)
    {
        this->buzzerTh->playCaptureMelody();
        this->getRawImg();
        this->drawImg(false,0,0,0,true,0);
        waitForResponse = true;
        this->keyTh->setLeds(false, false, true);
    }
}

void MainWindow::on_externalButton_pressed()
{
    this->on_streamingImg_clicked();
}

void MainWindow::on_matchRateSlider_sliderMoved(int position)
{
    QString s = QString::number(position);
    s.append("%");
    ui->currentRate->setText(s);
    ui->updateButton->setEnabled(true);

    this->setImgMatchRate();
}


void MainWindow::on_ResetButton_clicked()
{
    this->waitForResponse = false;
    this->curIdx = -1;
	this->viewIdx = 0;
	this->res->clear();
	
	this->netTh->resetFlag = true;
	
	for(int i = 0 ; i < D_UI_NUMBER_OF_LOWER_UI_IMGS; i++)
	{
		index[i] = 0;
	}

    ui->curStep->setText("Barcode");

    cv::Mat img = res->getClearImg();

    ui->preCapturedImg->resize(img.cols, img.rows);
    ui->preCapturedImg->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));

	updateLowerUI(viewIdx);
}

void MainWindow::on_leftButton_clicked()
{
    if(resourceFin == false)    return;

    this->viewIdx -= D_UI_NUMBER_OF_LOWER_UI_IMGS;//D_UI_NUMBER_OF_LOWER_UI_IMGS;
    if(viewIdx < 0 )
    {
        viewIdx = 0;
    }
    //this->curIdx = viewIdx;
    updateCurIdx(viewIdx);
    updateLowerUI(viewIdx);
}

void MainWindow::on_rightButton_clicked()
{
    if(resourceFin == false)    return;

    this->viewIdx += D_UI_NUMBER_OF_LOWER_UI_IMGS;
    if(viewIdx > this->res->getSize() - 1)
    {
        viewIdx = this->res->getSize() -1;
        if(viewIdx < 0 ) viewIdx = 0;
    }

    //this->curIdx = viewIdx;
    updateCurIdx(viewIdx);
    updateLowerUI(viewIdx);
}


void MainWindow::drawImg(bool draw, int x, int y,int matchRate, bool result, uchar errCode)
{
    QTime time;
    cv::Mat img = this->preCapturedMatImg; //this->camTh->getCapturedImg();

    time.start();
    cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH*4/7, D_CAMERA_DISPLAYED_HEIGHT*4/7), 0, 0, CV_INTER_LINEAR);
    ui->preCapturedImg->resize(img.cols, img.rows);
    cv::cvtColor(img, img, CV_BGR2RGB);
    if(draw == true)
    {
        if(result == true)
        {
            this->keyTh->setLeds(false, true, false);
            this->buzzerTh->playCaptureResultOKMelody();
            
            cv::rectangle(img, Point(0,0), Point(img.cols-5, img.rows), Scalar(0,255,0), 10);

            // update img
            if(this->curIdx != -1)
			{
                res->setImg(this->curIdx,img);
			}

            updateCurIdx(curIdx + 1);
			
			if(this->curIdx % D_UI_NUMBER_OF_LOWER_UI_IMGS == 0)
			{
				updateLowerUI(this->curIdx);
			}
			else
			{
				updateLowerUI(-1);
			}
        }
        else
        {
            this->keyTh->setLeds(true, false, false);
            this->buzzerTh->playWrongMelody();
            this->vibTh->ngVibrate();
            cv::rectangle(img, Point(0,0), Point(img.cols-5, img.rows), Scalar(255,0,0), 10);
        }

        QString rate = "";
        if(matchRate < 0) matchRate = 0;
        if(matchRate > 100) matchRate = 100;
        rate = QString::number(matchRate);
        rate.append("%");
        this->ui->matchRateResult->setText(rate);

        switch(errCode)
        {
        case 0:
            if(result == true)
            {
                this->ui->errCode->setText("Success.");
            }
            else
            {
                this->ui->errCode->setText("Failed.");
            }
            break;
        case 1:
            this->ui->errCode->setText("Item Remained.");
            break;

        case 2:
            this->ui->errCode->setText("Already Registered.");
            break;

        case 3:
            this->ui->errCode->setText("PreSeq Fail.");
            break;

        case 4:
        default:
            this->ui->errCode->setText("Unkown Error.");
            break;
        }
    }

    ui->preCapturedImg->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));

    int elapsed = time.elapsed();

    cout<<"[Time] drawImg : "<<elapsed<<endl;
}
void MainWindow::updateLowerUI(int indexStart)
{
    static int preIdx = 0;
	int idx;
    cv::Mat img;
    QTime time;

    time.start();
	indexStart = indexStart == -1 ? preIdx : indexStart;
	
	this->viewIdx = indexStart;
	
	qDebug() << "start" << indexStart;
	
	//this->viewIdx = indexStart == -1 ? preIndex : indexStart;
	//indexStart = indexStart == -1 ? preIndex : indexStart;
	
	qDebug() << "start" << indexStart;
	
    for(int i = 0 ; i < D_UI_NUMBER_OF_LOWER_UI_IMGS; i++)
    {
        img = res->getImgAndIdx(i + indexStart, &idx);
        //index[i] = idx;
        index[i] = this->viewIdx + i;
        // qDebug() << "updateLowerUI:" << index[i];

        // cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH*3/7, D_CAMERA_DISPLAYED_HEIGHT*3/7), 0, 0, CV_INTER_LINEAR);
        // cv::cvtColor(img, img, CV_BGR2RGB);
        qDebug() << capturedImg[i]->size().width() << ","<< capturedImg[i]->height();
        cv::resize(img, img, Size(capturedImg[i]->width(), capturedImg[i]->height()), 0, 0, CV_INTER_LINEAR);

        QPixmap qimg = QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888));
        QIcon ButtonIcon(qimg);
        //capturedImg[i]->resize(img.cols, img.rows);
        capturedImg[i]->setIconSize(qimg.rect().size());
        capturedImg[i]->setIcon(ButtonIcon);
    }
	
	preIdx = indexStart;

    int elapsed = time.elapsed();
    cout << "updateLowerUI :" << elapsed << endl;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    // qDebug() << index;
    bool camEnable = index == 1 ? true : false;

    this->camTh->enableStreaming(camEnable);

	if(index == 1)
	{
		this->netTh->userSettingFlag = true;
        this->keyTh->setLeds(false, true, false);
	}
    else
    {
        this->keyTh->setLeds(false, false, false);
	}
	
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

    //netTh->setImgRate(pos);

    this->res->setImgAccuracy(this->curIdx, pos);
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

void MainWindow::setCellInfo()   // test ok
{
    if(this->netTh == NULL) return;

    cellInfo.clear();

    cellInfo.append(ui->cellinfocb->currentText());

    // qDebug() << process;

    tempQs = cellInfo.toLatin1();

    netTh->setCellInfo(tempQs.data());
}


void MainWindow::on_factorycb_currentTextChanged(const QString &arg1)
{
    // qDebug() << arg1;
    this->setProcess();
}

void MainWindow::on_cellinfocb_currentTextChanged(const QString &arg1)
{
    this->setCellInfo();
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
void MainWindow::updateCurIdx(int idx)
{
    int temp_accuracy = 0;

    this->curIdx = idx;
    this->viewIdx = this->curIdx;

    QString s = "";
    QString ss = "";

    qDebug() << this->curIdx << ":" << this->res->getSize();
    if(this->curIdx >= this->res->getSize())
    {

        if(this->res->getSize() == 0)
        {
            s.append("Barcode");
        }
        else
        {
            s.append("OK");
            this->buzzerTh->playFinMelody();
            this->on_ResetButton_clicked();
        }

        this->curIdx = this->res->getSize();

        ui->currentRate->setText("0%");
        ui->matchRateSlider->setSliderPosition(0);
        ui->currentProcess->setText("N/A");
    }
    else
    {
        s.append("STEP ");
        s.append(QString::number(this->curIdx+1));
        temp_accuracy = this->res->getImgAccuracy(this->curIdx);
        ss.append(QString::number(temp_accuracy));
        ss.append("%");
        ui->currentRate->setText(ss);
        ui->matchRateSlider->setSliderPosition(temp_accuracy);
        ui->currentProcess->setText(s);
    }

    // QString s = "STEP ";
    // s.append(QString::number(this->curIdx+1));
    ui->curStep->setText(s);



}

void MainWindow::imgClickEvent(int idx)
{
    //if( index[idx] > 0 )
    {
        // this->curIdx = index[idx];		
        //int newindex = this->res->getIndexOf(index[idx]);
        int newindex = viewIdx + idx;
        qDebug() << idx << ":" << newindex;

        if(newindex != -1)
        {
            this->updateCurIdx(newindex);
            int view = this->curIdx - this->curIdx % 5;

            // this->curIdx -= this->curIdx % 5;
            updateLowerUI(view);
        }
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

void MainWindow::on_updateButton_clicked()
{
    ui->updateButton->setEnabled(false);

    emit updateDbReq();
}
