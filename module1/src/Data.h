#ifndef MODULE1_DATA
#define MODULE1_DATA

    #include "DataReader.h"
    #include "DataSender.h"

    void dataHandler(){
        Reader reader;
        Serial.println("DATA HANDLER");
        delay(500);
        String data = reader.readData();
        Sender sender(data);
        sender.sendData();
    }
    
#endif