#ifndef TEMPERATUREMEASUREMENT_H
#define TEMPERATUREMEASUREMENT_H

#include "DHT.h"

class TemperatureMeasurement
{
public:
    TemperatureMeasurement(long&, DHT&);

    bool init();
    bool run();

private:
    bool _init;
    long& _deviceId;
    DHT& _sensor;
};

#endif
