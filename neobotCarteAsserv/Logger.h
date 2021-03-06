#ifndef LOGGER_H
#define LOGGER_H

#include "Comm.h"

class Logger
{
public:
	Logger(Comm* comm, bool enableDebug, bool useComm);

    void useComm(bool use);

    void print(const String& string);
    void println(const String& string);

    void print(int value);
    void println(int value);

    void print(double value);
    void println(double value);

private:
    Comm* _comm;
    String _partialString;
	bool _enable, _useComm;
};

#endif // LOGGER_H

