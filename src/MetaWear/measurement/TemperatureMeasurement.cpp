#include "TemperatureMeasurement.h"
//#include "ComposedRecord.h"
//#include "CharValue.h"
//#include "IntegerValue.h"
//#include "FloatValue.h"

TemperatureMeasurement::TemperatureMeasurement(long& deviceId, DHT& sensor) :
    _deviceId(deviceId),
    _sensor(sensor)
{
    _init = false;
}

bool TemperatureMeasurement::init()
{
    if (_init)
        return false;

    _init = true;
    return true;
}

bool TemperatureMeasurement::run()
{
    int err;
    float data[2] = { 0.0, 0.0 };

    err = _sensor.readData();
    if (err == 0) {
      data[0] = _sensor.ReadTemperature(CELCIUS);
      data[1] = _sensor.ReadHumidity();
    } else {
      return false;
    }

    /*
    ComposedRecord record;
    IntegerValue msgId(106);
    IntegerValue devId(_deviceId);
    FloatValue tempValue(data[0], 2);
    FloatValue humValue(data[1], 2);
    if ((!record.add(msgId)) || (!record.add(devId)) || (!record.add(tempValue)) || (!record.add(humValue)))
        return false;
    */

    /**
     * Perform periodic tasks here like comparing to a threshold or updating BT client
     */

    return true;
}
