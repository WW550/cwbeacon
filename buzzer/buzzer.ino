// CW traning with buzzer or speaker or both and push button trigger by N4EAC
// Feel free to copy - modify

const int buttonPin = 2;     // Push button input
const int tonePin = 9;       // Transistor driving speaker
const int buzzerPin = 8;     // Buzzer output
const int ledPin = 13;       // Onboard LED

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Button active LOW
  pinMode(tonePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  bool buttonPressed = digitalRead(buttonPin) == LOW;

  if (buttonPressed) {
    digitalWrite(buzzerPin, HIGH);     // Activate buzzer
    digitalWrite(ledPin, HIGH);        // LED ON
    tone(tonePin, 710);                // Louder tone through transistor
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    noTone(tonePin);
  }

  delay(10); // Simple debounce
}
