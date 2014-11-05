#include "DeviceFeedback.h"
#include <stdlib.h>
#include <string.h>

#define MSG_CLOSE_RELAY 1
#define MSG_OPEN_RELAY 2

DeviceFeedback::DeviceFeedback(PwmOut speaker) :
    _speaker(speaker),
    _thread(DeviceFeedback::thread_func, this)
{
}

void DeviceFeedback::closeRelay()
{
    uint8_t *msg;

    msg = _mail.alloc();
    *msg = MSG_CLOSE_RELAY;
    _mail.put(msg);
}

void DeviceFeedback::openRelay()
{
    uint8_t *msg;

    msg = _mail.alloc();
    *msg = MSG_OPEN_RELAY;
    _mail.put(msg);
}

void DeviceFeedback::thread()
{
    osEvent evt; uint8_t *msg;
    bool relayState = false;

    while (true) {
        if ((evt = _mail.get(1000)).status == osEventMail) {
            msg = (uint8_t*)evt.value.p;
            switch (*msg) {
            case MSG_CLOSE_RELAY:
                if (!relayState) {
                    relayState = true;
                    for (float i=2000.0; i<10000.0; i+=100) {
                        _speaker.period(1.0/i);
                        _speaker = 0.5;
                        Thread::wait(20);
                    }
                    _speaker = 0.0;
                }
                break;
            case MSG_OPEN_RELAY:
                if (relayState) {
                    relayState = false;
                    for (float i=10000.0; i>2000.0; i-=100) {
                        _speaker.period(1.0/i);
                        _speaker = 0.5;
                        Thread::wait(20);
                    }
                    _speaker = 0.0;
                }
                break;
            }
            _mail.free(msg);
        }

        if (relayState) {
            _speaker.period(1.0/10000);
            _speaker = 0.5;
            Thread::wait(20);
            _speaker = 0.0;
        }
    }
}

void DeviceFeedback::thread_func(void const *arg)
{
    DeviceFeedback *that;
    that = (DeviceFeedback*)arg;
    that->thread();
}
