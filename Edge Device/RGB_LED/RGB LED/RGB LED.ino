#include <Mqtt.h>
#include <stdlib.h>

#define ROOT_TOPIC "dev"
#define DEVICE_NAME "MOOD"

Mqtt MqttManager;

const char *broker_ip = "192.168.30.18";
const char *ssid = "Campus7_Room3_2.4GHz";
const char *pwd = "12345678";

int redLed = D8;
int greenLed = D6;
int blueLed = D5;

void sub_callback(char* topic, byte* payload, unsigned int length);

char topic[50] = {0, };

void setup() {

  sprintf(topic,"%s/%s/received_data", ROOT_TOPIC, DEVICE_NAME);

  Serial.begin(115200);
  MqttManager.init(ssid, pwd);
  MqttManager.setBroker(broker_ip, topic /*구독할 토픽 이름*/, sub_callback);        // Broker에게 등록을 하는 동시에 subscribe 진행

  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  analogWrite(redLed, 255);
  analogWrite(greenLed, 255);
  analogWrite(blueLed, 255);

}

void loop() {
  MqttManager.run();

}

void sub_callback(char* topic, byte* payload, unsigned int length) {
  
  char *ctrl_msg = (char *)payload;
  char *rgb_msg = strtok(ctrl_msg, ":");
  char *brightness_msg = strtok(NULL, ":");

  int brightness = atoi((char *)brightness_msg);
  byte rgb_elem[3] = {0, };
  
  hexStringToByte(rgb_msg, rgb_elem);

  // Serial.print("R value : ");
  // Serial.println(rgb_elem[0]);
  
  // Serial.print("G value : ");
  // Serial.println(rgb_elem[1]);

  // Serial.print("B value : ");
  // Serial.println(rgb_elem[2]);

  int rlb = rgb_elem[0] * brightness / 100;
  int glb = rgb_elem[1] * brightness / 100;
  int blb = rgb_elem[2] * brightness / 100;

  analogWrite(redLed, rlb);
  analogWrite(greenLed, glb);
  analogWrite(blueLed, blb);
  
}

void hexStringToByte(char *rgb_msg, byte * result){

  int digit = 0;
  int elem_set = 0;

  for(int elem_idx = 0; elem_idx < 3; elem_idx++){
    
    digit = 16;
    
    for(int i = 0; i < 2; i++){
        
      if(rgb_msg[elem_set + i] >= 'A')                   //  Hex 문자가 A 이상일 때
        result[elem_idx] += (rgb_msg[elem_set + i] - 'A' + 10) * digit;
      
      else if(rgb_msg[elem_set + i] > '0')               // Hex 문자가 숫자일 때
        result[elem_idx] += (rgb_msg[elem_set + i] - '0') * digit;
    
      digit /= 16;
    }
    
    elem_set += 2;

  }
  // debug
  // Serial.print("R value : ");
  // Serial.println(result[0]);
  
  // Serial.print("G value : ");
  // Serial.println(result[1]);

  // Serial.print("B value : ");
  // Serial.println(result[2]);  
}