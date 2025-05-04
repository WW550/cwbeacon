Designed with Arduino NANO as test platform.

This software will key Morse code (CW) by toggling a digital pin (D2) to ground. 
<p>Observe how your transceiver accepts key input. It will not generate tones.</p>
Use this code at your own risk.

This code involves controlling a digital output pin to simulate Morse code by turning it on and off according to the desired message. This pin can be connected to an LED for visual indication or to the base of an NPN transistor (with an appropriate resistor) to key a transmitter. 

<B>Hardware Setup:</B>

Visual Indicator: Connect an LED with a suitable current-limiting resistor between keyPin (D2) and ground. This will flash the LED in Morse code.

Transmitter Keying: To key a transmitter, connect keyPin to the base of an NPN transistor through a resistor (e.g., 1kΩ). Connect the collector to the transmitter's keying line and the emitter to ground. This setup allows the Arduino to safely control the transmitter's keying circuit.

<B>Notes:</B>

The dotDuration is calculated based on the desired words per minute (WPM). Adjust the wpm variable to change the speed.

The getMorseCode function provides Morse code representations for alphanumeric characters. You can expand this function to include additional symbols as needed.

Ensure that the keyPin is capable of sourcing enough current for your specific application. When controlling external devices, always consider using appropriate interfacing components like transistors or opto-isolators to protect the Arduino.

<B>Versions:</B>
<p>There are two (2) versions.</p>
<p>cwbeacon.ino = a simple CW beacon message without a tail tone</p>
<p>cwbeacontone.ino = a simple CW beacon message with a tail tone</p>

<p>The code is set with 15 wpm. Also note the beacon message has my callsign and my grid location. You should change that information before going on the air.</p>

<I>This approach provides a straightforward method to create a Morse code beacon.</I>

----------------------------------
<p>Notes for future - testing in progress</p>
<p>NPN transistor BF199 (manufactured by NXP):</p>
<p>D2 (Arduino) ─── 1kΩ ───> Base (BF199 pin 1)</p>
<p>Emitter (BF199 pin 2) ───> GND</p>
<p>Collector (BF199 pin 3) ───> TX key line</p>
