#include <SoftwareSerial.h>
#include <RoboCore_SMW_SX1262M0.h>
#include <Arduino.h>
#include <avr/sleep.h>

const int NUM_LEVEL = 3;
const int NUM_SENSOR = 4;

const int SENSOR1 = 3;
const int SENSOR2 = 4;
const int SENSOR3 = 5;
const int SENSOR4 = 6;

const int levels[NUM_LEVEL][NUM_SENSOR] = {{0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};

int sensors[NUM_SENSOR];

SoftwareSerial ss(10, 11);
SMW_SX1262M0 lorawan(ss);

CommandResponse response;

const char DEVADDR[] = "0010c962";
const char APPSKEY[] = "7ac292fbb93b911305bc276d73f04b97";
const char NWKSKEY[] = "e99145f6a3fbbaaf961d520272c84a40";

const unsigned long PAUSE_TIME = 5000;
unsigned long timeout;

void setupKora();
void sendData(String data);

String readData();
void readSensors();
String checkLevel();
String checkError(String data);

void setup()
{
  pinMode(SENSOR1, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(SENSOR3, INPUT_PULLUP);
  pinMode(SENSOR4, INPUT_PULLUP);
  Serial.begin(9600);

  setupKora();
}

void loop()
{
  Serial.println("LOOP");
  delay(500);

  String data = readData();
  sendData(data);
  delay(10000);
}

void setupKora()
{
  ss.begin(9600);
  lorawan.reset();

  response = lorawan.set_JoinMode(SMW_SX1262M0_JOIN_MODE_ABP);
  if (response == CommandResponse::OK)
  {
    Serial.println(F("Mode set to ABP"));
  }
  else
  {
    Serial.println(F("Error setting the join mode"));
  }

  char deveui[16];
  response = lorawan.get_DevEUI(deveui);
  if (response == CommandResponse::OK)
  {
    Serial.print(F("DevEUI: "));
    Serial.write(deveui, 16);
    Serial.println();
  }
  else
  {
    Serial.println(F("Error getting the Device EUI"));
  }

  response = lorawan.set_DevAddr(DEVADDR);
  if (response == CommandResponse::OK)
  {
    Serial.print(F("Device Address set ("));
    Serial.write(DEVADDR, 8);
    Serial.println(')');
  }
  else
  {
    Serial.println(F("Error setting the Device Address"));
  }

  response = lorawan.set_AppSKey(APPSKEY);
  if (response == CommandResponse::OK)
  {
    Serial.print(F("Application Session Key set ("));
    Serial.write(APPSKEY, 32);
    Serial.println(')');
  }
  else
  {
    Serial.println(F("Error setting the Application Session Key"));
  }

  response = lorawan.set_NwkSKey(NWKSKEY);
  if (response == CommandResponse::OK)
  {
    Serial.print(F("Network Session Key set ("));
    Serial.write(NWKSKEY, 32);
    Serial.println(')');
  }
  else
  {
    Serial.println(F("Error setting the Network Session Key"));
  }

  response = lorawan.save();
  if (response == CommandResponse::OK)
  {
    Serial.println(F("Settings saved"));
  }
  else
  {
    Serial.println(F("Error on saving"));
  }

  Serial.println(F("Joining the network"));
  lorawan.join();
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
  Serial.println("CHECK LEVEL");
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
  for (int i = 0; i < NUM_SENSOR; i++)
  {
    sensors[i] = digitalRead(SENSOR1 + i);
    // sensors[0] = 0;
    delay(500);
    Serial.println(sensors[i]);
  }
}

String readData()
{
  delay(500);
  String data;
  readSensors();

  delay(500);
  data += checkLevel();
  data += checkError(data);
  delay(500);

  return data;
}

void sendData(String data)
{
  if (timeout < millis())
  {
    if (lorawan.isConnected())
    {
      Serial.print(F("Data: "));
      Serial.println(data);
      response = lorawan.sendX(1, data);
      timeout = millis() + PAUSE_TIME;

      if (response == CommandResponse::OK)
      {
        Serial.println(F("Message sent"));
      } else if(response == CommandResponse:: ERROR){
        Serial.println(F("ERROR"));
      } else if(response == CommandResponse:: BUSY){
        Serial.println(F("Busy"));
      } else if(response == CommandResponse:: NO_NETWORK){
        Serial.println(F("No network"));
      }else
      {
        Serial.println(F("Data"));
      }
    }
    else
    {
      Serial.println('.');
      timeout = millis() + 5000;
    }
  }
}