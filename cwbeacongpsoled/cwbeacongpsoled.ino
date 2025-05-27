// CW Beacon with GPS Time Restriction, OLED Display, GPS Lock Indicator, and Debug Output – With OLED Init Fix

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int keyPin = 2;        // CW keying pin
const int gpsRxPin = 3;      // GPS module TX → Arduino D3
const int gpsTxPin = 4;      // Not used, but required for SoftwareSerial

const int wpm = 15;
const char* message = "VVV DE N4EAC/B FM18FW ";
const int dotDuration = 1200 / wpm;
const int longToneDuration = 6000;

SoftwareSerial gpsSerial(gpsRxPin, gpsTxPin);
TinyGPSPlus gps;

unsigned long lastBlinkTime = 0;
bool blinkState = false;

void setup() {
  pinMode(keyPin, OUTPUT);
  digitalWrite(keyPin, LOW);
  gpsSerial.begin(9600); // comment out for memory test if needed
  Serial.begin(9600);

  delay(1000); // Give OLED time to power up
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found. Check wiring or I2C address."));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("CW BEACON");
  display.display();
  delay(2000);
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("CW BEACON DE N4EAC");

  if (gps.time.isValid() && gps.date.isValid()) {
    int hour = gps.time.hour();
    int minute = gps.time.minute();
    int second = gps.time.second();

    int day = gps.date.day();
    int month = gps.date.month();
    int year = gps.date.year();

    int dow = calculateDayOfWeek(year, month, day);
    const char* dowStr[] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

    // DEBUG OUTPUT
    Serial.print("Time valid: ");
    Serial.print(gps.time.isValid());
    Serial.print(" | Date valid: ");
    Serial.print(gps.date.isValid());
    Serial.print(" | UTC Hour: ");
    Serial.print(hour);
    Serial.print(" | DOW: ");
    Serial.print(dow);
    Serial.println();

    display.setCursor(0, 10);
    display.println(dowStr[dow]);

    display.setCursor(0, 23);
    display.print("UTC ");
    if (hour < 10) display.print("0");
    display.print(hour);
    display.print(":");
    if (minute < 10) display.print("0");
    display.print(minute);
    display.print(":");
    if (second < 10) display.print("0");
    display.print(second);

    // Blink GPS lock indicator
    if (millis() - lastBlinkTime > 500) {
      blinkState = !blinkState;
      lastBlinkTime = millis();
    }
    if (blinkState) {
      display.fillRect(120, 0, 6, 6, SSD1306_WHITE);
    } else {
      display.fillRect(120, 0, 6, 6, SSD1306_BLACK);
    }

    display.display();

    // ✅ Only send CW if all conditions met
    if (dow >= 2 && dow <= 6 && hour >= 10 && hour < 22) {
      Serial.println(">>> Sending CW <<<");
      sendMessage(message);
      delay(5000);
    }
  } else {
    Serial.println("GPS time or date not valid.");
    display.setCursor(0, 16);
    display.println("Waiting for GPS...");
    display.display();
  }
}

int calculateDayOfWeek(int year, int month, int day) {
  if (month < 3) {
    month += 12;
    year -= 1;
  }
  int k = year % 100;
  int j = year / 100;
  int h = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
  return ((h + 6) % 7);
}

void sendMessage(const char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) {
    char c = toupper(msg[i]);
    if (c == ' ') {
      delay(dotDuration * 7);
    } else {
      sendChar(c);
      delay(dotDuration * 3);
    }
  }
  delay(dotDuration * 7);
  keyDown(longToneDuration);
}

void sendChar(char c) {
  const char* code = getMorseCode(c);
  if (code == NULL) return;

  for (int i = 0; code[i] != '\0'; i++) {
    if (code[i] == '.') {
      keyDown(dotDuration);
    } else if (code[i] == '-') {
      keyDown(dotDuration * 3);
    }
    delay(dotDuration);
  }
}

void keyDown(int duration) {
  digitalWrite(keyPin, HIGH);
  delay(duration);
  digitalWrite(keyPin, LOW);
}

const char* getMorseCode(char c) {
  switch (c) {
    case 'A': return ".-";
    case 'B': return "-...";
    case 'C': return "-.-.";
    case 'D': return "-..";
    case 'E': return ".";
    case 'F': return "..-.";
    case 'G': return "--.";
    case 'H': return "....";
    case 'I': return "..";
    case 'J': return ".---";
    case 'K': return "-.-";
    case 'L': return ".-..";
    case 'M': return "--";
    case 'N': return "-.";
    case 'O': return "---";
    case 'P': return ".--.";
    case 'Q': return "--.-";
    case 'R': return ".-.";
    case 'S': return "...";
    case 'T': return "-";
    case 'U': return "..-";
    case 'V': return "...-";
    case 'W': return ".--";
    case 'X': return "-..-";
    case 'Y': return "-.--";
    case 'Z': return "--..";
    case '1': return ".----";
    case '2': return "..---";
    case '3': return "...--";
    case '4': return "....-";
    case '5': return ".....";
    case '6': return "-....";
    case '7': return "--...";
    case '8': return "---..";
    case '9': return "----.";
    case '0': return "-----";
    case '?': return "..--..";
    case '=': return "-...-";
    case ',': return "--..--";
    case '/': return "-..-.";
    default: return NULL;
  }
}
