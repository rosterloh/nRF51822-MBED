/**
 * @file    MMA845x.h
 * @brief   Device driver - MMA845x 3-axis accelerometer IC
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

#ifndef MMA845X_H
#define MMA845X_H

#include "mbed.h"

/** Using the Sparkfun SEN-10955
 *
 * Example:
 * @code
 *  #include "mbed.h"
 *  #include "MMA845x.h"
 *

 *
 *  int main()
 *  {

 *  }
 * @endcode
 */


/**
 *  @class MMA845x_DATA
 *  @brief API abstraction for the MMA845x 3-axis accelerometer IC data
 */
class MMA845x_DATA
{
public:

    volatile uint16_t _x;   /*!< volatile data variable */
    volatile uint16_t _y;   /*!< volatile data variable */
    volatile uint16_t _z;   /*!< volatile data variable */

    /** Create the MMA845x_DATA object initialized to the parameter (or 0 if none)
     *  @param x - the init value of _x
     *  @param y - the init value of _y
     *  @param x - the init value of _z
     */
    MMA845x_DATA(uint16_t x = 0, uint16_t y = 0, uint16_t z = 0) : _x(x), _y(y), _z(z) {}

    /** Overloaded '=' operator to allow shorthand coding, assigning objects to one another
     *  @param rhs - an object of the same type to assign ourself the same values of
     *  @return this
     */
    MMA845x_DATA &operator= (MMA845x_DATA const &rhs)
    {
        _x = rhs._x;
        _y = rhs._y;
        _z = rhs._z;

        return *this;
    }

    /** Overloaded '=' operator to allow shorthand coding, assigning objects to one another
     *  @param val - Assign each data member (_x, _y, _z) this value
     *  @return this
     */
    MMA845x_DATA &operator= (uint16_t const val)
    {
        _x = _y = _z = val;

        return *this;
    }

    /** Overloaded '==' operator to allow shorthand coding, test objects to one another
     *  @param rhs - the object to compare against
     *  @return 1 if the data members are the same and 0 otherwise
     */
    bool operator== (MMA845x_DATA const &rhs) const
    {
        return ((_x == rhs._x)&&(_y == rhs._y)&&(_z == rhs._z)) ? 1 : 0;
    }

};

/**
 *  @class MMA845x
 *  @brief API abstraction for the MMA845x 3-axis accelerometer IC
 */
class MMA845x
{
public:

    /**
     *  @enum MMA845x_SA0
     *  @brief Possible terminations for the ADDR pin
     */
    enum MMA845x_SA0
    {
        SA0_VSS = 0, /*!< SA0 connected to VSS */
        SA0_VDD      /*!< SA0 connected to VDD */
    };

    /**
     *  @enum MMA845x_WHO_AM_I
     *  @brief Device ID's that this class is compatible with
     */
    enum MMA845x_WHO_AM_I
    {
        MMA8451 = 0x1a, /*!< MMA8451 WHO_AM_I register content */
        MMA8452 = 0x2a, /*!< MMA8452 WHO_AM_I register content */
        MMA8453 = 0x3a, /*!< MMA8453 WHO_AM_I register content */
    };

    /**
     *  @enum MMA845x_REGISTER
     *  @brief The device register map
     */
    enum MMA845x_REGISTER
    {
        STATUS = 0x0,
        OUT_X_MSB, OUT_X_LSB, OUT_Y_MSB, OUT_Y_LSB, OUT_Z_MSB, OUT_Z_LSB,

        F_SETUP = 0x9, TRIG_CFG, // only available on the MMA8451 variant

        SYSMOD = 0xb,
        INT_SOURCE, WHO_AM_I, XYZ_DATA_CFG, HP_FILTER_CUTOFF, PL_STATUS,
        PL_CFG, PL_COUNT, PL_BF_ZCOMP, P_L_THS_REG, FF_MT_CFG, FF_MT_SRC,
        FF_MT_THS, FF_MT_COUNT,

        TRANSIENT_CFG = 0x1d,
        TRANSIENT_SRC, TRANSIENT_THS, TRANSIENT_COUNT, PULSE_CFG, PULSE_SRC,
        PULSE_THSX, PULSE_THSY, PULSE_THSZ, PULSE_TMLT, PULSE_LTCY, PULSE_WIND,
        ASLP_COUNT, CTRL_REG1, CTRL_REG2, CTRL_REG3, CTRL_REG4, CTRL_REG5,
        OFF_X, OFF_Y, OFF_Z
    };

    /** Create the MMA845x object
     *  @param i2c - A defined I2C object
     *  @param int1 - A defined InterruptIn object
     *  @param int2 - A defined InterruptIn object
     *  @param i2c_addr - Connection of the address line
     */
    MMA845x(I2C &i2c, InterruptIn &int1, InterruptIn &int2, MMA845x_SA0 const i2c_addr);

    /** Get the X data
     *  @return The last valid reading from the accelerometer
     */
    uint16_t getX(void) const;

    /** Get the Y data
     *  @return The last valid reading from the accelerometer
     */
    uint16_t getY(void) const;

    /** Get the Z data
     *  @return The last valid reading from the accelerometer
     */
    uint16_t getZ(void) const;

    /** Get the XYZ data structure
     *  @return The last valid reading from the accelerometer
     */
    MMA845x_DATA getXYZ(void) const;

    void enableDataReadyMode(void) const;
    void enableMotionMode(void) const;
    void enablePulseMode(void) const;
    void enableOrientationMode(void) const;
    void enableTransitMode(void) const;
    void enableAutoSleepMode(void) const;
    void enableFIFOMode(void) const;

    /** Put the MMA845x in the lowest possible power mode and suspend operation
     */
    void disable(void);

    /** Write to a register (exposed for debugging reasons)
     *  Note: most writes are only valid in stop mode
     *  @param reg - The register to be written
     *  @param data - The data to be written
     */
    void writeRegister(uint8_t const reg, uint8_t const data) const;

    /** Read from a register (exposed for debugging reasons)
     *  @param reg - The register to read from
     *  @return The register contents
     */
    uint8_t readRegister(uint8_t const reg) const;

    /** print the register map and values to the console
     */
    void registerDump(void) const;

private:

    I2C         *_i2c;
    InterruptIn *_int1;
    InterruptIn *_int2;
    uint8_t      _i2c_addr;
    MMA845x_DATA _data;

    void init(void) const;
};

#endif
