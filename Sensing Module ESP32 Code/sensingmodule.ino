
#include <BH1750.h>
#include <Wire.h>


BH1750 lightMeter;
#include <SimpleDHT.h>
int pinDHT11 = 4;
SimpleDHT11 dht11(pinDHT11);
#include <FirebaseESP32.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "WiFi.h"
#define WIFI_SSID "haseeb"
#define WIFI_PASSWORD "200718"
#define FIREBASE_HOST "https://data-logger-31875-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "seMbzJgT54r9KiWlw8wRe2tVzp31m1uOQy03gORY"
const char* ntpServer = "time.google.com";
FirebaseData fbdo;
FirebaseData getdata1;
String formattedTime;
 String currentDate ;
     int cur_Minute =0;
    int pre_Minute =0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday",
"Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May",
"June", "July", "August", "September", "October", "November", "December"};

float temp = 0;
float humi = 0;

void setup() {
  Serial.begin(9600);
    Wire.begin();

  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));
  Serial.println("Start");
  WIFI_Setup();  
    timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(18000);
     
  Serial.println("Connected with success");
   
}

int entryCount = 0;


void loop() {
     


  float v1=0;
  float v2=0;
  float v3=0;
  
  float i1=0;
  float i2=0;
  float i3=0;
 
  float adc1=analogRead(36);
  float adc2=analogRead(39);
  float adc3=analogRead(34);
  for(int a=0;a<1000;a++)
  {
  float adc4=analogRead(35);if(adc4>i1){i1=adc4;}
  float adc5=analogRead(32);if(adc5>i2){i2=adc5;}
  float adc6=analogRead(33);if(adc6>i3){i3=adc6;}
  }

  i1=abs(2791-i1);
  i1=i1/272.00;

  
  i2=abs(2831-i2);
  i2=i2/271.85;

  
  i3=abs(2805-i3);
  i3=i3/737.8;


  v1=adc1/111.65;
  v2=adc2/115.55;
  v3=adc3/5.766;

  float soc=(18*v2)-152.2;
if(soc>100){soc=100;}
if(soc<10){soc=10;}
  float lux = lightMeter.readLightLevel();
 

   Serial.print(" ");Serial.print(v1);
   Serial.print(" ");Serial.print(v2);
   Serial.print(" ");Serial.print(v3);
   Serial.print(" ");Serial.print(i1);
   Serial.print(" ");Serial.print(i2);
   Serial.print(" ");Serial.print(i3);
   Serial.print(" ");Serial.print(soc); 
   Serial.print(" "); Serial.println(lux);

   
   time1();
   DHT();
   

   Firebase.setString(fbdo,"Real_time/Log",currentDate);

   Firebase.setString(fbdo,"Real_time/PV Panel Voltage",v1);
   Firebase.setString(fbdo,"Real_time/PV Panel Current",i1);
   Firebase.setString(fbdo,"Real_time/PV Panel Power",(i1*v1));
   
   Firebase.setString(fbdo,"Real_time/Battery Voltage",v2);
   Firebase.setString(fbdo,"Real_time/Battery Current",i2);
    Firebase.setString(fbdo,"Real_time/Battery SOC",soc);
    
   Firebase.setString(fbdo,"Real_time/AC Load Voltage",v3);
   Firebase.setString(fbdo,"Real_time/AC Load Current",i3);
   Firebase.setString(fbdo,"Real_time/AC Load Power",(i3*v3));

   
   Firebase.setString(fbdo,"Real_time/LUX (Light Intensity)",lux);
   Firebase.setString(fbdo,"Real_time/Temperature",temp);
   Firebase.setString(fbdo,"Real_time/Humidity",humi);
  
   

   


if(cur_Minute != pre_Minute)
{
 pre_Minute=cur_Minute;
if (Firebase.ready()){
 String key = String(++entryCount);
    
    Firebase.setString(fbdo, "/Log/" + key, String(currentDate));
    
    Firebase.setString(fbdo, "/PV Panel Voltage/" + key, String(v1));
    Firebase.setString(fbdo, "/PV Panel Current/" + key, String(i1));
    Firebase.setString(fbdo, "/PV Panel Power/" + key, String(i1*v1));

    Firebase.setString(fbdo, "/Battery Voltage/" + key, String(v2));
    Firebase.setString(fbdo, "/Battery Current/" + key, String(i2));
    Firebase.setString(fbdo, "/Battery SOC/" + key, String(soc)); 

    
    Firebase.setString(fbdo, "/AC Load Voltage/" + key, String(v3));
    Firebase.setString(fbdo, "/AC Load Current/" + key, String(i3));
    Firebase.setString(fbdo, "/AC Load Power/" + key, String(i3*v3));
   
    Firebase.setString(fbdo, "/LUX (Light Intensity)/" + key, String(lux)); 
    Firebase.setString(fbdo, "/Temperature/" + key, String(temp));
    Firebase.setString(fbdo, "/Humidity/" + key, String(humi));  
    
     
     
}}}

 
void time1() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
   formattedTime = timeClient.getFormattedTime();
   formattedTime.replace(":", "$");
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  
  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);    
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  String currentMonthName = months[currentMonth-1];
  int currentYear = ptm->tm_year+1900;
   currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + 
   String(monthDay) + "_" + String(weekDay)+"_";
 // currentDate+="  ";
  currentDate+=formattedTime ;
  formattedTime.replace(":", "$");
  
  Serial.print("Current date: ");
  Serial.println(currentDate);
  Serial.println("");
     cur_Minute = timeClient.getMinutes();
    
  } 

void WIFI_Setup(void)
{ 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}
 
void DHT(void) {
byte temperature = 0;
byte humidity = 0;

// start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");

temp=temperature;
humi=humidity; 
}
