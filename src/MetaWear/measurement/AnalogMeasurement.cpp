#include "AnalogMeasurement.h"
//#include "ComposedRecord.h"
//#include "CharValue.h"
//#include "IntegerValue.h"
//#include "FloatValue.h"

AnalogMeasurement::AnalogMeasurement(long& deviceId, AnalogIn& analog1, AnalogIn& analog2) :
    _deviceId(deviceId),
    _analog1(analog1),
    _analog2(analog2)
{
    _init = false;
}

bool AnalogMeasurement::init()
{
    if (_init)
        return false;

    _init = true;
    return true;
}

bool AnalogMeasurement::run()
{
    //ComposedRecord record;
    //IntegerValue msgId(107);
    //IntegerValue devId(_deviceId);
    //FloatValue analog1(_analog1.read(), 1);
    //FloatValue analog2(_analog2.read(), 1);
    //if ((!record.add(msgId)) || (!record.add(devId)) || (!record.add(analog1)) || (!record.add(analog2)))
    //    return false;

    /**
     * Perform periodic tasks here like comparing to a threshold or updating BT client
     */
    return true;
}
