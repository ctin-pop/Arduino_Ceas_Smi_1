#include <SFE_BMP180.h>
#include <Wire.h>
#include <DS3231.h>
#include <Time.h>
#include <TimeLib.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);
SFE_BMP180 bmp180;
DS3231 RTC(SDA, SCL);

const int buzz = 2;

time_t t;
time_t alarm;
boolean alarmIsSet;
int x;

boolean selActive;
boolean longPressActive;
long selTimer;
const long longPressTime = 1000;

bool FORMAT_ORA; //1 for AM/PM, 0 for 24H
int SYNC;

void setup() {
  
  lcd.begin(16,2);
  RTC.begin();
  bmp180.begin();
  selActive = false;
  longPressActive = false;
  selTimer = 0;
  alarmIsSet = false;

  FORMAT_ORA = 0;
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

  lcd.setCursor(10,1);
  if(alarmIsSet){
    if(t>alarm){
      lcd.write('B');
      digitalWrite(buzz, HIGH);
      if(x >= 0&& x < 10)
        alarmIsSet = false;
    }
    else
      lcd.write('A');
  }
  else{
    lcd.write(' ');
    digitalWrite(buzz, LOW);
  }

  digitalClockDisplay(t);
}

time_t syncWithRTC(){
  return RTC.getUnixTime(RTC.getTime());
}

time_t syncOff(){
  return 0;
}

time_t syncWithBLU(){
  return 0;
}

boolean selActiveMenu;
boolean longPressActiveMenu;
long selTimerMenu;

void mainMenu(){
  int sel = 0;
  int NUM_MENUS = 5;
  boolean enableEdit = false;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MENIU  PRINCIPAL");
  delay(400);
  
  while(true){
    x = analogRead(A0);
    
    
    if(x > 715 && x < 725){
      if(selActiveMenu == false){
        selActiveMenu = true;
        selTimerMenu = millis();
      }

      if((millis() - selTimerMenu > longPressTime) && (longPressActiveMenu == false)){
        longPressActiveMenu = true;
        lcd.clear();
        return;
      }
    }
    
    else{
      if(selActiveMenu == true){
        if(longPressActiveMenu == true){
          longPressActiveMenu = false;
        }
        else{
          enableEdit = !enableEdit;
          delay(300);
        }

        selActiveMenu = false;
      }
    }
    
    lcd.setCursor(0,1);

    if(sel < 0)
      sel = sel + NUM_MENUS;
    
    switch(sel){
      case 0:
      lcd.print("Setare ceas       ");
      if(enableEdit){
        configTime();
        enableEdit = false;
        lcd.print("MENIU  PRINCIPAL");
      }
      else if(x > 125 && x < 135){
        sel = (sel + 1)%NUM_MENUS;
        delay(300);
      }
      else if(x > 300 && x < 310){
        sel = sel - 1;
        delay(300);
      }
      break;
      case 1:
      lcd.print("Setare alarma     ");
      if(enableEdit){
        configAlarm();
        enableEdit = false;
        lcd.print("MENIU  PRINCIPAL");
      }
      else if(x > 125 && x < 135){
        sel = (sel + 1)%NUM_MENUS;
        delay(300);
      }
      else if(x > 300 && x < 310){
        sel = sel - 1;
        delay(300);
      }
      break;
      case 2:
      lcd.print("Format ora ");
      if(enableEdit && ((x > 125 && x < 135) || (x > 300  && x < 310))){
        FORMAT_ORA = !FORMAT_ORA;
        delay(300);
      }
      else if(x > 125 && x < 135){
        sel = (sel + 1)%NUM_MENUS;
        delay(300);
      }
      else if(x > 300 && x < 310){
        sel = sel - 1;
        delay(300);
      }
      if(FORMAT_ORA)
        lcd.print("AM/PM");
      else
        lcd.print("  24H");
      break;
      case 3:
      lcd.print("Sincronizare:");
      
      if(SYNC < 0)
        SYNC = SYNC + 3;
      SYNC = SYNC % 3;

      switch(SYNC){
        case 0:
        lcd.print("OFF");
        setSyncProvider(syncOff);
        setSyncInterval(0);
        break;
        case 1:
        lcd.print("RTC");
        setSyncProvider(syncWithRTC);
        setSyncInterval(10);
        break;
        case 2:
        lcd.print("BLU");
        setSyncProvider(syncWithBLU);
        setSyncInterval(10);
        break;
      }
      
      if(enableEdit){
        if(x > 125 && x < 135){
          SYNC = (SYNC + 1)%3;
          delay(300);
        }
        else if(x > 300 && x < 310){
          SYNC = SYNC - 1;
          delay(300);
        }
      }
      else{
        if(x > 125 && x < 135){
          sel = (sel + 1)%NUM_MENUS;
          delay(300);
        }
        else if(x > 300 && x < 310){
          sel = sel - 1;
          delay(300);
        }
      }
            
      if(x > 125 && x < 135){
        SYNC = (SYNC + 1)%3;
        delay(300);
      }
      else if(x > 300 && x < 310){
        SYNC = (SYNC - 1)%3;
        delay(300);
      }
      break;
      case 4:
      lcd.print("Temp & Pres     ");
      if(enableEdit){
        displayTemp();
        lcd.print("MENIU  PRINCIPAL");
        enableEdit = false;
      }
      else{
        if(x > 125 && x < 135){
          sel = (sel + 1)%NUM_MENUS;
          delay(300);
        }
        else if(x > 300 && x < 310){
          sel = sel - 1;
          delay(300);
        }
      }
      break;
    }
    
  }
}

