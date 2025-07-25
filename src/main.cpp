#include <Arduino.h>
#include "pid_custom.h"

pid controlador;

unsigned long t1,t2;
uint16_t delta = 50;
const float Kp = 0, Kd = 0.0f, Ki = 32.0f;
const uint8_t pin = 34; 
const uint8_t pin_base = 12;

//float referencia = 1170;
float referencia = 800;
float factor = 1; //factor para llevar de dBm a voltaje del detectro RMS

void setup() {
  controlador.setup(pin, pin_base, Kp, Ki, Kd, delta);
  Serial.begin(115200);
  for(int i=0;i<6;i++){
    digitalWrite(pin_base+i,(i==0 || i==5)?0:1);
  }
  t1 = millis();
}

void loop() {
  t2 = millis();
  controlador.getMeasure(15);//15 muestras
  

  if(t2 - t1 >= delta)//polling
  {
    t1 = t2;
    controlador.calculate_out(referencia);
    controlador.writePort();
  }
  Serial.print("in=");
  Serial.println(controlador.in_k);
  Serial.print("error=");
  Serial.println(controlador.error[0]);
  Serial.print("out=");
  Serial.println(controlador.output);
}