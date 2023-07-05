void loop() 
{ while(1)
    {
      long lop1 = micros();
      // Serial.print("lop1 = ");
      // Serial.println(lop1);
      controlVoltage();
      UDPgetServerData();
      Period = motPeriod();
      setSpeedNow();
      fuzzyReg(float(motSpeed), motSpeedNow);
      serialOut();

      //снимаем показания датчиков температуры и тока
      // и отправляем данные на сервер
      sendData();
      // UDPRead();
      // delay(1000);
      long lop2 = micros();
      // Serial.print("lop2 = ");
      // Serial.println(lop2);
      long lop = lop2 - lop1;
      // Serial.print("Voltage time:");
      // Serial.println((t_U*100)/lop);
    }
}