void displayTemp(){
  int x;
  char status;
  double T, P;
  while(true){
  x = analogRead(A0);
  if(x > 715 && x < 725){
    lcd.clear();
    delay(1000);
    return;
  }
  status = bmp180.startTemperature();

  if (status != 0) {
    delay(1000);
    status = bmp180.getTemperature(T);

    if (status != 0) {
      status = bmp180.startPressure(3);

      if (status != 0) {
        delay(status);
        status = bmp180.getPressure(P, T);

        if (status != 0) {
          lcd.setCursor(0, 0);
          lcd.print("Pres: ");
          lcd.print(P);
          lcd.println("hPa  ");

          lcd.setCursor(0,1);
          lcd.print("Temp: ");
          lcd.print(T);
          lcd.println("C    ");
        }
      }
    }
  }
  }
}

boolean selActiveCfg;
boolean longPressActiveCfg;
long selTimerCfg;

boolean selActiveAlarm;
boolean longPressActiveAlarm;
long selTimerAlarm;

void configAlarm(){
  TimeElements tm;
  t = now();
  boolean enableEdit = false;
  if(!alarmIsSet)
    breakTime(t, tm);
  else
    breakTime(alarm, tm);
  int sel = 0;
  
  
  delay(1000);
  lcd.clear();
  x = analogRead(A0);

  lcd.setCursor(0,0);
  lcd.print("ALARMA:");

  selActiveAlarm = false;
  longPressActiveAlarm = false;
  selTimerAlarm = 0;
  
  do{
  x = analogRead(A0);

  if(x > 715 && x < 725){
    if(selActiveAlarm == false){
      selActiveAlarm = true;
      selTimerAlarm = millis();
    }

    if((millis() - selTimerAlarm > longPressTime) && (longPressActiveAlarm == false)){
      longPressActiveAlarm = true;
      alarm = makeTime(tm);
      if(alarm < t)
        tm.Day = tm.Day + 1;
      alarm = makeTime(tm);
      lcd.clear();
      delay(1000);
      alarmIsSet = true;
      return;
    }
  }
    
  else{
    if(selActiveAlarm == true){
      if(longPressActiveAlarm == true){
        longPressActiveAlarm = false;
      }
      else{
        enableEdit = !enableEdit;
        delay(300);
      }

      selActiveAlarm = false;
    }
  }
  lcd.setCursor(7,0);

  if(!enableEdit)
    if(x > 125 && x < 135){
      sel = (sel + 1)%3;
      delay(300);
    }
    else if(x > 300 && x < 310){
      sel = sel - 1;
      delay(300);
    }
  
  switch(sel){
    case 0:
    lcd.print("      ORA");
    if(enableEdit)
      tm.Hour = modifDate(x, tm.Hour, sel);
    break;
    
    case 1:
    lcd.print("  MINUTUL");
    if(enableEdit)
      tm.Minute = modifDate(x, tm.Minute, sel);
    break;
    
    case 2:
    lcd.print("  SECUNDA");
    if(enableEdit)
      tm.Second = modifDate(x, tm.Second, sel);
    break;
  }

  lcd.setCursor(0,1);
  printDigits(tm.Hour);
  lcd.write(":");
  printDigits(tm.Minute);
  lcd.write(":");
  printDigits(tm.Second);
  
  }while(true);


}

