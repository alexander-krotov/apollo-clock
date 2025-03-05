// Apollo DS-2300 clock with ESP32 C3 super mini
//
// https://github.com/alexander-krotov/apollo-clock
//
// Copyright (c) 2024 Alexander Krotov.
//
// NTP related code is derived from TimeNTP sample.

#include <Arduino.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <DS3231.h>
#include <GyverPortal.h>

// PCB version:
// 1 - "reverse" pcb: indicator tubes are on the opposite side of the control components
// 0 - "straight" pcb: indicator tubes are on the same side as the control components
#define REVERSE 0

// 74HC595 control pins
#define SER_PIN 4
#define RCK_PIN 3
#define SCK_PIN 2
#define ONE_PIN 1

// DS3231 pins and speeed
#define SDA_PIN 8
#define SCL_PIN 9
#define WIRE_SPEED 100000

// Maximum brightness level
const int MAX_BRIGHTNESS=16;

// Display blink time in milliseconds.
const int BLINK_TIME=50;

// NTP update interval in seconds
const int NTP_UPDATE_INTERVAL=3000;

// Our fake TZ
const struct timezone tz = {0, 0};

// Clock global configuration.
char ntpServerName[80] = "fi.pool.ntp.org";
signed char clock_tz = 2; // Timezone shift (could be negative)
unsigned char clock_brightness = MAX_BRIGHTNESS; // Clock brightness (1-MAX_BRIGHTNESS).
unsigned char clock_12 = 0;  // If non-zero clock is 12h, otherwise 24h
unsigned char clock_leading_0;  // Show hour leading 0
unsigned char clock_bar_mode = 0;   // bar mode
unsigned char clock_use_ntp = true;  // Use NTP switch
unsigned char clock_use_rtc = true;  // Use RTC switch

// Clock eeprom data address.
const int eeprom_addr=12;

// If we do not have WiFi we wait 60 seconds in the configuration portal.
const int WIFI_MANAGER_TIMEOUT=60;

// Real time clock
DS3231 myRTC;

// Web interface
GyverPortal ui;

// Initialize the network.
bool initialize_network()
{
  WiFiManager wm;
  // wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the name "NixieClock".
  wm.setConfigPortalTimeout(WIFI_MANAGER_TIMEOUT);
  bool res = wm.autoConnect("NixieClock");
  wm.stopWebPortal();

  return res;
}

// Set clock time to H:M:S
void set_clock_time(unsigned int h, unsigned int m, unsigned int s)
{
  log_printf("set time: %02u:%02u:%02u\n", h, m, s);

  // Check time sanity. Uninitialized RTC might give strange values.
  if (h<24 && m<60 && s<60) {
    struct timeval tv = {0};
    tv.tv_sec = h*60*60+m*60+s;
    // Set current time
    settimeofday(&tv, &tz);
  }
}

// Get time from RTC and set it as clock time.
void get_time_from_rtc()
{
  bool h12Flag;
  bool pmFlag;
  unsigned int h = myRTC.getHour(h12Flag, pmFlag);
  unsigned int m = myRTC.getMinute();
  unsigned int s = myRTC.getSecond();

  log_printf("set time from RTC: %02u:%02u:%02u\n", h, m, s);

  // Check time for sanity.
  if (h<24 && m<60 && s<60) {
    set_clock_time(h, m, s);
  }
}

