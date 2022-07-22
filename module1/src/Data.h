#ifndef MODULE1_DATA
#define MODULE1_DATA

    #include "DataReader.h"
    #include "DataSender.h"

    void dataHandler(){
        Reader reader;
        String data = reader.readData();
        Sender sender(data);
    }
    
#endif