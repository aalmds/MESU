#ifndef MODULE1_POWER
#define MODULE1_POWER

    #include <avr/sleep.h>
    #include <Arduino.h>
    #include "Data.h"
    

    void wakeup(){
        sleep_disable();
        detachInterrupt(0);
        Serial.println("WAKE UP");
        delay(100);
        dataHandler();
    }
    
    
    void sleep(){
        Serial.println("SLEEP");
        delay(100);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        attachInterrupt(digitalPinToInterrupt(SENSOR3), wakeup, LOW);
        sleep_cpu();
    }
    
#endif