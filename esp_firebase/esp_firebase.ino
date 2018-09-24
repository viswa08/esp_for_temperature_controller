#include <ESP8266WiFi.h>

#include <OneWire.h>
#include <DallasTemperature.h>
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
float CurrentTemperature, CurrentVoltage, AcCurrent;
int test=25;
float val;
float MIN,MAX;
float EEPROMMinValue,EEPROMMaxValue;
float CLOUDMinValue, CLOUDMaxValue;
float Value;
int RData;

String ReadTheData,T1,T2;
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

}
void loop() 
{
    delay(500);
    ReadIncomingData();
    Firebase.pushFloat("sensor1/temperature",CurrentTemperature );
    // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
     
      delay(2000);
  }

      Firebase.pushFloat("sensor1/voltage",CurrentVoltage );
    // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
     
      delay(2000);
  }

        Firebase.pushFloat("sensor1/Amps",AcCurrent );
    // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
     
      delay(2000);
  }

      Firebase.setFloat("sensor1/EEprom min temperature",EEPROMMinValue);
          // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
     
      delay(2000);
  }

        Firebase.setFloat("sensor1/EEprom max temperature",EEPROMMaxValue);
          // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
     
      delay(2000);
  }
  
  CLOUDMinValue = Firebase.getFloat("Sensor1/minimum temperature");
  CLOUDMaxValue = Firebase.getFloat("Sensor1/maximum temperature");

  if (CLOUDMinValue !=0) {
                  T1 = "MINTEMP:";
                  T1+=CLOUDMinValue;
                  T1.toCharArray(SendTheData,300);
                  Serial.write(SendTheData);
                  delay(100);
             }
           if (CLOUDMaxValue !=0) {
                  T1 = "MAXTEMP:";
                  T1+=CLOUDMaxValue;
                  T1.toCharArray(SendTheData,300);
                  Serial.write(SendTheData);
                  delay(100);
           }
           
}


 void ReadIncomingData() {
  delay(150);
      ReadTheData ="";
       while(Serial.available()){
            char b =  Serial.read();
            ReadTheData += b; //Wire.read();
       }

          if (ReadTheData.indexOf("TEMPA:") >=0) {
                         T1 = ReadTheData.substring(6,8);
                         T2 = ReadTheData.substring(8,10);
                         CurrentTemperature = (T1.toFloat()*100 + T2.toFloat())/100;
                         Serial.print("Temperature = ");Serial.println(CurrentTemperature);
                       
          }
           else if (ReadTheData.indexOf("VOLTS:") >=0) {
                         T1 = ReadTheData.substring(6,9);
                         CurrentVoltage = T1.toFloat();
                        // Serial.print("voltage = ");Serial.println(CurrentVoltage);
         
          }
          else if (ReadTheData.indexOf("AMPS :") >=0) {
                         T1 = ReadTheData.substring(6,9);
                         AcCurrent = T1.toFloat();
                        // Serial.print("Current = ");Serial.println(AcCurrent);
                        
          }
          else if (ReadTheData.indexOf("MIN-EEPROM:") >=0) {
                  T1 = ReadTheData.substring(11,13);
                  T2 = ReadTheData.substring(13,15);
                  EEPROMMinValue = (T1.toFloat()*100 + T2.toFloat())/100;
                  //Serial.print("EE min = ");Serial.println(EEPROMMinValue);
                  
          }
          else if (ReadTheData.indexOf("MAX-EEPROM:") >=0) {
                  T1 = ReadTheData.substring(11,13);
                  T2 = ReadTheData.substring(13,15);
                   EEPROMMaxValue = (T1.toFloat()*100 + T2.toFloat())/100;
                   //Serial.print("EE max = ");Serial.println(EEPROMMaxValue);
          }
                       /*if(CurrentTemperature > 27)
                          {
                          thing.call_endpoint("temp_point");
                          }*/
}



