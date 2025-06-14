# midi-to-matrix
Using Arduino to Inject MIDI into older keyboards

Aprender
this one should be loaded first, it will check each pin and tell you how many transitions (low to high and high to low) that pin had during 100ms.
depending on that number, the pin is probably a row or a column.

MapearNotas
this one should bem loaded second, with the keyboard off. Arduino will control the matrix and will ask you to press each key from left to right.
at the end, it will generate a json with midi numbers and row and column pairs.

MIDItoMatrix
the final one, the one that should do the trick.
