// CW beacon by N4EAC for Arduino Nano
// Use for PTT and audio line. D2 = audio and D3 = key

// Pin configuration
const int tonePin = 2;  // D2 for CW tone (720 Hz)
const int pttPin  = 3;  // D3 for PTT keying

// Morse timing configuration
const int wpm = 15;
const int toneFreq = 720;
const int toneDuration = 1200 / wpm; // Proper dot duration (ms)

// Morse message
const char* message = "VVV DE N4EAC/B FM18FW";

// Morse code table (ITU standard A-Z, 0-9)
const char* morseTable[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",       // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",     // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",            // S-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." // 0-9
};

void setup() {
  pinMode(tonePin, OUTPUT);
  pinMode(pttPin, OUTPUT);
  digitalWrite(pttPin, LOW);
}

void loop() {
  // Key radio
  digitalWrite(pttPin, HIGH);

  // Send CW message
  sendMorse(message);

  // Send 5-second carrier tone
  tone(tonePin, toneFreq);
  delay(5000);
  noTone(tonePin);

  // Unkey radio
  digitalWrite(pttPin, LOW);

  // Wait 10 seconds before repeating
  delay(10000);
}

const char* getMorse(char c) {
  if (c >= 'A' && c <= 'Z') return morseTable[c - 'A'];
  if (c >= '0' && c <= '9') return morseTable[c - '0' + 26];
  return ""; // Unsupported characters
}

void sendMorse(const char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) {
    char c = toupper(msg[i]);

    if (c == ' ') {
      delay(toneDuration * 7); // Inter-word gap
      continue;
    }

    const char* morse = getMorse(c);

    for (int j = 0; morse[j] != '\0'; j++) {
      tone(tonePin, toneFreq);

      if (morse[j] == '.') {
        delay(toneDuration); // Dot
      } else {
        delay(toneDuration * 3); // Dash
      }

      noTone(tonePin);
      delay(toneDuration); // Inter-element gap
    }

    delay(toneDuration * 2); // Inter-letter gap (total = 3 units)
  }
}
