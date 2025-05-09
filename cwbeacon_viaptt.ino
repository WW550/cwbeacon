// CW beacon by N4EAC for Arduino Nano
// Use for PTT and audio line - sends 6 sec tone at the end and unkey radio for 10 seconds

// Pin configuration
const int tonePin = 2;     // D2 for CW tone (720 Hz)
const int pttPin  = 3;     // D3 for PTT keying
const int cwLedPin = 5;    // D5 for CW flashing LED
const int pttLedPin = 4;   // D4 for PTT status LED

// Morse timing configuration
const int wpm = 15;
const int toneFreq = 720;
const int toneDuration = 1200 / wpm; // Dot duration in milliseconds

// Morse message
const char* message = "VVV DE N4EAC/B FM18FW";

// Morse code table (A-Z and 0-9)
const char* morseTable[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",       // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",     // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",            // S-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." // 0-9
};

void setup() {
  pinMode(tonePin, OUTPUT);
  pinMode(pttPin, OUTPUT);
  pinMode(cwLedPin, OUTPUT);
  pinMode(pttLedPin, OUTPUT);
  digitalWrite(pttPin, LOW);
  digitalWrite(pttLedPin, LOW);
}

void loop() {
  // Key the radio
  digitalWrite(pttPin, HIGH);
  digitalWrite(pttLedPin, HIGH);
  delay(1000); // this allows to key the radio 1 second before sending the code

  // Send CW Morse message
  sendMorse(message);

  // Send a 6-second tone
  delay(2000);
  tone(tonePin, toneFreq);
  digitalWrite(cwLedPin, HIGH);
  delay(6000);
  noTone(tonePin);
  digitalWrite(cwLedPin, LOW);

  // Unkey the radio
  delay(1000); // hold key for 1 second before unkey
  digitalWrite(pttPin, LOW);
  digitalWrite(pttLedPin, LOW);
  

  // Wait 10 seconds before next cycle
  delay(10000);
}

// Get Morse code for a character
const char* getMorse(char c) {
  if (c >= 'A' && c <= 'Z') return morseTable[c - 'A'];
  if (c >= '0' && c <= '9') return morseTable[c - '0' + 26];
  if (c == '?') return "..--..";
  if (c == '=') return "-...-";
  if (c == ',') return "--..--";
  if (c == '/') return "-..-.";
  return ""; // Ignore unsupported characters
}

// Send Morse code as tone and LED blinks
void sendMorse(const char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) {
    char c = toupper(msg[i]);

    if (c == ' ') {
      delay(toneDuration * 7); // Inter-word space
      continue;
    }

    const char* morse = getMorse(c);

    for (int j = 0; morse[j] != '\0'; j++) {
      tone(tonePin, toneFreq);
      digitalWrite(cwLedPin, HIGH);

      if (morse[j] == '.') {
        delay(toneDuration); // Dot
      } else {
        delay(toneDuration * 3); // Dash
      }

      noTone(tonePin);
      digitalWrite(cwLedPin, LOW);
      delay(toneDuration); // Intra-character gap
    }

    delay(toneDuration * 2); // Inter-character gap (total = 3 units)
  }
}
