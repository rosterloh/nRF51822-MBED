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

#include "MMA8452.h"
#include "mbed.h"

#ifdef MMA8452_DEBUG
// you need to define Serial pc(USBTX,USBRX) somewhere for the below line to make sense
extern Serial pc;
#define MMA8452_DBG(...) pc.printf(__VA_ARGS__); pc.printf("\r\n");
#else
#define MMA8452_DBG(...)
#endif

// Connect module at I2C address using I2C port pins sda and scl
MMA8452::MMA8452(PinName sda, PinName scl, int frequency) : _i2c(sda, scl) , _frequency(frequency) {
   MMA8452_DBG("Creating MMA8452");

   // set I2C frequency
   _i2c.frequency(_frequency);

   // setup read and write addresses for convenience
   _readAddress   = MMA8452_ADDRESS | 0x01;
   _writeAddress  = MMA8452_ADDRESS & 0xFE;

   // set some defaults
   _bitDepth = BIT_DEPTH_UNKNOWN;
   setBitDepth(BIT_DEPTH_12);
   _dynamicRange = DYNAMIC_RANGE_UNKNOWN;
   setDynamicRange(DYNAMIC_RANGE_2G);

   MMA8452_DBG("Done");
}


// Destroys instance
MMA8452::~MMA8452() {}

// Setting the control register bit 1 to true to activate the MMA8452
int MMA8452::activate() {
    // perform write and return error code
    return logicalORRegister(MMA8452_CTRL_REG_1,MMA8452_ACTIVE_MASK);
}

// Setting the control register bit 1 to 0 to standby the MMA8452
int MMA8452::standby() {
    // perform write and return error code
    return logicalANDRegister(MMA8452_CTRL_REG_1,MMA8452_STANDBY_MASK);
}

// this reads a register, applies a bitmask with logical AND, sets a value with logical OR,
// and optionally goes into and out of standby at the beginning and end of the function respectively
int MMA8452::maskAndApplyRegister(char reg, char mask, char value, int toggleActivation) {
   if(toggleActivation) {
       if(standby()) {
          return 1;
       }
   }

   // read from register
   char oldValue = 0;
   if(readRegister(reg,&oldValue)) {
      return 1;
   }

   // apply bitmask
   oldValue &= mask;

   // set value
   oldValue |= value;

   // write back to register
   if(writeRegister(reg,oldValue)) {
      return 1;
   }

   if(toggleActivation) {
       if(activate()) {
          return 1;
       }
   }
   return 0;
}

int MMA8452::setDynamicRange(DynamicRange range, int toggleActivation) {
   _dynamicRange = range;
   return maskAndApplyRegister(
      MMA8452_XYZ_DATA_CFG,
      MMA8452_DYNAMIC_RANGE_MASK,
      range,
      toggleActivation
   );
}

int MMA8452::setDataRate(DataRateHz dataRate, int toggleActivation) {
   return maskAndApplyRegister(
       MMA8452_CTRL_REG_1,
       MMA8452_DATA_RATE_MASK,
       dataRate<<MMA8452_DATA_RATE_MASK_SHIFT,
       toggleActivation
   );
}

int MMA8452::setBitDepth(BitDepth depth,int toggleActivation) {
   _bitDepth = depth;
   return maskAndApplyRegister(
      MMA8452_CTRL_REG_1,
      MMA8452_BIT_DEPTH_MASK,
      depth<<MMA8452_BIT_DEPTH_MASK_SHIFT,
      toggleActivation
   );
}

char MMA8452::getMaskedRegister(int addr, char mask) {
   char rval = 0;
   if(readRegister(addr,&rval)) {
      return 0;
   }
   return (rval&mask);
}

int MMA8452::isXYZReady() {
   return getMaskedRegister(MMA8452_STATUS,MMA8452_STATUS_ZYXDR_MASK)>0;
}

int MMA8452::isXReady() {
   return getMaskedRegister(MMA8452_STATUS,MMA8452_STATUS_XDR_MASK)>0;
}

int MMA8452::isYReady() {
   return getMaskedRegister(MMA8452_STATUS,MMA8452_STATUS_YDR_MASK)>0;
}

int MMA8452::isZReady() {
   return getMaskedRegister(MMA8452_STATUS,MMA8452_STATUS_ZDR_MASK)>0;
}


