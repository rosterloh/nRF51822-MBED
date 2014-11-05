#pragma once

// Authors: Ashley Mills, Nicholas Herriot
/* Copyright (c) 2013 Vodafone, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// the SparkFun breakout board defaults to 1, set to 0 if SA0 jumper on the bottom of the board is set
// see the Table 10. I2C Device Address Sequence in Freescale MMA8452Q pdf

#include "mbed.h"

//#define MMA8452_DEBUG 1

// More info on MCU Master address can be found on section 5.10.1 of http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MMA8452Q
#define SA0 1
#if SA0
  #define MMA8452_ADDRESS 0x3A // 0x1D<<1  // SA0 is high, 0x1C if low -
#else
  #define MMA8452_ADDRESS 0x38 // 0x1C<<1
#endif

// Register descriptions found in section 6 of pdf
#define MMA8452_STATUS 0x00        // Type 'read' : Status of the data registers
#define MMA8452_OUT_X_MSB 0x01     // Type 'read' : x axis - MSB of 2 byte sample
#define MMA8452_OUT_X_LSB 0x02     // Type 'read' : x axis - LSB of 2 byte sample
#define MMA8452_OUT_Y_MSB 0x03     // Type 'read' : y axis - MSB of 2 byte sample
#define MMA8452_OUT_Y_LSB 0x04     // Type 'read' : y axis - LSB of 2 byte sample
#define MMA8452_OUT_Z_MSB 0x05     // Type 'read' : z axis - MSB of 2 byte sample
#define MMA8452_OUT_Z_LSB 0x06     // Type 'read' : z axis - LSB of 2 byte sample

// register definitions
#define MMA8452_XYZ_DATA_CFG 0x0E

#define MMA8452_SYSMOD 0x0B        // Type 'read' : This tells you if device is active, sleep or standy 0x00=STANDBY 0x01=WAKE 0x02=SLEEP
#define MMA8452_WHO_AM_I 0x0D      // Type 'read' : This should return the device id of 0x2A

#define MMA8452_PL_STATUS 0x10     // Type 'read' : This shows portrait landscape mode orientation
#define MMA8452_PL_CFG 0x11        // Type 'read/write' : This allows portrait landscape configuration
#define MMA8452_PL_COUNT 0x12      // Type 'read' : This is the portraint landscape debounce counter
#define MMA8452_PL_BF_ZCOMP 0x13   // Type 'read' :
#define MMA8452_PL_THS_REG 0x14    // Type 'read' :

#define MMA8452_FF_MT_CFG 0X15     // Type 'read/write' : Freefaul motion functional block configuration
#define MMA8452_FF_MT_SRC 0X16     // Type 'read' : Freefaul motion event source register
#define MMA8452_FF_MT_THS 0X17     // Type 'read' : Freefaul motion threshold register
#define MMA8452_FF_COUNT 0X18      // Type 'read' : Freefaul motion debouce counter

#define MMA8452_ASLP_COUNT 0x29    // Type 'read/write' : Counter settings for auto sleep
#define MMA8452_CTRL_REG_1 0x2A    // Type 'read/write' :
#define MMA8452_CTRL_REG_2 0x2B    // Type 'read/write' :
#define MMA8452_CTRL_REG_3 0x2C    // Type 'read/write' :
#define MMA8452_CTRL_REG_4 0x2D    // Type 'read/write' :
#define MMA8452_CTRL_REG_5 0x2E    // Type 'read/write' :

// Defined in table 13 of the Freescale PDF
/// xxx these all need to have better names
#define STANDBY 0x00                        // State value returned after a SYSMOD request, it can be in state STANDBY, WAKE or SLEEP
#define WAKE 0x01                           // State value returned after a SYSMOD request, it can be in state STANDBY, WAKE or SLEEP
#define SLEEP 0x02                          // State value returned after a SYSMOD request, it can be in state STANDBY, WAKE or SLEEP
#define ACTIVE 0x01                         // Stage value returned and set in Control Register 1, it can be STANDBY=00, or ACTIVE=01

#define TILT_STATUS 0x03        // Tilt Status (Read only)
#define SRST_STATUS 0x04        // Sample Rate Status Register (Read only)
#define SPCNT_STATUS 0x05       // Sleep Count Register (Read/Write)
#define INTSU_STATUS 0x06       // Interrupt Setup Register
#define MODE_STATUS 0x07        // Mode Register (Read/Write)
#define SR_STATUS 0x08          // Auto-Wake and Active Mode Portrait/Landscape Samples per Seconds Register (Read/Write)
#define PDET_STATUS 0x09        // Tap/Pulse Detection Register (Read/Write)
#define PD_STATUS 0xA           // Tap/Pulse Debounce Count Register (Read/Write)

// masks for enabling/disabling standby
#define MMA8452_ACTIVE_MASK 0x01
#define MMA8452_STANDBY_MASK 0xFE

// mask for dynamic range reading and writing
#define MMA8452_DYNAMIC_RANGE_MASK 0xFC

// mask and shift for data rate reading and writing
#define MMA8452_DATA_RATE_MASK 0xC7
#define MMA8452_DATA_RATE_MASK_SHIFT 0x03

// mask and shift for general reading and writing
#define MMA8452_WRITE_MASK 0xFE
#define MMA8452_READ_MASK 0x01

// mask and shift for bit depth reading and writing
#define MMA8452_BIT_DEPTH_MASK 0xFD
#define MMA8452_BIT_DEPTH_MASK_SHIFT 0x01

// status masks and shifts
#define MMA8452_STATUS_ZYXDR_MASK 0x08
#define MMA8452_STATUS_ZDR_MASK 0x04
#define MMA8452_STATUS_YDR_MASK 0x02
#define MMA8452_STATUS_XDR_MASK 0x01

/**
 * Wrapper for the MMA8452 I2C driven accelerometer.
 */
