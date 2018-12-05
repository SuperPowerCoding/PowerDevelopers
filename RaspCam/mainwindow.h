#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "Camera/camera.h"
#include "Network/network.h"
#include "Hardware/Buzzer/buzzer.h"
#include "Hardware/Key/key.h"
#include "Resource/resource.h"

namespace Ui {
class MainWindow;
}


#define MAX_CAPURES  5
#define ERR_NONE                0
#define ERR_ITEM_REMAINED       1
#define ERR_ALREADY_REGISTERED  2
#define ERR_PRE_SEQ_FAIL        3
#define ERR_UNKNOWN              4


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_exitButton_clicked();



    void streamImg();

    void getRawImg();

    void on_matchRateSlider_sliderMoved(int position);
	
	void updateIPResult();

    void updateResource();

    // external button
    void on_externalButton_pressed();

    void on_ResetButton_clicked();

    void on_leftButton_clicked();
    void on_rightButton_clicked();

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

    // capture!!
    void on_streamingImg_clicked();

    // lower ui clicked
    void on_img0_clicked();
    void on_img1_clicked();
    void on_img2_clicked();
    void on_img3_clicked();
    void on_img4_clicked();

    void on_cellinfocb_currentTextChanged(const QString &arg1);

    void on_updateButton_clicked();

private:
    Ui::MainWindow *ui;
    Camera * camTh = NULL;
    Network * netTh= NULL;
    Buzzer * buzzerTh= NULL;
    Key * keyTh = NULL;

    void updateCurIdx(int idx);

    void drawImg(bool draw, int x, int y,int matchRate, bool result, uchar errcode);

    void updateLowerUI(int startIdx);

    QPoint m_down;
    QPoint m_up;

    Resource * res;
	cv::Mat preCapturedMatImg;


    int curIdx;
    int maxIdx;
    int viewIdx;

    QPushButton * capturedImg[D_UI_NUMBER_OF_LOWER_UI_IMGS];
    // Mat img[D_UI_NUMBER_OF_LOWER_UI_IMGS];
    int index[D_UI_NUMBER_OF_LOWER_UI_IMGS];

    bool resourceFin;

    void setIpAddress();
    QString IP;
    QByteArray tempQs; //temp data for QString -> char*

    void setProcess();
    QString process;

    void setCellInfo();
    QString cellInfo;

    void setPort();
    void setImgMatchRate();


    void imgClickEvent(int idx);

    bool waitForResponse;

signals :
    void updateRawImgFin();
    void updateDbReq();
};

#endif // MAINWINDOW_H
