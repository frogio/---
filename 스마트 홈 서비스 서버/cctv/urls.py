from django.urls import path
from cctv.views import *     # Views를 import 시킴으로써 MQTT 프로토콜을 설정한다.


urlpatterns = [
    path("stream/",stream ,name="stream"),
]