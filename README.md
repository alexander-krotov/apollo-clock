# Digital clock with Apollo DA-2300 indicators

## Setup instructions

Easy setup instructions: https://github.com/alexander-krotov/apollo-clock/blob/main/setup.md

## Hardware

Clock is built around 6 Apollo DA-2300 vintage indicators. The indicators are very energy inefficient, sinc a lot of current (around 15mA per segment) and require direct drive.
ESP32 C3 super mini module drives the clock, it controls every individual segment via 6 74hc595 shift registers, each output bit is amplified with 6 uln2803 darlington transistor arrays.
In addition to that 2 pins are used to drive LEDs in separator columns (resistors were needed to limit the current).
DS3231 with a backup battery is used as RTC clock.

Whole thing eats quite a lot of current (around 600 mA on average) and so requires quite powerful usb power supply.

Clock video:
https://youtu.be/Xn6VzxSFZeI

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/clock.jpg?raw=true)

Hardware schematics:
https://oshwlab.com/alexander.krotov/micronixie

References to components:
- Apollo DA-2300 https://www.tube-tester.com/sites/nixie/dat_arch/Numitron_Apollo.pdf
- ESP32 C3 super mini https://dl.artronshop.co.th/ESP32-C3%20SuperMini%20datasheet.pdf

## Software

The firmware is built with Arduino IDE 2.
WifiManager is used to connect to WiFi network.

Web GUI is build with GyverPortal and updates the clock controls:
- the timezone shift
- the NTP server name
- 12/24h display mode
- should it show the hours leading 0 or not
- brightness (random blinks of individual digits to dim them)
- sets the time (manually) 

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/config.png?raw=true)

When connected to WiFi it updates the time using NTP every 3000 seconds.

D3231 TwoWire interface is used for RTC.

74hc595 shift registers are programmed via 3 pins (nothing fancy there).

References to software components:
- WiFi Manager https://github.com/tzapu/WiFiManager
- DS3231 https://github.com/NorthernWidget/DS3231
- GyverPortal https://github.com/GyverLibs/GyverPortal
