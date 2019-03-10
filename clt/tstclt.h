#ifndef TSTCLT_H
#define TSTCLT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

using namespace std;
class tstclt
{
    private:
        int serverPort;
        string serverAdress;
        int connectionTimeout;
        int sock;
        struct sockaddr_in domain;

    public:
        tstclt();
        ~tstclt();
        bool init(const char* srvAdr, int srvPort, int connectionTimeout);
        void sendMsg(const char *floodStr);
        void shutDown();
};

#endif // TSTCLT_H
