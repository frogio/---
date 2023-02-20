from django.http import HttpResponse
from MQTT.mqtt_manager import ledStatusInfo, isCompleteLightCommu, global_ctrl_led
import json
# import MQTT

def ctrl_led(request, place, msg):
    global isCompleteLightCommu
    if(place == "all"):
        global_ctrl_led(msg)
        
        if(msg == "status"):
            print("isEnd : " + str(isCompleteLightCommu)) 
            while(isCompleteLightCommu == False): None         # 통신이 완료될 때 까지 기다린다.
            return HttpResponse(json.dumps(ledStatusInfo),status = 200)

    return HttpResponse(place + " : " + msg)