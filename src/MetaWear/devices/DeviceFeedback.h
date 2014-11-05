#ifndef DEVICEFEEDBACK_H
#define DEVICEFEEDBACK_H

#include <stddef.h>
#include "mbed.h"
#include "rtos.h"

class DeviceFeedback
{
public:
    DeviceFeedback(PwmOut speaker);

    void closeRelay();
    void openRelay();

protected:
    void thread();
    static void thread_func(void const*);

private:
    PwmOut _speaker;
    Thread _thread;
    Mail<uint8_t, 16> _mail;
};

#endif
