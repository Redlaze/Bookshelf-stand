#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <ESP32Ping.h>
// WiFi network name and password:
const char * networkName = "APP308_VIV";
const char * networkPswd = "IgorVladimiro";

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const int udpPort = 3334;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiMulti WiFiMulti; 
WiFiUDP udp;
// AsyncUDP Udp;
IPAddress remote_ip (192, 168, 31, 142);
// Инициализируем Wi-Fi модуль
void UDPInit()
  {
    // WiFiMulti.addAP("APP308_VIV", "IgorVladimiro"); 
    WiFiMulti.addAP("APP308_VIV", "IgorVladimiro");
    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");
    while(WiFiMulti.run() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(500);
    // const uint16_t port = 3333;
    const char * host = "192.168.31.142"; // ip or dns   
    const uint16_t port = 3334;
    //const char * host = "89.108.99.89"; // ip or dns

    Serial.print("Connecting to ");
    Serial.println(host); 
    udp.begin(WiFi.localIP(),udpPort);

    connected = true;   
    // Use WiFiClient class to create TCP connections
    // if (!client.connect(host, port)) 
    // {
    //   Serial.println("Connection failed.");
    //   Serial.println("Waiting 0.5 seconds before retrying...");
    //   delay(500);
    //   return;
    // }  
  }

void ping()
{
  int ret = Ping.ping("192.168.31.79", 3);
  Serial.println(ret);
  int avg_time_ms = Ping.averageTime();
  Serial.println(avg_time_ms);
    if(ret) {
    Serial.println("Success!!");
  } else {
    Serial.println("Error :(");
  }
}
// void WiFiEvent(WiFiEvent_t event){
//     switch(event) {
//       case ARDUINO_EVENT_WIFI_STA_GOT_IP:
//           //When connected set 
//           Serial.print("WiFi connected! IP address: ");
//           Serial.println(WiFi.localIP());  
//           //initializes the UDP state
//           //This initializes the transfer buffer
//           udp.begin(WiFi.localIP(),udpPort);
//           connected = true;
//           break;
//       case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
//           Serial.println("WiFi lost connection");
//           connected = false;
//           break;
//       default: break;
//     }
// }

// Отправка сформированного пакета из 20 байтов
void UDPSend()
  {
    udp.beginPacket("192.168.31.142",udpPort);
    udp.write((byte*)&dataArray, sizeof(dataArray));
    udp.endPacket();
  }

// Прослушивание веб-сервера для получения нового значения частоты 
void UDPRead(void)
  {
    // client_time = millis();
    udp.flush();
    // Serial.println("Start");
    udp.beginPacket("192.168.31.142",udpPort);
    udp.printf("GD");
    udp.endPacket();
    if (udp.parsePacket()>0)
      {
        fr = "";
        char packetData[5];
        udp.read(packetData, sizeof(packetData));
        for(int k=0; k<5;k++)
        {
          if ((packetData[k])=='*')
          {
            break;
          }
          fr+=String(packetData[k]);
        }
        // Serial.print("Fr:");
        // Serial.println(fr);
        flag = 1;      
      } 
      while (udp.parsePacket()) 
        {
          udp.flush();
        }       
    // client.stop();
  }





// Получения данных с сервера о частоте и формирования строки в целое число
String UDPgetServerData(void)
  {
    if (millis()-clientTime>=500)
      {
        clientTime = millis(); 
        UDPRead();
        if (flag == 0)
          {
            return ""; // Если ничего не пришло, ничего не возвращаем
          }
        if (flag == 1)
        {
          motSpeed = fr.toInt();
          flag = 0;
          return "OK";
        }
      }
    if (millis()-clientTime<3000)
      {
        return "";
      }        
  }


String UDPFirstServerData()
  {
    UDPRead();     
    if (flag == 0)
      {
        return ""; // Если ничего не пришло, ничего не возвращаем
      }
    if (flag == 1)
    {
      motSpeed = fr.toInt();
      return "OK";
    }       
    // client.stop();
  }  