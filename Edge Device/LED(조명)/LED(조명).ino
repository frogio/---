#include <Mqtt.h>
#include <stdlib.h>

#define ROOT_TOPIC "dev"

#define DEVICE_NAME "LED"
#define PLACE_LIVING "LIVINGROOM"
#define PLACE_BATH "BATHROOM"
#define PLACE_ROOM1 "ROOM1"
#define PLACE_ROOM2 "ROOM2"
#define PLACE_ROOM3 "ROOM3"
#define LIGHTS_OUT_ALL "LIGHTS_OUT_ALL"

#define EXIT_PUB "exit_pub"

Mqtt MqttManager;

const char *broker_ip = "192.168.30.18";
const char *ssid = "Campus7_Room3_2.4GHz";
const char *pwd = "12345678";

struct LEDinfo{
  int pinNo;
  int status;
};

bool isSubscribed = false;
bool isPublished = false;

char chk_opt[10] = EXIT_PUB;
char place_in_loop[10] = {0, };


// structure 구조
struct LEDinfo Led1 = {D6, LOW},    // 거실
                Led2 = {D1, LOW},   // 화장실
                Led3 = {D2, LOW},   // 방 1
                Led4 = {D5, LOW},   // 방 2
                Led5 = {D3, LOW};   // 방 3

void sub_callback(char* topic, byte* payload, unsigned int length);


char topic[50] = {0, };

void setup() {

  MqttManager.init(ssid, pwd);

  sprintf(topic, "%s/%s/#", ROOT_TOPIC, DEVICE_NAME);   // 구독할 토픽 dev/LED/#
  MqttManager.setBroker(broker_ip, topic /*구독할 토픽 이름*/, sub_callback);
  
  pinMode(Led1.pinNo, OUTPUT);
  pinMode(Led2.pinNo, OUTPUT);
  pinMode(Led3.pinNo, OUTPUT);
  pinMode(Led4.pinNo, OUTPUT);
  pinMode(Led5.pinNo, OUTPUT);
  
  digitalWrite(Led1.pinNo, LOW);
  digitalWrite(Led2.pinNo, LOW);
  digitalWrite(Led3.pinNo, LOW);
  digitalWrite(Led4.pinNo, LOW);
  digitalWrite(Led5.pinNo, LOW);

}

void loop() {

  MqttManager.run();

  
  if(isSubscribed == false && strcmp(chk_opt, "status") == 0) {     // LED의 status를 묻는 경우   
      // Serial.println("isWorking?");

      // Serial.print("opt : ");
      // Serial.println(chk_opt);  

      char topic[50] = {0, };      
      sprintf(topic, "%s/%s/status", DEVICE_NAME, place_in_loop);		// status를 알려주는 토픽 조립    // LED/<Place>/status
  
      int status = 0;
    
      if(strcmp(place_in_loop, PLACE_LIVING) == 0)   // 거실
        status = Led1.status;
        // topic : dev/LED/LIVINGROOM
        
      else if(strcmp(place_in_loop, PLACE_BATH) == 0 )    // 화장실
        status = Led2.status;
        // topic : dev/LED/BATHROOM

      else if(strcmp(place_in_loop, PLACE_ROOM1) == 0 )    // 방1
        status = Led3.status;
        // topic : dev/LED/ROOM1
      
      else if(strcmp(place_in_loop, PLACE_ROOM2) == 0 )    // 방2
        status = Led4.status;
        // topic : dev/LED/ROOM2

      else if(strcmp(place_in_loop, PLACE_ROOM3) == 0 )    // 방3
        status = Led5.status;
        // topic : dev/LED/ROOM3

      // Serial.print("statue : ");
      // Serial.println(status);
      MqttManager.publish(topic,status);     // LED/<Place>/status
      strcpy(chk_opt, EXIT_PUB);      
      // Serial.println("isWorking?2");
      // isPublished = true;
  }
  else if(strcmp(chk_opt, EXIT_PUB) == 0) return;          // 읽을 토픽이 오지 않을 경우

}


