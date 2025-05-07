//  Eje-X joystick pin = A1
//  Eje-Y joystick pin = A0
//  Trim potenciometro pin = A2
//  Boton pin = 2

 
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  140 // Minimo del ancho de pulso (out of 4096)
#define SERVOMAX  520 // Maximo del ancho de pulso (out of 4096)

int xval;
int yval;

int lexpulse;
int rexpulse;

int leypulse;
int reypulse;

int uplidpulse;
int lolidpulse;
int altuplidpulse;
int altlolidpulse;

int trimval;

const int analogInPin = A0;
int sensorValue = 0;
int outputValue = 0;
int switchval = 0;

void setup() 
{
  pinMode(analogInPin, INPUT);
  pinMode(2, INPUT);
 
  pwm.begin();
  
  pwm.setPWMFreq(60);  // Los servos se actualizan a 60Hz

  delay(10);
}

void setServoPulse(uint8_t n, double pulse) 
{
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us por segundo
  pulselength /= 60;   // 60 Hz
  pulselength /= 4096;  // 12 bits de resolucion
  pulse *= 1000000;  // convierte en us
  pulse /= pulselength;

}

void loop() {

  xval = analogRead(A1); ///////////////////////////////////esto hay que cambiar para controlar por bt
    lexpulse = map(xval, 0,1023, 220, 440);
    rexpulse = lexpulse;

    switchval = digitalRead(2);
    
    
  yval = analogRead(A0); ///////////////////////////////////esto hay que cambiar para controlar por bt
    leypulse = map(yval, 0,1023, 250, 500);
    reypulse = map(yval, 0,1023, 400, 280);

  trimval = analogRead(A2);
    trimval=map(trimval, 320, 580, -40, 40);
     uplidpulse = map(yval, 0, 1023, 400, 280);
        uplidpulse -= (trimval-40);
          uplidpulse = constrain(uplidpulse, 280, 400);
     altuplidpulse = 680-uplidpulse;

     lolidpulse = map(yval, 0, 1023, 410, 280);
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
