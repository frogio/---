import cv2

class CCTVStream:

    def __init__(self):
        self.camObj = cv2.VideoCapture(0)       # 웹 캠을 opencv 와 연결한다.
        self.CCTVLog("initialization success")

    def __iter__(self):

        while(True):
            retVal, mat = self.camObj.read()

            if(not retVal):
                break

            retVal, jpgImg = cv2.imencode(".jpg", mat)
            jpgBin = bytearray(jpgImg.tobytes())
            # opencv 이미지를 jpeg 포맷으로 변환.
            
            # 비동기 처리 / 반복해서 jpeg 이미지를 찍어 리턴한다.
            yield(b'--myboundary\n'
                    b'Content-Type:image/jpeg\n'
                    b'Content-Length: ' + f"{len(jpgBin)}".encode() + b'\n'
                    b'\n' + jpgBin + b'\n')
        
    def CCTVLog(self, msg):
        print("CCTV> " + msg)

