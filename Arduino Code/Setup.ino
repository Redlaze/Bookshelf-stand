
// uint8_t numofrules = 0;



// struct rule rules[12];

void setup()
  {
    // WifiInit();
    Serial.begin(115200);
    UDPInit();
    motorInit();
    tempInit();
    // инициализируем обратную связь двигателя
    pinMode(19, INPUT);
    // ждем данные от сервера
    while(UDPFirstServerData()!="OK");
    setBrakeDir();
    delay(100);
    
    addrule(fN0, AND, fN0, fN0);
    addrule(fVLN, AND, fVLN, fVLP);
    addrule(fVLP, AND, fVLP, fVLN);
    addrule(fLN, AND, fSN, fVLP);
    addrule(fLP, AND, fSP, fVLN);
    addrule(fSN, AND, fSN, fSP);
    addrule(fSP, AND, fSP, fSN);
    addrule(fMN, AND, fMP, fSP);
    addrule(fMN, AND, fMN, fLP);
    addrule(fMP, AND, fMN, fSN);
    addrule(fMP, AND, fMP, fLN);
  }  

void fuzzyReg(float s_true, float s_now)
  {
    if ((millis()-fuzzyTime)>10) 
      {
        if (s_tr==0 and s_true!=0)
          {
            error = 0;
            derror = 0;
            // s_now = 0;
            out = 200;
            Serial.println("Setup");
            setMotorSpeed(0);
            delay(100);
            pwm.setPWM(PWMmotStop,0, 4095);   
            pwm.setPWM(PWMmotDir,0, 4095);
            delay(100);  
            setBrakeDir();
            delay(100);
            // setMotorSpeed(200);
          }
        s_tr = s_true;
        fuzzyTime = millis();
        error = s_now - s_true;
        derror = (error - previousError);
        out += int(processRules(error, derror));
        if (out > 2000) 
          {
            out = 2000;
          }
        if (s_true==0 and out < 0) 
          {
            out=0;
          }
        if (out<-1000)
        {
          out=0; 
        }
        setMotorSpeed(out);
        previousError = error;          
      }   
  }


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