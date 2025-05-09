// CW beacon by N4EAC for Arduino Nano
// Use for PTT and audio line
// CW Beacon Extended Morse Code Support

const int ledPin = 13;    // LED pin (built-in LED)
const int pttPin = 12;    // Push-To-Talk control pin
const int tonePin = 9;    // Tone output pin

const int wpm = 20;       // Words per minute
const int dotLength = 1200 / wpm;             // Duration of a dot in milliseconds
const int dashLength = dotLength * 3;         // Duration of a dash
const int intraCharGap = dotLength;           // Gap between dots and dashes within a character
const int interCharGap = dotLength * 3;       // Gap between characters
const int wordGap = dotLength * 7;            // Gap between words

const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?=,/. ";
const char* morse[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",     // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",   // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",          // S-Z
  "-----", ".----", "..---", "...--", "....-", ".....",              // 0-5
  "-....", "--...", "---..", "----.",                                // 6-9
  "..--..", "-...-", "--..--", "-..-.",                              // ? = , /
  ""                                                                 // Space (handled as word gap)
};

const char message[] = "CQ CQ DE TEST/VA?=,.";  // Message to transmit

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pttPin, OUTPUT);
  pinMode(tonePin, OUTPUT);
  digitalWrite(pttPin, LOW);
  digitalWrite(ledPin, LOW);
}

void loop() {
  transmitMessage(message);
  delay(10000);  // Wait 10 seconds before repeating the message
}

void transmitMessage(const char* msg) {
  digitalWrite(pttPin, HIGH);  // Activate PTT
  delay(10);                   // Short delay to stabilize

  for (int i = 0; msg[i] != '\0'; i++) {
    char c = toupper(msg[i]);
    const char* code = getMorseCode(c);

    if (code != NULL) {
      if (c == ' ') {
        delay(wordGap);  // Word gap
      } else {
        transmitCharacter(code);
        delay(interCharGap);  // Gap between characters
      }
    }
  }

  digitalWrite(pttPin, LOW);  // Deactivate PTT
}

const char* getMorseCode(char c) {
  for (unsigned int i = 0; i < sizeof(characters) - 1; i++) {
    if (characters[i] == c) {
      return morse[i];
    }
  }
  return NULL;  // Character not found
}

void transmitCharacter(const char* code) {
  for (int i = 0; code[i] != '\0'; i++) {
    if (code[i] == '.') {
      sendDot();
    } else if (code[i] == '-') {
      sendDash();
    }
    delay(intraCharGap);  // Gap between symbols within a character
  }
}

void sendDot() {
  tone(tonePin, 800);           // Generate tone at 800 Hz
  digitalWrite(ledPin, HIGH);   // Turn on LED
  delay(dotLength);             // Duration of dot
  noTone(tonePin);              // Stop tone
  digitalWrite(ledPin, LOW);    // Turn off LED
}

void sendDash() {
  tone(tonePin, 800);           // Generate tone at 800 Hz
  digitalWrite(ledPin, HIGH);   // Turn on LED
  delay(dashLength);            // Duration of dash
  noTone(tonePin);              // Stop tone
  digitalWrite(ledPin, LOW);    // Turn off LED
}
