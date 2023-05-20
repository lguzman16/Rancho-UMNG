//Librerias
#include <SPI.h>//https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h>//https://github.com/miguelbalboa/rfid
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>

//Constantes

#define SS_PIN 5
#define RST_PIN 0
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);


//Variables
float temperaturaC;
byte nuidPICC[4] = {0, 0, 0, 0};
byte nuidVacas[3][4] ={{89,243,182,109},{9,94,183,109},{67,234,210,166}};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
Servo servo;
float presion, humedad;
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
  delay(1000);
}

void loop(){
  leerRFID();
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
                humedad=analogRead(14);
                Serial.print("Humedad:");
                Serial.print(humedad);
                presion=analogRead(15);
                Serial.print("Presion:");
                Serial.println(presion);
                servo.write(90);
                delay(1000);
                servo.write(0);
              
              break;
              case 1: //Vaca 2
              sensors.requestTemperatures(); 
                temperaturaC = sensors.getTempCByIndex(0);
                Serial.print("Temperatura:");
                Serial.print(temperaturaC);
                Serial.print("°C");
                humedad=analogRead(14);
                Serial.print("Humedad:");
                Serial.print(humedad);
                presion=analogRead(15);
                Serial.print("Presion:");
                Serial.println(presion);
                servo.write(90);
                delay(5000);
                servo.write(0);
              break;
              case 2: //Vaca 3
             sensors.requestTemperatures(); 
                temperaturaC = sensors.getTempCByIndex(0);
                Serial.print("Temperatura:");
                Serial.print(temperaturaC);
                Serial.print("°C");
                humedad=analogRead(14);
                Serial.print("Humedad:");
                Serial.print(humedad);
                presion=analogRead(15);
                Serial.print("Presion:");
                Serial.println(presion);
                servo.write(90);
                delay(5000);
                servo.write(0);
              
              break;
              default:
              break;
              }
            }
        }
        }
}

void printDec(byte *buffer, byte bufferSize){
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], DEC);
  }
}
