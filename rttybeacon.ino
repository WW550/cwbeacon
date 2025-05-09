#include <Arduino.h>

// Pin configuration
const int tonePin     = 2; // D2: FSK tone & melody
const int pttPin      = 3; // D3: PTT keying
const int rttyLedPin  = 5; // D5: Bit indicator LED
const int pttLedPin   = 4; // D4: PTT status LED

// RTTY FSK tones
const int markFreq  = 2125;
const int spaceFreq = 2295;

// RTTY timing
const float baudRate   = 45.45;
const int bitDuration  = 1000 / baudRate; // ~22 ms per bit

// Octave-up melody frequencies
const int melodyNotes[] = {523, 659, 784, 1047}; // C5, E5, G5, C6
const int noteDuration = 100;
const int noteGap = 30;

const char* rttyMessage = "VVV DE N4EAC/B FM18FW";

uint8_t getBaudot(char c) {
  c = toupper(c);
  switch (c) {
    case 'A': return 0b00011; case 'B': return 0b11010;
    case 'C': return 0b01100; case 'D': return 0b00010;
    case 'E': return 0b00101; case 'F': return 0b01111;
    case 'G': return 0b11000; case 'H': return 0b10100;
    case 'I': return 0b00111; case 'J': return 0b01011;
    case 'K': return 0b10010; case 'L': return 0b01110;
    case 'M': return 0b11011; case 'N': return 0b01101;
    case 'O': return 0b10011; case 'P': return 0b11101;
    case 'Q': return 0b10101; case 'R': return 0b01010;
    case 'S': return 0b11111; case 'T': return 0b10000;
    case 'U': return 0b00110; case 'V': return 0b11110;
    case 'W': return 0b10110; case 'X': return 0b11100;
    case 'Y': return 0b10111; case 'Z': return 0b10001;
    case '/': return 0b11111;
    default:  return 0b00000;
  }
}

void setup() {
  pinMode(tonePin, OUTPUT);
  pinMode(pttPin, OUTPUT);
  pinMode(rttyLedPin, OUTPUT);
  pinMode(pttLedPin, OUTPUT);
  digitalWrite(pttPin, LOW);
  digitalWrite(pttLedPin, LOW);
}

void loop() {
  // 1 second before TX
  digitalWrite(pttPin, HIGH);
  digitalWrite(pttLedPin, HIGH);
  delay(1000);

  // Send RTTY message
  for (int i = 0; rttyMessage[i] != '\0'; i++) {
    char c = rttyMessage[i];
    if (c == ' ') {
      delay(bitDuration * 7);
    } else {
      sendBaudotChar(c);
    }
  }

  // Stop RTTY tone
  noTone(tonePin);

  // Play melody (still keyed)
  delay(3000);
  playMelody();

  // 1s delay after melody then unkey
  delay(1000);
  digitalWrite(pttPin, LOW);
  digitalWrite(pttLedPin, LOW);

  // Wait before next cycle
  delay(10000);
}

void sendBaudotChar(char c) {
  uint8_t code = getBaudot(c);
  sendRTTYBit(0); // Start bit
  for (int i = 0; i < 5; i++) {
    sendRTTYBit(code & 0x01);
    code >>= 1;
  }
  sendRTTYBit(1); // Stop bit
  delay(bitDuration / 2);
}

void sendRTTYBit(bool bit) {
  tone(tonePin, bit ? markFreq : spaceFreq);
  digitalWrite(rttyLedPin, bit ? HIGH : LOW);
  delay(bitDuration);
  digitalWrite(rttyLedPin, LOW);
}

void playMelody() {
  for (int i = 0; i < 4; i++) {
    tone(tonePin, melodyNotes[i]);
    delay(noteDuration);
    noTone(tonePin);
    delay(noteGap);
  }
}
