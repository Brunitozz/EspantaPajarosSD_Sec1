//cable usb a microusb
#include<WiFi.h>
#include<ThingSpeak.h>
#include "DNT.h"
//definimos el pin de datos del sensor que estará conectado al pin 23 del ESPDevKit
#define DHIPIN 23
//definimos el tipo de sensor que estamos utilizando en un DNT11
#define DHITYPE DHT11//DHT 11
//se actualizan algunos parametros del DHT11 con los puntos definidos anteriormente
DHT dht(DHIPIN,DHITYPE);
//definimos como constantes Char las credenciales de acceso a la red WIFI
const char* ssid = "wifi prrona";
const char* password = "gfdc1986";
//definimos las crdenciales para la conexión a la plataforma
unsigned long channelID = 1624539;
const char* WriteAPIKEY = "905XV3V6VWBQLJW0";
//definimos el cliente wifi que usaremos
WiFiClient cliente;


//iniciamos la funcion setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  //imprimimos una frase, e iniciamos nuestro sensor DHI
  Serial.println("Sensores Instalados y Listos");
  dht.begin();
  //iniciamos la conexion a la red wifi, y se imprimiran caracteres indicando el tiempo que tarde la conexion
  WiFi.begin(ssid.password);
  while(WiFi.status() != WL_CONECTED){
    delay(500);
    Serial.print(".");
  }
  //una vez conectado, se imprimirá una frase y se iniciará la conexion a la plataforma usando el cliente definido anteriormente
  Serial.println("Conectado al WiFi");
  ThingSpeak.begin(cliente);
  delay(5000);
}


//iniciamos la funcion loop
void loop() {
  // put your main code here, to run repeatedly:
  //usamos un retardo de 5 segundos, y utilizamos la funcion medicion
  delay(5000);
  medicion();
  //hacemos la conexion y envio de datos a la plataforma, utilizando las credenciales definidas anteriormente
  ThingSpeak.writeFields(channelID,WriteAPIKEY);
  //imprimimos una frase indicando el envio, y agregamos un retardo de 10 segundos
  Serial.println("Datos enviados a ThingSpeak");
  delay(10000);
}


//definimos la funcion medicion
void medicion(){
  //realizamos la lectura de la temperatura y humedad del sensor
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();
  //imprimimos los valores obtenidos en el terminal Serial
  Serial.print("Temperatura registrada: ");
  Serial.print(temperatura);
  Serial.print("°C");
  Serial.print("Humedad registrada: ");
  Serial.print(humedad);
  Serial.print("%");
  Serial.print("===============================================================");
  //indicamos el orden de envio por campos o Field, en el orden definido de la plataforma, junto a los valores del envio
  ThingSpeak.setField(1,temperatura);
  ThingSpeak.setField(2,humedad);
}
