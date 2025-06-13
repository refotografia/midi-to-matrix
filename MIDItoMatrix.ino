// 1. Includes
#include <MIDI.h>

// 2. Constants and Pin Definitions
// Define which Arduino pins go to which matrix lines
const uint8_t rowPins[6]    = {2, 3, 4, 5, 6, 7};
const uint8_t columnPins[7] = {8, 9, 10, 11, 12, A0, A1};

// 3. MIDI Setup
MIDI_CREATE_DEFAULT_INSTANCE();

// 4. Structs and Globals
struct VirtualKey {
  bool active;
  uint8_t row;
  uint8_t column;
};

VirtualKey activeKeys[42]; // 6 x 7

// 5. Helper Functions
// Map MIDI note number to matrix row/column
bool noteToMatrix(uint8_t note, uint8_t &row, uint8_t &col) {
  uint8_t n = note - 36; // adjust this to match your MIDI range
  if (n >= 42) return false;
  row = n / 7;
  col = n % 7;
  return true;
}

// 6. MIDI Handlers
void handleNoteOn(byte channel, byte pitch, byte velocity) {
  uint8_t row, col;
  if (noteToMatrix(pitch, row, col)) {
    uint8_t idx = row * 7 + col;
    activeKeys[idx] = {true, row, col};
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  uint8_t row, col;
  if (noteToMatrix(pitch, row, col)) {
    uint8_t idx = row * 7 + col;
    activeKeys[idx].active = false;
  }
}

// 7. Setup
void setup() {
  for (uint8_t i = 0; i < 7; i++) pinMode(columnPins[i], INPUT);
  for (uint8_t i = 0; i < 6; i++) pinMode(rowPins[i], INPUT);

  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  Serial.begin(31250); // MIDI baud rate
}

// 8. Main Loop
void loop() {
  MIDI.read();

  for (uint8_t i = 0; i < 42; i++) {
    if (!activeKeys[i].active) continue;

    uint8_t row = activeKeys[i].row;
    uint8_t col = activeKeys[i].column;

    if (digitalRead(rowPins[row]) == LOW) {
      pinMode(columnPins[col], OUTPUT);
      digitalWrite(columnPins[col], LOW);
      delayMicroseconds(100);
      pinMode(columnPins[col], INPUT);
    }
  }
}
