#include <Time.h>
#include <TimeLib.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);
time_t t;
int x;

void setup() {
  
  lcd.begin(16,2);
  
}

void loop() {
  x = analogRead(A0);
  if((x > 715 && x < 725))
    configTime();
  else
    digitalClockDisplay(t);
  t = now();
}

void configTime(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CONFIGURARE");
  delay(2000);
}

void digitalClockDisplay(time_t t){
  lcd.setCursor(0,0);
  lcd.print(String(hour(t)));
  printDigits(minute(t));
  printDigits(second(t));
  lcd.write(" ");
  lcd.print(weekday(t));
  lcd.setCursor(0,1);
  lcd.print(String(day(t)));
  lcd.write("/");
  lcd.print(String(month(t)));
  lcd.write("/");
  lcd.print(String(year(t)));
}

void printDigits(int digits){
  lcd.write(":");
  if(digits < 10)
    lcd.write("0");
  lcd.print(String(digits));
}
