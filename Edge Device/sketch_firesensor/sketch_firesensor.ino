#include <Mqtt.h>
#include <Analog.h>
#include <string.h>

#define SEND_WARNING_MSG 3 * 1000
#define ROOT_TOPIC "dev"
#define DEVICE_NAME "FIRE"

Mqtt MqttManager;

const char *broker_ip = "192.168.30.18";
const char *ssid = "Campus7_Room3_2.4GHz";
const char *pwd = "12345678";

int timer_id = 0;

int flame = 0;

void read_sensor();

void setup() {

  Serial.begin(115200);                                       // 시리얼 통신을 시작, 통신 속도는 9600
  MqttManager.init(ssid, pwd);
  MqttManager.setBroker(broker_ip);        // setServer를 호출해야 client에 연결함
  // timer_id = MqttManager.setInterval(SEND_INTERVAL_NORMAL, read_sensor);

}

void loop() {
  MqttManager.run();
  read_sensor();
}

void read_sensor(){
  char topic[50] = {0, };

  flame = analogRead(A0);
  Serial.print("flame =");  
  Serial.println(flame);

  sprintf(topic,"%s/%s/alert_flameDetected", ROOT_TOPIC, DEVICE_NAME);

  if(flame > 500){          // 화재 발생
    Serial.println("WARNING! flame detected");
    MqttManager.publish(topic, "True");  
  }
  else{                      // 화재가 발생하지 않음 
    Serial.println("INFO normal state");  
    MqttManager.publish(topic, "False");
  }

  delay(SEND_WARNING_MSG);
 
}
