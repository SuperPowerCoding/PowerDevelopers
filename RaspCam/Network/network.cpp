#include "network.h"
#include <QDebug>

#include "parserInfo.h"
#include "tcp_sock.h"
#include <cstring>
#include <QTime>

extern void setNetworkHandler(Network* net);
extern int transfer_proc_init(void);
extern int transfer_data_proc(void);
extern int requestAnalysisToServer(char *image, unsigned int size, unsigned char idx, int step, uchar isFinal);
extern int notifyNumOfProcessSeq(char *PS, unsigned int *cnt);
extern int updateProcAccuracyFromDB(int accuracy, int item);
extern int mysql_connect(void);
extern void mysql_disconnect(void);

Network::Network(unsigned int msecPollingPeriod, Resource * res)
{
    this->pollingPeriod = msecPollingPeriod;
    this->_exit = false;

    this->res = res;
	this->resetFlag = false;
	this->userSettingFlag = false;
	
    // plz write somthing want to initialize
    setNetworkHandler(this);

}


void Network::exit()
{
    this->_exit = true;
}

void Network::run()
{


    qDebug() << "execute Network func" << endl;

    //initialize func
    transfer_proc_init();

    while(!this->_exit)
    {

        // interrupt : updateRawImg
        // emit updateRawImg();
        // reset case : flag
        // user config chagned: flag

        transfer_data_proc();

        //emit imgProcessFin();
        // sleep msec
        msleep(this->pollingPeriod);
    }

}

void Network::setRawImgData(uchar * data, int size, int index, int accuracy, int step, uchar isFinal)
{
    this->rawDataImg = data;
    this->rawDataImgSize = size;
    this->rawDataIndex = index;
    this->rawDataAccuracy = accuracy;
    this->step = step;
    this->isFinal = isFinal;
}

uchar * Network::getRawImgData(void)
{
    return this->rawDataImg;
}

/*
 * 1. setup init value all of them
 * 2. check ip addr and port number
 * 3. check the cell and process number with working around
 * 4. bring up such item_id and acton_type from db
 * 5. wait user action . . .
 * 6. if user do action, then call func(number of step, accuracy, imageSize, imageData);
 * 7. when response of result is OK from Analisis server, let step move the next step.
*/

void Network::sendRawImgData()
{
    // plz fill out code
    QTime time;
    //qDebug() << this->rawDataImgSize << endl;
    time.start();
    cout<<"[TIME] getRawImg sec : "<<time.second()<<" ms : "<<time.msec()<<endl;

   int idx =this->rawDataIndex;
   int accuracy = this->rawDataAccuracy;
   requestAnalysisToServer((char*)this->rawDataImg, this->rawDataImgSize, idx, accuracy, this->step, this->isFinal); //index will be switched by step .
}

void Network::setIpResults(int x, int y,int rate, bool res, unsigned char err_code)
{

    printf("network::setIpResults\n");
    this->ipResult.x = x;
    this->ipResult.y = y;
    this->ipResult.matchRate = rate;
    this->ipResult.result = res;
    this->ipResult.err_code = err_code;
}


void Network::setServerIpAddress(char * ip)
{
    // 192.168.000.001 = 3*4 + 3 = 12 + 3 = 15 + 1(NULL)
    if(strlen(ip) > 15) return;
    this->settingMutex.lock();    
    strcpy(this->ipAddress,ip);
    this->settingMutex.unlock();
}

char * Network::getServerIpAddress()  //////////
{
    // wait
    this->settingMutex.lock();
    this->settingMutex.unlock();
    return this->ipAddress;
}

// port
void Network::setPort(int port)
{
    this->settingMutex.lock();
    this->port = port;
    this->settingMutex.unlock();
}

int Network::getPort()   /////////
{
    // wait
    this->settingMutex.lock();
    this->settingMutex.unlock();

    return this->port;
}

// process
void Network::setProcess(char * proc)
{
    // process m1, m2 ...
    if(strlen(proc) != 2) return;
    this->settingMutex.lock();
    strcpy(this->process,proc);
    this->settingMutex.unlock();
}

char * Network::getProcess() ///////
{
    // wait
    this->settingMutex.lock();
    this->settingMutex.unlock();

    return this->process;
}


void Network::setCellInfo(char * info)
{
    // process m1, m2 ...   
	if(strlen(info) >= 20) return;	
    this->settingMutex.lock();
    strcpy(this->cellInfo,info);
    this->settingMutex.unlock();
}

char * Network::getCellInfo() ///////
{
    // wait
    this->settingMutex.lock();
    this->settingMutex.unlock();

    return this->cellInfo;
}

// rate : image match rate
void Network::setImgRate(int rate)
{
    this->settingMutex.lock();
    this->imgMatchRate = rate;
    this->settingMutex.unlock();
}

int Network::getImgRate(void) ////////
{
    // wait
    this->settingMutex.lock();
    this->settingMutex.unlock();

    return this->imgMatchRate;
}

void Network::updateDbAccuracies(void)
{
    int totalCnt = this->res->getAccChangedSize();

    int ret = mysql_connect();

    if(ret == 0)
    {
        for(int i=0; i<totalCnt; i++)
        {
            if(this->res->getAccChangedFlag(i) == true)
            {
                int tmpItemNum = this->res->getImgIdx(i);
                int tmpAccuracy = this->res->getImgAccuracy(i);

                if(tmpItemNum != 0)
                {
                    updateProcAccuracyFromDB(tmpAccuracy, tmpItemNum);
                }
            }
        }

        mysql_disconnect();
    }
}
