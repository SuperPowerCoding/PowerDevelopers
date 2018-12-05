#ifndef NETWORK_H
#define NETWORK_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include "config.h"
#include "Resource/resource.h"

class Network : public QThread
{
    Q_OBJECT

public:
    Network(unsigned int msecPollingPeriod, Resource * res);

    void setRawImgData(uchar * data, int size, int index, int accuracy, int step, uchar isFinal);
	uchar * getRawImgData(void);


    void exit();
    bool isEnable();
	
	//result data
    struct IpResult
	{
		int x;
		int y;
        int matchRate;
		bool result;
        unsigned char err_code;
	}ipResult;    

    static Network *getInstance() { return &net_instance; }

    void setIpResults(int x, int y,int rate, bool res, unsigned char err_code);

    // user setting
    // ip address
    void setServerIpAddress(char * ip);
    char * getServerIpAddress();

    // port
    void setPort(int port);
    int getPort();

    // process
    void setProcess(char * proc);
    char * getProcess();

    void setCellInfo(char * proc);
    char * getCellInfo();

    // rate : image match rate
    void setImgRate(int rate);
    int getImgRate(void);
    Resource * res;

	bool resetFlag;
	bool userSettingFlag;


private:
    unsigned int pollingPeriod;
    bool _exit;
    bool _enabled;
    void run();
    
    // user setting
    char ipAddress[16];
    char process[3];
    char cellInfo[20];
    int port = 8080;
    int imgMatchRate = 65;
    QMutex settingMutex;


    // image informations
	uchar * rawDataImg;
	int rawDataImgSize;
    int rawDataIndex;
    int rawDataAccuracy;
    int step;
    unsigned char isFinal;

    static Network net_instance;    
	
	

private slots:
    void sendRawImgData();
    void updateDbAccuracies();
	
signals :
    void imgProcessFin();       // image process fininshed
    void resourceUpdateFin();   // update resource finished

};





#endif // NETWORK_H
