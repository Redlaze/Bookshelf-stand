// #include <math.h>
// #include <Arduino.h>
// #include <Enum_test.h>
// #define fVLN -40 // Очень большое отрицательное
// #define fLN -30 // Большое отрицательное
// #define fMN -20 // Среднее отрицательное
// #define fSN -10 // Малое отрицательное
// #define fN0 -10 // Нулевое
// #define fSP 10 // Малое положительное
// #define fMP 20 // Среднее положительное
// #define fLP 30 // Большое положительное
// #define fVLP 40 // Очень большое положительное

// #define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))
// #define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))




// uint8_t numofrules = 0;

// struct rule
//   {
//     int8_t fe;
//     op_type op;
//     int8_t fde;
//     int8_t z;
//   };

// struct rule rules[12];

// void addRules(void)
//   {
//     addrule(fN0, AND, fN0, fN0);
//     addrule(fVLN, OR, fVLN, fVLP);
//     addrule(fVLP, OR, fVLP, fVLN);
//     addrule(fLN, AND, fSN, fVLP);
//     addrule(fLP, AND, fSP, fVLN);
//     addrule(fSN, AND, fSN, fSP);
//     addrule(fSP, AND, fSP, fSN);
//     addrule(fMN, AND, fMP, fSP);
//     addrule(fMN, AND, fMN, fLP);
//     addrule(fMP, AND, fMN, fSN);
//     addrule(fMP, AND, fMP, fLN);
//   }  

// void fuzzyReg(int s_true, int s_now)
//   {
//     if ((millis()-fuzzyTime)>100) 
//       {
//         if (s_tr==0 and s_true!=0)
//           {
//             error = 0;
//             derror = 0;
//             s_now = 0;
//             Serial.println("Setup");
//             setMotorSpeed(0);
//             delay(100);
//             pwm.setPWM(PWMmotStop,0, 4095);   
//             pwm.setPWM(PWMmotDir,0, 4095);
//             delay(100);  
//             setBrakeDir();
//             delay(100);
//             // setMotorSpeed(200);
//           }
//         s_tr = s_true;
//         fuzzyTime = millis();
//         error = s_now - s_true;
//         derror = error - previousError;
//         out += (int)processRules(error, derror);
//         if (out > 800) 
//           {
//             out = 800;
//           }
//         if (out < 200) 
//           {
//             out = 0;
//           }
//         setMotorSpeed(out);
//         previousError = error;          
//       }   
//   }


// double mu(int8_t x, int8_t center)
//   {
//     return exp(-(pow(x-center,2)/(2*pow(40,2))));
//   }

// void addrule(int8_t fe, op_type op, int8_t fde, int8_t z)
//   {
//     rules[numofrules].fe = fe;
//     rules[numofrules].op = op;
//     rules[numofrules].fde = fde;
//     rules[numofrules].z = z;
//     numofrules++;
//   }

// double processRules (int8_t e, int8_t de)
//   {
//     double summ_alpha_c = 0, summ_alpha = 0;
//     for (int i=0; i<(int)(numofrules);i++)
//       {
//         double alpha = 0, mue = 0, mude = 0;
//         mue = mu(e, rules[i].fe);
//         mude = mu(de, rules[i].fde);
//         alpha = rules[i].op==0?MIN(mue,mude):MAX(mue,mude);
//         summ_alpha_c += (alpha*rules[i].z);
//         summ_alpha += alpha;
//       } 
//     return summ_alpha_c/summ_alpha;         
//   }