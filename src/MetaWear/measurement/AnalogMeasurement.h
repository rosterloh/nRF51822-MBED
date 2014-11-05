#ifndef ANALOGMEASUREMENT_H
#define ANALOGMEASUREMENT_H

#include "mbed.h"

class AnalogMeasurement
{
public:
    AnalogMeasurement(long&, AnalogIn&, AnalogIn&);

    bool init();
    bool run();

private:
    bool _init;
    long& _deviceId;
    AnalogIn& _analog1;
    AnalogIn& _analog2;
};

#endif
