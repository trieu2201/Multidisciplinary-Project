#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

// Wifi parameters
#define WLAN_SSID       "HoaiGiang"
#define WLAN_PASS       "hoaigiang1012"

// Light Resistor parameter
#define LEDPIN           D1

// DHT parameters
#define DHTPIN          D4

#define DHTTYPE         DHT11

DHT dht(DHTPIN, DHTTYPE);
float hum = 0;
float temp = 0;
float moisture = 0;
byte  PR = 0;

void setup() {
  Serial.begin(115200);
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  delay(10);
  dht.begin();
  pinMode(A0,INPUT);
  pinMode(D1,INPUT);
}

void loop() {
    String temp1, hum1, moisture1, PR1;
    WiFiClient wifiClient;
  
    hum   = dht.readHumidity();
    temp  = dht.readTemperature();
    Serial.println(hum);
    Serial.println(temp);
    
    moisture = analogRead(A0);
    Serial.println(moisture);

    PR = digitalRead(D1);
    Serial.println(PR);

    temp1 = String(temp, 3);
    temp1 = "{\"value\" :" + temp1 + "}" ; 

    hum1 = String(hum, 3);
    hum1 = "{\"value\" :" + hum1 + "}" ; 

    moisture1 = String(moisture, 3);
    moisture1 = "{\"value\" :" + moisture1 + "}" ; 

    PR1 = String(PR, 3);
    PR1 = "{\"value\" :" + PR1 + "}" ; 
    
    HTTPClient http;
    
    http.begin(wifiClient, "http://danganhapi1.herokuapp.com/api/temperature"); 
    http.addHeader("Content-Type", "application/json");
    
    //int httpCode = http.POST("{\"value\":}");
    int httpCode_t = http.POST(temp1);   
    String payload_t = http.getString();                  

    Serial.println(httpCode_t);  
    Serial.println(payload_t);
 
    http.end(); 
    
    http.begin(wifiClient, "http://danganhapi1.herokuapp.com/api/humidity/"); 
    http.addHeader("Content-Type", "application/json");
 
    int httpCode_h = http.POST(hum1);   
    String payload_h = http.getString();              
 
    Serial.println(httpCode_h);   
    Serial.println(payload_h);
 
    http.end(); 

    http.begin(wifiClient, "http://danganhapi1.herokuapp.com/api/landHumidity"); 
    http.addHeader("Content-Type", "application/json");
 
    int httpCode_m = http.POST(moisture1);   
    String payload_m = http.getString();              
 
    Serial.println(httpCode_m);   
    Serial.println(payload_m);
 
    http.end(); 

    http.begin(wifiClient, "http://danganhapi1.herokuapp.com/api/photoresistor"); 
    http.addHeader("Content-Type", "application/json");
 
    int httpCode_p = http.POST(PR1);   
    String payload_p = http.getString();              
 
    Serial.println(httpCode_p);   
    Serial.println(payload_p);
 
    http.end(); 

    delay(600000);
    
    
}
