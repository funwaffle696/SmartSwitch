//#include <WiFi.h>
//#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include "StringLog.h"

/* WiFi network name and password */
const char *ssid = "Iwatch";
const char *pwd = "753753753";

//const char *ssid = "dlink-91EC";
//const char *pwd = "753753753";

// IP address to send UDP data to.
// it can be ip address of the server or 
// a network broadcast address
// here is broadcast address
//const char * udpAddress = "192.168.1.100";
const char *udpAddress = "172.20.10.3";
//const char *udpAddress = "192.168.0.103";
//const char *udpAddress = "192.168.0.108";
const int udpPort = 50001;

//create UDP instance
WiFiUDP udp;

void setup(){
  Serial.begin(115200);
  
  //Connect to the WiFi network
   WiFi.begin(ssid, pwd);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //This initializes udp and transfer buffer
  udp.begin(udpPort);
}

void loop(){
  //data will be sent to server
  //uint8_t buffer[50] = "hello world";
  uint8_t buffer[50] = "Device_003";
  //send hello world to server
  udp.beginPacket(udpAddress, udpPort);
  udp.write(buffer, 11);
  udp.endPacket();
  memset(buffer, 0, 50);
  //processing incoming packet, must be called before reading the buffer
  udp.parsePacket();

  //---回傳成功---
  udp.beginPacket(udpAddress, udpPort);
  udp.write("Device_003 success");
  udp.endPacket();
  
  //receive response from server, it will be HELLO WORLD
  if(udp.read(buffer, 50) > 0){
    Serial.print("Server to client: ");
    Serial.println((char *)buffer);

    //---當收到回傳值為a時 啟動馬達
    if(*buffer=='a')
    {
      Serial.println("hello world");
      digitalWrite(D1, HIGH);
      //delay(3000);
      //digitalWrite(D1, LOW);
      //delay(3000);
    }
    //---當收到回傳值為b時 停止馬達
    else if(*buffer=='b')
    {
      Serial.println("hello world");
      digitalWrite(D1, LOW);
      //delay(3000);
      //digitalWrite(D1, LOW);
      //delay(3000);
    }
    else
    {
      Serial.println("no hello world");
    }        
  }
  //Wait for 1 second
  delay(1000);
}
