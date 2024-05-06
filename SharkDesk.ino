#include <Keyboard.h>

// Pin assignments
const int pins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int numPins = sizeof(pins) / sizeof(pins[0]);
const char keysNormal[] = {'q', 'w', 'e', 'r', 'a', 's', 'd', 'f'};
const char keysMacro[] = {'a', 'c', 'x', 'v', 'z', 'y', 'w', 'n'};
bool keyStates[numPins] = {false};  // Tracks whether each key is pressed
bool isMarco;

void setup() {
  Keyboard.begin();
  for (int i = 0; i < numPins; i++) {
    pinMode(pins[i], INPUT_PULLUP);
  }
}

void loop() {
  checkMarcoToggle(); // Check toggle between modes

  for (int i = 0; i < numPins; i++) {
    handleKeyPress(pins[i], keysNormal[i], KEY_LEFT_CTRL, keysMacro[i], i);
  }
}

void handleKeyPress(int pin, char normalKey, int modifierKey, char macroKey, int index) {
  bool currentState = digitalRead(pin) == LOW; // true if pressed
  if (currentState != keyStates[index]) {
    if (currentState) {
      if (!isMarco) {
        Keyboard.press(normalKey);
      } else {
        Keyboard.press(modifierKey);
        Keyboard.press(macroKey);
        Keyboard.release(modifierKey); // Release modifier if no longer needed
      }
    } else {
      Keyboard.release(normalKey);
      if (isMarco) {
        Keyboard.release(macroKey);
      }
    }
    keyStates[index] = currentState;
  }
}

void checkMarcoToggle() {
  // Toggle macro mode if both pinA (pin 2) and pinH (pin 9) are held down for 3 seconds
  if (digitalRead(pins[0]) == LOW && digitalRead(pins[7]) == LOW) {
    unsigned long startTime = millis();
    while (digitalRead(pins[0]) == LOW && digitalRead(pins[7]) == LOW) {
      if (millis() - startTime > 3000) {
        isMarco = !isMarco; // Toggle macro mode
        delay(500); // Debounce delay to avoid rapid toggling
        break;
      }
    }
  }
}
