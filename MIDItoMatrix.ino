// 1. Includes
#include <MIDI.h>

// 2. Constants and Pin Definitions
// Define which Arduino pins go to which matrix lines
const uint8_t rowPins[6]    = {2, 3, 4, 5, 6, 7};
const uint8_t columnPins[7] = {8, 9, 10, 11, 12, A0, A1};

// 3. MIDI Setup
MIDI_CREATE_DEFAULT_INSTANCE();

// 3. Key Mapping (imported manually from JSON)
const uint8_t NUM_KEYS = 37;

struct KeyMapping {
  uint8_t note;
  uint8_t row;
  uint8_t col;
};

KeyMapping noteMap[NUM_KEYS] = {
  {36, 0, 0}, {37, 0, 1}, {38, 0, 2}, // <- exemplo
  // TODO: completar com seu JSON mapeado
  {73, 5, 6}
};


// 4. Structs and Globals
struct VirtualKey {
  bool active;
  uint8_t row;
  uint8_t column;
};

VirtualKey activeKeys[NUM_KEYS]; // mapeamento 1:1

// 5. Helper Functions
// Map MIDI note number to matrix row/column
bool getMatrixPosition(uint8_t note, uint8_t &row, uint8_t &col) {
  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    if (noteMap[i].note == note) {
      row = noteMap[i].row;
      col = noteMap[i].col;
      return true;
    }
  }
  return false;
}

// 6. MIDI Handlers
void handleNoteOn(byte channel, byte pitch, byte velocity) {
  uint8_t row, col;
  if (getMatrixPosition(pitch, row, col)) {
    activeKeys[pitch - 36] = {true, row, col};  // offset de base 36
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  uint8_t row, col;
  if (getMatrixPosition(pitch, row, col)) {
    activeKeys[pitch - 36].active = false;
  }
}

// 7. Setup
void setup() {
  for (uint8_t i = 0; i < 7; i++) pinMode(columnPins[i], INPUT);
  for (uint8_t i = 0; i < 6; i++) pinMode(rowPins[i], INPUT);

  MIDI.begin(5); // Chose channel 5 for this first keyboard "Antonelli 2381"
  // have to remember to change this to 6, 7, 8... as I install this on other keyboards
  // and make stickers!!!
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  Serial.begin(31250); // MIDI baud
}

// 8. Main Loop
void loop() {
  MIDI.read();

  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    if (!activeKeys[i].active) continue;

    uint8_t row = activeKeys[i].row;
    uint8_t col = activeKeys[i].column;

    if (digitalRead(rowPins[row]) == LOW) {
      pinMode(columnPins[col], OUTPUT);
      digitalWrite(columnPins[col], LOW);
      delayMicroseconds(30);
      pinMode(columnPins[col], INPUT);
    }
  }
}
