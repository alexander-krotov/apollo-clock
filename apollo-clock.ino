// Apollo DS-2300 clock.
//
// Copyright (c) 2024 Alexander Krotov.

#include <Arduino.h>
#include <WiFiManager.h>
#include <EEPROM.h>

#define SER_PIN 4
#define RCK_PIN 3
#define SCK_PIN 2
#define ONE_PIN 1

#define LED_PIN 8

char ntpServerName[80] = "fi.pool.ntp.org";
signed char clock_tz = 2; /* Intentionally 1-byte integer. */
signed char clock_brightness = 16; /* Intentionally 1-byte integer. */

bool initialize_network()
{
  char timezone_shift[20];
  char brightness_str[20] = "16";
  char n12[3]="24";

  // Read the EEPROM settings. 
  clock_tz = (signed char)EEPROM.read(212);
  clock_brightness = EEPROM.read(213);
  EEPROM.readString(215, ntpServerName, sizeof(ntpServerName)-1);
  clock_tz = 2;
  clock_brightness = 16;

  itoa(clock_brightness, brightness_str, 10);
  itoa(clock_tz, timezone_shift, 10);

  WiFiManager wm;
  // wm.resetSettings();

  WiFiManagerParameter tz("timezone_shift", "Timezone shift", timezone_shift, 8);
  WiFiManagerParameter brightness("brightness", "Brightness", brightness_str, 8);
  WiFiManagerParameter ntp_server("ntp_server", "NTP Server", ntpServerName, sizeof(ntpServerName)-1);
  WiFiManagerParameter ntp_server("n12", "12/24", n12, sizeof(n12)-1);

  wm.addParameter(&tz);
  wm.addParameter(&brightness);
  wm.addParameter(&ntp_server);

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the name "NixieClock".
  bool res = wm.autoConnect("NixieClock"); // anonymous ap

 if (tz.getValue()) {
    int v = atoi(tz.getValue());
    if (v>=-12 && v<=12) {
      if (clock_tz != v) {
        clock_tz = v;
        EEPROM.write(212, clock_tz);
        EEPROM.commit();
      }
    }
  }

  if (brightness.getValue()) {
    int v = atoi(brightness.getValue());
    if (v<=255) {
      if (clock_brightness != v) {
        clock_brightness = v;
        EEPROM.write(213, clock_brightness);
        EEPROM.commit();
      }
    }
  }

  if (ntp_server.getValue()) {
    strncpy(ntpServerName, ntp_server.getValue(), sizeof(ntpServerName)-1);
    EEPROM.writeString(215, ntpServerName);
    EEPROM.commit();
  }

  return res;
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_PIN, OUTPUT);
  pinMode(SER_PIN, OUTPUT);
  pinMode(RCK_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(ONE_PIN, OUTPUT);

  // initialize_network();
}

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
  2|16|32|64|128   // 9
};

// Display is 6 decimal digits.
void show_disply(int *display)
{
  int i, j;

  digitalWrite(RCK_PIN, LOW); 
  for (int i=0; i<6; i++) {
    digitalWrite(SCK_PIN, LOW);
    digitalWrite(ONE_PIN, LOW);
    digitalWrite(SER_PIN, HIGH);
    digitalWrite(SCK_PIN, HIGH);
    digitalWrite(ONE_PIN, HIGH);
      
    for (j=1; j<8; j++) {
      digitalWrite(SCK_PIN, LOW);
      digitalWrite(ONE_PIN, LOW);
      digitalWrite(SER_PIN, digits[display[i]]&(1<<j) ? HIGH: LOW);
      digitalWrite(SCK_PIN, HIGH);
      digitalWrite(ONE_PIN, HIGH);
    }
  }
  digitalWrite(RCK_PIN, HIGH);
}

// the loop function runs over and over again forever
void loop() {
  static int cnt;
  cnt++;
  int clock_display[6];

  digitalWrite(LED_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                   // wait
  digitalWrite(LED_PIN, LOW);   // turn the LED off by making the voltage LOW
  delay(500);                   // wait

  for (int i=0; i<6; i++) {
    clock_display[i] = cnt%10;
  }
  show_disply(clock_display);
}
