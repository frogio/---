from django.http import HttpResponse
import json

from device.mongoDB_manager import MongoDBManager
from MQTT.mqtt_manager import request_realtime, request_normal, is_flame_detected    #isFlameDetected는 mqtt_manager의 전역 변수

# from MQTT.mqtt_manager import 

COLLECTION_NAME = "SensorValue"

def get_sensor_latest_value(request, devname):

    def get():                                     # DB 내용 조회
        sensor_value = MongoDBManager().get_data({'Device Name' : devname}, COLLECTION_NAME).sort("_id", -1)
        # MongoDB는 싱글톤 클래스
        # MongoDBManger가 리턴하는 커서객체(데이터 리스트)를 가장 마지막에 들어온 순서대로 정렬한다.
        response = sensor_value[0]
        del response['_id']

        return HttpResponse(json.dumps(response), status = 200)

    if(request.method == "GET"):                    # 요청이 GET 일 때
        return get()

def change_realtime(request):              # 모든 센서 디바이스의 모드를 리얼타임 모드로 변경한다. 
                                           # (센서 디바이스가 2초 간격으로 데이터를 전송함)
    request_realtime()
    return HttpResponse("Change sensor device mode into realtime", status = 200)
    
def change_normal(request):              # 모든 센서 디바이스의 모드를 노말 모드로 변경한다. 
                                         # (센서 디바이스가 5분 간격으로 데이터를 전송함)
    request_normal()
    return HttpResponse("Change sensor device mode into normal", status = 200)

def flame_detected(request):

    sensor_msg = {
         "Device Name" : "FIRE",
         "Value" : "False"
    }

    if(is_flame_detected() == True):        # 화재가 감지될 경우
        # print("Send alert signal")
        sensor_msg["Value"] = "True"
        # switch_flame_detected()
        return HttpResponse(json.dumps(sensor_msg), status = 200)



    return HttpResponse(json.dumps(sensor_msg), status = 200)
    
