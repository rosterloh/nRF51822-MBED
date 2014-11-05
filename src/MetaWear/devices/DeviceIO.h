#ifndef DEVICEIO_H
#define DEVICEIO_H

#include "mbed.h"
#include "DHT.h"
#include "MMA8452.h"
#include "DeviceFeedback.h"

class DeviceIO
{
public:
    DeviceIO(void);

    bool buttonPressed();
    DeviceFeedback& deviceFeedback();
    AnalogIn& analog1();
    AnalogIn& analog2();
    DHT& temperatureSensor();
    MMA8452& accelerometer();

private:
    DigitalIn _userButton;
    AnalogIn _analog1;
    AnalogIn _analog2;
    PwmOut _speaker;
    DHT _temperatureSensor;
    MMA8452 _accelerometer;
    DeviceFeedback _deviceFeedback;
};

#endif
