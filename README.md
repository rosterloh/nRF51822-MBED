# nRF51822-MBED

Build system for the [Nordic Semiconductor nRF51822 SoC](https://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF51822) and [MBED](http://mbed.org/)

### Build Dependencies
* [CMake](http://www.cmake.org/)
* [GNU Tools for ARM Embedded Processors](https://launchpad.net/gcc-arm-embedded/)

## Setup Requirements

For Windows development the easiest way to get this set up is with [Chocolatey](https://chocolatey.org/). One installed setup the dependencies as follows:

```bash
$ cinst git make cmake gcc-arm-embedded
```

### Submodules
* [mbed-src](https://github.com/mbedmicro/mbed)
* [BLE_API](https://github.com/rgrover/BLE_API)
* [nRF51822](https://github.com/rgrover/nRF51822)
