#pragma once

#ifndef configs_h
#define configs_h

    #include <Arduino.h>

    #define SS_PIN          D4 //SDA Pini
    #define RST_PIN         D3

    #define buzzerPin       D8
    #define yesilLed        D1
    #define kirmiziLed      D2
    #define wifiLed         D0

    // wifi ayarları
    #ifndef STASSID
    #define STASSID "GlobalMedia" // wifi ismi
    #define STAPSK  "G11globala16" // parola
    #endif

    #define SERVER "192.168.1.44"
    #define SERVER_PORT 48578
    #define TIMEOUT 5000

    // Serial bağlantı ayarları
    #define SerialBoud 9600

    // seri monitör iletişimini açmak için SerialDebugger tanımlamasını
    // yorum satırı yapın
    #define SerialDebugger

    #ifdef SerialDebugger
        #define serialbegin() Serial.begin(SerialBoud);\
        while(!Serial);
        #define sserial(x) Serial.print(x)
        #define sserialn(x) Serial.println(x)
    #else
        #define serialbegin() {}
        #define serial(x) {}
        #define serialn(x) {}
    #endif


#endif
