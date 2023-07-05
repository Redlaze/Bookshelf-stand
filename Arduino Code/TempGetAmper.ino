#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 17  // DS18B20 pin D31 on Mega
#define AMPER 32
#define TEMPERATURE_PRECISION 12
#define freq_motor 19
#define U_VKB 33
OneWire oneWire(ONE_WIRE_BUS);

// передаем объект oneWire объекту sensors: 
DallasTemperature DS18B20(&oneWire);

//Данные по датчикам температуры
DeviceAddress ADRTCoil1 = {0x28, 0x47, 0xD0, 0x79, 0xA2, 0x00, 0x03, 0xD0}; //
DeviceAddress ADRTCoil2 = {0x28, 0x5D, 0x48, 0x79, 0xA2, 0x00, 0x03, 0xCD}; //
DeviceAddress ADRStator = {0x28, 0xDE, 0x0A, 0x79, 0xA2, 0x00, 0x03, 0x14}; //

// переменная для хранения адресов устройств
DeviceAddress Thermometer;

int deviceCount = 0;
float coefAmper = 2.8; // коэффициент преобразования значния ЦАП в действительный тока 
float DS_data[3];
bool OK_DS = 0;

// Инициализация датчиков температуры
void tempInit()
  {
    pinMode(AMPER, INPUT);
    DS18B20.begin();

    // найти устройства на шине
    // Serial.println("Locating devices...");
    // Serial.print("Found ");
    deviceCount = DS18B20.getDeviceCount();
    // Serial.print(deviceCount, DEC);
    // Serial.println(" devices.");
    // Serial.println("");
    
    // Serial.println("Printing addresses...");
    for (int i = 0;  i < deviceCount;  i++)
    {
      // Serial.print("Sensor ");
      // Serial.print(i+1);
      // Serial.print(" : ");
      DS18B20.getAddress(Thermometer, i);
      // printAddress(Thermometer);
      for (uint8_t i = 0; i < 8; i++)
      {
        // Serial.print("0x");
        // if (Thermometer[i] < 0x10) Serial.print("0");
        // Serial.print(Thermometer[i], HEX);
        // if (i < 7) Serial.print(", ");
      }
      // Serial.println("");
      
    }
    
    DS18B20.setResolution(ADRTCoil1, 12);
    DS18B20.setResolution(ADRTCoil2, 12);
    DS18B20.setResolution(ADRStator, 12);
  }


void Get_Temper_Serial()
  {   
    DS_data[0] = DS18B20.getTempC(ADRTCoil1);
    DS_data[1] = DS18B20.getTempC(ADRTCoil2);
    DS_data[2] = DS18B20.getTempC(ADRStator);      // получили данные мониторинга температуры

    DS18B20.requestTemperatures();                // новое преобразование
      
    // Serial.print("TCoil1:\t\t");
    // Serial.println(DS_data[0], 3);
    
    // Serial.print("TCoil2:\t\t");
    // Serial.println(DS_data[1], 3);

    // Serial.print("ADRStator:\t");
    // Serial.println(DS_data[2], 3);                // получили данные мониторинга температуры
    
    if ((DS_data[0]>0.0 && DS_data[0]<70) && (DS_data[1]>0.0 && DS_data[1]<70) && (DS_data[2]>0.0 && DS_data[2]<70))
      {
      OK_DS=1;
      }
  }


// считывание значения с датчика тока
float getAmperage()
  {
    float amperage = analogRead(AMPER)/coefAmper;
    return amperage; 
  }

// вычисление среднего значения тока
void controlVoltage(void)
  {
    // long t1 = micros();     
    U_count += getU();
    // long t2 = micros();
    // t_U = t2 - t1;
    // Serial.println(t_U);
    if (U_tact==10000)
        {
          U_v = U_count / (10000*92.6173);
          Serial.print("Control amperage = ");
          Serial.println(U_v);
          U_tact = 0;
          U_count = 0;
        }
    U_tact++;    
  }

float getU(void)
  {
    float U = analogRead(U_VKB);
    return U;
  }



// формирование пакета данных в байтах
void dataPacket(byte dataArray[20])
  {
    float motS = float(motSpeed);     
    dataArray[0] = ((uint8_t*)&DS_data[0])[0];
    dataArray[1] = ((uint8_t*)&DS_data[0])[1];
    dataArray[2] = ((uint8_t*)&DS_data[0])[2];
    dataArray[3] = ((uint8_t*)&DS_data[0])[3];

    dataArray[4] = ((uint8_t*)&DS_data[1])[0];
    dataArray[5] = ((uint8_t*)&DS_data[1])[1];
    dataArray[6] = ((uint8_t*)&DS_data[1])[2];
    dataArray[7] = ((uint8_t*)&DS_data[1])[3];
    
    dataArray[8] = ((uint8_t*)&DS_data[2])[0];
    dataArray[9] = ((uint8_t*)&DS_data[2])[1];
    dataArray[10] = ((uint8_t*)&DS_data[2])[2];
    dataArray[11] = ((uint8_t*)&DS_data[2])[3];

    dataArray[12] = ((uint8_t*)&amper)[0];
    dataArray[13] = ((uint8_t*)&amper)[1];
    dataArray[14] = ((uint8_t*)&amper)[2];
    dataArray[15] = ((uint8_t*)&amper)[3];

    dataArray[16] = ((uint8_t*)&motS)[0];
    dataArray[17] = ((uint8_t*)&motS)[1];
    dataArray[18] = ((uint8_t*)&motS)[2];
    dataArray[19] = ((uint8_t*)&motS)[3]; 

    dataArray[20] = ((uint8_t*)&U_v)[0];
    dataArray[21] = ((uint8_t*)&U_v)[1];
    dataArray[22] = ((uint8_t*)&U_v)[2];
    dataArray[23] = ((uint8_t*)&U_v)[3];     
  }


void sendData(void)
  {
    if ((millis()-diagTime)>diagPeriod)
      {
        diagTime = millis();
        Get_Temper_Serial();
        amper = getAmperage();
        // Serial.print("amperage: ");
        // Serial.println(amper);
        // формируем пакет в байтах  
        dataPacket(dataArray);
        // побайтово отправляем данные на сервер
        UDPSend();
      }
  }