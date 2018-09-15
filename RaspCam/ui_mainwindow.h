/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *SettingTab;
    QGroupBox *groupBox;
    QLabel *label_3;
    QComboBox *ipcb1;
    QComboBox *ipcb2;
    QComboBox *ipcb3;
    QComboBox *ipcb4;
    QComboBox *portcb;
    QLabel *label_4;
    QGroupBox *groupBox_2;
    QSlider *matchRateSlider;
    QLabel *label_5;
    QLabel *currentRate;
    QGroupBox *groupBox_3;
    QComboBox *factorycb;
    QPushButton *exitButton;
    QWidget *CameraTab;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *preCapturedImg;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *capturedImgLayout;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ResetButton;
    QLabel *curStep;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *streamingImg;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(487, 320);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(90, 90, 90, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        MainWindow->setPalette(palette);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(-2, -1, 491, 321));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        tabWidget->setFont(font);
        SettingTab = new QWidget();
        SettingTab->setObjectName(QStringLiteral("SettingTab"));
        groupBox = new QGroupBox(SettingTab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 461, 81));
        groupBox->setContextMenuPolicy(Qt::NoContextMenu);
        groupBox->setAutoFillBackground(false);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 20, 67, 21));
        ipcb1 = new QComboBox(groupBox);
        ipcb1->setObjectName(QStringLiteral("ipcb1"));
        ipcb1->setGeometry(QRect(10, 40, 51, 21));
        ipcb2 = new QComboBox(groupBox);
        ipcb2->setObjectName(QStringLiteral("ipcb2"));
        ipcb2->setGeometry(QRect(70, 40, 51, 21));
        ipcb3 = new QComboBox(groupBox);
        ipcb3->setObjectName(QStringLiteral("ipcb3"));
        ipcb3->setGeometry(QRect(130, 40, 51, 21));
        ipcb4 = new QComboBox(groupBox);
        ipcb4->setObjectName(QStringLiteral("ipcb4"));
        ipcb4->setGeometry(QRect(190, 40, 51, 21));
        portcb = new QComboBox(groupBox);
        portcb->setObjectName(QStringLiteral("portcb"));
        portcb->setGeometry(QRect(270, 40, 61, 21));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(270, 10, 67, 21));
        groupBox_2 = new QGroupBox(SettingTab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 110, 461, 81));
        matchRateSlider = new QSlider(groupBox_2);
        matchRateSlider->setObjectName(QStringLiteral("matchRateSlider"));
        matchRateSlider->setGeometry(QRect(100, 50, 321, 26));
        matchRateSlider->setMaximum(100);
        matchRateSlider->setValue(65);
        matchRateSlider->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 20, 67, 21));
        currentRate = new QLabel(groupBox_2);
        currentRate->setObjectName(QStringLiteral("currentRate"));
        currentRate->setGeometry(QRect(10, 40, 61, 41));
        QFont font1;
        font1.setPointSize(25);
        currentRate->setFont(font1);
        currentRate->setTextFormat(Qt::AutoText);
        groupBox_3 = new QGroupBox(SettingTab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 190, 461, 80));
        factorycb = new QComboBox(groupBox_3);
        factorycb->setObjectName(QStringLiteral("factorycb"));
        factorycb->setGeometry(QRect(10, 40, 171, 21));
        exitButton = new QPushButton(groupBox_3);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(370, 40, 85, 28));
        tabWidget->addTab(SettingTab, QString());
        CameraTab = new QWidget();
        CameraTab->setObjectName(QStringLiteral("CameraTab"));
        QPalette palette1;
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QBrush brush3(QColor(175, 175, 175, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush);
        QBrush brush4(QColor(215, 215, 215, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush4);
        QBrush brush5(QColor(87, 87, 87, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush5);
        QBrush brush6(QColor(117, 117, 117, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        CameraTab->setPalette(palette1);
        CameraTab->setFont(font);
        verticalLayoutWidget = new QWidget(CameraTab);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(310, 20, 171, 121));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        preCapturedImg = new QLabel(verticalLayoutWidget);
        preCapturedImg->setObjectName(QStringLiteral("preCapturedImg"));

        verticalLayout->addWidget(preCapturedImg);

        horizontalLayoutWidget = new QWidget(CameraTab);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(60, 220, 371, 66));
        capturedImgLayout = new QHBoxLayout(horizontalLayoutWidget);
        capturedImgLayout->setSpacing(15);
        capturedImgLayout->setContentsMargins(11, 11, 11, 11);
        capturedImgLayout->setObjectName(QStringLiteral("capturedImgLayout"));
        capturedImgLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayoutWidget_2 = new QWidget(CameraTab);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(310, 140, 171, 80));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        ResetButton = new QPushButton(horizontalLayoutWidget_2);
        ResetButton->setObjectName(QStringLiteral("ResetButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ResetButton->sizePolicy().hasHeightForWidth());
        ResetButton->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(ResetButton);

        curStep = new QLabel(horizontalLayoutWidget_2);
        curStep->setObjectName(QStringLiteral("curStep"));
        curStep->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(curStep);

        leftButton = new QPushButton(CameraTab);
        leftButton->setObjectName(QStringLiteral("leftButton"));
        leftButton->setGeometry(QRect(10, 220, 41, 64));
        sizePolicy.setHeightForWidth(leftButton->sizePolicy().hasHeightForWidth());
        leftButton->setSizePolicy(sizePolicy);
        leftButton->setMinimumSize(QSize(0, 0));
        rightButton = new QPushButton(CameraTab);
        rightButton->setObjectName(QStringLiteral("rightButton"));
        rightButton->setGeometry(QRect(440, 220, 41, 64));
        sizePolicy.setHeightForWidth(rightButton->sizePolicy().hasHeightForWidth());
        rightButton->setSizePolicy(sizePolicy);
        streamingImg = new QPushButton(CameraTab);
        streamingImg->setObjectName(QStringLiteral("streamingImg"));
        streamingImg->setGeometry(QRect(10, 20, 291, 181));
        tabWidget->addTab(CameraTab, QString());
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "Network Settings", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "IP Address", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Port", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Image Process Settings", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Match Rate", Q_NULLPTR));
        currentRate->setText(QApplication::translate("MainWindow", "65%", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Factory Process", Q_NULLPTR));
        exitButton->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(SettingTab), QApplication::translate("MainWindow", "Settings", Q_NULLPTR));
        preCapturedImg->setText(QString());
        ResetButton->setText(QApplication::translate("MainWindow", "RESET", Q_NULLPTR));
        curStep->setText(QApplication::translate("MainWindow", "Barcode", Q_NULLPTR));
        leftButton->setText(QApplication::translate("MainWindow", "\342\227\200", Q_NULLPTR));
        rightButton->setText(QApplication::translate("MainWindow", "\342\226\266", Q_NULLPTR));
        streamingImg->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(CameraTab), QApplication::translate("MainWindow", "Camera", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
