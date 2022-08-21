#include <SoftwareSerial.h>
#include <RoboCore_SMW_SX1262M0.h>
#include <Arduino.h>
#include <avr/sleep.h>

const int NUM_LEVEL = 5;
const int NUM_SENSOR = 4;
const int INFO = 4;

const int SENSOR1 = 3;
const int SENSOR2 = 4;
const int SENSOR3 = 5;
const int SENSOR4 = 6;

const int levels[NUM_LEVEL][NUM_SENSOR] = {
  {1, 1, 1, 1}, 
  {0, 1, 1, 1}, 
  {0, 0, 1, 1}, 
  {0, 0, 0, 1}, 
  {0, 0, 0, 0}
};

int sensors[NUM_SENSOR] = {0, 0, 0, 0};
char data[INFO];

SoftwareSerial ss(10, 11);
SMW_SX1262M0 lorawan(ss);

CommandResponse response;

const char DEVADDR[] = "0010c962";
const char APPSKEY[] = "7ac292fbb93b911305bc276d73f04b97";
const char NWKSKEY[] = "e99145f6a3fbbaaf961d520272c84a40";

const unsigned long PAUSE_TIME = 5000;
unsigned long timeout;

void setupKora();
void resetData();
void readData();
void readSensors();
int checkLevel();
void checkError(int level);
void sendData();

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
  resetData();

  readData();
  sendData();

  delay(30000);
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

  for(int i = 0; i < 20; ++i){
    response = lorawan.join();
    if (response == CommandResponse::OK)
    {
      Serial.println(F("Joining the network"));
      break;
    }
    else
    {
      Serial.println(F("Error on joining the network, trying again"));
    }
  }


}

void resetData()
{
  data[0] = '0';
  for (int i = 1; i < INFO; ++i)
  {
    data[i] = '1';
  }
}

void checkError()
{
  delay(500);
  for (int i = 0; i < NUM_SENSOR; ++i)
  {
    for (int j = i + 1; j < NUM_SENSOR; ++j)
    {
      if (sensors[i] && !sensors[j])
      {
        delay(500);
        data[i] = '0';
        break;
      }
    }
  }
  delay(500);
}

int checkLevel()
{
  int level = 5; // error level

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
  return level;
}

void readSensors()
{
  Serial.println("Sensors:");
  for (int i = 0; i < NUM_SENSOR; i++)
  {
    sensors[i] = digitalRead(SENSOR1 + i);
    delay(500);
    Serial.println(sensors[i]);
  }
}

void readData()
{
  delay(500);
  readSensors();

  delay(500);
  data[0] = checkLevel() + '0';
  checkError();
  delay(500);
}

void sendData()
{
  if (timeout < millis())
  {
    if (lorawan.isConnected())
    {
      Serial.println("Data: ");
      for (int i = 0; i < 4; ++i)
      {
        Serial.println(data[i]);
      }

      for (int i = 0; i < 10; ++i)
      {
        response = lorawan.sendX(1, data);
        timeout = millis() + PAUSE_TIME;
        if (response == CommandResponse::OK)
        {
          Serial.println(F("Message sent"));
          break;
        }
        else
        {
          Serial.println(F("Error in sending message, trying to join again"));
          lorawan.join();
        }
      }}
       else
      {
        timeout = millis() + PAUSE_TIME;
      }
    }
  }