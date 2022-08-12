#include <SoftwareSerial.h>
#include <RoboCore_SMW_SX1262M0.h>
#include <Arduino.h>
#include <avr/sleep.h>
#include <ArduinoJson.h>

const int NUM_LEVEL = 3;
const int NUM_SENSOR = 4;

const int SENSOR1 = 3;
const int SENSOR2 = 4;
const int SENSOR3 = 5;
const int SENSOR4 = 6;

const int levels[NUM_LEVEL][NUM_SENSOR] = {{0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};

int sensors[NUM_SENSOR];

char jsonString[] = "{\"id\":\"undefined\",\"date\":\"2022-07-19:11-07\",\"level\":\"-1\" ,\"error\":\" \"}";
DynamicJsonDocument dataJson(1024);

CommandResponse response;
const uint32_t P2P_FREQUENCY = 915200; // [kHz]
const unsigned long PAUSE_TIME = 60000; // [ms] (1 min)
unsigned long timeout;

SoftwareSerial ss(10, 11);

SMW_SX1262M0 sender(ss);

String checkError(String data);
void sendData();
String checkLevel();
void readSensors();
String readData();

void setup()
{
  pinMode(SENSOR1, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(SENSOR3, INPUT_PULLUP);
  pinMode(SENSOR4, INPUT_PULLUP);
  Serial.begin(9600);

  ss.begin(9600);
  sender.reset();

  DeserializationError error = deserializeJson(dataJson, jsonString);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
}

void loop()
{
  Serial.println("LOOP");
  delay(500);

  String data = readData();

  delay(500);
  sendData();

  delay(30000); // espera 2 minutos até a próxima leitura
}

String checkError(String data)
{
  Serial.println("CHECK ERROR");
  delay(500);
  String error = "";
  if (data != "-1")
  {
    return error;
  }

  for (int i = 0; i < NUM_SENSOR; ++i)
  {
    for (int j = i; j < NUM_SENSOR; ++j)
    {
      if (sensors[i] && !sensors[j])
      {
        delay(500);
        error += "_Sensor" + String(i);
        break;
      }
    }
  }
  delay(500);

  return error;
}

String checkLevel()
{
  // Serial.println("CHECK LEVEL");
  int level = -1;
  for (int i = 0; i < NUM_LEVEL; ++i)
  {
    int count = 0;
    for (int j = 0; j < NUM_SENSOR; ++j)
    {
      if (sensors[j] == levels[i][j])
      {
        count++;
      }
    }
    if (count == NUM_SENSOR)
    {
      level = i;
      break;
    }
  }
  return String(level);
}

void readSensors()
{
  Serial.println("READ SENSORS");
  for (int i = 0; i < NUM_SENSOR; i++){
    sensors[i] = digitalRead(SENSOR1+i);
    // sensors[0] = 0;
    delay(500);
    Serial.println(sensors[i]);
  }
}

String readData()
{
  delay(500);
  readSensors();

  delay(500);
  dataJson["level"] = checkLevel();

  delay(500);
  dataJson["error"] = checkError(dataJson["level"]);

  // dataJson["date"] += readDateAndTime();
  // dataJson["id"] += readArduinoId();
}

void sendData()
{
  Serial.println("SENDING DATA");
  delay(500);

  String data;
  serializeJson(dataJson, data);
  Serial.println(data);

  delay(500);
  if (timeout < millis())
  {

    response = sender.P2P_start(P2P_FREQUENCY, true, data.c_str()); // send only once

    delay(500);
    if (response == CommandResponse::OK)
    {
      Serial.println(F("Message sent"));
      Serial.println(data);
    }
    else
    {
      Serial.println(F("Error on sending the message"));
    }

    timeout = millis() + PAUSE_TIME;
  }
}