package com.example.iotproject

import android.content.Intent
import android.content.SharedPreferences
import android.graphics.PorterDuff
import android.os.Build.VERSION_CODES.P
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.example.iotproject.data.Dummy
import com.example.iotproject.databinding.FragmentTabTvBinding
import com.example.iotproject.service.APISensor
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response


class TabTvFragment : Fragment() {
    private var _binding: FragmentTabTvBinding? = null
    private val binding get() = _binding!!

    var channel: String = loadData() //저장된 채널 번호 불러오기
    var on_off: String = "OFF"


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        var now_channel :Int = 0


        //전원
        binding.switchTv.setOnCheckedChangeListener{ p, isChecked ->
            if(isChecked){
                // TV ON
                on_off="ON"
                setTvData(convertValue(on_off, channel))
            } else{
                // TV OFF
                on_off="OFF"
                setTvData(convertValue(on_off, channel))
            }
        }


        //채널 위로 가는 버튼
        binding.btnChannelUpTv.setOnClickListener{
            now_channel = binding.txtChannelTv.text.toString().toInt() + 1
            if(now_channel<10){
                channel = "0"+now_channel.toString()
                binding.txtChannelTv.setText(channel)
                setTvData(convertValue(on_off, channel))
            }
            else{
                channel = now_channel.toString()
                binding.txtChannelTv.setText(channel)
                setTvData(convertValue(on_off, channel))
            }
        }

        //채널 아래로 가는 버튼
        binding.btnChannelDownTv.setOnClickListener{
            var now_channel = binding.txtChannelTv.text.toString().toInt() - 1
            if(now_channel<=0 ){
                now_channel = 1
            }
            if(now_channel<10){
                channel = "0"+now_channel.toString()
                binding.txtChannelTv.setText(channel)
                setTvData(convertValue(on_off, channel))
            }
            else{
                channel = now_channel.toString()
                binding.txtChannelTv.setText(channel)
                setTvData(convertValue(on_off, channel))
            }

        }
    }

    private fun saveData(){
        //채널 번호 저장하기
        val pref = requireActivity().getPreferences(0)
        val edit = pref.edit()
        edit.putString("channel", binding.txtChannelTv.text.toString())
        edit.apply()
    }
    private fun loadData(): String{
        //채널 번호 불러오기
//        val pref = requireActivity().getPreferences(0)
//        binding.txtChannelTv.setText(pref.getString("channel", "01"))
//        return pref.getString("channel", "01")!
        return ""
    }

    fun convertValue(on_off : String, channel: String): String{
        return  channel + ":" + on_off      // TV 쿼리 조립
    }

    private fun setTvData(requestValue: String) {
        // TV 값을 보냄
//        APISensor.getService()
//            .setTvValue(requestValue)
//            .enqueue(object : Callback<Dummy> {
//                override fun onResponse(call: Call<Dummy>, response: Response<Dummy>) {
//
//                }
//                override fun onFailure(call: Call<Dummy>, t: Throwable) {
//                    Log.i("isWorking?", "Fail...")
//                }
//            })
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?, ): View? {
        _binding = FragmentTabTvBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onDestroy() {
        super.onDestroy()
        //Fragment가 종료되기 직전에 채널 저장
        saveData()
    }
}