from django.views.generic import TemplateView
from django.http import StreamingHttpResponse
from .cctv import CCTVStream

cctv = CCTVStream()

class camView(TemplateView):
    template_name = "cam.html"

    def get_context_data(self):
        context = super.get_context_data()
        return context

def stream(request):
    cctv.CCTVLog("start to streaming...")
    return StreamingHttpResponse(cctv, content_type="multipart/x-mixed-replace;boundary=--myboundary")