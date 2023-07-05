
// Вывод значений двигателя в Serial port
void serialOut(void)
  { 
    if (millis()-serialTime>10)
    {
      serialTime = millis();
      // Serial.print("motSpeed");Serial.print("\t");
      // Serial.print("motSpeedNow = ");Serial.print("\t");
      // Serial.print("motPWM = ");Serial.print("\n");
      // Serial.print(motSpeed);Serial.print("\t");
      // Serial.print(motSpeedNow);Serial.print("\t");
      // Serial.println(output);
      Serial.print(motSpeed);Serial.print(",");
      Serial.print(motSpeedNow);Serial.print(",");
      Serial.print(abs(motSpeed-motSpeedNow)/motSpeed*100);
      Serial.print(",");
      Serial.print(out);
      Serial.print(",");
      Serial.println(millis());
    }  
  }  