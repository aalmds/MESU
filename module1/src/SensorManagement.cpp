#include "SensorManagement.h"

using namespace Utils;

void setup(){
    pinMode(SENSOR1, INPUT_PULLUP);
    pinMode(SENSOR2, INPUT); 
    pinMode(SENSOR3, INPUT);
    pinMode(SENSOR4, INPUT);
    Serial.begin(9600);
}

void loop(){
    Serial.println("LOOP");
    sleep();
}