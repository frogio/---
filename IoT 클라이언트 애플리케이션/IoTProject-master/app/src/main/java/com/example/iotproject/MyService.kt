package com.example.iotproject

import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.app.Service.START_REDELIVER_INTENT
import android.content.Intent
import android.os.Build
import android.os.Handler
import android.os.IBinder
import android.os.Looper
import android.util.Log
import androidx.core.app.NotificationCompat
import com.example.iotproject.data.SensorData
import com.example.iotproject.service.APISensor
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response
import java.text.SimpleDateFormat
import kotlin.concurrent.timer

class MyService : Service() {
    var flame_value: String = ""
    //처음 실행
    override fun onCreate() {
        super.onCreate()
        var latestValue: Int = 2
        var currentValue: Int = 0
        var contentText: String = ""

        //1초마다 센서 값 확인 후 텍스트 변경하기
        var timer = timer(period = 1000, initialDelay = 1000){
            // notification channel 설정
            if(getFlameData()=="True"){ //화재가 감지되면 화재발생 문구 출력 및 현재 값 1로 변경
                contentText="화재 발생!!"
                currentValue = 1
                Log.i("flame If : ", "In")
            } else{ //화재가 감지되지 않으면 화재 감지중... 문구 출력 및 현재 값 0으로 변경
                contentText="화재 감지중..."
                currentValue = 0
            }
            //최근에 발생한 값과 현재 값이 다르면 센서 상태가 변했으므로 알림 재전송
            if(latestValue!=currentValue){
                latestValue=currentValue
                if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
                    val name = "화재감지센서 작동 채널"
                    val importance = NotificationManager.IMPORTANCE_HIGH
                    val notificationChannel = NotificationChannel("foreground channel", name, importance)
                    val notificationManager = getSystemService(NOTIFICATION_SERVICE) as NotificationManager
                    notificationManager.createNotificationChannel(notificationChannel)
                }
                val builder = NotificationCompat.Builder(this@MyService, "foreground channel")
                    .setSmallIcon(R.drawable.ic_baseline_home_24)
                    .setContentText(contentText)
                startForeground(1, builder.build())
            }
        }
    }
    //재실행
    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {

        return START_REDELIVER_INTENT
    }

    override fun onDestroy() {
        // 서비스 종료
        super.onDestroy()
    }

    override fun onBind(p0: Intent?): IBinder? {
        TODO("Not yet implemented")
    }

    private fun getFlameData(): String{

        // 화재 감지 값을 가져옴
        APISensor.getService()
            .getSensorValue("flame_detect")
            .enqueue(object : Callback<SensorData> {
                override fun onResponse(call: Call<SensorData>, response: Response<SensorData>) {
                    if(response.isSuccessful){
                        val data = response.body()
                        flame_value = data?.value!!
                        Log.i("flame Value : ", "data : " +  flame_value);
                    }
                }
                override fun onFailure(call: Call<SensorData>, t: Throwable) {
                    Log.i("isWorking?", "Fail...")
                }
            })
        return flame_value
    }


}