#ifndef MODULE1_DATA_READER 
#define MODULE1_DATA_READER

  #include <Wire.h>
  #include <RtcDS3231.h> 
  #include "Utils.h"
  
  RtcDS3231<TwoWire> rtc(Wire);
  
  using namespace Utils;

  class Reader{
    String data;

    String checkError(String data){
      delay(500);
      if(data != "-1"){
        return " ";
      }

      for(int i = 0; i < NUM_SENSOR; ++i){
        for(int j = i; j < NUM_SENSOR; ++j){
          if(sensors[i] && !sensors[j]){
            data += "_Sensor" + String(i);
            break;
          }
        }
      }
      delay(500);

      return data;
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
      /*for (int i = 0; i < NUM_SENSOR; ++i){
        sensors[i] = digitalRead(SENSOR1 + i);
      }
      */
    }

    /*
    RtcDateTime readDateAndTime(){
      rtc.Begin();
      RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

      if (!rtc.IsDateTimeValid()){
          rtc.SetDateTime(compiled);
      } else{
  
          RtcDateTime now = rtc.GetDateTime();
          if (now < compiled){
            rtc.SetDateTime(compiled);
          }
      }

        rtc.Enable32kHzPin(false);
        rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

        return rtc.GetDateTime();
    }

    String readArduinoId(){
      // Arduino ID
    }*/

    public:
    String readData(){
      readSensors();
      delay(500);
      data += checkLevel();
      delay(500);
      data += checkError(data);
      delay(500);
      Serial.println(data);
      delay(500);
      /*data += readDateAndTime();
      data += readArduinoId();*/
      return data;
    }
  };
#endif