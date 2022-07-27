#ifndef MODULE1_DATA_SENDER 
#define MODULE1_DATA_SENDER

    class Sender{
        String information;

        public:
        Sender(String data){
            information = data;
        }
        
        void sendData(){
            delay(1000);
            Serial.println("SENDING DATA");
        }
    };
#endif