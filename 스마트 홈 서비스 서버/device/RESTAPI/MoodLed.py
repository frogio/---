from django.http import HttpResponse

from MQTT.mqtt_manager import ctrl_mood     #isFlameDetected는 mqtt_manager의 전역 변수

def ctrl_mood_led(request, msg):
    ctrl_mood(msg + "\0")
    return HttpResponse(msg)