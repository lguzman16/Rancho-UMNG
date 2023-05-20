//Librerias
#include "UbidotsEsp32Mqtt.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>//https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h>//https://github.com/miguelbalboa/rfid
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>

//Constantes
const int humsuelo = 34;
/*Definimos primero el Token que nos brinda la plataforma Ubidots para hacer la conexión*/
const char *UBIDOTS_TOKEN = "BBFF-vQwS9OzeUueksBvOqzzoD67asAmOqo";
/*Definimos SSID y PASSWORD de nuestra red WiFi*/
const char *WIFI_SSID = "Redmi Note 9 DR";      
const char *WIFI_PASS = "RamirezLotta";     
/*Definimos el nombre de nuestro dispositivo, el cual aparecerá en la plataforma Ubidots*/
const char *DEVICE_LABEL = "esp32";
/*Definimos las variables que se medirán y que serán publicadas en la plataforma Ubidots*/
const char *VARIABLE_LABEL_1 = "Comida"; 
const char *VARIABLE_LABEL_2 = "Humedad"; 
const char *VARIABLE_LABEL_3 = "Temperatura"; 
const char *VARIABLE_LABEL_4 = "Identificacion"; 
const char *VARIABLE_SUB1 = "Puerta";
int puerta, ident=0;
Ubidots ubidots(UBIDOTS_TOKEN);
#define SS_PIN 5
#define RST_PIN 0
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);


//Variables
int valHumsuelo;
float temperaturaC;
byte nuidPICC[4] = {0, 0, 0, 0};
byte nuidVacas[3][4] ={{89,243,182,109},{9,94,183,109},{67,234,210,166}};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
Servo servo;
int humedad,presion;

void callback(char *topic, byte *payload, unsigned int length)
{
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  if((char)payload[0]=='0'){
      servo.write(0);
      delay(1000);
    }
    else if((char)payload[0]=='1'){
      servo.write(1);
      delay(1000);
      }
}

void setup() {
   //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  SPI.begin();
  rfid.PCD_Init();
  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();
  servo.attach(2, 500, 2500);
  servo.write(0);
  sensors.begin();
  servo.write(0);
  pinMode(humsuelo, INPUT);
  /*Incluimos un retardo de 1 segundo*/
  delay(1000);

  /*Ahora, se incluye las funciones de conexión de la Plataforma, como la conexión a internet con las credenciales de WiFi*/
  // ubidots.setDebug(true);  //Descomentar esto para que los mensajes de depuración estén disponibles
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  /*Se inicia la función de subscripción para los mensajes de retorno, que se definió líneas arriba*/
  ubidots.setCallback(callback);
  /*Se incluye las funciones de Setup y Reconnect, predefinidas de la librería*/
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_SUB1);
}

void loop(){
  leerRFID();
  valHumsuelo = map(analogRead(humsuelo), 4092, 0, 0, 100);
  presion=analogRead(35);
  if (!ubidots.connected())
  {
    ubidots.reconnect();
    puerta = ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_SUB1);
  }
  ubidots.loop();
}


void leerRFID(void ) { 
  ////Read RFID card
  for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if ( ! rfid.PICC_IsNewCardPresent())
      return;
  // Verify if the NUID has been readed
  if (  !rfid.PICC_ReadCardSerial())
      return;
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
  }
  Serial.print(F("RFID En decimal: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  for(int i =0;i<3;i++){
    int cont=0;
     presion=analogRead(15);
        for(int j=0;j<4;j++){
          if(nuidPICC[j]==nuidVacas[i][j]){
            cont++;
          }
          if(cont==4){
            switch (i){
              case 0: //Vaca 1
                sensors.requestTemperatures(); 
                temperaturaC = sensors.getTempCByIndex(0);
                Serial.print("Temperatura:");
                Serial.print(temperaturaC);
                Serial.print("°C");
                Serial.print("Humedad:");
                Serial.print(valHumsuelo);
                Serial.println(" %");
                Serial.print("Presion:");
                Serial.println(presion);
                servo.write(90);
                delay(1000);
                servo.write(0);
                ident=1;
              break;
              case 1: //Vaca 2
              sensors.requestTemperatures(); 
                temperaturaC = sensors.getTempCByIndex(0);
                Serial.print("Temperatura:");
                Serial.print(temperaturaC);
                Serial.print("°C");
                Serial.print("Humedad:");
                Serial.print(valHumsuelo);
                Serial.println(" %");
                Serial.print("Presion:");
                Serial.println(presion);
                servo.write(90);
                delay(5000);
                servo.write(0);
                ident=2;
              break;
              case 2: //Vaca 3
             sensors.requestTemperatures(); 
                temperaturaC = sensors.getTempCByIndex(0);
                Serial.print("Temperatura:");
                Serial.print(temperaturaC);
                Serial.print("°C");
                Serial.print("Humedad:");
                Serial.print(valHumsuelo);
                Serial.println(" %");
                Serial.print(" Presion:");
                Serial.print(presion);
                servo.write(90);
                delay(5000);
                servo.write(0);
                ident=3;
              
              break;
              default:
              break;
              }
            }
        }
        }
        ubidots.add(VARIABLE_LABEL_1, temperaturaC*2);
        ubidots.add(VARIABLE_LABEL_2, temperaturaC/10);
        ubidots.add(VARIABLE_LABEL_3, temperaturaC);
        ubidots.add(VARIABLE_LABEL_4, ident);
        ubidots.publish(DEVICE_LABEL);
        delay(1000);
}

void printDec(byte *buffer, byte bufferSize){
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], DEC);
  }
}
