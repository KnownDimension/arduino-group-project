

// Light Switch
int sensorpin = A6;
int ledPin = 3;
int buttonPin = 6;
int sensorValue = 0;
int outputValue = 0;
int rotaryPin = A0;
int rotaryValue = 0;
bool trueVal = false;
int buttonState = 0;
int brightness = 0;
int ledbright = 0;
bool buttonLock = false;


#include <U8x8lib.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);



void setup() {
  
  pinMode(ledPin, OUTPUT);
  pinMode(sensorpin, INPUT);
  pinMode(rotaryPin, INPUT);
  pinMode(buttonPin, INPUT);
  u8x8.begin();
  Serial.begin(9600);
}

void loop() {
  u8x8.clear();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  sensorValue = analogRead(sensorpin);
  rotaryValue = analogRead(rotaryPin);
  Serial.println(rotaryValue);
  Serial.println(sensorValue);





  if (sensorValue < rotaryValue && rotaryValue > 511) {
    analogWrite(ledPin, 255 - (sensorValue / 4));
    trueVal = true;
  } else if (sensorValue < rotaryValue && rotaryValue < 512) {
    analogWrite(ledPin, 255 - ((sensorValue / 4) * 2));
    trueVal = true;
  } else {
    analogWrite(ledPin, 0);
    trueVal = false;
  };


  
  u8x8.print(rotaryValue);
  u8x8.setCursor(0, 25);
  u8x8.print(trueVal);
  u8x8.setFlipMode(1);
  delay(200);
}