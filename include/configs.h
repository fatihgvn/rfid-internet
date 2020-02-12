#pragma once

#ifndef configs_h
#define configs_h

    #include <Arduino.h>

    #define SS_PIN D4 //SDA Pini
    #define RST_PIN D3

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