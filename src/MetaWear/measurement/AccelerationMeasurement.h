#ifndef ACCELERATIONMEASUREMENT_H
#define ACCELERATIONMEASUREMENT_H

#include "MMA8452.h"

class AccelerationMeasurement
{
public:
    AccelerationMeasurement(long&, MMA8452&);

    bool init();
    bool run();

private:
    bool _init, _test;
    long& _deviceId;
    MMA8452& _sensor;
};

#endif
