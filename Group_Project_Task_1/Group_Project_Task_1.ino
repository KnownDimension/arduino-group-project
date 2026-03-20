//Install These Libraries:
//Grove Temperature and Humidity Sensor By Seeed Studios, 
//U8g2 by oliver, 
//Grove BMP280 by Seeed Studio

#include <Arduino.h>
#include "DHT.h"
#include <U8x8lib.h>
#include "Seeed_BMP280.h"
#include <Wire.h>

BMP280 bmp280;

#define DHTTYPE DHT20
DHT dht(DHTTYPE);

//Defining oled Constrictions
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

//Buzzer 
const int BUZZER_PIN = 5;
bool buzzerOn = false;

//Potentiometer to set Temperature
const int POT_PIN = A0;            
const float MIN_SETPOINT = 10.0;   // Min
const float MAX_SETPOINT = 40.0;   // Max

const float HYST = 1.0; // ±0.5°C

//Bar display
void drawBar(uint8_t row, float value, float minVal, float maxVal) {
  float v = value;
  if (v < minVal) v = minVal;
  if (v > maxVal) v = maxVal;

  int barLength = map((int)((v - minVal) * 100), 0, (int)((maxVal - minVal) * 100), 0, 16);

  u8x8.setCursor(0, row);

  for (int i = 0; i < barLength; i++) u8x8.print("#");
  for (int i = barLength; i < 16; i++) u8x8.print("-");
}

//Potentiometer to temperature setpoint
float readSetpointC() {
  int raw = analogRead(POT_PIN);
  float sp = MIN_SETPOINT + ((MAX_SETPOINT - MIN_SETPOINT) * (raw / 1023.0f));
  return sp;
}

void setup() { //Setting up using the libraries
  Serial.begin(9600);
  Wire.begin();

  dht.begin();

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);

  if (!bmp280.init()) {
    Serial.println("BMP280 NOT FOUND!");
    while (1);
  }

  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Sensors initialized.");
}

void loop() {

  //Reading values from sensors
  float pressure = bmp280.getPressure();
  float altitude = bmp280.calcAltitude(pressure);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  //Potentiometer setpoint
  float setpointC = readSetpointC();
  float onThreshold  = setpointC + (HYST * 0.5f);
  float offThreshold = setpointC - (HYST * 0.5f);

  //Buzzer logic
  if (!isnan(temperature)) {
    if (!buzzerOn && temperature > onThreshold) {
      tone(BUZZER_PIN, 2000);
      buzzerOn = true;
    } 
    else if (buzzerOn && temperature < offThreshold) {
      noTone(BUZZER_PIN);
      buzzerOn = false;
    }
  }

  //Serial output
  Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" Pa");
  Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");
  Serial.print("Temp: "); Serial.print(temperature); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Setpoint: "); Serial.print(setpointC, 1); Serial.println(" C\n");

  //Oled output
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.clear();

  //Temperature display
  u8x8.setCursor(0, 0);
  u8x8.print("Temp: ");
  if (!isnan(temperature)) u8x8.print(temperature, 1);
  else u8x8.print("--.-");
  u8x8.print("\xB0");
  drawBar(1, isnan(temperature) ? 0 : temperature, 0, 40);

  //Buzzer setpoint
  u8x8.setCursor(0, 2);
  u8x8.print("Set: ");
  u8x8.print(setpointC, 1);
  u8x8.print("\xB0");
  drawBar(3, setpointC, 0, 40);

  //Humidity display
  u8x8.setCursor(0, 4);
  u8x8.print("Hum:  ");
  if (!isnan(humidity)) u8x8.print(humidity, 1);
  else u8x8.print("--.-");
  u8x8.print("%");
  drawBar(5, isnan(humidity) ? 0 : humidity, 0, 100);

  //Pressure display
  u8x8.setCursor(0, 6);
  u8x8.print("Press:");
  u8x8.print(pressure / 100.0, 1);
  u8x8.print("hPa");
  drawBar(7, pressure / 100.0, 900, 1100);

  delay(400); //Refresh timer
}