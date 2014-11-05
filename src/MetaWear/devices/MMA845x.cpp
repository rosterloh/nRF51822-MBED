/**
 * @file    MMA845x.cpp
 * @brief   Device driver - MMA845X 3-axis accelerometer IC
 * @author  sam grove
 * @version 1.0
 * @see     http://cache.freescale.com/files/sensors/doc/data_sheet/MMA8451Q.pdf
 * @see     http://cache.freescale.com/files/sensors/doc/data_sheet/MMA8452Q.pdf
 * @see     http://cache.freescale.com/files/sensors/doc/data_sheet/MMA8453Q.pdf
 *
 * Copyright (c) 2013
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MMA845x.h"

MMA845x::MMA845x(I2C &i2c, InterruptIn &int1, InterruptIn &int2, MMA845x_SA0 const i2c_addr)
{
    _i2c =  &i2c;
    _int1 = &int1;
    _int2 = &int2;

    _i2c_addr = (0x1c << 2) | (i2c_addr << 0x1);

    return;
}

void MMA845x::init(void) const
{
    uint8_t reg_val = 0;

    _i2c->frequency(400000);

    // Reset all registers to POR values
    MMA845x::writeRegister(CTRL_REG2, 0xFF);        //REG 0x2B
    do{
        // wait for the reset bit to clear
        reg_val = MMA845x::readRegister(CTRL_REG2) & 0x40;
    }while(reg_val);

    // setup the registers that are common among modes
    MMA845x::writeRegister(CTRL_REG1, 0xd8);        //REG 0x2A
    MMA845x::writeRegister(CTRL_REG2, 0x00);        //REG 0x2B
    MMA845x::writeRegister(CTRL_REG3, 0x00);        //REG 0x2C

    MMA845x::writeRegister(XYZ_DATA_CFG, 0x0);      //REG 0xE HPF / scale +/-2,4,8g
    MMA845x::writeRegister(HP_FILTER_CUTOFF, 0x0);  //REG 0xF HPF settings

    return;
}

void MMA845x::enableDataReadyMode(void) const
{
    MMA845x::init();
    MMA845x::writeRegister(SYSMOD, 0x1);        //REG 0x0B
    MMA845x::writeRegister(INT_SOURCE, 0x1);    //REG 0x0C
    // need to finish up these config registers..... 3/8/13

}

void MMA845x::enableMotionMode(void) const{}
void MMA845x::enablePulseMode(void) const{}

void MMA845x::enableOrientationMode(void) const
{
    uint16_t who_am_i = MMA845x::readRegister(WHO_AM_I);
    if(who_am_i != MMA8451)
    {
        error("%s %d: Feature not compatible with the connected device.\n", __FILE__, __LINE__);
    }

    return;
}

void MMA845x::enableTransitMode(void) const{}
void MMA845x::enableAutoSleepMode(void) const{}

void MMA845x::enableFIFOMode(void) const
{
    uint16_t who_am_i = MMA845x::readRegister(WHO_AM_I);
    if(who_am_i != MMA8451)
    {
        error("%s %d: Feature not compatible with the connected device.\n", __FILE__, __LINE__);
    }

    //MMA845x::writeRegister(

    return;
}

uint16_t MMA845x::getX(void) const
{
    return _data._x;
}

uint16_t MMA845x::getY(void) const
{
    return _data._y;
}

uint16_t MMA845x::getZ(void) const
{
    return _data._z;
}

MMA845x_DATA MMA845x::getXYZ(void) const
{
    return _data;
}

void MMA845x::writeRegister(uint8_t const reg, uint8_t const data) const
{
    char buf[2] = {reg, data};
    uint8_t result = 0;

    __disable_irq(); // Tickers and other timebase events can jack up the I2C bus for some deicse
    result = _i2c->write(_i2c_addr, buf, 2);
    __enable_irq();  // Just need to block during the transaction

    if(0 != result)
    {
        error("%s %d: I2c write failed\n", __FILE__, __LINE__);
    }

    return;
}

uint8_t MMA845x::readRegister(uint8_t const reg) const
{
    uint8_t result = 1, data = 0;

    __disable_irq(); // Tickers and other timebase events can jack up the I2C bus
    _i2c->start();
    result &= _i2c->write(_i2c_addr);
    result &= _i2c->write(reg);
    // issue a repeated start...
    _i2c->start();
    result &= _i2c->write(_i2c_addr | 0x01);
    // read with nak
    data = _i2c->read(0);
    _i2c->stop();
    __enable_irq();  // Just need to block during the transaction

    if(1 != result)
    {
        error("%s %d: I2C read failed\n", __FILE__, __LINE__);
    }

    return data;
}

void MMA845x::registerDump(void) const
{
    uint8_t reg_val = 0;

    for(int i=0; i<0x80; i++)
    {
        reg_val = MMA845x::readRegister(i);
        printf("Reg 0x%02x: 0x%02x \n", i, reg_val);
    }

    return;
}
