#ifndef TSETLOG_H
#define TSETLOG_H
#include <string>
#include <fstream>
#include <thread>
#include <unistd.h>

using namespace std;
class tsetlog
{
    private:
        int pipeDescr[2];
        string logFileName;
        ofstream logFileStream;
        thread listener;
        unsigned char* parse(unsigned char* start, const unsigned char* buf, const int size);
        void startListen();

    public:
        tsetlog();
        bool init(const char* logfileName);
        void logString(const unsigned char* str);
        void logClose();
};

struct logMsg
{
    unsigned char buff[256];
    int msgLen;
};

#endif // TSETLOG_H
