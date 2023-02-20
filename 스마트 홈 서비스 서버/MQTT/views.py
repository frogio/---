from django.http import HttpResponse
from . import mqtt_manager                        # mqtt_manager를 호출함으로써 mqtt 설정을 시작한다.

def index(request):
    return HttpResponse("MQTT Protocol Process")