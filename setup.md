# Setup instructions

This clock works best when connected to WiFi. Here you can find a detailed description of how to connect it to the home WiFi network.

## First setup

When turned on for the first time the clock displays "888888" and starts its own wifi access point called "NixieClock". This AP is visible for 1 minute, and you can connect to
it from computer or phone, and then access the clock setup with url http://192.168.4.1 (note, http:, not https:).
I recommend using a computer to connect to the clock, my Adndoid phone with Firefox refuses to work.

![WiFi Captive Portal Homepage](http://i.imgur.com/YPvW9eql.png)

You should see the WiFi Manager front page in the browser. Next click "Configure WiFi", you will see something similar to:

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/wifiselect.png?raw=true)

There you can choose your home network (click on its name), or enter the network (ssid) name mainully (in case it is not configured visible).
You also need to enter the network password.

Once you click "Save" the clock will try to connect to the WiFi network. When connected the clock will run its assigned IP address (something like "192 168 74 182") on the display,
and after that tries to get the time from the Internet, and finally starts to work as a clock (show the time). It might take a couple of seconds to get the time.

If 1 minute was not enough to complete the setup you can power off the clock and start from scratch.

Should you need more details about clock initial WiFi setup, you can refer it to WiFi Manager documentation: https://github.com/tzapu/WiFiManager

## Clock is configured

Next time you power on the clock it will try to connect to the known WiFi network automatically. If it fails to connect (network is not available) it starts the same
configuration AP NixieClock and will wait for the new network setup.

If the previously configured network is available, the clock will again run its assigned IP address on the display (something like "192 168 74 182") and then show the current time.

## Clock setting update

Clock has several configurable settings. You can access the clock config page using a url like http://192.168.74.182 (note http: not https:). The actual IP address might be different  
in your network, you should use the one the clock did show when powered on (you may want to write it down). Alternatively if your WiFi router shows the device names and their I
addresses, you can find it there: the clock device name should be something like "esp32c3-357010".

Clock setup page looks like:

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/config.png?raw=true)

Here you can update
- the timezone: 2 for Helsinki, 0 for London, Winter time. You might want to change it for summer time shift
- display brightness: 16 is maximal brightness. On smaller brightness levels the tubes are nor gradually dimmed, they start to blink instead
- 12/24 mode - preferred time format. the clock does not have AM/PM indication
- leading 0: preferred time format 02:13:14/2:13:14
- bar mode: separators bars mode. Bar could be set alway on, always off, blinking synchronously or blinking asynchronously
- use NTP: clock will try to get the time from the Internet (using NTP protocol)
- use RTC: clock has onboard Real-Time clock, that keeps the time in case WiFi is not available (backup coin battery is needed to keep the time when it is not powered from USB)
- NTP server name to synchronize time from. fi.pool.ntp.org and uk.pool.ntp.org are ok, or you can set any other NTP server name if you know what it is.

Click "Update" to update the settings. Settings are stored to clock memory and survive over powering off.

On the same page you can set the time manually. Note, if "use NTP" is on the clock will reset the time to the time it has read from NTP soon.



