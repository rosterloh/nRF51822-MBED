#include "DeviceIO.h"

DeviceIO::DeviceIO(void) :
    _userButton(P0_11),
    _analog1(P0_1),
    _analog2(P0_2),
    _speaker(P0_10),
    _temperatureSensor(P0_12, AM2302),
    _accelerometer(I2C_SDA0, I2C_SCL0, 40000),
    _deviceFeedback(_speaker)
{
}

bool DeviceIO::buttonPressed()
{
    return _userButton;
}

DeviceFeedback& DeviceIO::deviceFeedback()
{
    return _deviceFeedback;
}

AnalogIn& DeviceIO::analog1()
{
    return _analog1;
}

AnalogIn& DeviceIO::analog2()
{
    return _analog2;
}

DHT& DeviceIO::temperatureSensor()
{
    return _temperatureSensor;
}

MMA8452& DeviceIO::accelerometer()
{
    return _accelerometer;
}
