/**
 ******************************************************************************
 * @file    main.cpp
 * @author  Richard Osterloh (richard.osterloh@gmail.com)
 * @version V1.0.0
 * @date    04-November-2014
 * @brief   nRF51822 on MetaWear Board.
 ******************************************************************************
 * Main application configuration.
 ******************************************************************************
 */

#include "mbed.h"
#include "rtos.h"
#include "MbedAgent.h"

int main() {

  DeviceIO io;

  {
    uint8_t tries;
    MbedAgent agent(io);

    if (!agent.init()) {
      goto error;
    }

    tries = 3;
    do {
      if (agent.run())
        break;
    } while (--tries > 0);

    if (tries == 0) {
      goto error;
    }
  }
  return 0;

error:
    return 1;
}