class MMA8452 {

    public:

       enum DynamicRange {
           DYNAMIC_RANGE_2G=0x00,
           DYNAMIC_RANGE_4G,
           DYNAMIC_RANGE_8G,
           DYNAMIC_RANGE_UNKNOWN
       };

       enum BitDepth {
           BIT_DEPTH_12=0x00,
           BIT_DEPTH_8, // 1 sets fast read mode, hence the inversion
           BIT_DEPTH_UNKNOWN
       };

       enum DataRateHz {
          RATE_800=0x00,
          RATE_400,
          RATE_200,
          RATE_100,
          RATE_50,
          RATE_12_5,
          RATE_6_25,
          RATE_1_563,
          RATE_UNKNOWN
       };

       /**
        * Create an accelerometer object connected to the specified I2C pins.
        *
        * @param sda I2C data port
        * @param scl I2C clock port
        * @param frequency
        *
        */
      MMA8452(PinName sda, PinName scl, int frequency);

      /// Destructor
      ~MMA8452();

      /**
       * Puts the MMA8452 in active mode.
       * @return 0 on success, 1 on failure.
       */
      int activate();

      /**
       * Puts the MMA8452 in standby.
       * @return 0 on success, 1 on failure.
       */
      int standby();

       /**
        * Read the device ID from the accelerometer (should be 0x2a)
        *
        * @param dst pointer to store the ID
        * @return 0 on success, 1 on failure.
        */
      int getDeviceID(char* dst);

      /**
       * Read the MMA8452 status register.
       *
       * @param dst pointer to store the register value.
       * @ return 0 on success, 1 on failure.
       */
      int getStatus(char* dst);

      /**
       * Read the raw x, y, an z registers of the MMA8452 in one operation.
       * All three registers are read sequentially and stored in the provided buffer.
       * The stored values are signed 2's complement left-aligned 12 or 8 bit integers.
       *
       * @param dst The destination buffer. Note that this needs to be 3 bytes for
       * BIT_DEPTH_8 and 6 bytes for BIT_DEPTH_12. It is upto the caller to ensure this.
       * @return 0 for success, and 1 for failure
       * @sa setBitDepth
       */
      int readXYZRaw(char *dst);

      /// Read the raw x register into the provided buffer. @sa readXYZRaw
      int readXRaw(char *dst);
      /// Read the raw y register into the provided buffer. @sa readXYZRaw
      int readYRaw(char *dst);
      /// Read the raw z register into the provided buffer. @sa readXYZRaw
      int readZRaw(char *dst);

      /**
       * Read the x, y, and z signed counts of the MMA8452 axes.
       *
       * Count resolution is either 8 bits or 12 bits, and the range is either +-2G, +-4G, or +-8G
       * depending on settings. The number of counts per G are 1024, 512, 256 for 2,4, and 8 G
       * respectively at 12 bit resolution and 64, 32, 16 for 2, 4, and 8 G respectively at
       * 8 bit resolution.
       *
       * This function queries the MMA8452 and returns the signed counts for each axes.
       *
       * @param x Pointer to integer to store x count
       * @param y Pointer to integer to store y count
       * @param z Pointer to integer to store z count
       * @return 0 on success, 1 on failure.
       */
      int readXYZCounts(int *x, int *y, int *z);

