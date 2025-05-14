#include <WiFi.h>
#include <ESPUI.h>

// 📶 CONFIGURA TU RED WiFi 
const char* ssid = "________"        // nombre de la red
const char* password = "________";   // contraseña de la red

// 🧭 Coordenadas XY
int xval = 0;
int yval = 0;
Control *labelXY;  // Para mostrar X e Y en la interfaz

// 📦 Callback de botones
void move(Control *sender, int type) {
  if (type != B_DOWN) return;

  String id = sender->label;
// flechas de movivmiento
  if (id == "↑") Y += 1;
  else if (id == "↓") Y -= 1;
  else if (id == "←") X -= 1;
  else if (id == "→") X += 1;
  else if (id == "↖") { X -= 1; Y += 1; }
  else if (id == "↗") { X += 1; Y += 1; }
  else if (id == "↙") { X -= 1; Y -= 1; }
  else if (id == "↘") { X += 1; Y -= 1; }

  String pos = "X: " + String(X) + " | Y: " + String(Y);
  ESPUI.updateLabel(labelXY, pos);
  Serial.println(pos);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password); // red y contraseña

  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado ✅");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  // 🌐 Interfaz web
  ESPUI.begin("Joystick XY");

  // Fila 1: ↖ ↑ ↗
  ESPUI.beginRow();
  ESPUI.button("↖", &move, ControlColor::Wetaslphat);
  ESPUI.button("↑", &move, ControlColor::Wetaslphat);
  ESPUI.button("↗", &move, ControlColor::Wetaslphat);
  ESPUI.endRow();

  // Fila 2: ← Label →  
  ESPUI.beginRow();
  ESPUI.button("←", &move, ControlColor::Wetaslphat);
  labelXY = ESPUI.label("X: 0 | Y: 0");
  ESPUI.button("→", &move, ControlColor::Wetaslphat);
  ESPUI.endRow();

  // Fila 3: ↙ ↓ ↘
  ESPUI.beginRow();
  ESPUI.button("↙", &move, ControlColor::Wetaslphat);
  ESPUI.button("↓", &move, ControlColor::Wetaslphat);
  ESPUI.button("↘", &move, ControlColor::Wetaslphat);
  ESPUI.endRow();
}

void loop() {
  // Acá el codigo porque las variables xval e yval se controlan solas por el ESPUI
}
