// // Bibliotecas
#include <Arduino.h>
#include <Wire.h> 
#include <time.h>

// Arquivos

#include <confpwm.h>
#include <spwm.h>

// Definições do PWM e Ponte H

const int sentidoMotor1 = 2;  // Porta para definir o sentido de rotação 1.
const int sentidoMotor2 = 0;  // Porta para definir o sentido de rotação 2.

extern int degrau;

void setup() {
  Serial.begin(115200);
  pinMode(sentidoMotor1, OUTPUT);
  pinMode(sentidoMotor2, OUTPUT);

  // Define como output os pinos que definem o sentido de rotação do motor
  digitalWrite(sentidoMotor1, LOW);
  digitalWrite(sentidoMotor2, HIGH);


  // sPWM
  sPWM();
}


void loop() {
  // // Faz a medição de velocidade
  // measureVelocity();
  
  // // Faz a medição de corrente com filtro de media movel
  // measureCurrent();

  // Serial.println(" ");

  // Serial.println(degrau);

}

