#include <ESP8266WiFi.h>

#include <OneWire.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <Wire.h>
#include <FirebaseArduino.h>


// Data wire is plugged into pin 2 on the Arduino
//#define ONE_WIRE_BUS D2

// Set these to run example.
#define FIREBASE_HOST "led1-bb4f2.firebaseio.com"
#define FIREBASE_AUTH "YnUD8FAZgJu0QEL6pik5agrGLwjW3EBcrVx0Nbzq"
//#define WIFI_SSID "SRRD"
//#define WIFI_PASSWORD "sree2930"

float reading;
float CurrentTemperature, CurrentVoltage, AcCurrent,BugTemperature;
int test=25;
float val;
float MIN,MAX;
float EEPROMMinValue,EEPROMMaxValue;
float CLOUDMinValue, CLOUDMaxValue;
float Value;
int RData;

String ReadTheData,T1,T2,T1_bug,T2_bug,T3;
char SendTheData[300];

float CurrentMin, CurrentMax; 

//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);


void setup() {

  Serial.begin(9600);

    /* initialization for firebase */
  // connect to wifi.
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  //wifiManager.resetSettings();
   
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //Firebase.setFloat("sensor2/minimum temperature", 0);
  //Firebase.setFloat("sensor2/maximum temperature", 0); 

  
}
void loop() 
{
  
    delay(750);
    ReadIncomingData();
    //Get Minimum and maximum values from cloud
      CLOUDMinValue = Firebase.getInt("sensor2/minimum temperature");
      //Serial.println(CLOUDMinValue);
      CLOUDMaxValue = Firebase.getInt("sensor2/maximum temperature");   
      //Serial.print("Max value = ");Serial.println(CLOUDMaxValue);
      delay(200);
      if (CLOUDMinValue !=0) {
                  //Serial.println("setting minimum value");
                  
                  T1 = "MINTEMP:";
                  T2 = (int)CLOUDMinValue*100;
                  T3 = "\n";
                  T1 = T1+T2+T3;
                  T1.toCharArray(SendTheData,300);
                  Serial.print(SendTheData);
                  //Serial.print("MINTEMP= ");Serial.println(SendTheData);
                  delay(100);
             }
             if (CLOUDMaxValue !=0) {
              
                  T1 = "MAXTEMP:";
                  T2 = (int)CLOUDMaxValue*100;
                  T3 = "\n";
                  T1 = T1+T2+T3;
                  T1.toCharArray(SendTheData,300);
                  Serial.print(SendTheData);
                  delay(100);
           }
//    
//      if((CurrentTemperature > 4.00)&&(CurrentTemperature != BugTemperature))
//      {
//        Serial.print("temperature = ");Serial.println(CurrentTemperature);
//        Serial.print("Bug = ");Serial.println(BugTemperature);
//        Firebase.pushFloat("sensor2/temperature",CurrentTemperature);
//      }
//      else{
//        //Serial.println("garbage value");
//      }
//      if(CurrentVoltage>4.00){
//        //Serial.print("voltage = ");Serial.println(CurrentVoltage);
//        Firebase.pushFloat("sensor2/voltage",CurrentVoltage);
//      }
//      else{
//        //Serial.println("voltage garbage");
//      }
//      if(AcCurrent>0.00)
//      {
//        Firebase.pushFloat("sensor2/Amps",AcCurrent);
//      }
      
      
//      
//           if(EEPROMMaxValue != 0)
//           {
//            Serial.print("EEprom Max value = ");Serial.println(EEPROMMaxValue);
//           }
//           if(EEPROMMinValue != 0)
//           {
//            Serial.print("EEprom Min value = ");Serial.println(EEPROMMinValue);
//           }
           
           
}

void ReadIncomingData() {
      ReadTheData ="";
       while(Serial.available()){
            char b =  Serial.read();
            ReadTheData += b; //Wire.read();
       }
       

          if (ReadTheData.indexOf("TEMPA:") >=0) {
                       T1 = ReadTheData.substring(6,8);
                       T2 = ReadTheData.substring(8,10);
                       CurrentTemperature = (T1.toFloat()*100 + T2.toFloat())/100;
                       //Serial.print("temperature");Serial.println(CurrentTemperature);
                       if(CurrentTemperature > 10.00)
                       {
                        Firebase.setFloat("sensor2/temperature",CurrentTemperature);
                       }
          }
           else if (ReadTheData.indexOf("VOLTS:") >=0) {
                         T1 = ReadTheData.substring(6,9);
                         T1_bug = ReadTheData.substring(6,8);
                         T2_bug = ReadTheData.substring(8,10);
                         CurrentVoltage = T1.toFloat();
                         BugTemperature = (T1_bug.toFloat()*100 + T2_bug.toFloat())/100;
                         if(CurrentVoltage > 10.00)
                         {
                          Firebase.setFloat("sensor2/voltage",CurrentVoltage);
                         }
                         
          }
          else if (ReadTheData.indexOf("AMPS :") >=0) {
                         T1 = ReadTheData.substring(6,9);
                         AcCurrent = T1.toFloat();
                         Firebase.setFloat("sensor2/Amps",AcCurrent);
                         
          }
          else if (ReadTheData.indexOf("MIN-EEPROM:") >=0) {
                  T1 = ReadTheData.substring(11,13);
                  T2 = ReadTheData.substring(13,15);
                  EEPROMMinValue = (T1.toFloat()*100 + T2.toFloat())/100;
          }
          else if (ReadTheData.indexOf("MAX-EEPROM:") >=0) {
                  T1 = ReadTheData.substring(11,13);
                  T2 = ReadTheData.substring(13,15);
                   EEPROMMaxValue = (T1.toFloat()*100 + T2.toFloat())/100;
          }
                       /*if(CurrentTemperature > 27)
                          {
                          thing.call_endpoint("temp_point");
                          }*/
}


 
