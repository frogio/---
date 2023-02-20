################################################################
# import 순서      
# 
# urls.py -> views.py -> mqtt_manager.py 로 차례로 임포팅 되어 실행됨
#
################################################################
from device.mongoDB_manager import MongoDBManager
import paho.mqtt.client as mqtt
from django.http import HttpResponse
import json

BROKER_IP = "localhost" 
DEVICE_ROOT_TOPIC = "dev/#"                                 # 디바이스의 모든 루트 토픽을 가져감
REQUEST_REAL_TIME = "dev/all/setting_realtime"              # 모든 센서 디바이스 장비에게 실시간 데이터 요청
                                                            # _로 토픽의 타입과 데이터 정보를 나눈다.
                                                            # i.e. setting_realtime은 setting가 타입, realtime은 데이터 정보
                                                            # value_data는 value가 타입, data는 데이터 정보
REQUEST_MOOD_LAMP = "dev/MOOD/received_data"
REQUEST_LED = "dev/LED/"

DEVICE_LED_TOPIC = "LED/#"

COLLECTION_NAME = "SensorValue"
isFlameDetected = False                                     # 화재발생 여부 검사
ledRoomArray = ["LIVINGROOM", "BATHROOM", "ROOM1", "ROOM2", "ROOM3"]
ledStatusInfo = {}
lightCount = 0
isCompleteLightCommu = False


def printMQTTLog(msg):
    print("MQTT> " + msg)

def on_connect(client, userdata, flags, conn_state):
    printMQTTLog("connected with result code : " + str(conn_state))
    if(conn_state == 0):
        printMQTTLog("succeed to connect to broker")
    else:
        printMQTTLog("failed to connect to broker")

    printMQTTLog("server wait for receiving msg from device...")


def on_message(client, userdata, msg):  # subscribe 콜백 함수
    global isFlameDetected
    global ledStatusInfo
    global lightCount
    global isCompleteLightCommu

    if(len(msg.topic.split('/')) > 3):  # 순환 호출된 dev/LED/<Place>/status 토픽에 대한 처리는 하지 않음.
        return

    led, device_name, msg_info = msg.topic.split('/')                         #
    
    if(led == "LED"):                   # LED/<place>/status 토픽은 따로 처리한다.
        place = device_name
        status = msg_info
        value = round(float(msg.payload.decode()))
        printMQTTLog(place + " " + status + " : " + str(value))
        ledStatusInfo[place] = str(value)
        lightCount += 1

        if(lightCount == 5):
            isCompleteLightCommu = True
            print(isCompleteLightCommu)
            lightCount = 0
        return


    if(device_name == "LED"):           # dev/LED/# 토픽에 대한 순환 호출을 막는다.
        return

    msg_type, msg_detail = msg_info.split('_')                              # 메시지 타입와 메시지 정보를 분리한다.
    

    if(msg_type == "value"):                                                # 메시지 타입이 센서 일 경우

        value = float(msg.payload.decode()) 

        if(msg_detail == "data"):
            printMQTTLog("received data msg from " + device_name + ", value : " + str(value))

        sensor_msg = {
         'Device Name' : device_name,
         'Value' : value
        }                                                                   # 디바이스로부터 받은 정보를 딕셔너리화

        MongoDBManager().add_data(sensor_msg, COLLECTION_NAME)              # 디바이스로 부터 받아온 센서 데이터를 MongoDB에 저장함.
    
    elif(msg_type == "alert"):                                              # 메시지 타입이 alert 일 경우(화재 감지일 경우)
        if(msg_detail == "flameDetected"):                                  # 화재가 감지되었을 경우
            value = msg.payload.decode()                                    # decode 함수를 통해 binary string을 string으로 변환
            # print("value : " + msg.payload.decode())

            if(value == "True"):
                printMQTTLog("WARNING! flame was detected!")            
                isFlameDetected = True
                
            elif(value == "False"):
                printMQTTLog("flame was not detected...")            
                isFlameDetected = False
    elif(msg_type == "setting"):                                            # 메시지 타입이 setting일 경우
        return                                                              # 어떠한 작업도 하지 않음
    
def get_light_info(request):
    global ledStatusInfo
    return HttpResponse(json.dumps(ledStatusInfo),status = 200)


def is_flame_detected():                                                    # 화재 발생 감지
    global isFlameDetected
    printMQTTLog("client check if flame was detected...")
    return isFlameDetected

def switch_flame_detected():
    global isFlameDetected
    isFlameDetected = not isFlameDetected

def request_realtime():                          # 모든 센서 디바이스를 리얼타임으로 데이터를 전송하는 모드로 변경한다. (3초 간격으로 데이터 전송함)
    printMQTTLog("change sensor device mode into realtime")
    client.publish(REQUEST_REAL_TIME, "RealTime=True\0")

def request_normal():
    printMQTTLog("change sensor device mode into normal")
    client.publish(REQUEST_REAL_TIME, "RealTime=False\0")

def ctrl_mood(msg):
    printMQTTLog("change mood lamp brightness and color : " + msg)
    client.publish(REQUEST_MOOD_LAMP, msg);

def global_ctrl_led(msg):

    # dev/LED/lights_out off
    if(msg == "lights_out"):
        client.publish(REQUEST_LED + "LIGHTS_OUT_ALL", "off\0")     # dev/#을 구독하기로 했기에 재귀적으로 다시 dev메시지를 받게 된다.
        printMQTTLog("lights out all of LED")

    # dev/LED/lights_up on      
    elif(msg == "lights_up"):
        client.publish(REQUEST_LED + "LIGHTS_OUT_ALL", "on\0")
        printMQTTLog("lights up all of LED")
        
    elif(msg == "status"):
        global ledRoomArray
        global ledStatusInfo
        ledStatusInfo = {}
        for room in ledRoomArray:
            client.publish(REQUEST_LED + room + "/" + msg, "?")
        # 디바이스에게 모든 LED의 상태를 요청한다
        # dev/LED/Place/status
        
        printMQTTLog("get all of LED status")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

try:
    client.connect(BROKER_IP)
    client.subscribe(DEVICE_ROOT_TOPIC)           # 반드시 subscribe 함수를 호출해주어야 on_message가 호출됨
    client.subscribe(DEVICE_LED_TOPIC)
    client.loop_start()
except Exception as err:
    printMQTTLog("error occured while connect to broker")