void configTime(){
  int cHour = hour(t);
  int cMinute = minute(t);
  int cSecond = second(t);
  int cDay = day(t) - 1;
  int cMonth = month(t) - 1;
  int cYear = year(t) - 1970;
  int sel = 0;
  boolean enableEdit = false;
  
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
      lcd.clear();
      delay(1000);
      return;
    }
  }
    
  else{
    if(selActiveCfg == true){
      if(longPressActiveCfg == true){
        longPressActiveCfg = false;
      }
      else{
        enableEdit = !enableEdit;
      }

      selActiveCfg = false;
    }
  }
  
  lcd.setCursor(0,1);

  if(!enableEdit)
    if(x > 125 && x < 135){
      sel = (sel + 1)%6;
      delay(300);
    }
    else if(x > 300 && x < 310){
      sel = sel - 1;
      delay(300);
    }

  if(sel < 0)
    sel = sel + 6;
  switch(sel){
    case 0:
    lcd.print("Ora: ");
    lcd.print(cHour);
    if(enableEdit)
      cHour = modifDate(x, cHour, sel);
    break;
    
    case 1:
    lcd.print("Minutul: ");
    lcd.print(cMinute);
    if(enableEdit)
      cMinute = modifDate(x, cMinute, sel);
    break;
    
    case 2:
    lcd.print("Secunda: ");
    lcd.print(cSecond);
    if(enableEdit)
      cSecond = modifDate(x, cSecond, sel);
    break;
    
    case 3:
    lcd.print("Ziua: ");
    lcd.print(cDay + 1);
    if(enableEdit)
      cDay = modifDate(x, cDay, sel);
    break;
    
    case 4:
    lcd.print("Luna: ");
    lcd.print(cMonth + 1);
    if(enableEdit)
      cMonth = modifDate(x, cMonth, sel);
    break;
    
    case 5:
    lcd.print("Anul: ");
    lcd.print(cYear + 1970);
    if(enableEdit)
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
  if(FORMAT_ORA){
    if(hour(t) == 0)
      lcd.print("12");
    else
      lcd.print(hour(t)%12);
  }
  else{
    printDigits(hour(t));
  }
  lcd.write(":");
  printDigits(minute(t));
  lcd.write(":");
  printDigits(second(t));
  lcd.write(" ");
  if(!FORMAT_ORA)
    lcd.print("  ");
  else
    if(hour(t) >= 12)
      lcd.print("PM");
    else
      lcd.print("AM");
  lcd.print("  ");
  lcd.print(roWeekDay(weekday(t)));
  lcd.setCursor(0,1);
  lcd.print(String(day(t)));
  lcd.write("/");
  lcd.print(String(month(t)));
  lcd.write("/");
  lcd.print(String(year(t)));
  lcd.setCursor(10,1);
}

String roWeekDay(int wkd){
  switch(wkd){
    case 0: return "ERR";
    case 1: return "DUM";
    case 2: return "LUN";
    case 3: return "MAR";
    case 4: return "MIE";
    case 5: return "JOI";
    case 6: return "VIN";
    case 7: return "SAM";
  }
}

void printDigits(int digits){
  if(digits < 10)
    lcd.write("0");
  lcd.print(String(digits));
}
