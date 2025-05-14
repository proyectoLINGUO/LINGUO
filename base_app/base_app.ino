#include <ESPUI.h>

int X = 0;
int Y = 0;

// Callback para todos los botones de dirección
void move(Control *sender, int type) {
  if (type != B_DOWN) return; // Solo cuando se presiona

  String id = sender->label;

  if (id == "↑") {
    Y += 1;
  } else if (id == "↓") {
    Y -= 1;
  } else if (id == "←") {
    X -= 1;
  } else if (id == "→") {
    X += 1;
  } else if (id == "↖") {
    X -= 1; Y += 1;
  } else if (id == "↗") {
    X += 1; Y += 1;
  } else if (id == "↙") {
    X -= 1; Y -= 1;
  } else if (id == "↘") {
    X += 1; Y -= 1;
  }

  Serial.printf("X: %d, Y: %d\n", X, Y);
}

void setup() {
  Serial.begin(115200);
  ESPUI.begin("Joystick XY");

  // Fila 1
  ESPUI.beginRow();
  ESPUI.button("↖", &move, ControlColor::Turquoise);
  ESPUI.button("↑", &move, ControlColor::Turquoise);
  ESPUI.button("↗", &move, ControlColor::Turquoise);
  ESPUI.endRow();

  // Fila 2
  ESPUI.beginRow();
  ESPUI.button("←", &move, ControlColor::Peterriver);
  ESPUI.label("·"); // Botón central inactivo
  ESPUI.button("→", &move, ControlColor::Peterriver);
  ESPUI.endRow();

  // Fila 3
  ESPUI.beginRow();
  ESPUI.button("↙", &move, ControlColor::Emerald);
  ESPUI.button("↓", &move, ControlColor::Emerald);
  ESPUI.button("↘", &move, ControlColor::Emerald);
  ESPUI.endRow();
}

void loop() {
  // Aquí puedes utilizar X e Y como coordenadas de movimiento
}
