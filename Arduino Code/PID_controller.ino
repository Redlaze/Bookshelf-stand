// // Функция для расчета управляющего сигнала
// void calculatePWM(float s_true, float s_now)
//   {
//     if ((millis()-motTime)>100)
//       {
//         if (s_tr==0 and s_true!=0)
//           {
//             previousError = 0;
//             integral = 0;
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
//         motTime = millis();
//         // Расчет ошибок
//         error = s_true - s_now;
//         // if (error<3 and error>-3)
//         //   {
//         //     error = 0;
//         //     // motSpeedNow = s_true;
//         //   }
//         integral += error;
//         derivative = error - previousError;

//         // Расчет управляющего сигнала (ШИМ)
//         output = 200 + Kp * error + Ki * integral + Kd * derivative;

//         // Ограничение значения ШИМ-сигнала в диапазоне 0-255
//         if (output > 800) 
//           {
//             output = 800;
//           }
//         if (output < 200) 
//           {
//             output = 0;
//           }
//         // if (abs(PWMmot-(int)output)<70)
//         // {
//         //   PWMmot = (int)output;
//         // }  
//         // if (abs(PWMmot-(int)output)>=70)
//         // {
//         //   Serial.println("Too long delta");
//         // }
//         setMotorSpeed((int)output);
//         previousError = error;
//       }
//   }

// void returnDirStop(void)
//   {
//     if (s_tr==0 and s_true!=0)
//           {
//             e = 0;
//             de = 0;
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
//   }