      /// Read the x axes signed count. @sa readXYZCounts
      int readXCount(int *x);
      /// Read the y axes signed count. @sa readXYZCounts
      int readYCount(int *y);
      /// Read the z axes signed count. @sa readXYZCounts
      int readZCount(int *z);

      /**
       * Read the x, y, and z accelerations measured in G.
       *
       * The measurement resolution is controlled via setBitDepth which can
       * be 8 or 12, and by setDynamicRange, which can be +-2G, +-4G, or +-8G.
       *
       * @param x A pointer to the double to store the x acceleration in.
       * @param y A pointer to the double to store the y acceleration in.
       * @param z A pointer to the double to store the z acceleration in.
       *
       * @return 0 on success, 1 on failure.
       */
      int readXYZGravity(double *x, double *y, double *z);

      /// Read the x gravity in G into the provided double pointer. @sa readXYZGravity
      int readXGravity(double *x);
      /// Read the y gravity in G into the provided double pointer. @sa readXYZGravity
      int readYGravity(double *y);
      /// Read the z gravity in G into the provided double pointer. @sa readXYZGravity
      int readZGravity(double *z);

      /// Returns 1 if data has been internally sampled (is available) for all axes since last read, 0 otherwise.
      int isXYZReady();
      /// Returns 1 if data has been internally sampled (is available) for the x-axis since last read, 0 otherwise.
      int isXReady();
      /// Returns 1 if data has been internally sampled (is available) for the y-axis since last read, 0 otherwise.
      int isYReady();
      /// Returns 1 if data has been internally sampled (is available) for the z-axis since last read, 0 otherwise.
      int isZReady();

      /**
       * Reads a single byte from the specified MMA8452 register.
       *
       * @param addr The internal register address.
       * @param dst The destination buffer address.
       * @return 1 on success, 0 on failure.
       */
      int readRegister(char addr, char *dst);

      /**
       * Reads n bytes from the specified MMA8452 register.
       *
       * @param addr The internal register address.
       * @param dst The destination buffer address.
       * @param nbytes The number of bytes to read.
       * @return 1 on success, 0 on failure.
       */
      int readRegister(char addr, char *dst, int nbytes);

       /**
        * Write to the specified MMA8452 register.
        *
        * @param addr The internal register address
        * @param data Data byte to write
        */
      int writeRegister(char addr, char data);

       /**
        * Write a data buffer to the specified MMA8452 register.
        *
        * @param addr The internal register address
        * @param data Pointer to data buffer to write
        * @param nbytes The length of the data buffer to write
        */
      int writeRegister(char addr, char *data, int nbytes);

      int setDynamicRange(DynamicRange range, int toggleActivation=1);
      int setBitDepth(BitDepth depth, int toggleActivation=1);
      int setDataRate(DataRateHz dataRate, int toggleActivation=1);

      DynamicRange getDynamicRange();
      DataRateHz getDataRate();
      BitDepth getBitDepth();

      #ifdef MMA8452_DEBUG
      void debugRegister(char reg);
      #endif

    private:
      /**
       * Reads the specified register, applies the mask with logical AND, logical ORs the value
       * and writes back the result to the register. If toggleActivation is set to true then the
       * device is put in standby before the operation, and activated at the end.
       * Setting it to false is useful for setting options on a device that you want to keep in
       * standby.
       */
      int maskAndApplyRegister(char reg, char mask, char value, int toggleActivation);

      /// Reads the specified register, applies the mask with logical AND, and writes the result back.
      int logicalANDRegister(char addr, char mask);
      /// Reads the specified register, applies the mask with logical OR, and writes the result back.
      int logicalORRegister(char addr, char mask);
      /// Reads the specified register, applies the mask with logical XOR, and writes the result back.
      int logicalXORRegister(char addr, char mask);

      /// Converts the 12-bit two's complement number in buf to a signed integer. Returns the integer.
      int twelveBitToSigned(char *buf);
      /// Converts the 8-bit two's complement number in buf to a signed integer. Returns the integer.
      int eightBitToSigned(char *buf);

      /// Converts a count to a gravity using the supplied countsPerG. Returns the gravity.
      double convertCountToGravity(int count, int countsPerG);

      /// Reads the register at addr, applies the mask with logical AND, and returns the result.
      char getMaskedRegister(int addr, char mask);

      /// Get the counts per G for the current settings of bit depth and dynamic range.
      int getCountsPerG();

      I2C _i2c;
      int _frequency;
      int _readAddress;
      int _writeAddress;

      BitDepth _bitDepth;
      DynamicRange _dynamicRange;
};
