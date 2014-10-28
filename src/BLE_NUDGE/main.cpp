/*******************************************************************************
* Title                 :   System Initialisation
* Filename              :   main.cpp
* Author                :   Richard Osterloh
* Origin Date           :   22/07/2014
* Version               :   1.0.0
* Compiler              :   mbed compiler
* Target                :   Nordic nRF51822
* Notes                 :   None
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials   Description
* 22/07/2014      1.0.0             RO      Module Created.
*
*******************************************************************************/
/** \file main.cpp
 * \brief This module contains the main function and setup
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "mbed.h"
#include "Logger.h"
#include "Configuration.h"
#include "BLEDevice.h"
//#include "nRF51822n.h"
//#include "MMA8652.h"
#include "BuddiService.h"
#include "BatteryService.h"
#include "DeviceInformationService.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
//nRF51822n   nrf;
BLEDevice   ble;
DigitalOut  oneSecondLed(LED1);              /* LED1 is toggled every second. */
DigitalOut  advertisingStateLed(LED2);       /* LED2 is on when we are advertising, otherwise off. */
//AnalogIn   adc1(p0_0);
//MMA8652    acc1(I2C_SDA0, I2C_SCL0);

const static char     DEVICE_NAME[]         = "Nudge";
static const uint8_t  uuid128_list[]        = {BuddiServiceUUID_reversed,


void bluetoothInit();

static volatile bool triggerSensorPolling = false; /* set to high periodically to indicate to the main thread that
                                                    * polling is necessary. */
static Gap::ConnectionParams_t connectionParams;

uint8_t batteryLevel = 100;
BatteryService *batteryServicePtr = NULL;
BuddiService *buddiServicePtr;

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void timeoutCallback(void)
{
    DEBUG("Timeout!\r\n");
}

void connectionCallback(Gap::Handle_t handle, const Gap::ConnectionParams_t *params)
{
    DEBUG("Connected. Got handle %u\r\n", handle);
    DEBUG("Conn. params => min=%d, max=%d, slave=%d, supervision=%d\r\n",
          params->minConnectionInterval, params->maxConnectionInterval, params->slaveLatency, params->connectionSupervisionTimeout);

    connectionParams.minConnectionInterval        = Config::minConnectionInterval;
    connectionParams.maxConnectionInterval        = Config::maxConnectionInterval;
    connectionParams.slaveLatency                 = Config::slaveLatency;
    connectionParams.connectionSupervisionTimeout = Config::supervisionTimeout;
    if (ble.updateConnectionParams(handle, &connectionParams) != BLE_ERROR_NONE) {
        DEBUG("failed to update connection paramter\r\n");
    }

    advertisingStateLed = 0;
}

void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    DEBUG("Disconnected handle %u!\n\r", handle);
    switch(reason) {
        case Gap::REMOTE_USER_TERMINATED_CONNECTION:
            DEBUG("REASON: REMOTE_USER_TERMINATED_CONNECTION\r\n");
            break;
        case Gap::CONN_INTERVAL_UNACCEPTABLE:
            DEBUG("REASON: CONN_INTERVAL_UNACCEPTABLE\r\n");
            break;
        case Gap::LOCAL_HOST_TERMINATED_CONNECTION:
            DEBUG("REASON: LOCAL_HOST_TERMINATED_CONNECTION\r\n");
            break;
        default:
            DEBUG("REASON: UNKNOWN\r\n");
            break;
    }
    DEBUG("Restarting the advertising process\n\r");
    ble.startAdvertising();
    advertisingStateLed = 1;
}

void onUpdatesEnabled(Gap::Handle_t handle)
{
    DEBUG("Notifications enabled for %d\r\n", handle);
}

void onDataWritten(const GattCharacteristicWriteCBParams *params)
{
    if ((buddiServicePtr != NULL) && (params->charHandle == buddiServicePtr->getCommandCharacteristicHandle())) {
        uint16_t bytesRead = params->len;
        DEBUG("received %u bytes\n\r", bytesRead);
        //ble.updateCharacteristicValue(buddiServicePtr->getCommandNotificationHandle(), params->data, bytesRead);
    }
}

