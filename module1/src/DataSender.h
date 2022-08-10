#ifndef MODULE1_DATA_SENDER
#define MODULE1_DATA_SENDER

SoftwareSerial ss(10, 11);
SMW_SX1262M0 lorawan(ss);

class Sender
{
    String information;

public:
    static CommandResponse response;

    static const uint32_t P2P_FREQUENCY = 915200; // [kHz]

    static const unsigned long PAUSE_TIME = 60000; // [ms] (1 min)
    static unsigned long timeout;

    Sender(String data)
    {
        information = data;
    }

    void sendData()
    {
        delay(1000);
        Serial.println("SENDING DATA");
        if (timeout < millis())
        {
            // send a message
            response = lorawan.P2P_start(P2P_FREQUENCY, false, this->information.c_str()); // send only once
            if (response == CommandResponse::OK)
            {
                Serial.println(F("Message sent"));
            }
            else
            {
                Serial.println(F("Error on sending the message"));
            }

            timeout = millis() + PAUSE_TIME;
        }
    }
};
#endif