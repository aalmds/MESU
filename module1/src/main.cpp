#include <Arduino.h>
#define SENSOR1 7
#define SENSOR2 8
#define SENSOR3 9

#define NUM_SENSOR 3
#define NUM_LEVEL 4

int levels[NUM_LEVEL][NUM_SENSOR] = {{0, 1, 1}, {0, 0, 1}, {0, 0, 0}, {1, 1, 1}};
int sensors[NUM_SENSOR];

void setup()
{
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT); 
  pinMode(SENSOR3, INPUT);
  Serial.begin(9600);
}

int readSensors(){
  for(int i = 0; i < NUM_SENSOR; ++i){
    sensors[i] = digitalRead(SENSOR1 + i);   
  }

  int level = -1;
  for(int i = 0; i < NUM_LEVEL; ++i){
    int count = 0;
    for(int j = 0; j < NUM_SENSOR; ++j){
      if (sensors[j] == levels[i][j]){
         count++;  
      }
    }
    if (count == NUM_SENSOR){
      level = i;
      break;
    }
  }
  return level;
}


void loop()
{
  int level = readSensors();
}