/*
 * tcp_sock.cpp
 *
 *  Created on: Aug 26, 2018
 *      Author: charles
 */

#include "tcp_sock.h"
#include "parserInfo.h"

using namespace std;

char revBuffer[512];

tcp_client::tcp_client()
{
    sock = -1;
    port = 0;
    address = "";
}

/**
    Connect to a host on a certain port number
*/
int tcp_client::conn(string address , int port)
{
    //create socket if it is not already created
    if(sock == -1)
    {
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }

        cout<<"Socket created\n";
    }
    else{   /* OK , nothing */
    	cout << "already exist the sock" <<endl;
    }

    //setup address structure
    if((signed int)inet_addr(address.c_str()) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;

        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            cout<<"Failed to resolve hostname\n";

            return false;
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;

        for(int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];
            cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;

            break;
        }
    }
    //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }

    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return -1;
    }
    cout<<"Connected\n";
    return true;
}

/**
    Send data to the connected host
*/
#define PACK_LENGTH 512

bool tcp_client::send_data(jobInfo_t *data)
{
    unsigned int length = data->txPackInfo->image_size+data->txPackInfo->order_size+D_HEADER_SIZE;    
	printf("total length: %d\n", length);
    //length = 100000;
	unsigned int remainedLen = length;
	unsigned int sentDataPos =0;
    //Send some data
	while(remainedLen > 0)
	{
		unsigned int sentDataLen = (remainedLen > PACK_LENGTH)?PACK_LENGTH:remainedLen;
		if( send(sock , &data->txData[sentDataPos] , sentDataLen , 0) < 0)
		{
			perror("Send failed : ");
			return false;
		}
		//else cout << "sending..." << endl;

		remainedLen -= sentDataLen;
		sentDataPos += sentDataLen;
	}
    cout<<"____ data send complete ____\n";

    return true;
}

/**
    Receive data from the connected host
*/
bool tcp_client::receive(jobInfo_t *data)
{
    memset(revBuffer, 0, sizeof(revBuffer));

    //Receive a reply from the server
    if( recv(sock , revBuffer , sizeof(revBuffer) , 0) < 0)
    {
        puts("recv failed");
        return false;
    }

    cout << "receive data" << endl;
    data->callback((char *)revBuffer);

    return true;
}

bool tcp_client::close_sock(void)
{
    if(sock != 0)
    {
        close(sock);
        sock = -1;
        shutdown(sock, SHUT_RDWR);
    }
    else return false;

    return true;

}




