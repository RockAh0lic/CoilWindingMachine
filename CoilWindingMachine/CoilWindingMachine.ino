#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <String.h>
#include <SPI.h>
#include <AccelStepper.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

AccelStepper nema34(10, 11, 12);

#define dirPin_nema17 2
#define stepPin_nema17 3
#define MS1_nema17 22
#define MS2_nema17 23
#define MS3_nema17 24
#define stepsPerRevolution_nema17 3200

#define dirPin_nema34 13
#define stepPin_nema34 11
#define stepsPerRevolution_nema34 800

#define Clk 9
#define DT  8
#define SW  7

#define SPD_UP 4
#define SPD_DOWN 6

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

bool rotation = true; // true - clockwise false - counterclockwise
#define pas_tija 0.02


int nb_menu_items = 5;
File MENUfile;
File Coilsfile;
File Coilsno;
String curr_menu_item;
int saved_coils_no;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  //SD card
  while (!Serial);
  pinMode(53, OUTPUT);

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {

    Serial.println("initialization failed!");

    while (1);

  }

  Serial.println("initialization done.");

  lcd.print("GATA.");

  // check for MENU.txt file
  if (!SD.exists("MENU.txt")) {
    Serial.println("ERROR - Can't find MENU.txt file!");
    return;  // can't find index file
  }
  Serial.println("SUCCESS - Found MENU.txt file.");

  // check for Coils.txt file
  if (!SD.exists("Coils.txt")) {
    Serial.println("ERROR - Can't find Coils.txt file!");
    return;  // can't find Coils file
  }
  Serial.println("SUCCESS - Found Coils.txt file.");
  getCoilsno();
  delay(2500);

  pinMode(MS1_nema17, OUTPUT);
  pinMode(MS2_nema17, OUTPUT);
  pinMode(MS3_nema17, OUTPUT);
  ///NEMA 17 1/16 stepping
  digitalWrite(MS1_nema17, HIGH);
  digitalWrite(MS2_nema17, HIGH);
  digitalWrite(MS3_nema17, HIGH);

  pinMode(stepPin_nema17, OUTPUT);
  pinMode(dirPin_nema17, OUTPUT);
  pinMode(stepPin_nema34, OUTPUT);
  pinMode(dirPin_nema34, OUTPUT);
  // Initialise rotary encoder and push button
  // Set encoder pins as inputs
  pinMode(Clk, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(Clk);
  pinMode(SPD_UP, INPUT);
  pinMode(SPD_DOWN, INPUT);
}


class Coil {
  public:
    double wire_diam;
    int spins_per_dir;
    int nb_dir_change;
    double nema34_spd;
    String nume;
    Coil() {
      this->wire_diam = 0;
      this->spins_per_dir = 0;
      this->nb_dir_change = 0;
      this->nema34_spd = 0;
    }
    Coil(double wd, int spd, int dir_chg, double nema34) {
      this->wire_diam = wd;
      this->spins_per_dir = spd;
      this->nb_dir_change = dir_chg;
      this->nema34_spd = nema34;
    }
    ~Coil() {
      Serial.print("Object distructed.\n");
    }
    void loadCoilfromSD(int i) {
      String fname = String(i);
      fname += ".txt";
      Serial.println(fname);
      //Coilsfile = SD.open(fname, FILE_READ);
      String buffer;
      bool ok = false;
      File Coilf;
      while (!ok) {
        Coilf = SD.open(fname, FILE_READ);
        delay(100);
        if (Coilf.available()) {
          Serial.println("citim");
          buffer = Coilf.readStringUntil('\n');
          Serial.println(buffer);
          this->nume = buffer;
          buffer = Coilf.readStringUntil('\n');
          Serial.println(buffer);
          this->wire_diam = buffer.toDouble();
          buffer = Coilf.readStringUntil('\n');
          this->spins_per_dir = buffer.toInt();
          buffer = Coilf.readStringUntil('\n');
          this->nb_dir_change = buffer.toInt();
          buffer = Coilf.readStringUntil('\n');
          this->nema34_spd = buffer.toDouble();
          Serial.println(this->wire_diam);
          Serial.println(this->spins_per_dir);
          Serial.println(this->nb_dir_change);
          Serial.println(this->nema34_spd);
          ok = true;
          Coilf.close();
          delay(100);
        }
      }
      //Coil.close();
    }
    void SaveToSD(String i) {
      String fname = i;
      String buffer;
      fname += ".txt";
      Coilsfile = SD.open(fname, FILE_WRITE);
      Coilsfile.println(Write_Name());
      Coilsfile.println(this->wire_diam);
      Coilsfile.println(this->spins_per_dir);
      Coilsfile.println(this->nb_dir_change);
      Coilsfile.println(this->nema34_spd);
      Coilsfile.close();
      saved_coils_no;
      SD.remove("Coils.txt");
      Coilsno = SD.open("Coils.txt", FILE_WRITE);
      Coilsno.println(0);
      Coilsno.println(saved_coils_no);
      Coilsno.close();
    }
    String getName() {
      return this->nume;
    }
    double getWireDiam() {
      return this->wire_diam;
    }
    int getSpinsPerDir() {
      return this->spins_per_dir;
    }
    void setWireDiam(double wd) {
      this->wire_diam = wd;
    }
    void setSpinsPerDir(int spd) {
      this->spins_per_dir = spd;
    }
    void setNbDirChanges(int i) {
      this->nb_dir_change = i;
    }
    void setNema34Spd(double spd) {
      this->nema34_spd = spd;
    }
    void StartCoil() {
      int total_nema34_spins = nb_dir_change;
      int curr_spins = 0;
      int dir = HIGH;
      digitalWrite(dirPin_nema17, HIGH);
      int counter1 = 0;
      int currentStateCLK1;
      int lastStateCLK1;
      int curr_speed = 100;
      while (total_nema34_spins) {
        int curr_spins = this->spins_per_dir;
        while (curr_spins) {
          ////1 rev
          if (digitalRead(SPD_UP) == HIGH) {
            curr_speed -= 5;
            delay(50);
          }
          else if (digitalRead(SPD_DOWN) == HIGH) {
            curr_speed += 5;
            delay(50);
          }
          int i = 400;
          int counter = 0;
          while (i) {
            if (digitalRead(SPD_UP) == HIGH ) {
              curr_speed -= 5;
            }
            if (digitalRead(SPD_DOWN) == HIGH ){
              curr_speed += 5;
            }
            if (curr_speed < 5)
              curr_speed = 1;
            Serial.println(curr_speed);
            digitalWrite(stepPin_nema34, HIGH);
            delay(curr_speed);
            digitalWrite(stepPin_nema34, LOW);
            delay(curr_speed);
            counter++;
            int steps = wire_diam / 0.0025 / 400 * 2;
            while (steps) {
              digitalWrite(stepPin_nema17, HIGH);
              delay(1);
              digitalWrite(stepPin_nema17, LOW);
              delay(1);
              steps--;
            }
            i--;
            if (checkAbort())
              return;
          }
          //move_carriage_x_mm(this->wire_diam);
          ////1 rev
          curr_spins--;
          if (checkAbort())
            return;
        }
        dir = !dir;
        digitalWrite(dirPin_nema17, dir);
        total_nema34_spins--;
        if (checkAbort())
          return;
      }
    }
};

