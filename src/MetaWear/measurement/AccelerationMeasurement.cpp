#include "AccelerationMeasurement.h"
//#include "ComposedRecord.h"
//#include "CharValue.h"
//#include "IntegerValue.h"
//#include "FloatValue.h"

AccelerationMeasurement::AccelerationMeasurement(long& deviceId, MMA8452& sensor) :
    _deviceId(deviceId),
    _sensor(sensor)
{
    _init = false;
}

bool AccelerationMeasurement::init()
{
    if (_init)
        return false;

    _sensor.setBitDepth(MMA8452::BIT_DEPTH_12);
    _sensor.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
    _sensor.setDataRate(MMA8452::RATE_100);

    char devId = 0;
    _sensor.getDeviceID(&devId);
    _test = (devId == 0x2A);
    _init = true;

    return true;
}

bool AccelerationMeasurement::run()
{
    double x = 0, y = 0, z = 0;

    if (!_test)
        return false;

    if(!_sensor.isXYZReady()) {
        wait(0.01);        
    }
    //_sensor.readXYZRaw(&x, &y, &z);
    //_sensor.readXYZCounts(&x, &y, &z);
    _sensor.readXYZGravity(&x, &y, &z);

    /*
    ComposedRecord record;
    IntegerValue msgId(106);
    IntegerValue devId(_deviceId);
    FloatValue xValue(data[0], 2);
    FloatValue yValue(data[1], 2);
    FloatValue zValue(data[2], 2);
    if ((!record.add(msgId)) || (!record.add(devId)) || (!record.add(xValue)) || (!record.add(yValue)) || (!record.add(zValue)))
        return false;
    */

    /**
     * Perform periodic tasks here like comparing to a threshold or updating BT client
     */

    return true;
}