void sub_callback(char* topic, byte* payload, unsigned int length) {
  
  isSubscribed = true;                   // subscribe와 publish가 동시에 일어나지 못하도록 만듦

  // Serial.println(topic);
  // payload[length] = '\0';               // Test용 코드
  
  char *place = strtok(topic,"/");			// root 토픽 분리                                           // dev
  place = strtok(NULL,"/");					   	// Device Name 토픽 분리                                    // LED
  place = strtok(NULL,"/");      				// 장소 <Place> 분리                                        // <Place>
  char * opt = strtok(NULL,"/"); 				// 토픽 옵션 (status : 해당 플레이스에서 전등이 켜진 여부)    // status
  
  if(opt != NULL){                       // status 출력
    strcpy(chk_opt, opt);
    strcpy(place_in_loop, place);
  }
  // <Place> : Livingroom, bathroom, room1~3, lights_out
  


  // dev/LED/<Place> Value : on / off
  // <place>
  // LIVINGROOM
  // BATHROOM
  // ROOM1 ~ 3
  // LIGHTS_OUT_ALL

  // char * msg = (char *)payload;   // on / off

  // Serial.println((char *)payload);
  // Serial.println(place);

  if(strcmp(place,PLACE_LIVING) == 0){
    if(strcmp(msg, "on") == 0) {
      Led1.status = HIGH;
      digitalWrite(Led1.pinNo, HIGH);
    }
    else if(strcmp(msg, "off") == 0) {
      Led1.status = LOW;
      digitalWrite(Led1.pinNo, LOW);
    }
  }

  else if(strcmp(place,PLACE_BATH) == 0) {
    if(strcmp(msg, "on") == 0) {
      Led2.status = HIGH;
      digitalWrite(Led2.pinNo, HIGH);      
    }
    else if(strcmp(msg, "off") == 0) {
      Led2.status = LOW;
      digitalWrite(Led2.pinNo, LOW);
    }
  }

  else if(strcmp(place,PLACE_ROOM1) == 0) {
    if(strcmp(msg, "on") == 0) {
      Led3.status = HIGH;
      digitalWrite(Led3.pinNo, HIGH);
    }
    else if(strcmp(msg, "off") == 0) {
      Led3.status = LOW;
      digitalWrite(Led3.pinNo, LOW);
    }
  }

  else if(strcmp(place,PLACE_ROOM2) == 0) {
    if(strcmp(msg, "on") == 0) {
      Led4.status = HIGH;
      digitalWrite(Led4.pinNo, HIGH);
    }
    else if(strcmp(msg, "off") == 0) {
      Led4.status = LOW;
      digitalWrite(Led4.pinNo, LOW);
    }
  }

  else if(strcmp(place,PLACE_ROOM3) == 0) {
    if(strcmp(msg, "on") == 0) {
      Led5.status = HIGH;
      digitalWrite(Led5.pinNo, HIGH);
    }
    else if(strcmp(msg, "off") == 0){
      Led5.status = LOW;
      digitalWrite(Led5.pinNo, LOW);
    }
  }
  else if(strcmp(place, LIGHTS_OUT_ALL) == 0){
    
    if(strcmp(msg, "on") == 0) {
      Led5.status = HIGH;
      Led4.status = HIGH;
      Led3.status = HIGH;
      Led2.status = HIGH;
      Led1.status = HIGH;
      digitalWrite(Led5.pinNo, HIGH);
      digitalWrite(Led4.pinNo, HIGH);
      digitalWrite(Led3.pinNo, HIGH);
      digitalWrite(Led2.pinNo, HIGH);
      digitalWrite(Led1.pinNo, HIGH);
    }
    else if(strcmp(msg, "off") == 0){
     	Led5.status = LOW;
      Led4.status = LOW;
      Led3.status = LOW;
      Led2.status = LOW;
      Led1.status = LOW;
      digitalWrite(Led5.pinNo, LOW);
      digitalWrite(Led4.pinNo, LOW);
      digitalWrite(Led3.pinNo, LOW);
      digitalWrite(Led2.pinNo, LOW);
      digitalWrite(Led1.pinNo, LOW); 
    }
    
  }
  isSubscribed = false;
}
