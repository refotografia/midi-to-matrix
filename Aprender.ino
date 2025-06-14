const int numPins = 13;
int pins[numPins] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, A0, A1};

unsigned long startTime = 0;
const unsigned long duration = 100; // duração da medição em milissegundos

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Contando transições de estado por pino...");

  for (int i = 0; i < numPins; i++) {
    pinMode(pins[i], INPUT);
  }

  for (int i = 0; i < numPins; i++) {
    int lastState = digitalRead(pins[i]);
    int transitions = 0;
    startTime = millis();

    while (millis() - startTime < duration) {
      int currentState = digitalRead(pins[i]);
      if (currentState != lastState) {
        transitions++;
        lastState = currentState;
      }
    }

    Serial.print("Pino ");
    Serial.print(pins[i]);
    Serial.print(" -> ");
    Serial.print(transitions);
    Serial.println(" transições");
  }

  Serial.println("Fim da leitura.");
}

void loop() {
  // Nada aqui
}
