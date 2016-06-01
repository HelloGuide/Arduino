#include <Stepper.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 12, 2, 3, 4, 5);
// stepper motor revolution degree
int degree = 0;
int stepsPerRevolution = 200;
// initialize Stepper Motor Pins
Stepper myStepper(stepsPerRevolution, 9, 8, 7, 6);

void setup() {
  lcd.begin(16,2);
  // set the speed at 60 rpm:
  myStepper.setSpeed(30);
  // initialize the serial port for connect to raspberry pi
  Serial.begin(9600);
}

void loop() {
  // buffer for lcd string
  char temp[20];
  if(Serial.available()){
    
    // read mode ( 0 = Name, 1 = degree, 2 = distance )
    char ch = Serial.read();
    int mode = (ch - '0');
    if(mode == 0)
    {
      lcd.setCursor(0,0);
      byte leng = Serial.readBytes(temp,18);
      
      for(int i = 0; i < leng; i++){
        lcd.print(temp[i]);
      }
    }
    else if(mode == 2)
    { char chLen = Serial.read();
      int leng = (chLen - '0');
      for(int i = 0; i < leng; i++){        
        char val = Serial.read();
        degree = (degree * 10) + (val - '0');  
      }
      if(degree > 180){
        degree = degree -360;
      }
      myStepper.step(degree*0.5555556);
      // initialize lcd and stepper motor
      initialize();    
    }
    else if(mode == 1)
    {
      char chLen = Serial.read();
      int leng = (chLen - '0');
      int dist = 0;
      for(int i = 0; i < leng; i++){        
        char val = Serial.read();
        dist = (dist * 10) + (val - '0');  
      }
      lcd.setCursor(0,2);
      lcd.print(dist);
      lcd.print("m");
      
    }
  }
  delay(500);
}

void initialize(){
   delay(2000);    
   lcd.clear();
   myStepper.step(-degree*0.5555556);
   degree = 0;
}


