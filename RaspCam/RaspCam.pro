#-------------------------------------------------
#
# Project created by QtCreator 2018-08-25T20:14:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RaspCam
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    Camera/camera.cpp \
    Network/network.cpp \
    Network/parserTask.cpp \
    Network/tcp_sock.cpp \
    Hardware/Buzzer/buzzer.cpp \
    Hardware/Key/key.cpp \
    Resource/resource.cpp \
    Network/parsemysql.cpp \
    Hardware/LaserSensor/laserSensor.cpp \
    Hardware/VibrationMotor/vibMotor.cpp

HEADERS  += mainwindow.h \
    Camera/camera.h \
    config.h \
    Network/network.h \
    Hardware/Buzzer/buzzer.h \
    Network/parserInfo.h \
    Network/tcp_sock.h \
    Hardware/Buzzer/tones.h \
    Hardware/Buzzer/melody.h \
    Hardware/Key/key.h \
    Resource/resource.h \
    Network/parsemysql.h \
    Hardware/hardware.h \
    Hardware/LaserSensor/inc/vl53l0x_api.h \
    Hardware/LaserSensor/inc/vl53l0x_api_calibration.h \
    Hardware/LaserSensor/inc/vl53l0x_api_core.h \
    Hardware/LaserSensor/inc/vl53l0x_api_ranging.h \
    Hardware/LaserSensor/inc/vl53l0x_api_strings.h \
    Hardware/LaserSensor/inc/vl53l0x_def.h \
    Hardware/LaserSensor/inc/vl53l0x_device.h \
    Hardware/LaserSensor/inc/vl53l0x_interrupt_threshold_settings.h \
    Hardware/LaserSensor/inc/vl53l0x_platform.h \
    Hardware/LaserSensor/inc/vl53l0x_platform_log.h \
    Hardware/LaserSensor/inc/vl53l0x_tuning.h \
    Hardware/LaserSensor/inc/vl53l0x_types.h \
    Hardware/LaserSensor/laserSensor.h \
    Hardware/VibrationMotor/vibMotor.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/c++/6.3.0/
INCLUDEPATH += /usr/include/mysql

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_tracking -lopencv_videoio -lraspicam_cv -lraspicam -lwiringPi -lwiringPiDev
LIBS += -L/usr/lib/arm-linux-gnueabihf -lmysqlclient

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lVL53L0X_Rasp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lVL53L0X_Rasp
else:unix: LIBS += -L$$PWD/./ -lVL53L0X_Rasp

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/libVL53L0X_Rasp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/libVL53L0X_Rasp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/VL53L0X_Rasp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/VL53L0X_Rasp.lib
else:unix: PRE_TARGETDEPS += $$PWD/./libVL53L0X_Rasp.a
