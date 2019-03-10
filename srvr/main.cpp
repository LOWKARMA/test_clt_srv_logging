//#include <QCoreApplication>
#include <iostream>
#include "testsrv.h"

using namespace std;
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        string usage = "using: " + (string)argv[0] + " PORT_NUMBER\n\r";
        perror(usage.c_str());
        return 1;
    }
    int portNumber = strtoul(argv[1], NULL, 10);
    testsrv srv;

    if(srv.init(portNumber, "tmp.log"))
    {
        srv.start();
        srv.shutDown();
    }
    return 0;
}