int MMA8452::getDeviceID(char *dst) {
   return readRegister(MMA8452_WHO_AM_I,dst);
}

int MMA8452::getStatus(char* dst) {
   return readRegister(MMA8452_STATUS,dst);
}

MMA8452::DynamicRange MMA8452::getDynamicRange() {
   char rval = 0;
   if(readRegister(MMA8452_XYZ_DATA_CFG,&rval)) {
      return MMA8452::DYNAMIC_RANGE_UNKNOWN;
   }
   rval &= (MMA8452_DYNAMIC_RANGE_MASK^0xFF);
   return (MMA8452::DynamicRange)rval;
}

MMA8452::DataRateHz MMA8452::getDataRate() {
   char rval = 0;
   if(readRegister(MMA8452_CTRL_REG_1,&rval)) {
      return MMA8452::RATE_UNKNOWN;
   }
   // logical AND with inverse of mask
   rval = rval&(MMA8452_DATA_RATE_MASK^0xFF);
   // shift back into position
   rval >>= MMA8452_DATA_RATE_MASK_SHIFT;
   return (MMA8452::DataRateHz)rval;
}

// Reads xyz
int MMA8452::readXYZRaw(char *dst) {
   if(_bitDepth==BIT_DEPTH_UNKNOWN) {
      return 1;
   }
   int readLen = 3;
   if(_bitDepth==BIT_DEPTH_12) {
      readLen = 6;
   }
   return readRegister(MMA8452_OUT_X_MSB,dst,readLen);
}

int MMA8452::readXRaw(char *dst) {
   if(_bitDepth==BIT_DEPTH_UNKNOWN) {
      return 1;
   }
   int readLen = 1;
   if(_bitDepth==BIT_DEPTH_12) {
      readLen = 2;
   }
   return readRegister(MMA8452_OUT_X_MSB,dst,readLen);
}

int MMA8452::readYRaw(char *dst) {
   if(_bitDepth==BIT_DEPTH_UNKNOWN) {
      return 1;
   }
   int readLen = 1;
   if(_bitDepth==BIT_DEPTH_12) {
      readLen = 2;
   }
   return readRegister(MMA8452_OUT_Y_MSB,dst,readLen);
}

int MMA8452::readZRaw(char *dst) {
   if(_bitDepth==BIT_DEPTH_UNKNOWN) {
      return 1;
   }
   int readLen = 1;
   if(_bitDepth==BIT_DEPTH_12) {
      readLen = 2;
   }
   return readRegister(MMA8452_OUT_Z_MSB,dst,readLen);
}

int MMA8452::readXYZCounts(int *x, int *y, int *z) {
   char buf[6];
   if(readXYZRaw((char*)&buf)) {
       return 1;
   }
   if(_bitDepth==BIT_DEPTH_12) {
     *x = twelveBitToSigned(&buf[0]);
     *y = twelveBitToSigned(&buf[2]);
     *z = twelveBitToSigned(&buf[4]);
   } else {
     *x = eightBitToSigned(&buf[0]);
     *y = eightBitToSigned(&buf[1]);
     *z = eightBitToSigned(&buf[2]);
   }

   return 0;
}

int MMA8452::readXCount(int *x) {
   char buf[2];
   if(readXRaw((char*)&buf)) {
       return 1;
   }
   if(_bitDepth==BIT_DEPTH_12) {
     *x = twelveBitToSigned(&buf[0]);
   } else {
     *x = eightBitToSigned(&buf[0]);
   }
   return 0;
}

int MMA8452::readYCount(int *y) {
   char buf[2];
   if(readYRaw((char*)&buf)) {
       return 1;
   }
   if(_bitDepth==BIT_DEPTH_12) {
     *y = twelveBitToSigned(&buf[0]);
   } else {
     *y = eightBitToSigned(&buf[0]);
   }
   return 0;
}

int MMA8452::readZCount(int *z) {
   char buf[2];
   if(readZRaw((char*)&buf)) {
       return 1;
   }
   if(_bitDepth==BIT_DEPTH_12) {
     *z = twelveBitToSigned(&buf[0]);
   } else {
     *z = eightBitToSigned(&buf[0]);
   }
   return 0;
}

double MMA8452::convertCountToGravity(int count, int countsPerG) {
   return (double)count/(double)countsPerG;
}

