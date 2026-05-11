#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int potensioPin = A0;
const int pinLED = 5; 
const int batas = 45;
const bool nyala = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

}

void loop() {
  
  val = analogRead(potensioPin);

  pos = map(val,
            0,
            1023,
            0,  
            90);

  if (pos >= batas){

    digitalWrite(pinLED, LOW);

    lcd.setCursor(0, 0);
    lcd.print("Lampu Menyala");

    lcd.setCursor(0, 1);
    lcd.print(pos);

  }else{

    lcd.setCursor(0, 0);
    lcd.print("lampu Mati");

  } 

  delay(100);
}
