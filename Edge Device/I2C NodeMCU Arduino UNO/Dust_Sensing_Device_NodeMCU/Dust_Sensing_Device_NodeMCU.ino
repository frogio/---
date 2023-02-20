#include <Mqtt.h>
#include <string.h>
#include <Wire.h>

Mqtt MqttManager;

#define SEND_INTERVAL_NORMAL 5 * 60 * 1000
#define SEND_INTERVAL_REALTIME 1 * 1000
#define ROOT_TOPIC "dev"
#define DEVICE_NAME "DUST"

//I2C
#define DUST_SENSOR_ADDR 1
#define REQ_SENSOR_DATA 0x1

const char *REQUEST_REAL_TIME = "dev/all/setting_realtime";

const char *broker_ip = "192.168.30.18";
const char *ssid = "Campus7_Room3_2.4GHz";
const char *pwd = "12345678";

int timer_id = 0;

// int temp = 0;

void read_sensor();

void setup() {
  
  Serial.begin(9600);
  
  MqttManager.init(ssid, pwd);  
  MqttManager.setBroker(broker_ip, REQUEST_REAL_TIME, sub_callback);        // setServer를 호출해야 client에 연결함
  timer_id = MqttManager.setInterval(SEND_INTERVAL_NORMAL, read_sensor);
  Wire.begin(D1, D2);

}

void loop() {  // put your main code here, to run repeatedly:
  MqttManager.run();


}

void sub_callback(char* topic, byte* payload, unsigned int length) {

  Serial.println((char *)payload);

  if(strcmp((char *)payload, "RealTime=True") == 0){
      Serial.println("change device mode into realtime");
      MqttManager.deleteTimer(timer_id);
      timer_id = MqttManager.setInterval(SEND_INTERVAL_REALTIME, read_sensor);
  }

  else if(strcmp((char *)payload, "RealTime=False") == 0){
      Serial.println("change device mode into normal");
      MqttManager.deleteTimer(timer_id);    
      timer_id = MqttManager.setInterval(SEND_INTERVAL_NORMAL, read_sensor);
  }

}

void read_sensor(){
  // Wire.beginTransmission(DUST_SENSOR_ADDR);
  // Wire.write(REQ_SENSOR_DATA);
  // Wire.endTransmission();

  char topic[50] = {0, };

  Wire.requestFrom(DUST_SENSOR_ADDR, 4);
  
  uint8_t data;
  Wire.readBytes(&data, sizeof(uint8_t));
  
  sprintf(topic,"%s/%s/value_data", ROOT_TOPIC, DEVICE_NAME);
  MqttManager.publish(topic, (float)data);

  Serial.println(data);


}
