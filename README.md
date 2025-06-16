<p><h1>midi-to-matrix</h1></p>
<p>Using Arduino to Inject MIDI into older keyboards</p>
<p></p>
<p></p>
<p><h2>MapearNotas</h2></p>
<p>this one should be loaded first, with the keyboard disconnected from the main circuit. Arduino will control the matrix and will ask you to press each key from left to right.
at the end, it will generate a json with midi numbers and pins. it should be obvious which are selectPins and which are dataPins. it will work if the approach digitalWrite(LOW) on a pin and digitalRead() with INPUT_PULLUP on other pin is correct for this circuit. only after this one not working is that we should try to write HIGH and read as INPUT.</p>
<p></p>
<p><h2>MIDItoMatrix</h2></p>
<p>the one that should do the trick. need to update this one with selectPins and dataPins in their arrays and add a json with matched pairs. </p>

