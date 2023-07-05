#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

const int PWMmotRun = 8;
const int PWMmotStop = 9;
const int PWMmotDir = 10;

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


// инициализация пинов, отвечающих за мотор
void motorInit()
  {
    Wire.begin(SDA_PIN,SCL_PIN);
    pwm.begin(); // Инициализация модуля PCA9685
    pwm.setPWMFreq(1000); // Устанавливаем частоту ШИМ (1000 Гц)

    // Устанавливаем начальную скорость вращения вала
    setMotorSpeed(0); // Здесь 0 - минимальная скорость, 4095 - максимальная скорость  
  }


// установка мотора на определенную скорость speed
void setMotorSpeed(int speed) 
  {
    pwm.setPWM(PWMmotRun, 0, speed); // Устанавливаем скорость вращения двигателя
  }


// установка реверса и тормоза (LOW/HIGH)
void setBrakeDir()
  {
      pwm.setPWM(PWMmotStop,0, 0);   
      pwm.setPWM(PWMmotDir,0, 0);  
  }


// Плавный пуск и разгон с последующем регулированием
// s_true -- желаемая частота вращения, полученная с сервера
// s_now -- фактическая частота вращения, полученная по обратной связи
void reg_run(float s_true, float s_now)
  {
    if ((millis()-motTime)>200)
      {
        motTime = millis();
        float s_porgesh = 6.0; // допустимая погрешность частоты
        float pwm_step = 40.0; // шаг регулироваия

        // Если фактическая частота находится в доверительном интервале,
        // то плавно разгоняем двигатель до заданной частоты.
        // В противном случае, понижаем ШИМ для получаения заданной частоты.
        if (s_now<(s_true-s_porgesh))
          {
            motPWM += pwm_step;
          }
        if (s_now>(s_true+s_porgesh))
          {
            if (motPWM>0)
              {
                motPWM -= 10;
              }      
          }
        } 
    setMotorSpeed(int(motPWM));  
  }


// Вычисляем фактическую частоту вращения двигателя 
// через подсчет периода между соседними
// импульсами ШИМ.
// Также в функции предусмотрена защита от бесконечного цикла
// при отсутсвии движения мотора
int motPeriod(void)
  {
    t1 = 0; // начало отсчета сигнала в положении HIGH
    t2 = 0; // конец сигнала в положении HIGH
    t_start = micros(); // время захода программы в данную функцию
    // Пока сигнал находится в нуле, переопределяем t1 до тех пор,
    // пока сигнал станет HIGH
    // при ожидании более 0,1 секунды выходим из циклов, возвращая 0
    while (digitalRead(19)!=0)
      {
        t1 = micros();
        if ((t1-t_start)>100000)
          {
            break;
          }
        if (t1==0)
          {
            break;
          }    
      }
    while (digitalRead(19)!=1)
      {
        if ((t1-t_start)>100000)
          {
            break;
          }     
        if (t1==0)
          {
            break;
          }
      }   
    t_start = micros();
    while (digitalRead(19)!=0)
      {
        t2 = micros();
        if ((t2-t_start)>100000)
          {
            break;
          }
        if (t1==0)
          {
            return 0;
          }    
      }
    return t2-t1;    
  }

// Вычисление действительной частоты вращения двигателя
void setSpeedNow(void)
  {
    if (Period>0 and Period<100000)
      {
        motSpeedNow = 1000000/float(Period);
      }  
    if (motPWM==0)
      {
        motSpeedNow = 0;
      }  
  }    