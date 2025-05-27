// CW Beacon with GPS Time Restriction (Weekdays only) by N4EAC – Modified May 2025

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const int keyPin = 2;        // CW keying pin
const int gpsRxPin = 3;      // GPS module TX → Arduino D3
const int gpsTxPin = 4;      // Not used, but required for SoftwareSerial

const int wpm = 15;
const char* message = "VVV DE N4EAC/B FM18FW "; // Message to transmit
const int dotDuration = 1200 / wpm;
const int longToneDuration = 6000; // milliseconds

SoftwareSerial gpsSerial(gpsRxPin, gpsTxPin);
TinyGPSPlus gps;

void setup() {
  pinMode(keyPin, OUTPUT);
  digitalWrite(keyPin, LOW);
  gpsSerial.begin(9600);
}

void loop() {
  // Parse GPS data
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // Proceed if GPS time and date are valid
  if (gps.time.isValid() && gps.date.isValid()) {
    int hour = gps.time.hour(); // UTC hour
    int day = gps.date.day();
    int month = gps.date.month();
    int year = gps.date.year();

    // Calculate day of week (0=Sat, 1=Sun, ..., 6=Fri)
    int dow = calculateDayOfWeek(year, month, day);

    // Allow beacon only Monday (2) to Friday (6), 10:00–21:59 UTC
    if (dow >= 2 && dow <= 6 && hour >= 10 && hour < 22) {
      sendMessage(message);
      delay(5000);
    }
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
  // Convert Zeller's result to: 0=Saturday, 1=Sunday, ..., 6=Friday
  return ((h + 6) % 7);
}

void sendMessage(const char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) {
    char c = toupper(msg[i]);
    if (c == ' ') {
      delay(dotDuration * 7); // Word gap
    } else {
      sendChar(c);
      delay(dotDuration * 3); // Letter gap
    }
  }

  delay(dotDuration * 7); // Pause before tail tone
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
    delay(dotDuration); // Inter-element gap
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
