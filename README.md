Here is a key Morse code by toggling a digital pin (e.g., D2) to ground, you can use the following approach.

This method involves controlling a digital output pin to simulate Morse code by turning it on and off according to the desired message. This pin can be connected to an LED for visual indication or to the base of an NPN transistor (with an appropriate resistor) to key a transmitter.

Hardware Setup:

Visual Indicator: Connect an LED with a suitable current-limiting resistor between keyPin (D2) and ground. This will flash the LED in Morse code.

Transmitter Keying: To key a transmitter, connect keyPin to the base of an NPN transistor through a resistor (e.g., 1kΩ). Connect the collector to the transmitter's keying line and the emitter to ground. This setup allows the Arduino to safely control the transmitter's keying circuit.

Notes:

The dotDuration is calculated based on the desired words per minute (WPM). Adjust the wpm variable to change the speed.

The getMorseCode function provides Morse code representations for alphanumeric characters. You can expand this function to include additional symbols as needed.

Ensure that the keyPin is capable of sourcing enough current for your specific application. When controlling external devices, always consider using appropriate interfacing components like transistors or opto-isolators to protect the Arduino.

This approach provides a straightforward method to create a Morse code beacon.
