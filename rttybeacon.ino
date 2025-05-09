// Experimental RTTY beacon by N4EAC

#include <Arduino.h>

// Pin configuration
const int tonePin     = 2; // D2: FSK audio output
const int pttPin      = 3; // D3: PTT keying
const int rttyLedPin  = 5; // D5: Bit indicator LED
const int pttLedPin   = 4; // D4: PTT status LED

// RTTY FSK tone frequencies (standard 170 Hz shift)
const int markFreq  = 2125; // Logic 1
const int spaceFreq = 2295; // Logic 0

// RTTY timing parameters
const float baudRate   = 45.45;
const int bitDuration  = 1000 / baudRate; // ~22 ms per bit

// Message to send
const char* rttyMessage = "VVV DE N4EAC/B FM18FW";

// Simplified Baudot encoding (A-Z, /)
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
    case '/': return 0b11111; // Placeholder for unsupported punctuation
    default: return 0b00000;  // Null for unsupported
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
  // 1 second before transmission — PTT ON
  digitalWrite(pttPin, HIGH);
  digitalWrite(pttLedPin, HIGH);
  delay(1000);

  // Send RTTY message
  for (int i = 0; rttyMessage[i] != '\0'; i++) {
    char c = rttyMessage[i];
    if (c == ' ') {
      delay(bitDuration * 7); // Inter-word space
    } else {
      sendBaudotChar(c);
    }
  }

  // Stop tone generation
  noTone(tonePin);

  // Hold PTT for 1 more second after transmission
  delay(1000);
  digitalWrite(pttPin, LOW);
  digitalWrite(pttLedPin, LOW);

  // Wait 10 seconds before repeating
  delay(10000);
}

void sendBaudotChar(char c) {
  uint8_t code = getBaudot(c);

  // Start bit (0)
  sendRTTYBit(0);

  // Send 5 data bits (LSB first)
  for (int i = 0; i < 5; i++) {
    sendRTTYBit(code & 0x01);
    code >>= 1;
  }

  // Stop bits: 1.5 bits
  sendRTTYBit(1);
  delay(bitDuration / 2);
}

void sendRTTYBit(bool bit) {
  tone(tonePin, bit ? markFreq : spaceFreq);
  digitalWrite(rttyLedPin, bit ? HIGH : LOW);
  delay(bitDuration);
  digitalWrite(rttyLedPin, LOW);
}
