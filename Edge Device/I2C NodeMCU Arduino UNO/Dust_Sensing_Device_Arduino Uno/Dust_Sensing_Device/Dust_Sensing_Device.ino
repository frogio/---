// #include "PMsensor.h"
#include <DustSensor.h>
#include <Wire.h>

#define REQ_SENSOR_DATA 0x01

#define DUST_SENSOR_ADDR 1
#define REQ_SENSOR_DATA 0x1

#define sensitivity  0.2
//미세먼지센서의 센싱 민감도 설정
//민감도의 숫자가 클 경우 : 센서 값의 변화가 민감함
//민감도의 숫자가 작을 경우 : 센서 값의 변화가 둔함

//DustSensor sensor;

DustSensor sensor;

const int sensorPin = A0;
const int sensorLED = 3;

void setup() {
  Wire.begin(DUST_SENSOR_ADDR);

  Serial.begin(115200);
  //PM.init(sensorLED, sensorPin);
  
  sensor.init(sensorLED, sensorPin);
  Wire.onRequest(requestMsg);

}

void loop() {  

}

void receiveMsg(int value){

}

void requestMsg(){

float data = 0;
  int err = DustSensorErrSuccess;

  if ((err = sensor.read(&data, true, sensitivity)) != DustSensorErrSuccess) {
    Serial.print("data Error = ");
    Serial.println(err);
    // delay(1500);
    return;
  }
  Serial.print("Sensor Data : ");
  Serial.println(data);
  
  Wire.write((uint8_t)data);
  // Wire.write()

//  Wire.write(arr);
}