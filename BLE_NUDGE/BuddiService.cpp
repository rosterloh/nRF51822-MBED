#include "BuddiService.h"

/**
 * Buddi Service Base UUID: 92D7DC20-xxxx-4DF8-84B1-AD8AF6E1EA4A
 */
const uint8_t  BuddiServiceBaseUUID[LENGTH_OF_LONG_UUID] = {
    0x92, 0xD7, 0xDC, 0x20, 0x00, 0x00, 0x4D, 0xF8,
    0x82, 0xB1, 0xAD, 0x8A, 0xF6, 0xE1, 0xEA, 0x4A,
};
const uint16_t BuddiServiceShortUUID                           = 0xBA55;
const uint16_t BuddiServiceCommandCharacteristicShortUUID      = 0xBB01;
const uint16_t BuddiServiceNotificationCharacteristicShortUUID = 0xBB02;
const uint8_t  BuddiServiceUUID[LENGTH_OF_LONG_UUID] = {
    0x92, 0xD7, 0xDC, 0x20, (uint8_t)(BuddiServiceShortUUID >> 8), (uint8_t)(BuddiServiceShortUUID & 0xFF), 0x4D, 0xF8,
    0x82, 0xB1, 0xAD, 0x8A, 0xF6, 0xE1, 0xEA, 0x4A,
};
const uint8_t BuddiServiceUUID_reversed[LENGTH_OF_LONG_UUID] = {
    0x4A, 0xEA, 0xE1, 0xF6, 0x8A, 0xAD, 0xB1, 0x82,
    0xF8, 0x4D, (uint8_t)(BuddiServiceShortUUID & 0xFF), (uint8_t)(BuddiServiceShortUUID >> 8), 0x20, 0xDC, 0xD7, 0x92
};
const uint8_t BuddiServiceCommandCharacteristicUUID[LENGTH_OF_LONG_UUID] = {
    0x92, 0xD7, 0xDC, 0x20, (uint8_t)(BuddiServiceCommandCharacteristicShortUUID >> 8), (uint8_t)(BuddiServiceCommandCharacteristicShortUUID & 0xFF), 0x4D, 0xF8,
    0x82, 0xB1, 0xAD, 0x8A, 0xF6, 0xE1, 0xEA, 0x4A,
};
const uint8_t BuddiServiceNotificationCharacteristicUUID[LENGTH_OF_LONG_UUID] = {
    0x92, 0xD7, 0xDC, 0x20, (uint8_t)(BuddiServiceNotificationCharacteristicShortUUID >> 8), (uint8_t)(BuddiServiceNotificationCharacteristicShortUUID & 0xFF), 0x4D, 0xF8,
    0x82, 0xB1, 0xAD, 0x8A, 0xF6, 0xE1, 0xEA, 0x4A,
};

/*

PwmOut      ledr(p22);                       // Red LED on PCA10000
PwmOut      ledg(p23);                       // Green LED on PCA10000
PwmOut      ledb(p21);                       // Blue LED on PCA10000

void init(BLEDevice &bleDevice)
{
    ledr               = 1.0f;
    ledg               = 1.0f;
    ledb               = 1.0f;
    ble                = &bleDevice;
    ble->addService(buddiService);
    DEBUG("Added buddi service\r\n");
}

void setLEDValue(uint8_t led, uint8_t value)
{
    float val = ((float)value)/255;
    DEBUG("Setting LED %u to %1.3f\r\n", led, val);
    switch(led)
    {
        case 0:
            ledg = val;
            break;
        case 1:
            ledr = val;
            break;
        case 2:
            ledb = val;
            break;
        default:
            DEBUG("Unknown LED %u selected\r\n", led);
            break;
    }
}
*/
