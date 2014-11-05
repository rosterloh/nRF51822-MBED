#include "MbedAgent.h"
#include "rtos.h"

MbedAgent::MbedAgent(DeviceIO& io) :
    _io(io),
    _temperatureMeasurement(_deviceId, _io.temperatureSensor()),
    _accelerationMeasurement(_deviceId, _io.accelerometer()),
    _analogMeasurement(_deviceId, _io.analog1(), _io.analog2()),
    _deviceId(0)
{
}

bool MbedAgent::init()
{
    if ((!_temperatureMeasurement.init()) ||
        (!_accelerationMeasurement.init()) ||
        (!_analogMeasurement.init())) {
        return false;
    }
    return true;
}

bool MbedAgent::run()
{
    //Thread::wait(5000);

    loop();
    return true;
}

void MbedAgent::loop()
{
    Timer timer;

    timer.start();
    while (true) {
        timer.reset();

        _temperatureMeasurement.run();
        _accelerationMeasurement.run();
        _analogMeasurement.run();

        while (timer.read() < MBED_AGENT_INTERVAL) {
            Thread::yield();
        }
    }
}
