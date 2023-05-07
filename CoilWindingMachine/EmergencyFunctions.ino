bool checkAbort(){
  unsigned long lastButtonPress1 = 0;
  int btnState1 = digitalRead(SW);
  if (btnState1 == LOW) {
   if (millis() - lastButtonPress1 > 50) {
      return true;
   }
   lastButtonPress1 = millis();
 }
 return false;
}

void flush_menu_item(){
  curr_menu_item = "";
}

void getCoilsno(){
  Coilsno = SD.open("Coils.txt", FILE_READ);
  String buffer;
  String buffer1;
  while (Coilsno.available()) {
    do{
      buffer = buffer1;
      buffer1 = Coilsno.readStringUntil('\n');
    }while(buffer1!= NULL);
    saved_coils_no = buffer.toInt();
    Serial.println(saved_coils_no);
    Coilsno.close();
  }
}
