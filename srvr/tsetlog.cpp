#include "tsetlog.h"
#include <cstring>
#include <iostream>

tsetlog::tsetlog()
{

}

bool tsetlog::init(const char* logfileName)
{
    if(!logfileName)
        return false;

    this->logFileName = logfileName;
    this->logFileStream.open(this->logFileName,  fstream::app | fstream::in);
    if(!this->logFileStream.is_open())
    {
        perror("cant create log file\n\r");
        return false;
    }

    if(pipe(this->pipeDescr) < 0)
    {
        perror("cant open pipe\r\n");
        return false;
    }

    this->listener = thread(&tsetlog::startListen, this);
    this->listener.detach();
    return true;
}

unsigned char* tsetlog::parse(unsigned char* start, const unsigned char* buf, const int size)
{
    if(start + (*start) >= buf + size)
        return NULL;
    return start + (*start) + 1;
}

void tsetlog::startListen()
{
    logMsg msg;
    int retVal = -1;
    unsigned char logStr[256]{0x00};
    unsigned char* ptr = NULL, *tmp = 0;
    while(1)
    {
        memset(&msg, 0x00, sizeof(logMsg));
        retVal = read(this->pipeDescr[0], &msg, sizeof(logMsg)); //get
        if(retVal == sizeof(logMsg))
        {
            ptr = msg.buff;
            do
            {
                memset(&logStr, 0x00, sizeof(logStr));
                tmp = parse(ptr, msg.buff, msg.msgLen);
                if(tmp)
                {
                    memcpy(logStr, ptr + 1, *ptr);
                    this->logFileStream << logStr << endl;
                    cout << logStr << endl;
                }
                ptr = tmp;
            }while(ptr);
        }
    }
}

void tsetlog::logString(const unsigned char* str)
{
    logMsg msg;
    msg.msgLen = strlen((const char*)str);
    if(msg.msgLen >= sizeof(msg.buff))
        msg.msgLen = sizeof(msg.buff) - 1;
    memcpy(msg.buff, str, msg.msgLen);
    msg.buff[msg.msgLen] = '\0';

    write(this->pipeDescr[1], &msg, sizeof(logMsg));
}

void tsetlog::logClose()
{
    close(this->pipeDescr[0]);
    close(this->pipeDescr[1]);
    this->listener.~thread();
    this->logFileStream.close();
}
