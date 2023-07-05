#include <math.h>
#include <stdio.h>
#include <stdint.h>
#define fVLN -70 // Очень большое отрицательное
#define fLN -50 // Большое отрицательное
#define fMN -30 // Среднее отрицательное
#define fSN 20 // Малое отрицательное
#define fN0 0 // Нулевое
#define fSP -20 // Малое положительное
#define fMP 30 // Среднее положительное
#define fLP 50 // Большое положительное
#define fVLP 70 // Очень большое положительное

#define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))


typedef enum { AND, OR } op_type;
struct rule
  {
    float fe;
    op_type op;
    float fde;
    float z;
  };
struct rule rules[12];
uint8_t numofrules = 0;

void addrule(float fe, op_type op, float fde, float z)
  {
    rules[numofrules].fe = fe;
    rules[numofrules].op = op;
    rules[numofrules].fde = fde;
    rules[numofrules].z = z;
    numofrules++;
  }

double mu(float x, float center)
  {
    return exp(-(pow(x-center,2)/(2*pow(40,2))));
  }


double processRules (float e, float de)
  {
    double summ_alpha_c = 0, summ_alpha = 0;
    for (int i=0; i<(int)(numofrules);i++)
      {
        double alpha = 0, mue = 0, mude = 0;
        mue = mu(e, rules[i].fe);
        mude = mu(de, rules[i].fde);
        alpha = rules[i].op==0?MIN(mue,mude):MAX(mue,mude);
        summ_alpha_c += (alpha*rules[i].z);
        summ_alpha += alpha;
      } 
    return summ_alpha_c/summ_alpha;         
  }  



int diagTime = 0; // таймер для отправки данных на сервер
float U_count = 0; // сумматор напряжения за 10 итераций (вычисление среднего напряжения)
int U_tact = 0; // счетчик для измерения среднего тока
long client_time = 0; // таймер для опроса сервера
long serialTime = 0;
long clientTime = 0;
long motTime = 0;
long fuzzyTime = 0;
long U_time = 0;
const int clientPeriod = 100;
const int diagPeriod = 10000;
const int serialPer = 10000;

long t_U = 0;



long t1 = 0; // начало отсчета сигнала в положении HIGH
long t2 = 0; // конец сигнала в положении HIGH
long t_start = micros();
float amper = 0;
float U_v = 0;
int Period = 0;
float motSpeedNow = 0.00;
int motSpeed = 0;
int motPWM = 200;
float s_tr = 0;
double output = 200;
int out = 200;
String fr = "";
byte dataArray[24]; // массив для упаковки данных

float derror = 0;
float error = 0;
int flag = 0;
// Параметры ПИД-регулятора
double Kp = 3;   // Коэффициент пропорциональной составляющей
double Ki = 1.5;   // Коэффициент интегральной составляющей
double Kd = 2;   // Коэффициент дифференциальной составляющей

// Переменные для хранения состояний ошибки
// double error = 0;         // Текущая ошибка
float previousError = 0; // Предыдущая ошибка
double integral = 0;      // Сумма ошибок
double derivative = 0;    // Изменение ошибки