
#define BLYNK_TEMPLATE_ID "TMPL2Y-9Ct_n6"
#define BLYNK_TEMPLATE_NAME "EspantaPajaros"
#define BLYNK_AUTH_TOKEN "kWGqveh0xkbun8Svo8QyCZEPVplwtbQF"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "NOMBRE";
char pass[] = "CONTRASEÑA";


//Pines
int const TriggerPin = 32;
int const EchoPin = 33;
int const PirPin = 13;
//int const bocina=2;
//Pir
int val=LOW;// LOW=0 y HIGH=1
int estadoPir=LOW;
int encender; //para blynk

//BLYNK
//Pin Virtual V0--> APAGAR/ENCENDER
BLYNK_WRITE(V0){
  encender = param.asInt();
  if(encender==1){
    Blynk.virtualWrite(V2,"ENCENDIDO");
  }
  if(encender==0){
    Blynk.virtualWrite(V2,"APAGADO");
  }
}
//EVENTOS
//Timer
BlynkTimer timer;
//Mostrar Deteccion Distancia Ultrasonido
void myTimerEvent(){
  Blynk.virtualWrite(V1,calcularDistanciaCm(TriggerPin,EchoPin));
}
//Mostrar Deteccion con PIR
void myTimerEvent1(){
  Blynk.virtualWrite(V3,digitalRead(PirPin));
}

void setup(){
  //Sensor Ultrasonido
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  //Sensor PIR
  pinMode(PirPin, INPUT);
  //Bocina
  //pinMode(bocina, OUTPUT);
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop(){
  Blynk.run();  
  timer.run();
  if (encender == 1) {
    myTimerEvent();
    //Pir
    val = digitalRead(PirPin);
    if(val==HIGH){
      //Accion
      Blynk.virtualWrite(V3,1);
      //Ultrasonido
      if(estadoPir==LOW){
        Blynk.virtualWrite(V2,"Se detectó movimiento");
        estadoPir=HIGH;
      }
    }
    else{
      //Accion
      Blynk.virtualWrite(V3,0);
      //Ultrasonido
      if(estadoPir==HIGH){
          Blynk.virtualWrite(V2,"No hay movimiento");
          estadoPir=LOW;
      }
    }
  }
  else{

  }
}

//Cálculos para la distancia
int calcularDistanciaCm(int TriggerPin, int EchoPin) {
   long tiempo;
   int distanciaCm;

  digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  tiempo = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
  distanciaCm = tiempo * 10 / 292/ 2;   //convertimos a distancia, en cm
  return distanciaCm;
}