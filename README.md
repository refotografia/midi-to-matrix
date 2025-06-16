<p><h1>midi-to-matrix</h1></p>
<p>Using Arduino to Inject MIDI into older keyboards</p>
<p></p>
<p></p>
<p><h2>MapearNotas</h2></p>
<p>this one should be loaded first, with the keyboard disconnected from the main circuit. Arduino will control the matrix and will ask you to press each key from left to right.
at the end, it will generate a json with midi numbers and pins. it should be obvious which are selectPins and which are dataPins.</p>
<p></p>
<p><h2>MIDItoMatrix</h2></p>
<p>the final one, the one that should do the trick.</p>

