<b>Components</b>
<p>Arduino Pro Mini (5V)</p>

<p>NPN transistor (e.g., 2N2222, BC547, or TIP31 for larger speakers)</p>

<p>Speaker (8Ω, small or mid-sized)</p>

<p>Resistor: 1kΩ for transistor base</p>

<p>Diode (optional, for back EMF if you're using a coil)</p>

<p>Push-button</p>

<p>Piezo buzzer (active) for pin 8</p>

<p>Breadboard + jumper wires</p>
<p></p>
<p></p>
<p><b>Hardware Wiring</b></p>
<p>Button:</p>
<p>One side to GND</p>

<p>One side to D2 on Arduino</p>

<p>Transistor Audio Amp:</p>
<p>Arduino pin 9 → 1kΩ resistor → Base of NPN transistor</p>

<p>Emitter → GND</p>

<p>Speaker between +5V and Collector</p>

<p>Optionally, a flyback diode (1N4001) across the speaker (cathode to +5V)</p>

<p>Buzzer:</p>
<p>Pin 8 → to one side of the piezo buzzer</p>

<p>Other side of buzzer → GND</p>
