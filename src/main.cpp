#include "configs.h"

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

WiFiClient client;

byte nuidPICC[4];
char key_id[20];
String key_id_str="";

void printHex(byte *buffer, byte bufferSize);

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // SPI BİRİMİNİ KUR VE YAPILANDIR. 

  pinMode(yesilLed,OUTPUT);
  pinMode(kirmiziLed,OUTPUT);
  pinMode(buzzerPin,OUTPUT);
  pinMode(wifiLed,OUTPUT);

  digitalWrite(yesilLed,LOW);
  digitalWrite(kirmiziLed,LOW);
  digitalWrite(buzzerPin,LOW);


    Serial.println();
    Serial.println();
    Serial.println("Baglanamaya Calisiyor...");
    Serial.print("SSID : ");
    Serial.println(STASSID);

   WiFi.mode(WIFI_STA);  // WIFIYE BAGLAN.
   WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(wifiLed,HIGH);
    Serial.print(".");
    delay(200);
    digitalWrite(wifiLed,LOW);
    delay(200);
  }

  Serial.println("");
  Serial.println("WiFi Baglandi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(wifiLed,HIGH);
  
  rfid.PCD_Init(); // MFRC522 KUR VE YAPILANDIR.

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;  // KARTLARIN ID SINI BOS OLARAK DOLDUR BUFFER TEMIZLE.
  }

     
}
//---------------------------------------SETUP_BITISI-----------------------------------------------------
//*******************************************************************************************************
//
//
//
//
//*******************************************************************************************************
//------------------------------------------------------------------------------------------------------


//-----------------------------------------SONSUZ_DONGU_BASLANGICI---------------------------------------
void loop() {

  while(WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(wifiLed,LOW);
    delay(200);
    digitalWrite(wifiLed,HIGH);
    delay(200);
    Serial.println("kesildi");
      if(WiFi.status() == WL_CONNECTED)
    {
    digitalWrite(wifiLed,HIGH);
    Serial.println("WIFI GELDI");
    break;
     }
  }


  
  
 

  // Yeni bir kart varmı bak
  if ( ! rfid.PICC_IsNewCardPresent())
  return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
  return;
    
    for (byte i = 0; i < 4; i++) 
    {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();   
}


void printHex(byte *buffer, byte bufferSize) 
{
  Serial.println("Baglanmaya Calisiyor... ");
  Serial.print(SERVER);
  Serial.print(':');
  Serial.println(SERVER_PORT);

  // Use WiFiClient class to create TCP connections
  if (!client.connect(SERVER, SERVER_PORT)) {
    Serial.println("Baglanilamadi...");
    delay(250);
    return;
  }

  // This will send a string to the server
  Serial.println("Servere Data Gönderiliyor...");
  if (client.connected()) {
     for (byte i = 0; i < bufferSize; i++) 
     {
    Serial.print(buffer[i], HEX);
    key_id_str += String(buffer[i],HEX);
    //key_id_str += key_id[i];
    }
    Serial.println();
    //client.println(RFID_KART_NUMBER_ID);
    key_id_str.toUpperCase();
    Serial.println(key_id_str);   // KANKA AZ DA SEN DELLEN :D
    client.print(key_id_str);
    key_id_str="";

  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Cevap vermekte gecikti....!");
      client.stop();
      delay(600);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("Serverden gelen Data : ");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.println(ch);

    if(ch=='1')
      {
        Serial.println("Kapi Acildi...");
        digitalWrite(yesilLed,HIGH);
        digitalWrite(buzzerPin,HIGH);
        delay(250);
        digitalWrite(buzzerPin,LOW);
        delay(100);
        digitalWrite(buzzerPin,HIGH);
        delay(100);
        digitalWrite(buzzerPin,LOW);
        digitalWrite(yesilLed,LOW);
      }
      else
      {
        Serial.println("Erisim Engellendi");
         digitalWrite(kirmiziLed,HIGH);
          digitalWrite(buzzerPin,HIGH);
          delay(500);
          digitalWrite(buzzerPin,LOW);
          delay(50);
          digitalWrite(buzzerPin,HIGH);
          delay(100);
          digitalWrite(buzzerPin,LOW);
          digitalWrite(kirmiziLed,LOW);
      }
  }

  // Close the connection
  Serial.println();
  Serial.println("Baglanti Sonlandirildi...");
  client.stop();
}