// #include "esp_wifi.h"
// #include <WiFi.h>
// #include <WiFiMulti.h>
// #include <SPI.h>
// #include "Arduino.h"

// WiFiMulti WiFiMulti;
// WiFiClient client;


// // Инициализируем Wi-Fi модуль
// void WifiInit()
//   {
//     WiFiMulti.addAP("APP308_VIV", "IgorVladimiro"); 
//     Serial.println();
//     Serial.println();
//     Serial.print("Waiting for WiFi... ");
//     while(WiFiMulti.run() != WL_CONNECTED) 
//     {
//       Serial.print(".");
//       delay(500);
//     }

//     Serial.println("");
//     Serial.println("WiFi connected");
//     Serial.println("IP address: ");
//     Serial.println(WiFi.localIP());

//     delay(500);
//     // const uint16_t port = 3333;
//     const char * host = "192.168.31.142"; // ip or dns   
//     const uint16_t port = 3334;
//     //const char * host = "89.108.99.89"; // ip or dns

//     Serial.print("Connecting to ");
//     Serial.println(host);    
//     // Use WiFiClient class to create TCP connections
//     if (!client.connect(host, port)) 
//     {
//       Serial.println("Connection failed.");
//       Serial.println("Waiting 0.5 seconds before retrying...");
//       delay(500);
//       return;
//     }  
//   }


// // Отправка сформированного пакета из 20 байтов
// void ClientSend()
//   {
//     if (!client.connect("192.168.31.142", 3334)) 
//       {

//         Serial.println("Connection to host failed");
//         return;
//       }
//     client.write((byte*)&dataArray, sizeof(dataArray));
//     // client.stop();
//   }


// // Прослушивание веб-сервера для получения нового значения частоты 
// void ClientRead(void)
//   {
//     // client_time = millis();
//     client.connect("192.168.31.142", 3338);
//     delay(300);
//     fr = "";
//     while (client.available()) 
//     {
//       flag = 1;
//       char c = client.read();
//       fr+=(String)c;
//     }
//     // client.stop();
//   }


// // Разрыв подключения с сервером
// void client_stop()
//   {
//     client.stop();
//   }


// // Получения данных с сервера о частоте и формирования строки в целое число
// String getServerData(void)
//   {
//     if (millis()-clientTime>=3000)
//       {
//         clientTime = millis(); 
//         ClientRead();
//         if (flag == 0)
//           {
//             return ""; // Если ничего не пришло, ничего не возвращаем
//           }
//         if (flag == 1)
//         {
//           motSpeed = fr.toInt();
//           flag = 0;
//           return "OK";
//         }
//       }
//     if (millis()-clientTime<3000)
//       {
//         return "";
//       }        
//   }
// String getFirstServerData()
//   {
//     client.connect("192.168.31.142", 3338);
//     delay(300);
//     fr = "";
//     while (client.available()) 
//       {
//         char c = client.read();
//         fr+=(String)c;
//       }
//     if (fr.length()==0)
//       {
//         return ""; // Если ничего не пришло, ничего не возвращаем
//       }
//     if (fr.length()>0)
//     {
//       motSpeed = fr.toInt();
//       return "OK";
//     }       
//     // client.stop();
//   }  
