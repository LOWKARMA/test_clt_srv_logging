#include "tstclt.h"
#include <cstring>
#include <iostream>

tstclt::tstclt()
{
    this->serverPort = -1;
    this->connectionTimeout = -1;
    this->sock = -1;
    memset(&this->domain, 0x00, sizeof(domain));
}

tstclt::~tstclt()
{

}

bool tstclt::init(const char *srvAdr, int srvPort, int connectionTimeout)
{
    this->serverAdress = srvAdr;
    this->serverPort = srvPort;
    this->connectionTimeout = connectionTimeout;

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(!this->sock)
    {
        perror("cant establish socket\n\r");
        return false;
    }

    cout << "the socket was established well" << endl;

    this->domain.sin_family = AF_INET;
    this->domain.sin_port = htons(this->serverPort);
    this->domain.sin_addr.s_addr = inet_addr(this->serverAdress.c_str());

    if(connect(this->sock, (struct sockaddr*)&this->domain, sizeof(this->domain)) != 0)
    {
        perror("cant establish connection to the server: connect() error\n\r");
        this->shutDown();
        return false;
    }

    struct timeval tv;
    tv.tv_sec = this->connectionTimeout;
    tv.tv_usec = 0;

    fd_set fd;
    FD_ZERO(&fd);
    FD_SET(this->sock, &fd);

    if(!select(this->sock + 1, NULL, &fd, NULL, &tv))
    {
        perror("cant establish connection to the server: select() error\n\r");
        this->shutDown();
        return false;
    }

    cout << "client is ready for flooding" << endl;
    return true;
}

void tstclt::sendMsg(const char *floodStr)
{
    int strLen = strlen(floodStr);
    unsigned char* sndBuff = new unsigned char [strLen + 1];
    sndBuff[0] = strLen;
    memcpy(sndBuff + 1, floodStr, strLen);

    if(write(this->sock, sndBuff, strLen + 1) != strLen + 1)
        perror("flooding problem\n\r");
    else
       cout << "client send \"" << sndBuff << "\" to server" << endl;

    delete [] sndBuff;
}

void tstclt::shutDown()
{
    shutdown(this->sock, 2);
    close(this->sock);
    this->sock = -1;
}
