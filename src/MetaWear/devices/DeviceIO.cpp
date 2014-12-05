#include "DeviceIO.h"

/** Pin Mapping from MetaWear PPS v0.7 - http://mbientlab.com/docs/MetaWearPPSv0.7.pdf
 *
 *  DIO0/AIN0  P0.01  GPIO General purpose I/O and analog input.
 *  DIO1/AIN1  P0.02  GPIO General purpose I/O and analog input.
 *  DIO2/AIN2  P0.03  GPIO General purpose I/O and analog input.
 *  DIO3/AIN3  P0.05  GPIO General purpose I/O and analog input.
 *  DIO4/AIN4  P0.08  GPIO General purpose I/O.
 *  DIO5/AIN5  P0.09  GPIO General purpose I/O.
 *  DIO6/AIN6  P0.10  GPIO General purpose I/O.
 *  DIO7/AIN7  P0.16  GPIO General purpose I/O.
 *  HCD        P0.07  Haptic Open Drain MOSFET Pull-down.
 *  SDA        P0.14  Serial Bus I2C serial data.
 *  SCL        P0.12  Serial Bus I2C serial clock.
 *             P0.29  Switch Micro Push Button. Active Low.
 *             P0.04  Battery Voltage Monitor Analog voltage at VBAT / 2.
 *             P0.06  Accelerometer Interrupt INT2 Pin of MMA8452Q.
 */
DeviceIO::DeviceIO(void) :
    _userButton(P0_29),
    _analog1(P0_4),
    _analog2(P0_2),
    _speaker(P0_7),
    _temperatureSensor(P0_16, AM2302),
    _accelerometer(P0_14, P0_12, 40000),
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
