#include "tstclt.h"
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <iostream>


struct inputArgs
{
    string clientName;
    int serverPort;
    int connectionTimeout;
};

bool parseArgv(int argc, char** argv, inputArgs& args);
void getTimeStamp(string& timeStamp);

int main(int argc, char *argv[])
{
    inputArgs args;
    string floodingString;
    tstclt clt;
    if(!parseArgv(argc, argv, args))
    {
	string usage = "using: " + (string)argv[0] + " CLIENT_NAME SERVER_PORT CONNETION_TIMEOUT\n\r";
        perror(usage.c_str());
        return 1;
    }

    if(clt.init("127.0.0.1", args.serverPort, args.connectionTimeout))
    {
        string floodStr;
        getTimeStamp(floodStr);
        floodStr += " " + args.clientName;

        for(int i = 0; i < 5; ++i)
        {
            getTimeStamp(floodStr);
            floodStr += " " + args.clientName;
            clt.sendMsg(floodStr.c_str());
        }
        clt.shutDown();
    }
    return 0;
}

bool parseArgv(int argc, char** argv, inputArgs& args)
{
    if(argc != 4 || !argv)
        return false;

    args.clientName = argv[1];
    args.serverPort = strtoul(argv[2], NULL, 10);
    args.connectionTimeout = strtoul(argv[3], NULL, 10);

    return true;
}

void getTimeStamp(string& timeStamp)
{
    struct timeb timeInfo;
    char timeBuf[32];
    char stamp[32];

    ftime(&timeInfo);
    //[yyyy-mm-dd hh:mm:ss.ms] "им€_клиента"
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&timeInfo.time));
    sprintf(stamp, "[%s.%02u]", timeBuf, timeInfo.millitm);
    timeStamp = stamp;
}
