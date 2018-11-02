#include <ESP8266WiFi.h>

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

String ReadTheData,T1,T2,T1_bug,T2_bug,T3,T4;
char SendTheData[300];

float CurrentMin, CurrentMax; 

//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);


void setup() {

  Serial.begin(9600);

    /* initialization for firebase */
  // connect to wifi.
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectSensor1");
  wifiManager.resetSettings();
   
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //Firebase.setFloat("sensor2/minimum temperature", 0);
  //Firebase.setFloat("sensor2/maximum temperature", 0); 

  
}
void loop() 
{
  
    delay(750);
    ReadIncomingData();
    //Get Minimum and maximum values from cloud
      CLOUDMinValue = Firebase.getFloat("sensor1/minimum temperature");
      if(Firebase.failed()){
        Serial.println("cloud min failed");
        delay(1000);
      }
      //Serial.println(CLOUDMinValue);
      CLOUDMaxValue = Firebase.getFloat("sensor1/maximum temperature"); 
      if(Firebase.failed()){
        Serial.println("cloud max failed");
        
      }  
      //Serial.print("Max value = ");Serial.println(CLOUDMaxValue);
      delay(200);
      if (CLOUDMinValue !=0) {
                  //Serial.println("setting minimum value");
                  
                  T1 = "MINTEMP:";
                  //T2 = (int)CLOUDMinValue*100;
                  T4 = "\n";
                  T2 = String(CLOUDMinValue).substring(0,2);                  
                  T3 = String(CLOUDMinValue).substring(3,5);
                  T1 = T1+T2+T3+T4;
                  T1.toCharArray(SendTheData,300);
                  Serial.print(SendTheData);
                  //Serial.print("MINTEMP= ");Serial.println(SendTheData);
                  delay(100);
             }
             if (CLOUDMaxValue !=0) {
              
                  T1 = "MAXTEMP:";
                  //T2 = (int)CLOUDMaxValue*100;
                  T3 = "\n";
                  T2 = String(CLOUDMaxValue).substring(0,2); 
                  T3 = String(CLOUDMaxValue).substring(3,5);
                  T1 = T1+T2+T3+T4;
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
       
          switch((char) ReadTheData[0]){

            case 'T':
                       T1 = ReadTheData.substring(6,8);
                       T2 = ReadTheData.substring(8,10);
                       CurrentTemperature = (T1.toFloat()*100 + T2.toFloat())/100;
                       if(CurrentTemperature > 10.00)
                       {
                        Serial.print("Temperature = ");Serial.println(CurrentTemperature);
                        Firebase.setFloat("sensor1/temperature",CurrentTemperature);
                        if(Firebase.failed()){
                          Serial.println("Temperature upload failed");
                        }
                        else{
                          Serial.print("Temperature = ");Serial.println(CurrentTemperature);
                        }
                       }
                       break;
                       
            case 'A':
                      T1 = ReadTheData.substring(6,9);
                      AcCurrent = T1.toFloat();
                      //T1+="A";   
                      //Serial.print("Current  = "); Serial.println(T1);
                      Firebase.setFloat("sensor1/Amps",AcCurrent);
                      if(Firebase.failed()){
                          Serial.println("Amps upload failed");
                        }
                      break;

            case 'V':
                      T1 = ReadTheData.substring(6,9);
                      //T1+="v";
                      //Serial.print("Voltage  = "); Serial.println(T1);
                      CurrentVoltage = T1.toFloat();
                      Firebase.setFloat("sensor1/voltage",CurrentVoltage);
                      if(Firebase.failed()){
                          Serial.println("voltage upload failed");
                        }
                      break;

            case 'M':
                      if(ReadTheData.indexOf("MIN-EEPROM:") >=0){
                        T1 = ReadTheData.substring(11,13);
                        T2 = ReadTheData.substring(13,15);
                        EEPROMMinValue = (T1.toFloat()*100 + T2.toFloat())/100;
                        Firebase.setFloat("sensor1/EEprom min temperature",EEPROMMinValue)
                      }

                      else if (ReadTheData.indexOf("MAX-EEPROM:") >=0) {
                      T1 = ReadTheData.substring(11,13);
                      T2 = ReadTheData.substring(13,15);
                      EEPROMMaxValue = (T1.toFloat()*100 + T2.toFloat())/100;
                      Firebase.setFloat("sensor1/EEprom max temperature",EEPROMMaxValue)
                      }
                                              
          }     
}


 
