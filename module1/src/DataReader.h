#ifndef MODULE1_DATA_READER 
#define MODULE1_DATA_READER

  using namespace Utils;

  class Reader{
    String data;

    String checkError(String level){
      if(level == "-1"){
        return " ";
      }

      for(int i = 0; i < NUM_SENSOR; ++i){
        for(int j = i; j < NUM_SENSOR; ++j){
          if(sensors[i] && !sensors[j]){
            level = level + "_Sensor" + sensors[i];
            break;
          }
        }
      }
    }

    String checkLevel(){
      int level = -1;
      for (int i = 0; i < NUM_LEVEL; ++i){
        int count = 0;
        for (int j = 0; j < NUM_SENSOR; ++j){
          if (sensors[j] == levels[i][j]){
            count++;
          }
        }
        if (count == NUM_SENSOR){
          level = i;
          break;
        }
      }
      return String(level);
    }

    void readSensors(){
      for (int i = 0; i < NUM_SENSOR; ++i){
        sensors[i] = digitalRead(SENSOR1 + i);
      }
    }

    String readDateAndTime(){
      // Read from RTC
    }

    String readArduinoId(){
      // Arduino ID
    }

    public:
    String readData(){
      Serial.println("ENTREI");
      delay(100);

      readSensors;
      data += checkLevel();
      data += checkError(data);
      data += readDateAndTime();
      data += readArduinoId();

      return data;
    }
  };
#endif