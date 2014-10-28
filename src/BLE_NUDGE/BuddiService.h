/** BuddiService class
 *  Defines the Buddi Service and Characteristics
 *
 * Used as follows:
 * @code
 * #include BuddiService.h
 * BuddiService *buddiServicePtr;
 * BuddiService buddiService(ble);
 * buddiServicePtr = &buddiService;
 * @endcode
 */

#ifndef __H_BUDDI_SERVICE_H__
#define __H_BUDDI_SERVICE_H__

#include "UUID.h"
#include "BLEDevice.h"
#include "Logger.h"
#include "Configuration.h"

extern const uint8_t  BuddiServiceBaseUUID[LENGTH_OF_LONG_UUID];
extern const uint16_t BuddiServiceShortUUID;
extern const uint16_t BuddiServiceCommandCharacteristicShortUUID;
extern const uint16_t BuddiServiceNotificationCharacteristicShortUUID;

extern const uint8_t  BuddiServiceUUID[LENGTH_OF_LONG_UUID];
extern const uint8_t  BuddiServiceUUID_reversed[LENGTH_OF_LONG_UUID];

extern const uint8_t  BuddiServiceCommandCharacteristicUUID[LENGTH_OF_LONG_UUID];
extern const uint8_t  BuddiServiceNotificationCharacteristicUUID[LENGTH_OF_LONG_UUID];

/**
 * Storage for the value of the characteristics
 */
typedef struct {
    uint8_t module;
    uint8_t opcode;
    uint8_t data[15];
} Command_t;

class HeartRateService {
public:
    /**
     * Module types.
     */
    enum {
        MODULE_SWITCH        = 0x01,
        MODULE_LED           = 0x02,
        MODULE_ACCELEROMETER = 0x03,
        MODULE_TEMPERATURE   = 0x04,
        MODULE_GPIO          = 0x05,
        MODULE_NEO_PIXEL     = 0x06,
        MODULE_IBEACON       = 0x07,
        MODULE_HAPTIC        = 0x08,
        MODULE_BUDDI         = 0x10,
        MODULE_DEBUG         = 0xFE
    };
    static const unsigned COMMAND_BUFFER_SIZE       17;
    static const unsigned NOTIFICATION_BUFFER_SIZE  8;

public:
    /**
     * BuddiService Constructor.
     * @param _ble Reference to the underlying BLEDevice.
     */
    BuddiService(BLEDevice &_ble) :
        ble(_ble),
        commandPayload(),
        notificationPayload(),
        numBytesReceived(0),
        commandCharacteristic(BuddiServiceCommandCharacteristicUUID, commandPayload, 1, COMMAND_BUFFER_SIZE,
                         GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE),
        notificationCharacteristic(UARTServiceRXCharacteristicUUID, notificationPayload, 1, NOTIFICATION_BUFFER_SIZE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY) {
        setupService();
    }

    /**
     * Note: Command and Notification characteristics are to be interpreted from the viewpoint of the GATT client using this service.
     */
    uint16_t getCommandCharacteristicHandle() {
        return commandCharacteristic.getValueAttribute().getHandle();
    }

    /**
     * Note: Command and Notification characteristics are to be interpreted from the viewpoint of the GATT client using this service.
     */
    uint16_t getNotificationCharacteristicHandle() {
        return notificationCharacteristic.getValueAttribute().getHandle();
    }

    /**
     * This callback allows the Buddi service to receive updates to the
     * commandCharacteristic. The application should forward the call to this
     * function from the global onDataWritten() callback handler; or if that's
     * not used, this method can be used as a callback directly.
     */
    virtual void onDataWritten(const GattCharacteristicWriteCBParams *params) {
        if(!ble) {
            return;
        }

        if (params->charHandle == getCommandCharacteristicHandle()) {
            /**
             * @NOTE: if you are extending this class, be sure to also call
             * ble.onDataWritten(this, &ExtendedHRService::onDataWritten); in
             * your constructor.
             */
            uint16_t bytesRead = params->len;
            if (bytesRead != sizeof(Command_t)) {
                DEBUG("Invalid write into Command characteristic\r\n");
                return;
            } else {
                numBytesReceived   = bytesRead;
                memcpy(commandPayload, params->data, numBytesReceived);

                DEBUG("RX: MOD: 0x%X OP: 0x%X D0: 0x%X D1: 0x%X", commandPayload.module, commandPayload.opcode, commandPayload.data[0], commandPayload.data[1]);
                //ble->updateCharacteristicValue(notificationCharacteristic.getValueAttribute().getHandle(), commandPayload, bytesRead);
                switch(commandPayload.module) {
                    case MODULE_NEO_PIXEL:
                        setLEDValue(0, commandPayload.data[2]);
                        setLEDValue(1, commandPayload.data[3]);
                        setLEDValue(2, commandPayload.data[4]);
                        break;
                    default:
                        DEBUG("Module %u not implemented yet", commandPayload.module);
                        break;
                }
            }
        }
    }

private:
    void setupService(void) {
        static bool serviceAdded = false; /* We should only ever need to add the heart rate service once. */
        if (serviceAdded) {
            return;
        }

        GattCharacteristic *charTable[] = {&commandCharacteristic, &notificationCharacteristic};
        GattService         buddiService(BuddiServiceUUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

        ble.addService(buddiService);
        serviceAdded = true;

        ble.onDataWritten(this, &BuddiService::onDataWritten);
    }

private:
    BLEDevice           &ble;
    Command_t           commandPayload;
    uint8_t             commandPayload[17];      /**< The local buffer into which we receive
                                                  *   inbound data before forwarding it to the
                                                  *   application. */
    uint8_t             notificationPayload[8]; /**< The local buffer into which outbound data is
                                                 *   accumulated before being pushed to the
                                                 *   notificationCharacteristic. */
    uint8_t             numBytesReceived;
    GattCharacteristic  commandCharacteristic;
    GattCharacteristic  notificationCharacteristic;

#endif /* #ifndef __H_BUDDI_SERVICE_H__*/
