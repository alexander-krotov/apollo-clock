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

// 74HC595 control pins
#define SER_PIN 4
#define RCK_PIN 3
#define SCK_PIN 2
#define ONE_PIN 1

// ESP32 onboard LED pin
#define LED_PIN 8

// Maximum brightnes level
const int MAX_BRIGHTNES=16;

// Display blink time in milliseconds.
const int BLINK_TIME=50;

// Clodk global configuration.
char ntpServerName[80] = "fi.pool.ntp.org";
signed char clock_tz = 2; // Timezone shift (could be negative)
unsigned char clock_brightness = MAX_BRIGHTNES; // Clock brightnes (1-MAX_BRIGHTNES).
unsigned char clock_12 = 0;  // If non-zero clock is 12h, otherwise 24h
unsigned char clock_leading_0;  // Show hour leading 0

// Clock eeprom data address.
const int eeprom_addr=12;

// Real time clock
TwoWire w(0);
DS3231 myRTC (w);

// Initialize the network, and update the global configuration if
// needed.
bool initialize_network()
{
  char timezone_shift[20];
  char brightness_str[20];
  char n12_str[3]="24";
  char leading0[3]="n";

  if (clock_12) {
    strcpy(n12_str, "12");
  }

  if (clock_leading_0) {
    strcpy(leading0, "y");
  }

  itoa(clock_brightness, brightness_str, 10);
  itoa(clock_tz, timezone_shift, 10);

  WiFiManager wm;
  wm.resetSettings();

  WiFiManagerParameter tz("timezone_shift", "Timezone shift", timezone_shift, 8);
  WiFiManagerParameter brightness("brightness", "Brightness", brightness_str, 8);
  WiFiManagerParameter ntp_server("ntp_server", "NTP Server", ntpServerName, sizeof(ntpServerName)-1);
  WiFiManagerParameter n12("n12", "12/24", n12_str, sizeof(n12_str)-1);
  WiFiManagerParameter leading_zero("leading0", "Leading zero (y/n)", leading0, sizeof(leading0)-1);

  wm.addParameter(&tz);
  wm.addParameter(&brightness);
  wm.addParameter(&ntp_server);
  wm.addParameter(&n12);
  wm.addParameter(&leading_zero);

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the name "NixieClock".
  bool res = wm.autoConnect("NixieClock"); // anonymous ap

  const char *str_val = tz.getValue();
  if (str_val) {
    int v = atoi(str_val);
    if (v>=-12 && v<=12) {
      if (clock_tz != v) {
        clock_tz = v;
        EEPROM.write(eeprom_addr, clock_tz);
        EEPROM.commit();
      }
    }
  }

  str_val = brightness.getValue();
  if (str_val) {
    int v = atoi(str_val);
    if (v>0 && v<=MAX_BRIGHTNES) {
      if (clock_brightness != v) {
        clock_brightness = v;
        EEPROM.write(eeprom_addr+1, clock_brightness);
        EEPROM.commit();
      }
    }
  }

  str_val = n12.getValue();
  if (str_val) {
    int v = atoi(str_val);
    if (v == 12) {
      clock_12 = true;
    } else {
      clock_12 = false;
    }
    EEPROM.write(eeprom_addr+2, clock_12);
    EEPROM.commit();
  }

  str_val = leading_zero.getValue();
  if (str_val) {
    if (strcmp(str_val, "y")==0) {
      clock_leading_0 = true;
    } else {
      clock_leading_0 = false;
    }
    EEPROM.write(eeprom_addr+3, clock_leading_0);
    EEPROM.commit();
  }

  str_val = ntp_server.getValue();
  if (str_val) {
    strncpy(ntpServerName, str_val, sizeof(ntpServerName)-1);
    EEPROM.writeString(eeprom_addr+4, ntpServerName);
    EEPROM.commit();
  }

  return res;
}

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(SER_PIN, OUTPUT);
  pinMode(RCK_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(ONE_PIN, OUTPUT);
  // pinMode(LED_PIN, OUTPUT);
  w.setPins(8, 9);
  // Read the EEPROM settings.
  clock_tz = (signed char)EEPROM.read(eeprom_addr);
  clock_brightness = EEPROM.read(eeprom_addr+1);
  clock_12 = EEPROM.read(eeprom_addr+2);
  clock_leading_0  = EEPROM.read(eeprom_addr+3);
  EEPROM.readString(eeprom_addr+4, ntpServerName, sizeof(ntpServerName)-1);

  if (initialize_network()) {
    setSyncProvider(getNtpTime);
    setSyncInterval(300);
  }
}

// DS-2300 segment control bits.
//
//   64
// 2   128
//   32
// 4   16
//   8
static const int digits[] = {
  2|4|8|16|64|128, // 0
  16|128,          // 1
  4|8|32|64|128,   // 2
  8|16|32|64|128,  // 3
  2|16|32|128,     // 4
  2|8|16|32|64,    // 5
  2|4|8|16|32,     // 6
  16|64|128,       // 7
  255<<1,          // 8
  2|16|32|64|128  // 9
};

// Display is 6 decimal digits.
// If digit value is not 0-9 display blank value
void show_disply(int *display)
{
  int i, j;

  digitalWrite(RCK_PIN, LOW);
  for (int i=0; i<6; i++) {
    int d = display[i];
    if (d>=0 && d<=9) {
      // Translate digit to the segmnet bits
      d = digits[d];
    } else {
      // Blank display
      d = 0;
    }

    // Set the bar bit.
    digitalWrite(SCK_PIN, LOW);
    digitalWrite(ONE_PIN, LOW);
    digitalWrite(SER_PIN, HIGH);
    digitalWrite(SCK_PIN, HIGH);
    digitalWrite(ONE_PIN, HIGH);

    for (j=1; j<8; j++) {
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
  static int cnt;
  cnt++;
  int clock_display[6];

#if 0
  digitalWrite(LED_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                   // wait
  digitalWrite(LED_PIN, LOW);   // turn the LED off by making the voltage LOW
  delay(500);                   // wait
#endif
	struct timeval tv;
	// Read the current time
	gettimeofday(&tv, NULL);
	struct tm *tm = localtime(&tv.tv_sec);

  // By default show current time, reading it from RTC
  bool h12Flag;
  bool pmFlag;
  int h = tm->tm_hour; // int h = myRTC.getHour(h12Flag, pmFlag);
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
  int m = tm->tm_min; // int m = myRTC.getMinute();
  clock_display[2] = m/10;
  clock_display[3] = m%10;
  int s = tm->tm_sec;  // int s = myRTC.getSecond();
  clock_display[4] = s/10;
  clock_display[5] = s%10;

  show_disply(clock_display);
  delay(BLINK_TIME);
  if (clock_brightness < MAX_BRIGHTNES) {
    unsigned int r = rand();
    // randomly select digits to blank.
    for (int i=0; i<6; i++) {
      if (r&(1<<i)) {
        clock_display[i] = ' ';
      }
    }
    r >>= 6;
    // Display dimed screen for short time.
    show_disply(clock_display);
    delay( (r%(MAX_BRIGHTNES-clock_brightness))*BLINK_TIME/MAX_BRIGHTNES );
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
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      time_t secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      secsSince1900 = secsSince1900 - 2208988800UL + clock_tz * SECS_PER_HOUR;
      tm *ttm = localtime(&secsSince1900);
      myRTC.setSecond(ttm->tm_sec);
      myRTC.setMinute(ttm->tm_min);
      myRTC.setHour(ttm->tm_hour);
      return secsSince1900;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
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
