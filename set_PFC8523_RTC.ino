// Set the time on PCF8523 RTC connected via I2C
//
// Serial console will print current date and time in a loop.
//
// To set the time, send the command "set YYYY-MM-DD HH:MI:SS"
// with the date and time you desire. It will not be read until
// a '\n' character (when you press <enter> / <return> ).
//
// for example:
// "set 2022-06-29 02:46:00"
//
// If using the Arduino serial monitor, turn on timestamp to
// compare your computer's clock with the board's RTC. If you
// don't get it right the first time, keep on sending 'set'
// commands until you are happy. :-)
//
// Author: Trey Kinkead
//
#include "RTClib.h"
#include <time.h>

RTC_PCF8523 rtc;

auto& console = Serial;
const auto baud = 115200; // ignored for USB serial

void setup() {
  console.begin(baud);

#ifndef ESP8266
  while (!console) {
    // wait for serial port to connect. Needed for native USB
    delay(10);
  }
#endif

  if (!rtc.begin()) {
    while (true) {
      console.println("Couldn't find RTC");
      console.flush();
      delay(1000);
    }
  }

  
  if (!rtc.initialized() || rtc.lostPower()) {
    console.println("RTC is NOT initialized");
  }

  // ensure the RTC is running
  rtc.start();

  // set serial timeout very small so the main
  // loop doesn't wait on it
  console.setTimeout(1);  //ms
}

void loop() {
  static time_t last_t = 0;
  char buffer[100];

  time_t now_t = rtc.now().unixtime();
  // if the time is different than the last time through the loop, print it
  if (now_t != last_t) {
    last_t = now_t;
    char timestampString[20];
    strftime(timestampString, sizeof(timestampString), "%Y-%m-%dT%H:%M:%S", gmtime(&now_t));
    console.println(timestampString);
  }

  // attempt to read a line from the serial console
  if (auto n = console.readBytesUntil('\n', buffer, sizeof(buffer) - 1)) {
    buffer[n] = 0;
    String inputLine = String(buffer);
    inputLine.trim();
    auto i = inputLine.indexOf(' ');  // find the first space
    if (i) {
      String command = inputLine.substring(0, i);
      String everythingElse = inputLine.substring(i);
      everythingElse.trim();
      if (command == "set") {
        // everthingElse should be a date string in form YYYY-MM-DD HH:MI:SS
        // unfortunately, passing a bogus timestamp in won't make
        // newDT invalid. So try to get it right...
        auto newDT = DateTime(everythingElse.c_str());
        rtc.adjust(newDT);
        console.println("set RTC");
      }
    }
  }
}
