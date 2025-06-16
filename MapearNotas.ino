const int matrixPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, A0, A1};
const int numPins = sizeof(matrixPins) / sizeof(matrixPins[0]);

const int totalKeys = 37;
int currentNote = 36;

struct Key {
  int pinLow;
  int pinRead;
  int midiNote;
};

Key recordedKeys[totalKeys];
int keyCount = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Pressione cada tecla em ordem (grave → agudo)...");
}

void loop() {
  if (keyCount >= totalKeys) {
    Serial.println("\nMapeamento concluído. JSON gerado:\n[");
    for (int i = 0; i < totalKeys; i++) {
      Serial.print("  { \"pinLow\": ");
      Serial.print(recordedKeys[i].pinLow);
      Serial.print(", \"pinRead\": ");
      Serial.print(recordedKeys[i].pinRead);
      Serial.print(", \"note\": ");
      Serial.print(recordedKeys[i].midiNote);
      Serial.println(i < totalKeys - 1 ? " }," : " }");
    }
    Serial.println("]");
    while (true); // trava
  }

  Serial.print("\nAguardando tecla ");
  Serial.print(keyCount);
  Serial.print(" (nota ");
  Serial.print(currentNote);
  Serial.println(")...");
  bool keyDetected = false;

  while (!keyDetected) {
    for (int i = 0; i < numPins; i++) {
      pinMode(matrixPins[i], OUTPUT);
      digitalWrite(matrixPins[i], LOW);

      for (int j = 0; j < numPins; j++) {
        if (i == j) continue;

        pinMode(matrixPins[j], INPUT_PULLUP);

        if (digitalRead(matrixPins[j]) == LOW) {
          // encontrou combinação de pinos
          recordedKeys[keyCount] = {matrixPins[i], matrixPins[j], currentNote};
          Serial.print("Tecla detectada entre pinos ");
          Serial.print(matrixPins[i]);
          Serial.print(" e ");
          Serial.println(matrixPins[j]);
          keyCount++;
          currentNote++;
          keyDetected = true;
          delay(500); // debounce
          break;
        }

        pinMode(matrixPins[j], INPUT); // limpa
      }

      pinMode(matrixPins[i], INPUT);
      if (keyDetected) break;
    }
  }
}
