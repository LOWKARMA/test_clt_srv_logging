#include "testsrv.h"
#include <iostream>
#include <sys/poll.h>
#include <cstring>

testsrv::testsrv()
{
    memset(&this->domain, 0x00, sizeof(this->domain));
    this->portNumber = -1;
    this->sock = -1;
}

testsrv::~testsrv()
{

}

bool testsrv::init(int portNumber, char *logFile)
{
    this->portNumber = portNumber;

    if(!this->log.init(logFile))
    {
        perror("cant satr log\n\r");
        return false;
    }

    // create socket
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(!this->sock)
    {
        perror("cant establish socket\n\r");
        return false;
    }

    cout << "the socket was established well" << endl;

    this->domain.sin_family = AF_INET;
    this->domain.sin_port = htons(this->portNumber);
    this->domain.sin_addr.s_addr = INADDR_ANY;

    if(bind(this->sock, (struct sockaddr*)&this->domain, sizeof(this->domain)) < 0)
    {
        perror("cant bind socket");
        this->shutDown();
        return false;
    }

    cout << "bind - ok" << endl;

    if(listen(this->sock, 5) != 0)
    {
        perror("cant listen incoming connections\n\r");
        this->shutDown();
        return false;
    }

    cout << "the server is ready for incoming connections" << endl;

    return true;
}

void testsrv::shutDown()
{
    this->log.logClose();
    shutdown(this->sock, 2);
    close(this->sock);
    this->sock = -1;
    for(auto i : this->clientSockets)
        close(i);
    for(auto i : this->clientThreads)
        pthread_cancel(i);
}

void testsrv::start()
{
    int incomingSock = -1;
    wrapperParam paramToThread;
    while(this->sock)
    {
        incomingSock = accept(this->sock, NULL, NULL);
        if(incomingSock)
        {
            pthread_t tid = -1;
            this->clientSockets.push_back(incomingSock);
            //if(pthread_create(&tid, NULL, testsrv::connectionHandler, (void*)&incomingSock))
            paramToThread.sck = incomingSock;
            paramToThread.thisPtr = this;
            if(pthread_create(&tid, NULL, testsrv::connectionHandlerWarpper, (void*)&paramToThread))
            {
                this->clientThreads.push_back(tid);
            }
        }
    }
}

void* testsrv::connectionHandlerWarpper(void *arg)
{
   reinterpret_cast<wrapperParam*>(arg)->thisPtr->connectionHandler(reinterpret_cast<wrapperParam*>(arg)->sck);
   return NULL;
}

void* testsrv::connectionHandler(int sck)
{
    struct pollfd pfd;
    int recivedData = -1;
    int timeToDisconnect = 5000;
    unsigned char recvBuff[512]{0x00};
    memset(&pfd, 0x00, sizeof(pfd));

    pfd.fd = sck;
    pfd.events = POLLIN | POLLHUP | POLLRDNORM;

    cout << "thread created " << pthread_self() << endl;
    while(1)
    {
        if(poll(&pfd, 1, timeToDisconnect) > 0)
        {
            recivedData = recv(sck, recvBuff, sizeof(recvBuff), MSG_DONTWAIT);
            if(!recivedData)
                break;
            else
            {    
                this->log.logString(recvBuff);
            }
        }
    }
    pthread_exit(0);
}
