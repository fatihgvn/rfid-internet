#pragma once

#ifndef configs_h
#define configs_h

    #include <Arduino.h>

    #define DEVICE_ID       0

    #define SS_PIN          D4 //SDA Pini
    #define RST_PIN         D3

    #define buzzerPin       D8
    #define yesilLed        D1
    #define kirmiziLed      D2
    #define wifiLed         D0

    // wifi ayarları
    #ifndef STASSID
    #define STASSID "zemin kat" // wifi ismi
    #define STAPSK  "genclik55" // parola
    #endif

    #define SERVER "192.168.1.199"
    #define SERVER_PORT 48578
    #define TIMEOUT 5000

    // Serial bağlantı ayarları
    #define SerialBoud 9600

#endif
