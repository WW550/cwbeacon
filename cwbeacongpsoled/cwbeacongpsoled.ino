
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int keyPin = 2;
const int txLedPin = LED_BUILTIN;
const int gpsRxPin = 3;
const int gpsTxPin = 4;

const int wpm = 20;
const int dotDuration = 1200 / wpm;
const int longToneDuration = 6000;

const char* message = "VVV DE N4EAC/B FM18FW ";

SoftwareSerial gpsSerial(gpsRxPin, gpsTxPin);
TinyGPSPlus gps;

int calculateDayOfWeek(int year, int month, int day);

void setup() {
  pinMode(keyPin, OUTPUT);
  pinMode(txLedPin, OUTPUT);
  digitalWrite(keyPin, LOW);
  digitalWrite(txLedPin, LOW);
  gpsSerial.begin(9600);

  delay(100);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("CW BEACON");
  display.display();
  delay(2000);
}

void loop() {
  // STEP 1: WAIT FOR VALID GPS TIME
  while (!gps.time.isValid() || !gps.date.isValid()) {
    while (gpsSerial.available()) gps.encode(gpsSerial.read());
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("CW BEACON");
    display.setCursor(0, 12);
    display.println("Waiting for GPS...");
    display.display();
    delay(500);
  }

  // STEP 2: GET AND DISPLAY CURRENT TIME AND WEEKDAY
  // Re-read GPS bytes before display to ensure latest time
  for (unsigned long start = millis(); millis() - start < 1500;) {
    while (gpsSerial.available()) gps.encode(gpsSerial.read());
  }
  int hour = gps.time.hour();
  int minute = gps.time.minute();
  int second = gps.time.second();
  int year = gps.date.year();
  int month = gps.date.month();
  int day = gps.date.day();
  int dow = calculateDayOfWeek(year, month, day);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("CW BEACON");

  const char* dowStr[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  display.setCursor(0, 12);
  display.println(dowStr[dow]);

  display.setCursor(0, 25);
  display.print("UTC ");
  if (hour < 10) display.print("0");
  display.print(hour); display.print(":");
  if (minute < 10) display.print("0");
  display.print(minute); display.print(":");
  if (second < 10) display.print("0");
  display.print(second);

  display.display();
  delay(500); // Short delay to let user see update

  // STEP 3: SEND CW IF WITHIN ALLOWED TIME WINDOW (Mon–Fri, 10–22 UTC)
  if (dow >= 1 && dow <= 5 && hour >= 10 && hour < 22) {
    digitalWrite(txLedPin, HIGH);
    sendMessage(message);
    digitalWrite(txLedPin, LOW);
  }

  delay(2000); // Wait before repeating
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
  if (!code) return;

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

int calculateDayOfWeek(int year, int month, int day) {
  if (month < 3) { month += 12; year -= 1; }
  int k = year % 100;
  int j = year / 100;
  int h = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
  return (h + 6) % 7; // Sunday = 0, Monday = 1, ..., Saturday = 6
}

const char* getMorseCode(char c) {
  switch (c) {
    case 'A': return ".-";    case 'B': return "-...";  case 'C': return "-.-.";
    case 'D': return "-..";   case 'E': return ".";     case 'F': return "..-.";
    case 'G': return "--.";   case 'H': return "....";  case 'I': return "..";
    case 'J': return ".---";  case 'K': return "-.-";   case 'L': return ".-..";
    case 'M': return "--";    case 'N': return "-.";    case 'O': return "---";
    case 'P': return ".--.";  case 'Q': return "--.-";  case 'R': return ".-.";
    case 'S': return "...";   case 'T': return "-";     case 'U': return "..-";
    case 'V': return "...-";  case 'W': return ".--";   case 'X': return "-..-";
    case 'Y': return "-.--";  case 'Z': return "--..";
    case '1': return ".----"; case '2': return "..---"; case '3': return "...--";
    case '4': return "....-"; case '5': return "....."; case '6': return "-....";
    case '7': return "--..."; case '8': return "---.."; case '9': return "----.";
    case '0': return "-----"; case '?': return "..--..";case '=': return "-...-";
    case ',': return "--..--";case '/': return "-..-.";
    default: return nullptr;
  }
}
