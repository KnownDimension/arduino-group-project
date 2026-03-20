#include <Arduino.h>
#include <U8g2lib.h> 
#include <Wire.h> 
#include <U8x8lib.h>
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 

//pin locations on board (constant because position/value will never change)
const int buttonPin = 6;
const int ledPin = 4; 

//these values can change
int buttonState = 0; 
int buzzerPin = 5;

void setup() { 
  u8x8.setBusClock(100000);
  u8x8.begin();
  u8x8.setFlipMode(1); 
  pinMode(ledPin,OUTPUT); //sets the LED as an output
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

} 

void loop() { 
  buttonState = digitalRead(buttonPin); //reads the state of the button (if it is pressed or not)
  
  if (buttonState == HIGH) { 
    digitalWrite(ledPin, HIGH); // if button is pressed, LED turns on and timer until "GO" starts 
    u8x8.setFont(u8x8_font_chroma48medium8_r);  
    for (int i =5; i>0; i--) { //uses iteration to display the countdown until "STOP" phase ends
      u8x8.clear(); 
      u8x8.setCursor(0,0); 
      u8x8.print("STOP"); 
      u8x8.setCursor(0,1); 
      u8x8.print(i);
      delay(1000); 
    } 
    digitalWrite(ledPin, LOW); //light turns off after countdown (real life example of this would be the green man on the pedestrian lights)
    for (int i = 5; i > 0; i-- ) { //timer until "Go" phase ends
      if (i < 3){ //when timer nears end, buzzer intensifies
        analogWrite(buzzerPin, 128);
        delay(200);
        analogWrite(buzzerPin, 0);
        delay(200);
      } else{
        analogWrite(buzzerPin, 128);
      }

      u8x8.clear(); 
      u8x8.setCursor(0,0);
      u8x8.print("GO"); 
      u8x8.setCursor(0,1); 
      u8x8.print(i); 
      delay(1000); 
    } 
    u8x8.clear(); 
    u8x8.setCursor(0,0); 
    u8x8.print("STOP");
    digitalWrite(ledPin,HIGH); //light returns to "STOP" phase as a realistic representation of the pedestrial lights turning back to "red man"
} else{ //if button is not pressed STOP 
  digitalWrite(ledPin, HIGH); 
  u8x8.setFont(u8x8_font_chroma48medium8_r);  
  u8x8.setCursor(0,0);  
  u8x8.print("STOP"); 

} 

 

 

} 