// the setup function runs once when you press reset or power the board
void setup()
{
  Wire.begin();
  Wire.setClock(WIRE_SPEED);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(SER_PIN, OUTPUT);
  pinMode(RCK_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(ONE_PIN, OUTPUT);

  EEPROM.begin(100);

  read_eeprom_data();

  log_printf("\ndoing setup: clock_tz=%d clock_brightness=%d\n", clock_tz, clock_brightness);

  get_time_from_rtc();

  if (initialize_network()) {
    IPAddress myIP = WiFi.localIP();
    String ip_addr_str = myIP.toString();
    log_printf("AP IP address: %s\n", ip_addr_str.c_str());
    run_string_on_display(ip_addr_str.c_str());

    // start server portal
    ui.attachBuild(build);
    ui.attach(action);
    ui.start();
    log_printf("setup ui started\n");

    if (clock_use_ntp) {
      getNtpTime();
    }
  }
  log_printf("setup done\n");
}

// Read the config data from EEPROM.
void read_eeprom_data()
{
  // Read the EEPROM settings.
  clock_tz = (signed char)EEPROM.read(eeprom_addr);
  clock_brightness = EEPROM.read(eeprom_addr+1);
  clock_12 = EEPROM.read(eeprom_addr+2);
  clock_leading_0 = EEPROM.read(eeprom_addr+3);
  clock_bar_mode = EEPROM.read(eeprom_addr+4);
  clock_use_ntp = EEPROM.read(eeprom_addr+5);
  clock_use_rtc = EEPROM.read(eeprom_addr+6);
  EEPROM.readString(eeprom_addr+7, ntpServerName, sizeof(ntpServerName)-1);
  EEPROM.commit();
}

// Write the config data to EEPROM.
void write_eeprom_data()
{
  EEPROM.write(eeprom_addr, clock_tz);
  EEPROM.write(eeprom_addr+1, clock_brightness);
  EEPROM.write(eeprom_addr+2, clock_12);
  EEPROM.write(eeprom_addr+3, clock_leading_0);
  EEPROM.write(eeprom_addr+4, clock_bar_mode);
  EEPROM.write(eeprom_addr+5, clock_use_ntp);
  EEPROM.write(eeprom_addr+6, clock_use_rtc);
  EEPROM.writeString(eeprom_addr+7, ntpServerName);
  EEPROM.commit();
}

// DA-2300 segment control bits.
//
//   64           4
// 2   128   128     2
//   32           8
// 4   16     64    16
//   8           32
#if REVERSE
static const int digits[] = {
  0b01111011, // 0
  0b00001001, // 1
  0b00110111, // 2
  0b00011111, // 3
  0b01001101, // 4
  0b01011110, // 5
  0b01111110, // 6
  0b00001011, // 7
  0b01111111, // 8
  0b01001111  // 9
};
#else
static const int digits[] = {
  0b11011110, // 0
  0b10010000, // 1
  0b11101100, // 2
  0b11111000, // 3
  0b10110010, // 4
  0b01111010, // 5
  0b00111110, // 6
  0b11010000, // 7
  0b11111110, // 8
  0b11110010  // 9
};
#endif

// Display is 6 decimal digits.
// If digit value is not 0-9 display blank value
void show_disply(int *display, int *dots)
{
  int i, j;

  digitalWrite(RCK_PIN, LOW);
  for (int i=0; i<6; i++) {
#if REVERSE
    int d = display[5-i];
#else
    int d = display[i];
#endif
    if (d>=0 && d<=9) {
      // Translate digit to the segment bits
      d = digits[d];
    } else {
      // Blank display
      d = 0;
    }

    // Set the bar bit.
#if REVERSE
    if (dots[5-i]) {
      d |= 128;
    }
#else
    if (dots[i]) {
      d |= 1;
    }
#endif


    for (j=0; j<8; j++) {
      digitalWrite(SCK_PIN, LOW);
      digitalWrite(ONE_PIN, LOW);
      digitalWrite(SER_PIN, d&(1<<j) ? HIGH: LOW);
      digitalWrite(SCK_PIN, HIGH);
      digitalWrite(ONE_PIN, HIGH);
    }
  }
  digitalWrite(RCK_PIN, HIGH);
}

// the loop function runs over and over again forever
void loop()
{
  struct timeval tv;
  struct timezone tz = {0};
  // Read the current time
  gettimeofday(&tv, &tz);
  struct tm *tm = localtime(&tv.tv_sec);

  {
    static time_t s;
    if (s!=tv.tv_sec) {
      s = tv.tv_sec;
      if (s%100 == 0) {
        log_printf("LOOP %d:%02d:%02d\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
      }

      if (clock_use_ntp && s%NTP_UPDATE_INTERVAL==0) {
        getNtpTime();
      }
    }
  }

  int clock_display[6];

  // By default show current time
  int h = tm->tm_hour;
  if (clock_12 && h > 12) {
    h -= 12;
  }
  if (clock_leading_0 || h>=10) {
    clock_display[0] = h/10;
  } else {
    // Do not display anything instead of leading 0
    clock_display[0] = ' ';
  }
  clock_display[1] = h%10;
  int m = tm->tm_min;
  clock_display[2] = m/10;
  clock_display[3] = m%10;
  int s = tm->tm_sec;
  clock_display[4] = s/10;
  clock_display[5] = s%10;

  // Display separator bars
  int clock_dots[6] = {0, 0, 0, 0, 0, 0};

  switch (clock_bar_mode) {
    case 0:
      // No dots to show
      break;
    case 1:
      clock_dots[2] = tv.tv_usec >= 500000;
      clock_dots[4] = !clock_dots[2];
      break;
    case 2:
      clock_dots[2] = tv.tv_usec >= 500000;
      clock_dots[4] = clock_dots[2];
      break;
    case 3:
      clock_dots[2] = clock_dots[4] = 1;
      break;
  }

  show_disply(clock_display, clock_dots);

  delay(BLINK_TIME);

  if (clock_brightness < MAX_BRIGHTNESS) {
    unsigned int r = rand();
    // randomly select digits to blank.
    for (int i=0; i<6; i++) {
      if (r&(1<<i)) {
        clock_display[i] = ' ';
      }
    }
    r >>= 6;
    // Display dimmed screen for short time.
    show_disply(clock_display, clock_dots);
    delay( (r%(MAX_BRIGHTNESS-clock_brightness))*BLINK_TIME/MAX_BRIGHTNESS );
  }

  // Web UI tick.
  ui.tick();
}

// Display string as running display string
void run_string_on_display(const char *str)
{
    int display[6];
    int clock_dots[6] = {0};
    int len = strlen(str);

    log_printf("run_string_on_display: str=%s len=%d\n", str, len);

    for (int s=-6; s<=len; s++) {
      for (int i=0; i<6; i++) {
        if (i+s>=0 && i+s<len && str[i+s]>='0' && str[i+s]<='9') {
          // character fitst to the display and it is betweern 0 and 9.
          display[i] = str[i+s]-'0';
        } else {
          display[i] = ' ';
        }
      }

      show_disply(display, clock_dots);
      delay(500);
    }
}

// Following code is derived from TimeNTP sample
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  WiFi.hostByName(ntpServerName, ntpServerIP);
  log_printf("Transmit NTP Request: %s\n", ntpServerName);
  // get a random server from the pool
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      time_t secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      secsSince1900 = secsSince1900 - 2208988800UL + clock_tz * SECS_PER_HOUR;

      log_printf("Receive NTP Response %lu\n", (unsigned long)secsSince1900);

      tm *ttm = localtime(&secsSince1900);
      myRTC.setSecond(ttm->tm_sec);
      myRTC.setMinute(ttm->tm_min);
      myRTC.setHour(ttm->tm_hour);

      set_clock_time(ttm->tm_hour, ttm->tm_min, ttm->tm_sec);

      return secsSince1900;
    }
  }
  log_printf("No NTP Response :-(\n");
  return 0;
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

