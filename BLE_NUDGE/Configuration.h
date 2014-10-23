#ifndef _H_CONFIGURATION_H
#define _H_CONFIGURATION_H

/*
 * Configuration.h
 *
 * This file defines default configuration parameters like device name, connection parameters etc.
 */

namespace Config
{
const uint8_t deviceName[] = "Buddi Nudge";

const int advertisingInterval = 1600;   // 1s (0.625 ms units)

// default connection parameters conforming to Apple recommendations
const int minConnectionInterval = 16; // (1.25 ms units)
const int maxConnectionInterval = 40; // (1.25 ms units)
const int slaveLatency          = 0;
const int supervisionTimeout    = 500; // (10 ms units)

const int blockSize = 16; // in bytes
}

#endif //_H_CONFIGURATION_H
