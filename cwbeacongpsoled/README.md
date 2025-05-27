CW beacon with a GT-U7 GPS module so one may run the beacon on a schedule
WE will use a GT-U7 module hooked to the Arduino Nano, and use it to get UTC time from the GPS signal. The GT-U7 outputs NMEA sentences over serial, from which you can extract date/time info. There will be some calculation to detect day of the week as this example schedules my CW beacon only Monday through Friday 10:00 UTC through 22:00 UTC.

Hardware connection: <p></p>
Connect GT-U7 GPS module<p></p>
    • GT-U7 TX → Arduino D3 (SoftwareSerial RX)<p></p>
    • GT-U7 GND → Arduino GND<p></p>
    • GT-U7 VCC → Arduino 3.3V or 5V (depending on your module)<p></p>
    • GT-U7 RX → leave unconnected (not needed)<p></p>
<p></p>Connect OLED 0.96 display<p></p>
OLED pin VCC  to Arduino Nano 3.3v or 5v<p></p>
OLED pin GND to Arduino Nano GND<p></p>
OLED pin SCL (or SCK)  to Arduino Nano A5<p></p>
OLED pin SDA to Arduino Nano A4<p></p>
<p></p>
## Dependencies

This project requires the following Arduino libraries:

For the GPS module:

- [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus) by Mikal Hart
- SoftwareSerial (included with the Arduino IDE)

To install:
1. Open the Arduino IDE
2. Go to **Sketch > Include Library > Manage Libraries**
3. Search for `TinyGPSPlus` and install it

For the 0.96 OLED:

Adafruit_SSD1306
Adafruit_GFX
Adafruit BusIO 
To install:
1. Open the Arduino IDE
2. Go to **Sketch > Include Library > Manage Libraries**
3. For Adafruit_SSD1306, search for `SSD1306` and install it (only / no dependencies)
4. For Adafruit_GFX, search for “GFX” and install it (this iis a dependency needed)
5. For Adafruit_BusIO, search for “BusIO” and install it (this iis a dependency needed)