/**
 * Triggered periodically by the 'ticker' interrupt
 */
void periodicCallback(void)
{
    oneSecondLed = !oneSecondLed; /* Do blinky on LED1 while we're waiting for BLE events */
    triggerSensorPolling = true; /* Note that the periodicCallback() executes in
                                  * interrupt context, so it is safer to do
                                  * heavy-weight sensor polling from the main
                                  * thread.*/
}

void bluetoothInit()
{
    DEBUG("Bluetooth initialising...(%s)\r\n", ble.getVersion());
    ble.init();
    ble.setDeviceName(Config::deviceName);
    //ble.onTimeout(timeoutCallback);
    ble.onConnection(connectionCallback);
    ble.onDisconnection(disconnectionCallback);
    //ble.onDataSent(onDataSent);
    ble.onDataWritten(onDataWritten);
    ble.onUpdatesEnabled(onUpdatesEnabled);
    //ble.onUpdatesDisabled(onUpdatesDisabled);
    //ble.onConfirmationReceived(onConfirmationReceived);

    // Make sure we use our preferred conn. parameters
    connectionParams.minConnectionInterval        = Config::minConnectionInterval;
    connectionParams.maxConnectionInterval        = Config::maxConnectionInterval;
    connectionParams.slaveLatency                 = Config::slaveLatency;
    connectionParams.connectionSupervisionTimeout = Config::supervisionTimeout;
    ble.setPreferredConnectionParams(&connectionParams);
    ble.getPreferredConnectionParams(&connectionParams);
    DEBUG("Conn. params => min=%d, max=%d, slave=%d, supervision=%d\r\n",
          connectionParams.minConnectionInterval,
          connectionParams.maxConnectionInterval,
          connectionParams.slaveLatency,
          connectionParams.connectionSupervisionTimeout);

    // Initialise services
    BatteryService batteryService(ble);
    batteryServicePtr = &batteryService;
    DeviceInformationService deviceInfo(ble, "Buddi Ltd.", "BlueBand", "1234567890", "Hardware: 0", "Firmware: 0001", "Build: 0001");
    BuddiService buddiService(ble);
    buddiServicePtr = &buddiService;

    /* setup advertising */
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS, (const uint8_t *)uuid128_list, sizeof(uuid128_list));
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.setAdvertisingInterval(Config::advertisingInterval);
    ble.startAdvertising();
    advertisingStateLed = 1;
    DEBUG("Ready. Advertising.\r\n");
}

int main(void)
{
    DEBUG("Initialising Buddi Nudge | Built %s %s\n\r", __DATE__, __TIME__);

    for (int i = 0; i < 16; i++) {
        uuidlistrev[15 - i] = uuidlist[i];
    }

    // TODO: Add battery and dev info to advertising list
    bluetoothInit();

    oneSecondLed = 1;
    Ticker ticker;
    ticker.attach(periodicCallback, 1);
    //float acc_data[3];
    //int16_t acc_raw[3];
    /*
    DEBUG("MMA8652 Acc    = %X\r\n", acc1.getWhoAmI());
    acc1.ReadXYZ(acc_data);
    acc1.ReadXYZraw(acc_raw);
    DEBUG("MMA8652 Acc:  X:%1.3f Y:%1.3f Z:%1.3f (Raw X:%3d Y:%3d Z:%3d)\r\n", acc_data[0], acc_data[1], acc_data[2], acc_raw[0], acc_raw[1], acc_raw[2]);
    */
    while (true) {
        if (triggerSensorPolling) {
            triggerSensorPolling = false;

            /* Do blocking calls or whatever is necessary for sensor polling. */
            if (ble.getGapState().connected) {
                /* Update the battery measurement */
                //batteryLevel = adc1.read_u16()&0x0FFF) * 3.3/4096;
                batteryLevel--;
                if (batteryLevel == 1) {
                    batteryLevel = 100;
                }
                batteryServicePtr->updateBatteryLevel(batteryLevel);
            }

        } else {
            ble.waitForEvent();
        }
    }
}
