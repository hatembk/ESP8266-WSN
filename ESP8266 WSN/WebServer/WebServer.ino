#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

#include <FS.h>

#define ssid      "put your network ssid here"
#define password  ""

float temperature = 0 ;
float hum ;
long previousMillis  = 0 ; 
long interval = 1000 ;

IPAddress ip_esp_client ;  


ESP8266WebServer server ( 80 );
WebSocketsServer webSocket = WebSocketsServer(81);


void setupServer() {
  server.serveStatic("/", SPIFFS, "/index.html");
  server.on("/feed", handle_feed);
  server.on("/led", handle_led) ;
  server.onNotFound(handleNotFound);
  server.begin() ;
}

void setupWebSocketServer() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent) ;

}

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

void handle_feed() {
  String t = server.arg("temp");
  String h = server.arg("hum");
  ip_esp_client = server.client().remoteIP();
  Serial.println("Addresse du esp client " + ip_esp_client.toString()) ;
  
  sendData(t, h) ;
  Serial.println("temp : " + t ) ;
  Serial.println("hum : " + h) ;

}

void handle_led()
{
  String etat = server.arg("etat");
  
  Serial.println("Changing led state to ");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

void sendData(String temp , String hum) {
  
  String json = "{ \"temp\" :";
  json += "\"" + temp + "\"";
  json += "," ;
  json += "\"hum\" :" ;
  json += "\"" + hum + "\"" ;
  json += "}";
  
  webSocket.broadcastTXT(json.c_str(), json.length()) ;
}


void webSocketEvent(uint8_t num ,  WStype_t type , uint8_t * payload , size_t length )
{
       switch(type) {
        case WStype_TEXT:
        
            Serial.printf("Switching builtin led state %s\n", payload);
            String str = (char *) payload; 
            if(str=="1")
              {
                digitalWrite(BUILTIN_LED, HIGH);
              }
              else if(str=="0"){
                 digitalWrite(BUILTIN_LED, LOW);
              }
            break;
    }
}

void setup() {
  
  Serial.begin( 115200) ;
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  
  wifiConnect() ;
  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS Mount failed");
  } else {
à  !h
    Serial.println("SPIFFS Mount succesfull");
  }
  setupServer() ;
  setupWebSocketServer() ;
  
}

void loop() {
  webSocket.loop();
  server.handleClient();
}
