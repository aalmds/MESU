#include <Arduino.h>
#define SENSOR1 7
#define SENSOR2 8
void setup()
{
  // put your setup code here, to run once:
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  Serial.begin(9600);
}

int printSensors(int out_s1, int out_s2, int t)
{
  Serial.print(out_s1);
  Serial.print("   ");
  Serial.println(out_s2);
  delay(t);
}

void loop()
{
  int s1 = digitalRead(SENSOR1), s2 = digitalRead(SENSOR2);
  printSensors(s1, s2, 1000);
}