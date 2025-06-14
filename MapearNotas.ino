// com o circuito do teclado desligado, Arduino em controle da matriz

const int numRows = 6;
const int numCols = 7;
const int numKeys = 37;

int selectPins[numRows] = {2, 3, 4, 5, 6, 7};
int dataPins[numCols] = {8, 9, 10, 11, 12, A0, A1};

bool keyMap[numRows][numCols] = {false}; // matriz atual
bool lastKeyMap[numRows][numCols] = {false}; // matriz anterior

struct KeyPosition {
  int row;
  int col;
};

KeyPosition recordedKeys[numKeys];
int recordedCount = 0;

void setup() {
  Serial.begin(9600);

  // configura pinos
  for (int r = 0; r < numRows; r++) {
    pinMode(selectPins[r], OUTPUT);
    digitalWrite(selectPins[r], LOW);
  }

  for (int c = 0; c < numCols; c++) {
    pinMode(dataPins[c], INPUT_PULLUP); // pull-up para facilitar detecção
  }

  Serial.println("Pressione as 37 teclas do teclado em ordem (grave -> agudo)...");
}

void loop() {
  for (int r = 0; r < numRows; r++) {
    digitalWrite(selectPins[r], HIGH);
    delayMicroseconds(50); // pequeno delay para estabilidade

    for (int c = 0; c < numCols; c++) {
      bool pressed = digitalRead(dataPins[c]) == LOW;
      keyMap[r][c] = pressed;
    }

    digitalWrite(selectPins[r], LOW);
  }

  for (int r = 0; r < numRows; r++) {
    for (int c = 0; c < numCols; c++) {
      if (keyMap[r][c] && !lastKeyMap[r][c]) {
        // nova tecla detectada
        if (recordedCount < numKeys) {
          recordedKeys[recordedCount].row = r;
          recordedKeys[recordedCount].col = c;
          Serial.print("Tecla ");
          Serial.print(recordedCount);
          Serial.print(": linha ");
          Serial.print(r);
          Serial.print(", coluna ");
          Serial.println(c);
          recordedCount++;
        } else {
          Serial.println("Número máximo de teclas já registrado.");
        }
      }

      // atualiza estado anterior
      lastKeyMap[r][c] = keyMap[r][c];
    }
  }

  if (recordedCount >= numKeys) {
    Serial.println("\nMapeamento concluído. Resultado:");
    Serial.println("[");
    for (int i = 0; i < numKeys; i++) {
      Serial.print("  {\"row\": ");
      Serial.print(recordedKeys[i].row);
      Serial.print(", \"col\": ");
      Serial.print(recordedKeys[i].col);
      Serial.println(i < numKeys - 1 ? "}," : "}");
    }
    Serial.println("]");
    while (true); // trava o programa após finalizar
  }

  delay(10);
}
