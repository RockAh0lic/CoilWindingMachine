
void sel2(){
  double diam = 0;
  int nb_spins = 0;
  int dir_changes = 0;
  double counter1 = 0;
  int currentStateCLK1;
  int lastStateCLK1;
  String currentDir1 ="";
  unsigned long lastButtonPress1 = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set wire diam");
  lcd.setCursor(0,1);
  lcd.print("mm:");
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
       diam = counter1/4;
       lcd.setCursor(0,1);
       lcd.print("mm:");
       lcd.print(diam);
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
  diam = counter1/ 4;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wire diam SET!");
  delay(250);
  counter1 = 0;
  currentStateCLK1;
  lastStateCLK1;
  currentDir1 = "";
  lastButtonPress1 = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set nb of");
  lcd.setCursor(0,1);
  lcd.print("spins per dir");
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Spins:");
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
       if(counter1 < 1)
          counter1 = 1;
       //diam = counter1/100;
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
  nb_spins = counter1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Nb of spins SET!");
  delay(250);
  counter1 = 0;
  currentStateCLK1;
  lastStateCLK1;
  currentDir1 = "";
  lastButtonPress1 = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set nb of");
  lcd.setCursor(0,1);
  lcd.print("dir changes");
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dir changes:");
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
       if(counter1 < 1)
          counter1 = 1;
       //diam = counter1/100;
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
  dir_changes = counter1;
  bobina1.setWireDiam(diam);
  bobina1.setSpinsPerDir(nb_spins);
  bobina1.setNbDirChanges(dir_changes);
  bobina1.setNema34Spd(10);
  saved_coils_no++;
  bobina1.SaveToSD(String(saved_coils_no));
}