int MMA8452::getCountsPerG() {
 // assume starting with DYNAMIC_RANGE_2G and BIT_DEPTH_12
   int countsPerG = 1024;
   if(_bitDepth==BIT_DEPTH_8) {
      countsPerG = 64;
   }
   switch(_dynamicRange) {
      case DYNAMIC_RANGE_4G:
         countsPerG /= 2;
         break;
      case DYNAMIC_RANGE_8G:
         countsPerG /= 4;
         break;
      case DYNAMIC_RANGE_2G:
      case DYNAMIC_RANGE_UNKNOWN:
          break;
   }
   return countsPerG;
}

int MMA8452::readXYZGravity(double *x, double *y, double *z) {
   int xCount = 0, yCount = 0, zCount = 0;
   if(readXYZCounts(&xCount,&yCount,&zCount)) {
      return 1;
   }
   int countsPerG = getCountsPerG();

   *x = convertCountToGravity(xCount,countsPerG);
   *y = convertCountToGravity(yCount,countsPerG);
   *z = convertCountToGravity(zCount,countsPerG);
   return 0;
}

int MMA8452::readXGravity(double *x) {
   int xCount = 0;
   if(readXCount(&xCount)) {
      return 1;
   }
   int countsPerG = getCountsPerG();

   *x = convertCountToGravity(xCount,countsPerG);
   return 0;
}

int MMA8452::readYGravity(double *y) {
   int yCount = 0;
   if(readYCount(&yCount)) {
      return 1;
   }
   int countsPerG = getCountsPerG();

   *y = convertCountToGravity(yCount,countsPerG);
   return 0;
}

int MMA8452::readZGravity(double *z) {
   int zCount = 0;
   if(readZCount(&zCount)) {
      return 1;
   }
   int countsPerG = getCountsPerG();

   *z = convertCountToGravity(zCount,countsPerG);
   return 0;
}

// apply an AND mask to a register. read register value, apply mask, write it back
int MMA8452::logicalANDRegister(char addr, char mask) {
   char value = 0;
   // read register value
   if(readRegister(addr,&value)) {
      return 0;
   }
   // apply mask
   value &= mask;
   return writeRegister(addr,value);
}


// apply an OR mask to a register. read register value, apply mask, write it back
int MMA8452::logicalORRegister(char addr, char mask) {
   char value = 0;
   // read register value
   if(readRegister(addr,&value)) {
      return 0;
   }
   // apply mask
   value |= mask;
   return writeRegister(addr,value);
}

// apply an OR mask to a register. read register value, apply mask, write it back
int MMA8452::logicalXORRegister(char addr, char mask) {
   char value = 0;
   // read register value
   if(readRegister(addr,&value)) {
      return 0;
   }
   // apply mask
   value ^= mask;
   return writeRegister(addr,value);
}

// Write register (The device must be placed in Standby Mode to change the value of the registers)
int MMA8452::writeRegister(char addr, char data) {
    // what this actually does is the following
    // 1. tell I2C bus to start transaction
    // 2. tell slave we want to write (slave address & write flag)
    // 3. send the write address
    // 4. send the data to write
    // 5. tell I2C bus to end transaction

    // we can wrap this up in the I2C library write function
    char buf[2] = {0,0};
    buf[0] = addr;
    buf[1] = data;
    return _i2c.write(MMA8452_ADDRESS, buf,2);
    // note, could also do return writeRegister(addr,&data,1);
}

int MMA8452::eightBitToSigned(char *buf) {
   return (int8_t)*buf;
}

int MMA8452::twelveBitToSigned(char *buf) {
   // cheat by using the int16_t internal type
   // all we need to do is convert to little-endian format and shift right
   int16_t x = 0;
   ((char*)&x)[1] = buf[0];
   ((char*)&x)[0] = buf[1];
   // note this only works because the below is an arithmetic right shift
   return x>>4;
}

int MMA8452::writeRegister(char addr, char *data, int nbytes) {
    // writing multiple bytes is a little bit annoying because
    // the I2C library doesn't support sending the address separately
    // so we just do it manually

    // 1. tell I2C bus to start transaction
    _i2c.start();
    // 2. tell slave we want to write (slave address & write flag)
    if(_i2c.write(_writeAddress)!=1) {
       return 1;
    }
    // 3. send the write address
    if(_i2c.write(addr)!=1) {
       return 1;
    }
    // 4. send the data to write
    for(int i=0; i<nbytes; i++) {
       if(_i2c.write(data[i])!=1) {
          return 1;
       }
    }
    // 5. tell I2C bus to end transaction
    _i2c.stop();
    return 0;
}

