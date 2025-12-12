#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Límites del servo
#define SERVOMIN 140
#define SERVOMAX 520

// Posiciones de ojos
int eyeX = 380;
int eyeY = 380;

int curX = 380;
int curY = 380;

int stepSpeed = 8;

// Pulsos finales
int xpulse, ypulse;

// Párpados
int upperLeft = 300;
int lowerLeft = 420;
int upperRight = 300;
int lowerRight = 420;

bool blinkNow = false;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_EYES");
  Wire.begin(21, 22);  // SDA, SCL
  
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(300);
}

int smoothMove(int current, int target, int step) {
  if (current < target) current += step;
  else if (current > target) current -= step;
  return current;
}

void blink() {
  // cerrar
  for (int p = 300; p <= 440; p += 8) {
    pwm.setPWM(2, 0, p);         // Upper Left
    pwm.setPWM(3, 0, 680 - p);   // Lower Left
    pwm.setPWM(4, 0, p);         // Upper Right
    pwm.setPWM(5, 0, 680 - p);   // Lower Right
    delay(10);
  }

  delay(120);

  // abrir
  for (int p = 440; p >= 300; p -= 8) {
    pwm.setPWM(2, 0, p);
    pwm.setPWM(3, 0, 680 - p);
    pwm.setPWM(4, 0, p);
    pwm.setPWM(5, 0, 680 - p);
    delay(10);
  }
}

void loop() {

  if (SerialBT.available()) {
    char cmd = SerialBT.read();

    if (cmd == 'F') eyeY = 300;
    if (cmd == 'B') eyeY = 440;
    if (cmd == 'L') eyeX = 300;
    if (cmd == 'R') eyeX = 440;
    if (cmd == 'S') { eyeX = 380; eyeY = 380; }
    if (cmd == 'Y') blinkNow = true;
  }

  // movimiento suave
  curX = smoothMove(curX, eyeX, stepSpeed);
  curY = smoothMove(curY, eyeY, stepSpeed);

  xpulse = curX;
  ypulse = curY;

  // mover ambos ojos usando un servo por eje
  pwm.setPWM(0, 0, xpulse);         // SERVO X
  pwm.setPWM(1, 0, ypulse);         // SERVO Y

  if (blinkNow) {
    blink();
    blinkNow = false;
  }

  delay(20);
}
