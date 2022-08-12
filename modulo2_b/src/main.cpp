#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include "ArduinoJson.h"
#define RXp2 16
#define TXp2 17

char json[] = "{\"id\":\"9238\",\"level\":2,\"date\":\"2022-07-19:11-07\"}";

DynamicJsonDocument doc(1024);

const char* ssid = "CINGUESTS"; //Enter SSID
const char* password = "acessocin"; //Enter Password
const char* websockets_server_host = "172.22.79.2"; //Enter server adress
const uint16_t websockets_server_port = 8080; // Enter server port

using namespace websockets;

WebsocketsClient client;

// void connection(String output){
//     bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
//     if(connected){
//       Serial.println("Connected!");
//       serializeJson(doc, output);
//       client.send(output);
//       delay(4000);
//     }else Serial.println("Not Connected!");
// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("No Wifi!");
    return;
  }
  Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
  bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
  if(connected){
    Serial.println("Connected!");
    // String output;
    // serializeJson(doc, output);
    // client.send(output);
    // delay(4000);
  }else Serial.println("Not Connected!");
  
  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message){
      Serial.print("Got Message: ");
      Serial.println(message.data());
  });
}
void loop() {
    // Serial.println("Message Received: ");
    // String output = Serial2.readString();
    // Serial.println(output);
    // delay(5000);
    if(client.available()) {
        String output;
        serializeJson(doc, output);
        client.send(output);
        client.poll();
    }
    // String oi = Serial2.readString();
    //serializeJson(doc, oi);
    // client.send(oi);
    delay(5000);
}