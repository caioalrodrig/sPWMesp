#include <Arduino.h>
#include <math.h>

// Pino para alterar PWM
const int pinAlteraPWM = 15;

// Configurações PWM
const int motor  = 16;          // Define a porta de saída do sinal PWM.
int freq = 32;                  // Define a frequencia a ser utilizada
const int motorChannel = 0;
int resolution = 8;             // Define a resolução que será utilizada no PWM.

int degrau = 0;

// Senoide
float T = 1/freq;
bool aux = 1;
int tempo_p = 0, tempo_a = 0;
int dT = 0;
int i = 0;
float sine;
int sine_i;

void alteraFreq(){
    if( freq < 20000){
        freq = 2*freq;
        ledcSetup(motorChannel,freq, resolution);
        ledcWrite(motorChannel, 0);
        T = 1/freq;
    }
    else{
        ledcSetup(motorChannel,40000, resolution);
        ledcWrite(motorChannel, 0);
        T = 1/40000;
    }
}

void verificaPWM()
{
    pinMode(pinAlteraPWM, INPUT);
    attachInterrupt(pinAlteraPWM, alteraFreq, RISING);
}

void inicializaPWM()
{
    pinMode(motor, OUTPUT);

    // Atribui o canal ao GPIO que será controlado
    ledcAttachPin(motor, motorChannel);

    // Configura o LED PWM
    ledcSetup(motorChannel, freq, resolution);  
    ledcWrite(motorChannel, 179);

}

void senoide()
{
    // 
    
    for(int i = -180; i<=180; i++){
        sine = sin(i*(PI/180.0)*freq);
        sine_i = 179 + int(0.1*sine*255.0/2.0);
        ledcWrite(motorChannel, sine_i);
        // Serial.println(sine_i);
        // Serial.print(";");
        // Serial.print(sine_i);
        // Serial.print(";");
        // Serial.println(sine_i);
    }
}