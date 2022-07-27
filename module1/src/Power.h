#ifndef MODULE1_POWER
#define MODULE1_POWER

    #include <avr/sleep.h>
    #include <Arduino.h>
    #include "Data.h" 

    using namespace Utils;
    
    void wakeup(){
        sleep_disable();
        delay(100);
        detachInterrupt(0);
        delay(300);
        Serial.println("WAKE UP");
        delay(300);
        dataHandler();
    }
    
    void sleep(){
        Serial.println("SLEEP");
        delay(100);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        attachInterrupt(digitalPinToInterrupt(3), wakeup, LOW);
        sleep_cpu();
    }
    
#endif