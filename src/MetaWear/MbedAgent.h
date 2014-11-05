#ifndef MBEDAGENT_H
#define MBEDAGENT_H

#include <stddef.h>
#include "DeviceIO.h"
#include "TemperatureMeasurement.h"
#include "AccelerationMeasurement.h"
#include "AnalogMeasurement.h"

#define MBED_AGENT_INTERVAL 30.0

class MbedAgent
{
public:
    MbedAgent(DeviceIO&);

    bool init();
    bool run();

protected:
    void loop();

private:
    DeviceIO& _io;
    TemperatureMeasurement _temperatureMeasurement;
    AccelerationMeasurement _accelerationMeasurement;
    AnalogMeasurement _analogMeasurement;
    long _deviceId;
};

#endif
