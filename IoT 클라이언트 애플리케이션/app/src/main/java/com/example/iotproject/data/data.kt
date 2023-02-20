package com.example.iotproject.data

import com.google.gson.annotations.SerializedName


data class SensorData(
    @SerializedName("Device Name")
    val deviceName: String,
    @SerializedName("Value")
    val value: String
)

data class Dummy(
    val value:String
)