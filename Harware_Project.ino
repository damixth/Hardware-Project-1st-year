

#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t proximity_data = 0;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Servo myservo;
int pos = 0;


void setup() {

  mlx.begin();    //start the temp sensor
  apds.init();    //initialize the proximity sensor
  apds.enableProximitySensor(false);
  lcd.init();
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(11,OUTPUT); //Set Led pin
  pinMode(12,OUTPUT); //set LED pin

 for (pos = 0; pos <= 60; pos ++) {  //door position at the start
    // in steps of 1 degree
    myservo.write(pos);             
    delay(30);                       
  }
}


void loop() {

  digitalWrite(11,LOW);
  digitalWrite(12,LOW);
  lcd.backlight();
  String temperature = "";
  
  apds.readProximity(proximity_data);


  if (proximity_data == 255 && mlx.readObjectTempC() < 37.5) { //when the person has normal body temperature 

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Body Temp:");
    lcd.setCursor(5, 1);
    lcd.print(mlx.readObjectTempC()); //Display the temperature

    lcd.setCursor(9, 1);
    lcd.print(" C");
    lcd.display();
    digitalWrite(12,HIGH);  //switch on the green LED

     for (pos = 60; pos<= 120; pos++) {   //opening the normal path 
    // in steps of 1 degree
    myservo.write(pos);              
    delay(30);                       
  }
  
    delay(4000);  // wait 4 seconds
    
     for (pos = 120; pos>= 60; pos--) {   //close the door
    myservo.write(pos);              
    delay(30);                       
  }
  }

  
 else if (proximity_data == 255 && mlx.readObjectTempC() > 37.5) {  //when the person has high body temperature 
    
      noTone(5);
      // play a note on pin 8 for 500 ms:
      tone(8, 523, 500);

      lcd.clear();
    
      lcd.setCursor(1, 0);
      
      lcd.print("CRITICAL");

      lcd.setCursor(3, 1);
      lcd.print(mlx.readObjectTempC());
      
      lcd.setCursor(7, 1);
      lcd.print(" C");
     
      lcd.display();

    digitalWrite(11,HIGH);    //switch on the red light
    
    for (pos = 60; pos >= 0; pos--) {   //open the door to the other path
    
    myservo.write(pos);              
    delay(30);                       
  }

      delay(4000);    //wait for 4 seconds
  
   for (pos = 0; pos<= 60; pos++) {   //close the door
    
    myservo.write(pos);              
    delay(30);                       
  }


 }

  else if (proximity_data <= 255) {  //if temparature didn't catch well
    delay(2000);

    lcd.clear();
    
    lcd.setCursor(0, 0);
   
    lcd.print("NOT");
   
    lcd.setCursor(4, 0);
    lcd.print("DETECTED");
    lcd.display();
    
  }

}
