#include "DHT.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Wifi parameters
#define WLAN_SSID       "iPhone"
#define WLAN_PASS       "00000000"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "NeedAName"
#define AIO_KEY         "aio_orUI57evFSgygmTG6cZshWBD5BzW"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish SG_Temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sg-temp");
Adafruit_MQTT_Publish SG_Humi = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sg-humi");

// DHT parameters
#define DHTPIN          D4 //what digital pin we're connected to

#define DHTTYPE         DHT11 //DHT 11

DHT dht(DHTPIN, DHTTYPE);
byte hum = 0;  //Stores humidity value
byte temp = 0; //Stores temperature value

void setup() {
  Serial.begin(115200);
  Serial.println(F("Adafruit IO Example"));
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

  // connect to adafruit io
  connect();
  
  Serial.println("DHTxx test!");
  delay(10);
  dht.begin();
}

// connect to adafruit io via MQTT
void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}

void loop() {
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print((int)temp); Serial.print(" *C, "); 
  Serial.print((int)hum); Serial.println(" H");
  delay(5000);
   
   if (! SG_Temp.publish(temp)) {                     //Publish to Adafruit
      Serial.println(F("Failed"));
    } 
       if (! SG_Humi.publish(hum)) {                     //Publish to Adafruit
      Serial.println(F("Failed"));
    }
    else {
      Serial.println(F("Sent!"));
    }
}
