#include <WiFi.h>
#include <ESPUI.h>

// 📶 CONFIGURA TU RED WiFi
const char* ssid = "________";        // nombre de la red
const char* password = "13579000";    // contraseña de la red

// 🧭 Coordenadas XY
int X = 0;
int Y = 0;
uint16_t labelXY;  // ID del label en la interfaz

// ⏱️ Control para impresión periódica
unsigned long previousMillis = 0;
const long interval = 1000; // 1 segundo

// 📦 Callback de botones
void move(Control *sender, int type) {
  if (type != B_DOWN) return;

  String id = sender->label;

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
  WiFi.begin(ssid, password); // Conexión WiFi

  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado ✅");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  // 🌐 Interfaz web ESPUI
  ESPUI.begin("Joystick XY");

  // Botones de dirección
  ESPUI.button("↖", &move, ControlColor::Wetaslphat);
  ESPUI.button("↑", &move, ControlColor::Wetaslphat);
  ESPUI.button("↗", &move, ControlColor::Wetaslphat);

  ESPUI.button("←", &move, ControlColor::Wetaslphat);
  labelXY = ESPUI.label("Posición", ControlColor::Turquoise, "X: 0 | Y: 0");
  ESPUI.button("→", &move, ControlColor::Wetaslphat);

  ESPUI.button("↙", &move, ControlColor::Wetaslphat);
  ESPUI.button("↓", &move, ControlColor::Wetaslphat);
  ESPUI.button("↘", &move, ControlColor::Wetaslphat);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("Coordenadas actuales -> ");
    Serial.print("X: ");
    Serial.print(X);
    Serial.print(" | Y: ");
    Serial.println(Y);
  }
}
