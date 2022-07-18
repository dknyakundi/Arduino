#include <RTClib.h> 
#include <Servo.h>  
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


RTC_DS1307 rtc;
Servo dkServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

//variables for pins
int servopin = 10;
const int lights = 2;
int photopin = A0;
int lamp = 12;
int pirPin = 3;
int lm35 = A1;
int motionled = 8;
int fan = 7;
int val;
int del = 50;

float mv;
float cel;

int i;

int photoval;

int CurrentHour;
int CurrentMinute;

int motion;


// lights on time
const int ONlightH = 16;
const int ONlightM =0;
const int OFFlightH = 16;
const int OFFlightM = 1;


//drawing curtains
int open = 180;
int close = 0;


const int ONcurtH = 16;
const int ONcurtM =0;
const int OFFcurtH = 16;
const int OFFcurtM = 1;



void setup() {
  //pinmodes
  pinMode(lights, OUTPUT);
  pinMode(photopin,INPUT);
  pinMode(lamp, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(lm35,INPUT);
  pinMode(fan,OUTPUT);

  //ensuring all outputs are low
   digitalWrite(lamp, LOW);
   digitalWrite(lights, LOW);
   digitalWrite(fan, LOW);

  Serial.begin(9600);
  rtc.begin();
  
  //initialize LCD 
   lcd.begin();
   lcd.backlight();
   
  //line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  //attaching servopin
    dkServo.attach(servopin);

  //LCD welcome message
   lcd.setCursor(1, 0); 
   lcd.print("Welcome to"); 
   lcd.setCursor(1, 1);
   lcd.print("WaDaniel");

 
}


void loop() {

  //motion sensor
     motion = digitalRead(pirPin);

      if(motion==1){
        Serial.println("Motion detected");
         digitalWrite(motionled, HIGH);
       }
      else{
        Serial.println("No motion detected");
         digitalWrite(motionled, LOW);
       }
      delay(500);
 
  //RTC
   DateTime now = rtc.now();

    // Save check in time;
    CurrentHour = now.hour();
    CurrentMinute = now.minute();

    Serial.println(CurrentHour);
    Serial.println(CurrentMinute);

    //house lights
   if((CurrentHour==ONlightH) && (CurrentMinute == ONlightM)){
    digitalWrite(lights,HIGH);
    Serial.println("LIGHTS ON");
    }
   if((CurrentHour==OFFlightH) && (CurrentMinute == OFFlightM)){
    digitalWrite(lights,LOW);
    Serial.println("LIGHTS OFF");
    }

    //curtains
   if((CurrentHour==ONcurtH) && (CurrentMinute == ONcurtM)){
    for (i=0; i<= 180; i+= 1)
        {
           dkServo.write(i);             
           delay(50);                     
        }
    Serial.println("Curtains drawn");
    }

if((CurrentHour==OFFcurtH) && (CurrentMinute == OFFcurtM)){
      for (i=180; i>=0; i-= 1) 
        { 
           dkServo.write(i);            
           delay(50);                    
        }
      Serial.println("Curtains withdrawn");
      i=0;
    }
  delay(50); 

// security lights:
photoval = analogRead(photopin);
Serial.println(photoval);
delay(50);

  lcd.clear();
  lcd.setCursor(1, 0); 
  lcd.print("LDR value"); 
  lcd.setCursor(1, 1);
  lcd.print(photoval);

if(photoval>9){
digitalWrite(lamp,LOW);
}
if(photoval<9){
  digitalWrite(lamp,HIGH);
}

//lm35
val=analogRead(lm35);
mv=(val/1024.0)*5000;
cel=mv/10;
Serial.println("Temperature =");
Serial.println(val);
delay(50);

if (val>60){
  digitalWrite(fan,HIGH);
}

if (val<60){
  digitalWrite(fan,LOW);
}

}