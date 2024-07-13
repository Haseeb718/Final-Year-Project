

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

float Vin=A0;            /// SOLAR VOLTAGE  
float Vbat=A1;        // Battery Volt
 float x ;                    // for battery
  float y ;                  // for solar 


 int pwm=6;
 
   #define FULL 255 
  #define HALF 128 
  #define QUARTER 64 
  #define OFF 00 

void setup() 
{
  lcd.begin(16,2);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(Vin,INPUT);
  pinMode(Vbat,INPUT);

   pinMode(pwm,OUTPUT);
lcd.setCursor(0, 0);
  lcd.print("Charge");
  lcd.setCursor(0, 1);
  lcd.print("Controller");
   delay(2000);
   lcd.clear();

}

void loop()

{

     
     x= analogRead(Vin);
  
    float battery_v = x*0.061 ;      /// read solar voltage 
     lcd.setCursor(0,0);
     lcd.print("Battery(v)=");
      lcd.setCursor(11,0);
     lcd.print(battery_v);
   
         // Read the battery voltage i;e Vbat
     y= analogRead(Vbat) ;
   
    
     float solar_in = y*0.0163;    // read battery voltage  
     
    lcd.setCursor(0,1);
     lcd.print("Solar(v)=");
     lcd.setCursor(9,1);
     lcd.print(solar_in);
        delay(1000);
////////////////////////
  lcd.clear();
  delay(1000);


   if  ((solar_in>battery_v)  &&( battery_v>13.00)) 
   {
    analogWrite(pwm,0);

      lcd.setCursor(0,0);
      lcd.print("Battery Charged");
       lcd.setCursor(0,1);
      lcd.print("Charged 100%");
      
   }


 
   if  ((solar_in>battery_v)  && (battery_v>12.75 and battery_v<=13))  
   {
    analogWrite(pwm,125);

      lcd.setCursor(0,0);
      lcd.print("Battery Charging");
       lcd.setCursor(0,1);
      lcd.print("Charged 90%");
      
   }
   
     if  ((solar_in>battery_v)  && (battery_v>12.50 and battery_v<=12.75)) 
      {
    analogWrite(pwm,180);

      lcd.setCursor(0,0);
      lcd.print("Battery Charging");
             lcd.setCursor(0,1);
      lcd.print("Charged 80%");
      }
        
           if  ((solar_in>battery_v)  && (battery_v>12.30 and battery_v<=12.50)) 
      {
    analogWrite(pwm,190);
   
        lcd.setCursor(0,0);
      lcd.print("Battery Charging");
             lcd.setCursor(0,1);
      lcd.print("Charged 70%");
      }
        

  if  ((solar_in>battery_v)  && (battery_v>12.00 and battery_v<=12.30)) 
      {
    analogWrite(pwm,200);
   
 
      lcd.setCursor(0,0);
      lcd.print("Battery Charging");
      lcd.print("Charged 50%");
      }

  if  ((solar_in>battery_v)  && (battery_v>11.95 and battery_v<=12.00)) 
      {
    analogWrite(pwm,220);

      lcd.setCursor(0,0);
      lcd.print("Battery Charging");
      lcd.print("Charged 40%");
      }



  if  ((solar_in>battery_v)  && (battery_v>11.4 and battery_v<=11.95)) 
      {
    analogWrite(pwm,255);
 
    lcd.setCursor(0,0);
     lcd.print("Battery Charging");
     lcd.setCursor(0,1);
      lcd.print("Charged 20%");
      }

  if  ((solar_in>battery_v)  && (battery_v>10.5 and battery_v<=11.4))
      {
    analogWrite(pwm,255);
    
      lcd.setCursor(0,0);
      lcd.print("Battery Charging");
      lcd.setCursor(0,1);
      lcd.print("Charged 10%");
      }
      if ((solar_in>battery_v)  && (battery_v>10 and battery_v<=10.5))      //solar is low and  battery is not charging
      {
    analogWrite(pwm,255);
       
      lcd.setCursor(0,0);
      lcd.print("Battery Charging");
       lcd.setCursor(0,1);
      lcd.print("Charged 10%");
      }

     if  ((solar_in<battery_v))        //    solar is  low
      {
    analogWrite(pwm,OFF);

          lcd.setCursor(0,0);
      lcd.print("SOLAR LOW");
      }


         if  ((battery_v<10))        //    solar is  low
      {
    analogWrite(pwm,OFF);

          lcd.setCursor(0,0);
      lcd.print("Battery is dead");
      }



  
delay(1000);
lcd.clear();
delay(1000);


    
  }



  




 
  
