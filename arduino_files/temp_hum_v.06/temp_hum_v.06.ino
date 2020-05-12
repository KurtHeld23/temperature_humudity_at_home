/*
 Kultigin Bozdemir @2019
 Aim of the project: 
 to campute temperature and humudity at home, store it in the icloud, do some analyses aftermath. 
 */
//Libraries
#include <DHT.h>;
#include <Firebase_Arduino_WiFiNINA.h>;
#include <WiFiNINA.h> ;
#include <NTPClient.h>;
#include <WiFiUdp.h>;


#define FIREBASE_HOST "kinder-e44fb.firebaseio.com"  
#define FIREBASE_AUTH "JV8FKfweRJfhYC1BMI21YtOpjYxHVLefd35VZv9b"
#define WIFI_SSID "FRITZ!Box 7430 UV"//  FRITZ!Box 7430 UV,  iotlab, FabLab2
#define WIFI_PASSWORD "71665754758831935369"// 71665754758831935369 , iotlab18,FabLabKaL! 

//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
float hum;  //Stores humidity value
float temp; //Stores temperature value
String humdata; // Stores hum and time
String tempdata;// Stores temp with data

//Define Firebase data object
FirebaseData firebaseData;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);


void setup()
{
  Serial.begin(115200);
  dht.begin();

  timeClient.begin();
  
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Provide the autntication data
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

  Serial.println("-----------------------------------");
  Serial.println("----------Begin capturing temp and humudity-----------");
  Serial.println("-----------------------------------");
  Serial.println();


}

void loop()
{// Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    //delay(100); //Delay 0.1 sec.
    

  timeClient.update();
  //Serial.println(timeClient.getDay());
  Serial.println(timeClient.getFormattedDate());
  //Serial.println(timeClient.getFormattedTime());
  //Serial.println(timeClient.getEpochTime());
  Serial.println();
  Serial.println();
  
  humdata=(hum, timeClient.getFormattedDate());
  tempdata=(temp+timeClient.getFormattedDate();
  
  String path1 = "/humidity2:"; // creates a data path in firebase.
  Firebase.pushFloat(firebaseData, path1, humdata); // it stores humidity.
  String path2 = "/tempperature2:"; // creates a data path in firebase.
  Firebase.pushFloat(firebaseData, path2, tempdata); // it stores temperature.

  
  Firebase.pushString(firebaseData, path1, timeClient.getFormattedDate()); // it stores time in path 1.
  Firebase.pushString(firebaseData, path2, timeClient.getFormattedDate()); // it stores time in path 2.
  delay(10000);
  }
  
