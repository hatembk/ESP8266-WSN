#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define ssid      ""
#define password  ""

String serverHost = "http://host/feed";
String data;
float h;
float t;
long previousMillis  = 0 ;
long interval = 1000 ;
int led = 0 ; 

void wifiConnect()
{
  WiFi.begin ( ssid, password );
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );
}

void sendHttpRequest() {
  HTTPClient http;
  http.begin(serverHost);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST(data);
  http.writeToStream(&Serial);
  http.end();
}

void buildDataStream() {
  data = "temp=";
  data += String(t);
  data += "&hum=";
  data += String(h);
  Serial.println("- data stream: " + data);
}


void setup() {
  Serial.begin( 115200) ;
  wifiConnect() ;
}

void loop() {
  unsigned long now = millis() ;
  h = random(40, 50) * 1.001 ;
  t = random(20, 30) * 1.001 ;
  if (now - previousMillis > interval) {
      previousMillis = now ;
      buildDataStream();
      sendHttpRequest() ;
      Serial.println("Hum :" + String(h) ) ;
      Serial.println("Temp  :" + String(t) ) ;
    }
}
