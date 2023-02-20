package com.example.iotproject

import android.graphics.PorterDuff
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.SeekBar
import androidx.fragment.app.Fragment
import com.example.iotproject.data.Dummy
import com.example.iotproject.data.SensorData
import com.example.iotproject.databinding.FragmentTabLightBinding
import com.example.iotproject.service.APISensor
import com.google.gson.GsonBuilder
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import yuku.ambilwarna.AmbilWarnaDialog
import kotlin.math.*


class TabLightFragment : Fragment() {
    private var _binding: FragmentTabLightBinding? = null
    private val binding get() = _binding!!
    var colorValue : Int = (0xFFFFFFFF - 0x1).toInt()// 컬러 값
    var brightness: String = "0"// 밝기 값

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        //색상변경
        binding.btnColorChangeLight.setOnClickListener{
            openColorPicker();
        }

        //밝기변경
        binding.seekBarLight.setOnSeekBarChangeListener(object: SeekBar.OnSeekBarChangeListener{
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                // 값이 변할 때
                binding.txtBrightnessLight.setText(progress.toString())
                brightness = progress.toString()
                //서버에 변환된 값 전송
                setSensorData(convertValue(brightness, Integer.toHexString(colorValue)))
            }

            override fun onStartTrackingTouch(p0: SeekBar?) {
                // 터치 시작할 때
            }

            override fun onStopTrackingTouch(p0: SeekBar?) {
                // 터치 멈출 때
            }
        })
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        _binding = FragmentTabLightBinding.inflate(inflater, container, false)
        return binding.root
    }

    //ColorPicker 동작함수
    fun openColorPicker() {
        val colorPicker = AmbilWarnaDialog(requireContext(), colorValue, object : AmbilWarnaDialog.OnAmbilWarnaListener {
            override fun onCancel(dialog: AmbilWarnaDialog) {
                //취소
            }

            override fun onOk(dialog: AmbilWarnaDialog, color: Int) {
                //선택
                colorValue = color

                //버튼 색 변경
                val roundDrawable = resources.getDrawable(R.drawable.btn_light_circle)
                roundDrawable.setColorFilter(colorValue, PorterDuff.Mode.SRC_ATOP)

                binding.btnColorChangeLight.background = roundDrawable

                //서버에 변환된 값 전송
                setSensorData(convertValue(brightness, Integer.toHexString(colorValue)))
            }
        })
        colorPicker.show()
    }

    fun convertValue(brightness : String, colorValue: String): String{
        return  colorValue.substring(2) + ":" + brightness      // 무드등 쿼리 조립
    }

    private fun setSensorData(requestValue: String) {
        // 무드등 값을 보냄
        APISensor.getService()
            .setMoodValue(requestValue)
            .enqueue(object : Callback<Dummy> {
                override fun onResponse(call: Call<Dummy>, response: Response<Dummy>) {

                }
                override fun onFailure(call: Call<Dummy>, t: Throwable) {
                    Log.i("isWorking?", "Fail...")
                }
            })
    }

}