int MMA8452::readRegister(char addr, char *dst, int nbytes) {
    // this is a bit odd, but basically proceeds like this
    // 1. Send a start command
    // 2. Tell the slave we want to write (slave address & write flag)
    // 3. Send the address of the register (addr)
    // 4. Send another start command to delineate read portion
    // 5. Tell the slave we want to read (slave address & read flag)
    // 6. Read the register value bytes
    // 7. Send a stop command

    // we can wrap this process in the I2C library read and write commands
    if(_i2c.write(MMA8452_ADDRESS,&addr,1,true)) {
       return 1;
    }
    return _i2c.read(MMA8452_ADDRESS,dst,nbytes);
}

// most registers are 1 byte, so here is a convenience function
int MMA8452::readRegister(char addr, char *dst) {
    return readRegister(addr,dst,1);
}

MMA8452::BitDepth MMA8452::getBitDepth() {
   return _bitDepth;
}

#ifdef MMA8452_DEBUG
void MMA8452::debugRegister(char reg) {
   // get register value
   char v = 0;
   if(readRegister(reg,&v)) {
      MMA8452_DBG("Error reading specified register");
      return;
   }
   // print out details
   switch(reg) {
      case MMA8452_CTRL_REG_1:
         MMA8452_DBG("CTRL_REG_1 has value: 0x%x",v);
         MMA8452_DBG(" 7  ALSP_RATE_1: %d",(v&0x80)>>7);
         MMA8452_DBG(" 6  ALSP_RATE_0: %d",(v&0x40)>>6);
         MMA8452_DBG(" 5  DR2: %d",        (v&0x20)>>5);
         MMA8452_DBG(" 4  DR1: %d",        (v&0x10)>>4);
         MMA8452_DBG(" 3  DR0: %d",        (v&0x08)>>3);
         MMA8452_DBG(" 2  LNOISE: %d",     (v&0x04)>>2);
         MMA8452_DBG(" 1  FREAD: %d",      (v&0x02)>>1);
         MMA8452_DBG(" 0  ACTIVE: %d",     (v&0x01));
      break;

      case MMA8452_XYZ_DATA_CFG:
         MMA8452_DBG("XYZ_DATA_CFG has value: 0x%x",v);
         MMA8452_DBG(" 7  Unused: %d", (v&0x80)>>7);
         MMA8452_DBG(" 6  0: %d",      (v&0x40)>>6);
         MMA8452_DBG(" 5  0: %d",      (v&0x20)>>5);
         MMA8452_DBG(" 4  HPF_Out: %d",(v&0x10)>>4);
         MMA8452_DBG(" 3  0: %d",      (v&0x08)>>3);
         MMA8452_DBG(" 2  0: %d",      (v&0x04)>>2);
         MMA8452_DBG(" 1  FS1: %d",    (v&0x02)>>1);
         MMA8452_DBG(" 0  FS0: %d",    (v&0x01));
         switch(v&0x03) {
            case 0:
               MMA8452_DBG("Dynamic range: 2G");
            break;
            case 1:
               MMA8452_DBG("Dynamic range: 4G");
            break;
            case 2:
               MMA8452_DBG("Dynamic range: 8G");
            break;
            default:
               MMA8452_DBG("Unknown dynamic range");
            break;
         }
      break;

      case MMA8452_STATUS:
         MMA8452_DBG("STATUS has value: 0x%x",v);
         MMA8452_DBG(" 7  ZYXOW: %d",(v&0x80)>>7);
         MMA8452_DBG(" 6  ZOW: %d",  (v&0x40)>>6);
         MMA8452_DBG(" 5  YOW: %d",  (v&0x20)>>5);
         MMA8452_DBG(" 4  XOW: %d",  (v&0x10)>>4);
         MMA8452_DBG(" 3  ZYXDR: %d",(v&0x08)>>3);
         MMA8452_DBG(" 2  ZDR: %d",  (v&0x04)>>2);
         MMA8452_DBG(" 1  YDR: %d",  (v&0x02)>>1);
         MMA8452_DBG(" 0  XDR: %d",  (v&0x01));
      break;

      default:
         MMA8452_DBG("Unknown register address: 0x%x",reg);
      break;
   }
}
#endif
