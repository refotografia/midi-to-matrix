# Using Arduino to add a MIDI IN port to older musical keyboards.

Mostly done with help from chatGPT, remember ChatGPT can make mistakes. Check important info.


# 🔧 MapearNotas.ino

Purpose: This sketch should be uploaded first, with the keyboard disconnected from the main circuit.

Description:

    The Arduino will directly control the matrix.

    A prompt on the Serial Monitor will ask you to press each key from left to right.

    Each key press is recorded, capturing the pin pair involved.

    At the end, a JSON list is printed with MIDI note numbers and pin mappings.

    This will also reveal which pins are used as selectPins (usually rows) and dataPins (columns).

    It uses the following detection method:
    digitalWrite(LOW) on one pin and digitalRead() with INPUT_PULLUP on the others.
    (This works if the keyboard uses pull-up logic.)

    If no notes are detected, you may try the reverse approach:
    write HIGH and read with INPUT.

# 🎹 MIDItoMatrix.ino

Purpose: This is the main sketch that simulates key presses based on incoming MIDI notes.

Description:

    Should be uploaded after running MapearNotas.ino.

    You must update:

        The rowPins[] and columnPins[] arrays with your actual pin mappings.

        The noteMap[] array with the JSON output from MapearNotas.ino.

    This sketch listens for MIDI messages and activates the corresponding matrix position for each note.

    A test mode is supported: connect a button to a chosen pin to simulate some notes locally.
    This helps verify that the circuit is responding correctly, even without MIDI input.
