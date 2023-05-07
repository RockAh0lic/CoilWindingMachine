void sel1(){
  int counter1 = 0;
  int currentStateCLK1;
  int lastStateCLK1;
  String currentDir1 ="";
  unsigned long lastButtonPress1 = 0;
  lcd.clear();
  lcd.print("Select Coil:");
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
       else if(counter1> saved_coils_no)
         counter1 = saved_coils_no;
       bobina1.loadCoilfromSD(counter1);
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print(bobina1.getName());
       lcd.setCursor(0,1);
       lcd.print("WD:");
       lcd.print(bobina1.getWireDiam());
       lcd.print("    SP:");
       lcd.print(bobina1.getSpinsPerDir());
       
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
  if(counter1<= saved_coils_no)
    bobina1.loadCoilfromSD(counter1);
  else
    bobina1.loadCoilfromSD(saved_coils_no);
}
