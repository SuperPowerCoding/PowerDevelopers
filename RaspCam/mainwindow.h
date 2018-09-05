#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Camera/camera.h"
#include "Network/network.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_exitButton_clicked();

    void on_captureButton_clicked();

    void streamImg();

    void getRawImg();

    void on_matchRateSlider_sliderMoved(int position);
	
	void updateIPResult();

private:
    Ui::MainWindow *ui;
    Camera * camTh;
    Network * netTh;

    void drawImg(int idx,int x, int y, bool result, bool shift);

signals :
    void updateRawImgFin();
};

#endif // MAINWINDOW_H
