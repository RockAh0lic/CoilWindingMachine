void sel3(){
        int counter1 = 0;
        int currentStateCLK1;
        int lastStateCLK1;
        String currentDir1 ="";
        unsigned long lastButtonPress1 = 0;
        int dir, spd;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Set speed mm/sec:");
        lcd.setCursor(0,1);
        bool ok = false;
        while(true){
          currentStateCLK1 = digitalRead(Clk);
          if (currentStateCLK1 != lastStateCLK1  && currentStateCLK1 == 1){
            if (digitalRead(DT) != currentStateCLK1) {
              counter1 --;
              currentDir1 ="CCW";
            } else {
              // Encoder is rotating CW so increment
              counter1 ++;
              currentDir1 ="CW";
            }
            if(counter1 < 1)
              counter1 = 1;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Set speed steps/s:");
            lcd.setCursor(0,1);
            lcd.print(counter1);
          }
           lastStateCLK1 = currentStateCLK1;
           int btnState1 = digitalRead(SW);
           if (btnState1 == LOW) {
            if (millis() - lastButtonPress1 > 50) {
              break;
              ok = true;
            }
            lastButtonPress1 = millis();
          }
          delay(10);
         }
        spd = counter1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Speed SET!");
        delay(250);
        counter1 = 0;
        currentStateCLK1;
        lastStateCLK1;
        currentDir1 = "";
        lastButtonPress1 = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Set direction:");
        lcd.setCursor(0,1);
        ok = false;
        while(true){
          currentStateCLK1 = digitalRead(Clk);
          if (currentStateCLK1 != lastStateCLK1  && currentStateCLK1 == 1){
            if (digitalRead(DT) != currentStateCLK1) {
              counter1 --;
              currentDir1 ="CCW";
            } else {
              // Encoder is rotating CW so increment
              counter1 ++;
              currentDir1 ="CW";
            }
            if(counter < 0)
              counter  = 0;
            if(counter > 1)
              counter = 1;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Set direction:");
            lcd.setCursor(0,1);
            if(counter == 1)
              lcd.print(currentDir1);
            else
              lcd.print(currentDir1);
          }
           lastStateCLK1 = currentStateCLK1;
           int btnState1 = digitalRead(SW);
           if (btnState1 == LOW) {
            if (millis() - lastButtonPress1 > 50) {
              break;
              ok = true;
            }
            lastButtonPress1 = millis();
          }
          delay(10);
         }
         delay(350);
         if(currentDir1 == "CCW")
          dir = 1;
         else 
          dir = -1;
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Loading...");
         delay(500);
         spin_nema34(spd, dir);
}
