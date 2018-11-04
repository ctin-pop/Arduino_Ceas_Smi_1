#include <Time.h>
#include <TimeLib.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);
time_t t;
int x;

boolean selActive;
boolean longPressActive;
long selTimer;
const long longPressTime = 1000;

void setup() {
  
  lcd.begin(16,2);
  selActive = false;
  longPressActive = false;
  selTimer = 0;
}

void loop() {
  x = analogRead(A0);
  t = now();
  if(x > 715 && x < 725){
    if(selActive == false){
      selActive = true;
      selTimer = millis();
    }

    if((millis() - selTimer > longPressTime) && (longPressActive == false)){
      longPressActive = true;
      mainMenu();
    }
  }
    
  else{
    if(selActive == true){
      if(longPressActive == true){
        longPressActive = false;
      }

      selActive = false;
    }
  }

  digitalClockDisplay(t);
}

boolean selActiveMenu;
boolean longPressActiveMenu;
long selTimerMenu;

void mainMenu(){
  int sel = 0;
  int NUM_MENUS = 2 + 1;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MENIU  PRINCIPAL");
  
  while(true){
    x = analogRead(A0);
    
    
    if(x > 715 && x < 725){
      if(selActiveMenu == false){
        selActiveMenu = true;
        selTimerMenu = millis();
      }

      if((millis() - selTimerMenu > longPressTime) && (longPressActiveMenu == false)){
        longPressActiveMenu = true;
        return;
      }
    }
    
    else{
      if(selActiveMenu == true){
        if(longPressActiveMenu == true){
          longPressActiveMenu = false;
        }
        else{
          sel = (sel + 1) % NUM_MENUS;
        }

        selActiveMenu = false;
      }
    }
    
    lcd.setCursor(0,1);
    switch(sel){
      case 0:
      lcd.print("Setare ceas  ");
      break;
      case 1:
      lcd.print("Setare alarma");
      break;
      case 2:
      lcd.print("Format ora   ");
      break;
      case 3:
      lcd.print("Sincronizare ");
      break;
    }
  }
}

boolean selActiveCfg;
boolean longPressActiveCfg;
long selTimerCfg;

void configTime(){
  int cHour = hour(t);
  int cMinute = minute(t);
  int cSecond = second(t);
  int cDay = day(t) - 1;
  int cMonth = month(t) - 1;
  int cYear = year(t) - 1970;
  int sel = 0;
  
  delay(1000);
  lcd.clear();
  x = analogRead(A0);

  lcd.setCursor(0,0);
  lcd.print("CONFIGURARE CEAS:");

  selActiveCfg = false;
  longPressActiveCfg = false;
  selTimerCfg = 0;
  
  do{
  x = analogRead(A0);

  if(x > 715 && x < 725){
    if(selActiveCfg == false){
      selActiveCfg = true;
      selTimerCfg = millis();
    }

    if((millis() - selTimerCfg > longPressTime) && (longPressActiveCfg == false)){
      longPressActiveCfg = true;
      setTime(cHour, cMinute, cSecond, cDay + 1, cMonth + 1, cYear + 1970);
      delay(200);
      lcd.clear();
      return;
    }
  }
    
  else{
    if(selActiveCfg == true){
      if(longPressActiveCfg == true){
        longPressActiveCfg = false;
      }
      else{
        sel = (sel + 1)%6;
      }

      selActiveCfg = false;
    }
  }
  
  lcd.setCursor(0,1);
  
  switch(sel){
    case 0:
    lcd.print("Ora: ");
    lcd.print(cHour);
    cHour = modifDate(x, cHour, sel);
    break;
    
    case 1:
    lcd.print("Minutul: ");
    lcd.print(cMinute);
    cMinute = modifDate(x, cMinute, sel);
    break;
    
    case 2:
    lcd.print("Secunda: ");
    lcd.print(cSecond);
    cSecond = modifDate(x, cSecond, sel);
    break;
    
    case 3:
    lcd.print("Ziua: ");
    lcd.print(cDay + 1);
    cDay = modifDate(x, cDay, sel);
    break;
    
    case 4:
    lcd.print("Luna: ");
    lcd.print(cMonth + 1);
    cMonth = modifDate(x, cMonth, sel);
    break;
    
    case 5:
    lcd.print("Anul: ");
    lcd.print(cYear + 1970);
    cYear = modifDate(x, cYear, sel);
    break;
  }
  lcd.print("      ");
  }while(true);


}
const int periodicity[] = {24,60,60,9999,12,68};
int modifDate(int aRead, int dat, int type){
  if(aRead > 300 && aRead< 310){
    dat = dat - 1;
    if(dat < 0)
      dat = dat + periodicity[type];
    delay(300);
  }
  else if(x > 125 && x < 135){
    dat = dat + 1;
    delay(300);
  }

  return (dat % (periodicity[type]));
}

void digitalClockDisplay(time_t t){
  lcd.setCursor(0,0);
  printDigits(hour(t));
  lcd.write(":");
  printDigits(minute(t));
  lcd.write(":");
  printDigits(second(t));
  lcd.write(" ");
  lcd.print(roWeekDay(weekday(t)));
  lcd.write(" ");
  lcd.print("TEMP");
  lcd.setCursor(0,1);
  lcd.print(String(day(t)));
  lcd.write("/");
  lcd.print(String(month(t)));
  lcd.write("/");
  lcd.print(String(year(t)));
  lcd.write("    ");
}

String roWeekDay(int wkd){
  switch(wkd){
    case 0: return "ER";
    case 1: return "DU";
    case 2: return "LU";
    case 3: return "MA";
    case 4: return "MI";
    case 5: return "JO";
    case 6: return "VI";
    case 7: return "SA";
  }
}

void printDigits(int digits){
  if(digits < 10)
    lcd.write("0");
  lcd.print(String(digits));
}
