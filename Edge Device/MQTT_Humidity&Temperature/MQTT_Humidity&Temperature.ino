#include <Mqtt.h>
#include <Analog.h>
#include <DHT.h>
#include <string.h>

#define SEND_INTERVAL_NORMAL 5 * 60 * 1000
#define SEND_INTERVAL_REALTIME 1 * 1000
#define ROOT_TOPIC "dev"
#define DEVICE_NAME_HUMI "HUMI"
#define DEVICE_NAME_TEMP "TEMP"


Mqtt MqttManager;
DHT dht11(D7, DHT11);

const char *REQUEST_REAL_TIME = "dev/all/setting_realtime";

const char *broker_ip = "192.168.30.18";
const char *ssid = "Campus7_Room3_2.4GHz";
const char *pwd = "12345678";

int timer_id = 0;

void read_sensor();

void setup() {
  // put your setup code here, to run once:
  dht11.begin();
  MqttManager.init(ssid, pwd);
  MqttManager.setBroker(broker_ip, REQUEST_REAL_TIME, sub_callback);        // setServer를 호출해야 client에 연결함
  // timer_id = MqttManager.setInterval(SEND_INTERVAL_NORMAL, read_sensor);

}

void loop() {  // put your main code here, to run repeatedly:
  MqttManager.run();
}

void read_sensor(){
  float fh, fc;
  char topic[50] = {0, };

  fh = dht11.readHumidity();
  fc = dht11.readTemperature();

  if(isnan(fh) || isnan(fc)){
      Serial.print("Failed to measuring");
      return;
  }

  sprintf(topic,"%s/%s/value_data", ROOT_TOPIC, DEVICE_NAME_TEMP);
  
  MqttManager.publish(topic, fc);
  
  memset(topic, '\0', sizeof(topic));
  sprintf(topic,"%s/%s/value_data", ROOT_TOPIC, DEVICE_NAME_HUMI);

  MqttManager.publish(topic, fh);
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