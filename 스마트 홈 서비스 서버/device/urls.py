from django.urls import path
from device.RESTAPI import SensorData, MoodLed, LED

urlpatterns = [
    path("sensor/<str:devname>/latest_value", SensorData.get_sensor_latest_value),
    path("sensor/set_realtime", SensorData.change_realtime),
    path("sensor/set_normal", SensorData.change_normal),
    path("sensor/flame_detect", SensorData.flame_detected),
    path("mood/ctrl/<str:msg>", MoodLed.ctrl_mood_led),
    path("LED/<str:place>/<str:msg>", LED.ctrl_led),

]   # views.py를 매칭하는것 대신 RESTAPI의 파이썬 파일들을 매칭함.,