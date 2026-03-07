#include <string>

// Light Switch
int sensorpin = A6;
int ledPin = 4;
int sensorValue = 0;
int outputValue = 0;
int rotaryPin = A0;



#include <U8x8lib.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);



void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(sensorpin, INPUT);
  pinMode(rotaryPin, INPUT);
  u8x8.begin();
  Serial.begin(9600);
}

void loop() {
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  sensorValue = analogRead(sensorpin);
  rotaryValue = analogRead(rotaryPin);

  Serial.println(sensorValue);

  if (sensorValue < rotaryValue) {
    digitalWrite(ledPin, HIGH);
    trueVal = true
  }
  else {
    digitalWrite(ledPin, LOW);
    trueVal = false
  }

  u8x8.print(rotaryValue);
  u8x8.setCursor(0, 25);
  u8x8.print(trueVal);
  delay(200);
}