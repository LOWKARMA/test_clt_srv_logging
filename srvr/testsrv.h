#ifndef TESTSRV_H
#define TESTSRV_H


#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <vector>
#include <fstream>
#include "tsetlog.h"

using namespace std;


class testsrv
{
    private:
        unsigned int portNumber;
        int sock;
        struct sockaddr_in domain;
        vector<pthread_t>clientThreads;
        vector<int>clientSockets;

        tsetlog log;

        static void* connectionHandlerWarpper(void* arg);
        void* connectionHandler(int sck);


    public:
        testsrv();
        ~testsrv();
        bool init(int portNumber, char* logFile); // socket && bind
        void start(); //listen && phtread_create
        void shutDown();
};

struct wrapperParam
{
    testsrv* thisPtr;
    int sck;
};

#endif // TESTSRV_H