Coil bobina1;

String readLine(int menu_item) {
  MENUfile = SD.open("MENU.txt", FILE_READ);
  String buffer1;
  int ctr = 1;
  while (MENUfile.available() && ctr != menu_item) {
    buffer1 = MENUfile.readStringUntil('\n');
    Serial.println(buffer1); //Printing for debugging purpose
    ctr++;
  }
  MENUfile.close();
  return buffer1;
}

void spin_nema34(int spd, int dir) {
  delay(500);
  double steps = 0;
  if (dir == 1)
    // Set the spinning direction clockwise:
    digitalWrite(dirPin_nema34, HIGH);
  else
    digitalWrite(dirPin_nema34, LOW);
  double delay1 = 1000 / spd;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Revolutions: ");
  lcd.setCursor(0, 1);
  // Spin the stepper motor 1 revolution slowly:
  while (1) {
    // These four lines result in 1 step:
    digitalWrite(stepPin_nema34, HIGH);
    delay(delay1);
    digitalWrite(stepPin_nema34, LOW);
    delay(delay1);
    lcd.setCursor(0, 1);
    lcd.print(steps / stepsPerRevolution_nema34);
    if (checkAbort())
      return;
    steps++;
  }
  return;
}

void move_carriage_x_mm(double x) {
  double steps = x / 0.0025 * 2;
  lcd.print("Loading...");
  for (int i = 0; i < steps; i++) {
    //for (int i = 0; i < coil.Diameter_of_wire / pas_nema17; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin_nema17, HIGH);
    delay(1);
    digitalWrite(stepPin_nema17, LOW);
    delay(1);
    if (checkAbort())
      return;
  }
  lcd.clear();
  lcd.print("Done!");
  return;
}

void Execute(int selection) {
  selection--;
  if (selection == 1) {
    delay(250);
    sel1();
    return;
  }
  else if (selection == 2) {
    delay(250);
    sel2();
    return;
  }
  else if (selection == 3) {
    delay(250);
    sel3();
    return;
  }
  else if (selection == 4) {
    delay(250);
    sel4();
    return;
  }
  else if (selection == 5)
    delay(250);
  bobina1.StartCoil();
  return;
  return;
}


int valRotary, lastValRotary;

void loop() {

  // Read the current state of CLK
  currentStateCLK = digitalRead(Clk);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      currentDir = "CCW";
    } else {
      // Encoder is rotating CW so increment
      counter ++;
      currentDir = "CW";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);

    if (counter > 6)
      counter = 6;
    else if (counter < 1)
      counter = 1;

    curr_menu_item = readLine(counter);
    if (counter > 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(curr_menu_item);
      flush_menu_item();
      lcd.setCursor(0, 1);
      lcd.print(counter - 1);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Welcome!!");
    }
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Read the button state
  int btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
      Execute(counter);
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);
}
