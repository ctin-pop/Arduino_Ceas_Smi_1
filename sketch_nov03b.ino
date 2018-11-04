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
  t = now();
  /*if(x > 715 && x < 725)
    configTime();
  else*/
    digitalClockDisplay(t);
  
}

void configTime(){
  int sel = 0;

  x = analogRead(A0);
  if(x > 715 && x < 725){
    
  }
  else
  
  do{
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CONFIGURARE CEAS:");
  lcd.setCursor(0,1);
  switch(sel){
    case 0:
    lcd.print("Ora: ");
    break;
    case 1:
    lcd.print("Minutul: ");
    break;
    case 2:
    lcd.print("Secunda: ");
    break;
    case 3:
    lcd.print("Ziua: ");
    break;
    case 4:
    lcd.print("Luna: ");
    break;
    case 5:
    lcd.print("Anul: ");
    break;
  }
  }while(!(x > 715 && x < 725));
}

void digitalClockDisplay(time_t t){
  lcd.setCursor(0,0);
  lcd.print(String(hour(t)));
  printDigits(minute(t));
  printDigits(second(t));
  lcd.write(" ");
  lcd.print(roWeekDay(weekday(t)));
  lcd.setCursor(0,1);
  lcd.print(String(day(t)));
  lcd.write("/");
  lcd.print(String(month(t)));
  lcd.write("/");
  lcd.print(String(year(t)));
}

String roWeekDay(int wkd){
  switch(wkd){
    case 0: return "DUM";
    case 1: return "LUN";
    case 2: return "MAR";
    case 3: return "MIE";
    case 4: return "JOI";
    case 5: return "VIN";
    case 6: return "SAM";
  }
}

void printDigits(int digits){
  lcd.write(":");
  if(digits < 10)
    lcd.write("0");
  lcd.print(String(digits));
}
