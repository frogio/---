package com.example.iotproject.service

import com.example.iotproject.data.Dummy
import com.example.iotproject.data.SensorData
import retrofit2.http.GET
import retrofit2.http.Query
import retrofit2.Call
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import retrofit2.http.Header
import retrofit2.http.Path

private val BASE_URL = "http://192.168.35.113:8000/"


interface SensorDataService{
    @GET("/data/sensor/{dev}")
    fun getSensorValue(@Path("dev") dev: String): Call<SensorData>
    @GET("/data/mood/ctrl/{msg}")
    fun setMoodValue(@Path("msg") msg: String): Call<Dummy>
    @GET("/data/LED/{msg}")
    fun setLedValue(@Path("msg") msg: String): Call<Dummy>
    @GET("/data/TV/{msg}")
    fun setTvValue(@Path("msg") msg: String): Call<Dummy>
    @GET("/data/sensor/set_realtime")
    fun setRealTimeValue(): Call<Dummy>
    @GET("/data/sensor/set_normal")
    fun setNormalValue(): Call<Dummy>
}


object APISensor{
    private val retrofit = Retrofit.Builder()
        .baseUrl(BASE_URL)
        .addConverterFactory(GsonConverterFactory.create())
        .build()

    fun getService(): SensorDataService = retrofit.create(SensorDataService::class.java)
}