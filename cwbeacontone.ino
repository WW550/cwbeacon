
// Simple CW Beacon by N4EAC May 3 2025
// Outputs Morse code by toggling a digital pin (e.g., D2)

const int keyPin = 2;       // Digital pin used for keying
const int wpm = 15;         // Words per minute
const char* message = "VVV DE N4EAC/B FM18FW "; // Beacon message

// Calculate dot duration in milliseconds
const int dotDuration = 1200 / wpm;
const int longToneDuration = 4000; // Duration of the final long tone in milliseconds (4000 ms = 4 seconds)

void setup() {
  pinMode(keyPin, OUTPUT);
  digitalWrite(keyPin, LOW);
}

void loop() {
  sendMessage(message);
  delay(5000); // Wait 5 seconds before repeating the message
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

  // Final long tone
  delay(dotDuration * 7); // Short pause before tail
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
