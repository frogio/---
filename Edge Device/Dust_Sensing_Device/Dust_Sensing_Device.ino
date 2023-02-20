#include <Mqtt.h>
#include "PMsensor.h"

#define sensitivity  0.2
//미세먼지센서의 센싱 민감도 설정
//민감도의 숫자가 클 경우 : 센서 값의 변화가 민감함
//민감도의 숫자가 작을 경우 : 센서 값의 변화가 둔함

#define DHTPIN 6
#define DHTTYPE DHT11

const int sensorPin = A0;
const int sensorLED = 3;

PMsensor PM;
LiquidCrystal_I2C lcd(0x3F, 16, 2);
//LCD 이름 : 0x27 또는 0x3F 입력

DHT dht(DHTPIN, DHTTYPE);

void setEmoticon(float data) {
  if (data > 100) {
    lcd.createChar(4, topAngry1);
    lcd.createChar(5, topAngry2);
    lcd.createChar(6, bottomAngry1);
    lcd.createChar(7, bottomAngry2);
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }
  else if (data > 50) {
    lcd.createChar(4, topSoSo1);
    lcd.createChar(5, topSoSo2);
    lcd.createChar(6, bottomSoSo1);
    lcd.createChar(7, bottomSoSo2);
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
  }

  else {
    lcd.createChar(4, topSmile1);
    lcd.createChar(5, topSmile2);
    lcd.createChar(6, bottomSmile1);
    lcd.createChar(7, bottomSmile2);
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
  }
}

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);

  PM.init(sensorLED, sensorPin);
}

void loop() {


  float data = 0;
  int err = PMsensorErrSuccess;

  if ((err = PM.read(&data, true, sensitivity)) != PMsensorErrSuccess) {
    Serial.print("data Error = ");
    Serial.println(err);
    delay(1500);
    return;
  }
  Serial.println(data);
  delay(1500);
}
