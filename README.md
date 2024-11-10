# Digital clock with Apollo DA-2300 indicators

# Hardware

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

# Software

Work in progress.

The firmware is built with Arduino IDE 2.

NetworManager is used to connect to WiFi network and update the clock coontrols:
- the timezoe shift
- the NTP server name
- 12/24h display mode
- should it show the hours leading 0 or not
- brightnes (random blinks of individual digits to dim them)

When connected to WiFi it updates the time using NTP every 3000 seocnds.

D3231 TwoWire interface is used for RTC.

74hc595 shift registers are programmed via 3 pins (nothing fancy there).

Current status:
- display works
- NTP works
- storing the configuration in CMOS - does not work
- DS3231 interface - does not work
