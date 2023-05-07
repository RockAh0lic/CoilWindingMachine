void sel4(){
        double counter1 = 0;
        int currentStateCLK1;
        int lastStateCLK1;
        String currentDir1 ="";
        unsigned long lastButtonPress1 = 0;
        double mm;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Set distance (mm):");
        lcd.setCursor(0,1);
        bool ok = false;
        while(true){
          currentStateCLK1 = digitalRead(Clk);
          if (currentStateCLK1 != lastStateCLK1  && currentStateCLK1 == 1){
            if (digitalRead(DT) != currentStateCLK1) {
              counter1 -=0.1;
              currentDir1 ="CCW";
            } else {
              // Encoder is rotating CW so increment
              counter1 +=0.1;
              currentDir1 ="CW";
            }
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Set distance(mm):");
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
        mm = counter1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Distance SET!");
        delay(250);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Loading...");
        delay(500);
        if(mm < 0){
          digitalWrite(dirPin_nema17, LOW);
          mm *= -1;
        }
        else
          digitalWrite(dirPin_nema17, HIGH);
        move_carriage_x_mm(mm);
}