// Create a configuration form
void build()
{
  log_printf("BUILD\n");

  GP.BUILD_BEGIN();
  GP.PAGE_TITLE("Nixie clock");

  GP.THEME(GP_DARK);
  GP.FORM_BEGIN("/update");

  GP_MAKE_BLOCK_TAB(
    "Clock config",
    GP_MAKE_BOX(GP.LABEL("TimeZone shift:"); GP.NUMBER("clock_tz", "", clock_tz););
    GP_MAKE_BOX(GP.LABEL("Brightness:"); GP.SLIDER("clock_brightness", clock_brightness, 1, MAX_BRIGHTNESS););
    GP_MAKE_BOX(GP.LABEL("12/24 mode"); GP.SWITCH("clock_12", clock_12 ? true: false););
    GP_MAKE_BOX(GP.LABEL("Leading 0"); GP.SWITCH("clock_leading_0", clock_leading_0 ? true: false, 0););
    GP_MAKE_BOX(GP.LABEL("Bar mode"); GP.SELECT("clock_bar_mode", "Always off, Async, Sync, Always on", clock_bar_mode););
    GP_MAKE_BOX(GP.LABEL("Use NTP"); GP.SWITCH("clock_use_ntp", clock_use_ntp ? true: false););
    GP_MAKE_BOX(GP.LABEL("Use RTC"); GP.SWITCH("clock_use_rtc", clock_use_rtc ? true: false, 0););
    GP_MAKE_BOX(GP.LABEL("NTP Server name: "); GP.TEXT("clock_ntp_server", "local NTP server if you have", ntpServerName, "", sizeof(ntpServerName)-1););
  );
  GP.SUBMIT("UPDATE");

  GP.FORM_END();

  GP.FORM_BEGIN("/settime");

  time_t t = time(NULL);
  tm *ttm = localtime(&t);
  myRTC.setSecond(ttm->tm_sec);
  myRTC.setMinute(ttm->tm_min);
  myRTC.setHour(ttm->tm_hour);

  GPtime gptime (ttm->tm_hour, ttm->tm_min, ttm->tm_sec);

  GP_MAKE_BLOCK_TAB(
    "Time",
    GP_MAKE_BOX(GP.LABEL("Time :"); GP.TIME("time", gptime););
  );
  GP.SUBMIT("SET TIME");

  GP.FORM_END();

  GP.BUILD_END();
}

