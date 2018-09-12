#include "network.h"
#include <QDebug>

#include "parserInfo.h"
#include "tcp_sock.h"
#include <cstring>

extern void setNetworkHandler(Network* net);
extern int transfer_proc_init(void);
extern int transfer_data_proc(void);


Network::Network(unsigned int msecPollingPeriod, Resource * res)
{
    this->pollingPeriod = msecPollingPeriod;
    this->_exit = false;

    this->res = res;
	
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
        transfer_data_proc();

        //emit imgProcessFin();
        // sleep msec
        msleep(this->pollingPeriod);
    }

}

void Network::setRawImgData(uchar * data, int size, int index)
{
    this->rawDataImg = data;
    this->rawDataImgSize = size;
    this->rawDataIndex = index;
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
    //packinfo

   //data*
   qDebug() << this->rawDataImgSize << endl;

   packInfo_tx *pack = (packInfo_tx *)malloc(sizeof(packInfo_tx));
   //memset(pack, 0, sizeof(packInfo_tx));

   pack->cmd_type = CMD_TYPE_REQUEST; //fixed

   pack->action_type = ACT_BARCODE1D;
   pack->item_id = WORK_ORDER;

   pack->cell_num = 1;
   pack->process_num = 1;

   pack->accuracy = 100;

   pack->order_size = 0;
   //pack.image_size = this->rawDataImg->size();
   //pack.image_data = (char *)this->rawDataImg;
   //pack.image_data = reinterpret_cast<char*>(this->rawDataImg->data());
   pack->image_size = this->rawDataImgSize;
   pack->image_data = (char*)this->rawDataImg;
   //qDebug() << "image data handler: "<< pack.image_data <<endl;
   //printf("?????: %d\n", this->rawDataImg);
   printf("image size : %d\n", pack->image_size);
   
   printf("%p\n", pack);
   buildPacket(pack);


}

void Network::setIpResults(int x, int y, bool res)
{
    printf("network::setIpResults\n");
    this->ipResult.x = x;
    this->ipResult.y = y;
    this->ipResult.result = res;
}


void Network::setServerIpAddress(char * ip)
{
    // 192.168.000.001 = 3*4 + 3 = 12 + 3 = 15 + 1(NULL)
    if(strlen(ip) > 15) return;
    this->settingMutex.lock();
    strcpy(this->ipAddress,ip);
    this->settingMutex.unlock();
}

char * Network::getServerIpAddress()
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

int Network::getPort()
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

char * Network::getProcess()
{
    // wait
    this->settingMutex.lock();
    this->settingMutex.unlock();

    return this->process;
}

// rate : image match rate
void Network::setImgRate(int rate)
{
    this->settingMutex.lock();
    this->imgMatchRate = rate;
    this->settingMutex.unlock();
}

int Network::getImgRate(void)
{
    // wait
    this->settingMutex.lock();
    this->settingMutex.unlock();

    return this->imgMatchRate;
}

