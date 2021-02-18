#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

String password = "1234567";
const int servoCount = 10;

const int buzzer = 22;
String products[servoCount] = { "Cake" , "Hamburger" ,
                                "Chocolate" , "Chocolate milk" ,
                                "Milk" , "Snack" , "Fruit juice" ,
                                "Sandys" , "Gummi candy" , "Chewing gum"
                              };
int products_count[servoCount] = {3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3};
const int rs = 16, en = 17, d4 = 18, d5 = 19, d6 = 20, d7 = 21;

const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = {23 , 25 , 27 , 29};
byte colPins[COLS] = {31 , 33 , 35 , 37};
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'c', '0', '=', '+'}
};

Servo mainServo;
Servo machineServos[servoCount];
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16 , 2);
  lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print(" Welcome to the ");
  lcd.setCursor(0 , 1);
  lcd.print("Vending Machine!");


  mainServo.attach(13);
  mainServo.writeMicroseconds(2000);
  for (int i = 0 ; i < servoCount ; i++) {
    machineServos[i].attach(12 - i);
    machineServos[i].writeMicroseconds(2000);
  }

  pinMode(buzzer , OUTPUT);
  pinMode(40 , OUTPUT);
  digitalWrite(40 , HIGH);
  for (int j = 41 ; j <= 50 ; j++) {
    pinMode(j , OUTPUT);
    digitalWrite(j , LOW);
  }
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();
  if (key != NO_KEY) {
    if (key == '+') {
      lcd.clear();
      lcd.setCursor(0 , 0);
      lcd.print("ENTER PASSWORD :");
      lcd.setCursor(0 , 1);
      String pass = "";
      while (true) {
        key = keypad.waitForKey();
        if (isDigit(key)) {
          lcd.print("*");
          pass += key;
        }
        else if (key == '=') {
          if (pass == password) {
            lcd.clear();
            lcd.setCursor(0 , 0);
            lcd.print("   you'r in!!   ");
            lcd.setCursor(0 , 1);
            lcd.print(" wait a moment  ");
            mainServo.writeMicroseconds(1000);
            tone(buzzer , 2000);
            delay(500);
            digitalWrite(40 , LOW);
            for (int i = 0 ; i < servoCount ; i++) {
              machineServos[i].writeMicroseconds(1000);
            }
            delay(500);
            noTone(buzzer);
            for (int j = 41 ; j <= 50 ; j++) {
              digitalWrite(j , HIGH);
            }
            lcd.clear();
            lcd.setCursor(0 , 0);
            lcd.print("  press + when  ");
            lcd.setCursor(0 , 1);
            lcd.print("  you're done   ");
            while (true) {
              key = keypad.waitForKey();
              if (key == '+') {
                for (int i = 0 ; i < servoCount ; i++) {
                  machineServos[i].writeMicroseconds(2000);
                  products_count[i] = 3;
                }
                for (int j = 41 ; j <= 50 ; j++) {
                  digitalWrite(j , LOW);
                }
                tone(buzzer , 2000);
                delay(100);
                noTone(buzzer);
                delay(400);
                mainServo.writeMicroseconds(2000);
                digitalWrite(40 , HIGH);
                delay(500);
                break;
              }
            }
          }
          else {
            lcd.clear();
            lcd.setCursor(0 , 0);
            lcd.print("WRONG PASSWORD!!");
            lcd.setCursor(0 , 1);
            lcd.print("TRY AGAIN LATER!");
            delay(2000);
          }
          break;
        }
        else if (key == 'c') {
          lcd.clear();
          lcd.setCursor(0 , 0);
          lcd.print("ENTER PASSWORD :");
          lcd.setCursor(0 , 1);
          pass = "";
        }
        else if (key == '*') {
          break;
        }
      }
    }
    else if (isDigit(key)) {
      lcd.clear();
      lcd.setCursor(0 , 0);
      lcd.print("CODE : ");
      lcd.print(key);
      String code = "";
      code += key;
      while (true) {
        key = keypad.waitForKey();
        if (isDigit(key)) {
          lcd.print(key);
          code += key;
        }
        else if (key == '=') {
          int p = code.toInt() - 11;
          if (p > 9 || p < 0) {
            lcd.clear();
            lcd.setCursor(0 , 0);
            lcd.print("  there is NO   ");
            lcd.setCursor(0 , 1);
            lcd.print("  product ");
            lcd.print(code);
            delay(1500);
          }
          else if (products_count[p] <= 0) {
            lcd.clear();
            lcd.setCursor(0 , 0);
            lcd.print(" we are out of  ");
            lcd.setCursor(0 , 1);
            lcd.print("  this product  ");
            lcd.print(code);
            delay(1500);
          }
          else {
            lcd.clear();
            lcd.setCursor(0 , 0);
            lcd.print("  here you are  ");
            lcd.setCursor(0 , 1);
            int spaces = (16 - products[p].length()) / 2;
            lcd.setCursor(spaces , 1);
            lcd.print(products[p]);
            machineServos[p].writeMicroseconds(1000);
            digitalWrite(41 + p , HIGH);
            products_count[p] --;
            tone(buzzer , 2000);
            delay(100);
            noTone(buzzer);
            delay(900);
            machineServos[p].writeMicroseconds(2000);
            if (products_count[p] > 0) {
              digitalWrite(41 + p , LOW);
            }
            delay(500);
          }
          break;
        }
        else if (key == '*' || key == 'c') {
          break;
        }
      }
    }
    else if (key == '/') {
      lcd.clear();
      lcd.setCursor(0 , 0);
      lcd.print(" enter the code ");
      lcd.setCursor(0 , 1);
      lcd.print("       ");
      String code = "";
      while (true) {
        key = keypad.waitForKey();
        if (isDigit(key)) {
          lcd.print(key);
          code += key;
        }
        else if (key == '=') {
          int p = code.toInt() - 11;
          if (p > 9 || p < 0) {
            lcd.clear();
            lcd.setCursor(0 , 0);
            lcd.print("  there is NO   ");
            lcd.setCursor(0 , 1);
            lcd.print("  product ");
            lcd.print(code);
            delay(1500);
          }
          else {
            lcd.clear();
            int spaces = (16 - products[p].length()) / 2;
            lcd.setCursor(spaces , 0);
            lcd.print(products[p]);
            lcd.setCursor(0 , 1);
            lcd.print("       ");
            lcd.print(products_count[p]);
            delay(1500);
          }
          break;
        }
        else if (key == '*') {
          break;
        }
        else if (key == 'c') {
          lcd.clear();
          lcd.setCursor(0 , 0);
          lcd.print(" enter the code ");
          lcd.setCursor(0 , 1);
          lcd.print("       ");
          code = "";
        }
      }
    }
    lcd.clear();
    lcd.setCursor(0 , 0);
    lcd.print(" Welcome to the ");
    lcd.setCursor(0 , 1);
    lcd.print("Vending Machine!");
  }
}