void action(GyverPortal& p)
{
  log_printf("ACTION\n");

  if (p.form("/update")) {
    int n;
    bool update_time = false;

    log_printf("ACTION update\n");

    // Reed the new values, and check them for sanity.
    n = ui.getInt("clock_tz");
    if (n>=-12 && n<=12) {
      if (n!=clock_tz) {
        update_time = true;
      }
      clock_tz = n;
    }

    n = ui.getInt("clock_brightness");
    if (n>=1 && n<=MAX_BRIGHTNESS) {
      clock_brightness = n;
    }

    n = ui.getBool("clock_12");
    if (n>=0 && n<=1) {
      clock_12 = n;
    }

    n = ui.getBool("clock_leading_0");
    if (n>=0 && n<=1) {
      clock_leading_0 = n;
    }

    n = ui.getInt("clock_bar_mode");
    if (n>=0 && n<4) {
      clock_bar_mode = n;
    }

    n = ui.getBool("clock_use_ntp");
    if (n>=0 && n<=1) {
      clock_use_ntp = n;
    }

    n = ui.getBool("clock_use_rtc");
    if (n>=0 && n<=1) {
      clock_use_ntp = n;
    }

    String s = ui.getString("clock_ntp_server");
    if (s && strcmp(s.c_str(), ntpServerName) != 0) {
      strncpy(ntpServerName, s.c_str(), sizeof(ntpServerName)-1);
    }

    write_eeprom_data();

    if (update_time) {
      // Timezone changed, we shoule change the system time.
      if (clock_use_ntp) {
        getNtpTime();
      } else if (clock_use_rtc) {
        get_time_from_rtc();
      }
    }
  }

  if (p.form("/settime")) {
    GPtime gptime = ui.getTime("time");
    log_printf("Action Settime: %d:%02d:%02d\n", gptime.hour, gptime.minute, gptime.second);

    // Set time to RPC
    myRTC.setSecond(gptime.hour);
    myRTC.setMinute(gptime.minute);
    myRTC.setHour(gptime.second);

    set_clock_time(gptime.hour, gptime.minute, gptime.second);
  }
}
