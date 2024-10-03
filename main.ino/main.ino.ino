#include <vl53l0x.h>
#include <IRRemote.h>

// Por agora isso é uma cópia barata do código do sumozão. Não sei por onde continuar.

// Definições de constantes e enums

#define IRRECEIVERPIN 23

IRrecv irrecv(irReceiverPin);

//Presence Sensor Pins and Variables on the board
#define INFRASENSOR_RIGHT 5 //17 5
#define INFRASENSOR_LEFT 19 //23 22 
#define INFRASENSOR_MID 18
#define INFRASENSOR_LEFT_REF 99 //INSERIR O NUMERO NO ESP
#define INFRASENSOR_RIGHT_REF 98 //INSERIR O NUMERO NO ESP

enum States {
  READY, RUNNING, STOP
}

enum Tactics {
  MOVIMENTACAO, RADAR, INV_RADAR, SUICIDE
}

// definição de globais

States state = STOP;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

}
