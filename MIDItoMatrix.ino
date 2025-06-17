// Descrição:
// Este é o sketch principal que simula os toques no teclado físico
// com base nas mensagens MIDI recebidas.
// Deve ser usado após mapear as teclas com MapearNotas.ino.
// Inclui um modo de teste com botão físico para simular algumas notas.
//
// Autor: Guilherme + ChatGPT
// Data: Junho de 2025

// 1. Includes
#include <MIDI.h>

// 2. Constants and Pins

// rowPins = pinos de seleção (onde você escreve LOW para ativar uma linha)
// columnPins = pinos de leitura (INPUT_PULLUP), lidos para detectar passagem de corrente
const uint8_t rowPins[6]    = {A1, A0, 5, 4, 3}; // ajuste conforme seu circuito
const uint8_t columnPins[7] = {13, 12, 11, 10, 9, 8, 7, 6}; // ajuste conforme seu circuito

const uint8_t testSwitchPin = A2; // botão ligado ao GND, para entrar em modo de testes

MIDI_CREATE_DEFAULT_INSTANCE();

// 3. Mapeamento das Teclas
// Mapeado com base no JSON gerado por MapearNotas.ino
const uint8_t NUM_KEYS = 37;
const uint8_t OFFSET = 36; // Nota MIDI mais grave do seu teclado (ex: C2)

struct KeyMapping {
  uint8_t note;
  uint8_t row;
  uint8_t col;
};

// Substitua esse bloco com o JSON convertido para C++
// Exemplo gerado anteriormente:
KeyMapping noteMap[NUM_KEYS] = {
  {36, 0, 5},
  {37, 0, 4},
  {38, 0, 3},
  {39, 0, 2},
  {40, 0, 1},
  {41, 0, 0},
  {42, 1, 6},
  {43, 1, 5},
  {44, 1, 4},
  {45, 1, 3},
  {46, 1, 2},
  {47, 1, 1},
  {48, 1, 0},
  {49, 2, 6},
  {50, 2, 5},
  {51, 2, 4},
  {52, 2, 3},
  {53, 2, 2},
  {54, 2, 1},
  {55, 2, 0},
  {56, 3, 6},
  {57, 3, 5},
  {58, 3, 4},
  {59, 3, 3},
  {60, 3, 2},
  {61, 3, 1},
  {62, 3, 0},
  {63, 4, 6},
  {64, 4, 5},
  {65, 4, 4},
  {66, 4, 3},
  {67, 4, 2},
  {68, 4, 1},
  {69, 4, 0},
  {70, 5, 6},
  {71, 5, 5},
  {72, 5, 4},
  {73, 5, 3}
};

struct VirtualKey {
  bool active;
  uint8_t row;
  uint8_t column;
};

VirtualKey activeKeys[NUM_KEYS]; // mapeamento 1:1

// Função auxiliar: dado um número MIDI, retorna linha e coluna da matriz
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

// MIDI Handlers
void handleNoteOn(byte channel, byte pitch, byte velocity) {
  uint8_t row, col;
  if (getMatrixPosition(pitch, row, col)) {
    activeKeys[pitch - OFFSET] = {true, row, col};  // offset de base 36
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  uint8_t row, col;
  if (getMatrixPosition(pitch, row, col)) {
    activeKeys[pitch - OFFSET].active = false;
  }
}

// Modo de teste: simula duas notas
void simulateTestNotes() {
  static unsigned long lastToggle = 0;
  static bool toggle = false;
  unsigned long now = millis();

  if (now - lastToggle > 1000) {
    lastToggle = now;
    toggle = !toggle;

    digitalWrite(toggle ? HIGH : LOW);

    if (toggle) {
      handleNoteOn(5, 40, 100); // canal MIDI 5 (ajuste conforme necessário)
    } else {
      handleNoteOff(5, 40, 0); // canal MIDI 5 (ajuste conforme necessário)
    }
  }
}

void setup() {
// Configura todos os pinos inicialmente como INPUT (evita comportamento indesejado)
  for (uint8_t i = 0; i < 7; i++) pinMode(columnPins[i], INPUT);
  for (uint8_t i = 0; i < 6; i++) pinMode(rowPins[i], INPUT);

  pinMode(testSwitchPin, INPUT_PULLUP);

  MIDI.begin(5); // canal MIDI 5 (ajuste conforme necessário)
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  Serial.begin(31250); // MIDI baud
}

void loop() {
  if (digitalRead(testSwitchPin) == LOW) {
    simulateTestNotes(); // modo de teste ativado
    return;
  }
  
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
