from django.urls import path
from . import views     # Views를 import 시킴으로써 MQTT 프로토콜을 설정한다.

urlpatterns = [
	path("", views.index),
]