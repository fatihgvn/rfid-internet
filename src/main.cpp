#include "configs.h"

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   
int statuss = 0;
int out = 0;
WiFiClient client; // TCP bağlantı için obje

bool checkCard(String);

void setup() {
  // put your setup code here, to run once:
  pinMode(alarmPin, OUTPUT);
  pinMode(yesilLed, OUTPUT);
  pinMode(kirmiziLed, OUTPUT);
  
  serialbegin();
  SPI.begin();      
  mfrc522.PCD_Init();   

  
  sserialn();
  sserialn();
  sserial("Connecting to ");
  sserialn(STASSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    sserial(".");
  }

  sserialn();
  sserialn("WiFi connected");
  sserial("IP address: ");
  sserialn(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

  if (
    mfrc522.PICC_IsNewCardPresent() // Yeni kartlari tara
    && mfrc522.PICC_ReadCardSerial() // Kart okundu ise
    ) 
  {
    String card= "";


    // kartı oku
    for (byte i = 0; i < mfrc522.uid.size; i++)
      card.concat(String(mfrc522.uid.uidByte[i], HEX));
    
    card.toUpperCase();
    sserialn();

    //Kartin UID'sini seri porta yaz
    sserialn();
    sserial(" UID etiketi :");
    sserialn(card);

    if(checkCard(card)){
      sserialn("Giris Basarili!");

      digitalWrite(yesilLed, HIGH);

      digitalWrite(alarmPin, HIGH);
      delay(150);
      digitalWrite(alarmPin, LOW);
      delay(200);
      digitalWrite(alarmPin, HIGH);
      delay(200);
      digitalWrite(alarmPin, LOW);
      delay(1000);

      digitalWrite(yesilLed, LOW);
    }else{
      sserialn("Hatalı kart!");
      digitalWrite(kirmiziLed, HIGH);

      digitalWrite(alarmPin, HIGH);
      delay(600);
      digitalWrite(alarmPin, LOW);
      delay(1000);

      digitalWrite(kirmiziLed, LOW);
    }
  }
}

bool checkCard(String card){
  sserial("connecting to ");
  sserial(SERVER);
  sserial(':');
  sserialn(SERVER_PORT);
  bool ret = false;

  // Use WiFiClient class to create TCP connections
  if (!client.connect(SERVER, SERVER_PORT)) {
    sserialn("connection failed");
    ret = false;
  }else{
    client.println(card);
    char ch = '\0';
    long timecounter = millis();

    while (client.available() && millis()-timecounter < TIMEOUT)
      ch = static_cast<char>(client.read());
    
    if(millis() - timecounter > TIMEOUT){
      // server yanıt vermedi hatası!

      digitalWrite(sariLed, HIGH);
      digitalWrite(kirmiziLed, HIGH);
      digitalWrite(alarmPin, HIGH);
      delay(1000);
      digitalWrite(sariLed, LOW);
      digitalWrite(kirmiziLed, LOW);
      digitalWrite(alarmPin, LOW);

      ret = false;
    }else{
      if(ch == '1') ret = true;
      else ret = false;
    }
  }

  return ret;
}