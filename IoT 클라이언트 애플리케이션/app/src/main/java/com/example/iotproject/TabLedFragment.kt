package com.example.iotproject

import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.example.iotproject.data.Dummy
import com.example.iotproject.databinding.FragmentTabLedBinding
import com.example.iotproject.service.APISensor
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response

class TabLedFragment : Fragment() {
    private var _binding: FragmentTabLedBinding? = null
    private val binding get()= _binding!!

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        //전체 조명
        binding.switchEntireLed.setOnCheckedChangeListener{ p, isChecked ->
            if(isChecked){
                // 전체 조명 ON
                setLedData(convertValue("거실","on"))
            } else{
                // 전체 조명 OFF
            }
        }

        //거실 조명
        binding.switchLivingLed.setOnCheckedChangeListener{ p, isChecked ->
            if(isChecked){
                // 거실 조명 ON
            } else{
                // 거실 조명 OFF
            }
        }

        //화장실 조명
        binding.switchBathLed.setOnCheckedChangeListener{ p, isChecked ->
            if(isChecked){
                // 화장실 조명 ON
            } else{
                // 화장실 조명 OFF
            }
        }

        //방1 조명
        binding.switchRoom1Led.setOnCheckedChangeListener{ p, isChecked ->
            if(isChecked){
                // 방1 조명 ON
            } else{
                // 방1 조명 OFF
            }
        }

        //방2 조명
        binding.switchRoom2Led.setOnCheckedChangeListener{ p, isChecked ->
            if(isChecked){
                // 방2 조명 ON
            } else{
                // 방2 조명 OFF
            }
        }

        //방3 조명
        binding.switchRoom3Led.setOnCheckedChangeListener{ p, isChecked ->
            if(isChecked){
                // 방3 조명 ON
            } else{
                // 방3 조명 OFF
            }
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        _binding = FragmentTabLedBinding.inflate(inflater, container, false)
        return binding.root
    }

    fun convertValue(place : String, on_off: String): String{
        return  place.substring(2) + ":" + on_off      // LED 쿼리 조립
    }

    private fun setLedData(requestValue: String) {
        // 무드등 값을 보냄
        APISensor.getService()
            .setLedValue(requestValue)
            .enqueue(object : Callback<Dummy> {
                override fun onResponse(call: Call<Dummy>, response: Response<Dummy>) {

                }
                override fun onFailure(call: Call<Dummy>, t: Throwable) {
                    Log.i("isWorking?", "Fail...")
                }
            })
    }

}