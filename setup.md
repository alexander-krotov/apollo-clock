# Setup instructions

This clock works best when connected to wifi. Here you can find detailed description how to connect it to home wifi network.

## First setup

When turned on for the first time the clock displays "888888" and starts its own wifi access point called "NixiClock". This AP is visibe for 1 minute, and you can connect to
it from computer or phone, and then access the clock setup with url http://192.168.4.1 (note, http:, not https:).
I recommed using computer to connect to the clock, my Adndoid phone with Firefox refuses to work.

You shold see WiFi Manager frot page in the browser. Next click "Configure Wifi", you will see something similar to:

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/wifiselect.png?raw=true)

There you can choose your home network (click on its name), or enter the network (ssid) name mainully (in case it is not configured visible).
You also need to enter the network password.

Once you click "Save" the clock will try to connect to the WiFi network. When connected it will run its assigned IP address (something like "192 168 74 182") on display,
and after that tries it get the time from the Internet, and starts to work as a clock (show the time). It might take couple of seconds to get the time.

If 1 mintute was not enough to complete setup you can powere off the clock and start from scratch.

Should you need more details about clock initial WiFi setup, you can reffer it to WiFi manager documnetation: 

## Clock is configured

Next time you power on the clock it will try to connect to the konwn WiFi network autocmatically. If it fails to connect (netowrk is not available) it starts the same
configuration AP NixieClock and will wait the new network setup.

If the previously configured network is availble clock will again run its assigned IP address on the disply (something like "192 168 74 182") and then show the current time.

## Clock setting update

Clock has several configurable settingd. You can access the clock config page using url like http://192.168.74.182 (note http: not https:). The actual IP address might be diffrent 
in your nework, you should use the one the clock did show when powered on (you may wat ot write it down). Alternatilvely if you wifi router show the device names and their I
addresses, you can find it there: the clock device name should be something like "esp32c3-357010".

Clock setup page looks like:

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/config.png?raw=true)

Here you can update
- the timezone: 2 for Helsinki, 0 for London, Winter time. You might want to change it for summer time shift
- display brightnes: 16 is maximal brightnes. On smaller brightness levels the tubes are nor gradually dimmed, they start to blink instead
- 12/24 mode - preffered time format. the clock does not have AM/PM indication
- leading 0: preffered time format 02:13:14/2:13:14
- bar mode: separators bars mode. Bar could be set alway on, always off, blinking synchronously or blinking asynchronously
- use NTP: clock will try to get the time from the Internet (using NTP protocol)
- use RTC: clock has onboard Real-Time clock, that keeps the time in case WiFi is not available (backup coin battery is needed to keep the time when it is not powered from USB)
- NTP server name to syncronize time from. fi.pool.ntp.org and uk.pool.ntp.org are ok, or you can set any other NTP server name if you know what it is.

Click "Update" to update the settings. Settings are stored to clock memory and survive over powering off.

On the same page you can set the time manually. Note, if "use NTP" is on the clock will reset the time to the time it read from NTP soon.



