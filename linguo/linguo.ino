#include <Wire.h>
#include <SparkFun_APDS9960.h>
 
#define APDS9960_INT    2 // Needs to be an interrupt pin
// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <WiFi.h>
#include <ESPUI.h>
const char* ssid = "________"        // nombre de la red
const char* password = "________";   // contraseña de la red

int X = 0;
int Y = 0;
Control *labelXY;  // Para mostrar X e Y en la interfaz
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  140 // Minimo del ancho de pulso (out of 4096)
#define SERVOMAX  520 // Maximo del ancho de pulso (out of 4096)
int lexpulse,rexpulse;
int leypulse,reypulse;
int uplidpulse,lolidpulse,altuplidpulse,altlolidpulse;
int trimval;
int sensorValue = 0;
int outputValue = 0;
bool switchval = LOW;

#define toggle _  ///////////////////////////////////////////////          pin para cambiar de modo

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void movimionto_ojos()
{
    lexpulse = map(X, 0,1023, 220, 440);
    rexpulse = lexpulse;
    
    leypulse = map(Y, 0,1023, 250, 500);
    reypulse = map(Y, 0,1023, 400, 280);

  trimval = analogRead(A2);
    trimval=map(trimval, 320, 580, -40, 40);
     uplidpulse = map(Y, 0, 1023, 400, 280);
        uplidpulse -= (trimval-40);
          uplidpulse = constrain(uplidpulse, 280, 400);
     altuplidpulse = 680-uplidpulse;

     lolidpulse = map(Y, 0, 1023, 410, 280);
       lolidpulse += (trimval/2);
         lolidpulse = constrain(lolidpulse, 280, 400);      
     altlolidpulse = 680-lolidpulse;
    
      pwm.setPWM(0, 0, lexpulse);
      pwm.setPWM(1, 0, leypulse);

      if (switchval == HIGH) {
      pwm.setPWM(2, 0, 400);
      pwm.setPWM(3, 0, 240);
      pwm.setPWM(4, 0, 240);
      pwm.setPWM(5, 0, 400);
      }
      else if (switchval == LOW) {
      pwm.setPWM(2, 0, uplidpulse);
      pwm.setPWM(3, 0, lolidpulse);
      pwm.setPWM(4, 0, altuplidpulse);
      pwm.setPWM(5, 0, altlolidpulse);
      }

  delay(5);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void move(Control *sender, int type) 
{
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
  else if (id == "⚫") { switchval  = !switchval }

  

  String pos = "X: " + String(X) + " | Y: " + String(Y);
  ESPUI.updateLabel(labelXY, pos);
  Serial.println(pos);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
 
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);
 
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("Iniciacion completa del APDS-9960"));
  } else {
    Serial.println(F("Algo fallo mientsas se iniciaba el APDS9960!")); 
  }
 
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("El sensor de gestos esta funcionando"));
  } else {
    Serial.println(F("Algo fallo al iniciar esl sensor de gestos!"));
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
  ESPUI.begin("Joystick XY epico para el linguo");

  // Fila 1: ↖ ↑ ↗
  ESPUI.beginRow();
  ESPUI.button("↖", &move, ControlColor::Wetaslphat);
  ESPUI.button("↑", &move, ControlColor::Wetaslphat);
  ESPUI.button("↗", &move, ControlColor::Wetaslphat);
  ESPUI.endRow();

  // Fila 2: ← Label →  
  ESPUI.beginRow();
  ESPUI.button("←", &move, ControlColor::Wetaslphat);
  ESPUI.button("⚫", &move, ControlColor::Dark);
  ESPUI.button("→", &move, ControlColor::Wetaslphat);
  ESPUI.endRow();

  // Fila 3: ↙ ↓ ↘
  ESPUI.beginRow();
  ESPUI.button("↙", &move, ControlColor::Wetaslphat);
  ESPUI.button("↓", &move, ControlColor::Wetaslphat);
  ESPUI.button("↘", &move, ControlColor::Wetaslphat);
  ESPUI.endRow(); 
  
  pinMode(toggle,INPUT); //                                         selector de modo LOW=Sensor de Gestos || HIGH=wifi
  pinMode(2, INPUT); // pin de interrupcion del APDS9960
  
  pwm.begin();
  pwm.setPWMFreq(60);  // Los servos se actualizan a 60Hz

  delay(10);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setServoPulse(uint8_t n, double pulse) 
{
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us por segundo
  pulselength /= 60;   // 60 Hz
  pulselength /= 4096;  // 12 bits de resolucion
  pulse *= 1000000;  // convierte en us
  pulse /= pulselength;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() 
{
  switch(case)
  {
    case (digitalRead(toggle)==HIGH):        ////////////////        CONTROL WIFI
    
      break;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
    case (digitalRead(toggle)==LOW):         ///////////////        SENSOR DE GESTOS
        if( isr_flag == 1 ) 
          {
            detachInterrupt(0);
            handleGesture();
            isr_flag = 0;
            attachInterrupt(0, interruptRoutine, FALLING);
          }
      break;
    default:
      break;
  }
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void interruptRoutine() 
{
  isr_flag = 1;
}
void handleGesture() 
{
    if ( apds.isGestureAvailable() ) 
    {
      switch ( apds.readGesture() )    //// mas adelante revisar si ponemos un LCD
     {
      case DIR_UP:
        Serial.println("UP");
        for(int z=0;z<5;z++)
          {
            Y += 1;
          }
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        for(int z=0;z<5;z++)
          {
            Y -= 1;
            delay(10);
          }
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        for(int z=0;z<5;z++)
          {
            X -= 1;
            delay(10);
          }
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        for(int z=0;z<5;z++)
          {
            X += 1;
            delay(10);
          }
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        switchval=HIGH;
        break;
      case DIR_FAR:
        Serial.println("FAR");
        switchval=LOW;
        break;
      default:
        Serial.println("NONE");
        break;
    }
  }
}





 