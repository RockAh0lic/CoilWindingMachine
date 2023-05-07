char alphabet[40] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

String Write_Name() {
  lcd.clear();
  lcd.print("Loading...");
  delay(500);
  int counter1 = 0;
  int currentStateCLK1;
  int lastStateCLK1;
  unsigned long lastButtonPress1 = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Name:");
  lcd.setCursor(0, 1);
  bool ok = false;
  int char_nb = 0;
  String name1;
  while (!ok) {
    currentStateCLK1 = digitalRead(Clk);
    if (currentStateCLK1 != lastStateCLK1  && currentStateCLK1 == 1) {
      if (digitalRead(DT) != currentStateCLK1)
        counter1 --;
      else
        counter1 ++;
      if(counter1 < -1)
        counter1 = -1;
      if(counter1 > 26)
        counter1 = 26;
      lcd.setCursor(char_nb, 1);
      lcd.print(alphabet[counter1]);
      //lcd.print(counter1);
    }
    lastStateCLK1 = currentStateCLK1;
    int btnState1 = digitalRead(SW);
    if (btnState1 == LOW) {
      if (millis() - lastButtonPress1 > 50) {
        if(counter1 >= 0 && counter1 <= 25){
          name1 += alphabet[counter1];
          char_nb++;
          //ok = true;
        }
        else if(counter1 < 0 || counter1 > 25){
          ok = true;
          Serial.println( name1 );
        }
        else
          return name1;
      }
      lastButtonPress1 = millis();
      delay(10);   
    }
    delay(10);  
  }
  return name1;
}
