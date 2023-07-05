# Set the time on PCF8523 RTC connected via I2C

Serial console will print current date and time in a loop.

To set the time, send the command `set YYYY-MM-DD HH:MI:SS`
with the date and time you desire in ISO standard format. It will not be read until
a NEWLINE character is received (when you press _enter_ / _return_ ).

for example:
`set 2022-06-29 02:46:00`

If using the Arduino serial monitor, turn on timestamps to
compare your computer's clock with the board's RTC. If you
don't get it right the first time, keep on sending 'set'
commands until you are happy. :-)

Tested on
[Adafruit Feather RP2040](https://www.adafruit.com/product/4884)
and
[Adalogger FeatherWing - RTC + SD](https://www.adafruit.com/product/2922).

## Libraries required:

- [RTClib by Adafruit](https://github.com/adafruit/RTClib)


