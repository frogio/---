package com.example.iotproject


import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.webkit.WebSettings
import androidx.fragment.app.Fragment
import com.example.iotproject.databinding.FragmentTabCctvBinding
import android.webkit.WebViewClient;



class TabCctvFragment : Fragment() {
    private var _binding: FragmentTabCctvBinding? = null
    private val binding get()= _binding!!

    val cctv_url = "http://192.168.35.113/mjpeg/?mode=stream"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        var  mWebSettings: WebSettings = binding.wv.settings;
        //web view를 이용하여 cctv streaming 연결
        binding.wv.setWebViewClient(WebViewClient())
        mWebSettings.javaScriptEnabled = true //자바스크립트 허용
        mWebSettings.setSupportMultipleWindows(false) //새 창 띄우기 비허용
        mWebSettings.javaScriptCanOpenWindowsAutomatically = false // 멀티 뷰 비허용
        mWebSettings.loadWithOverviewMode = true //메타태그 허용
        mWebSettings.useWideViewPort = true //화면 사이즈 맞추기 허용
        mWebSettings.setSupportZoom(false) //화면 줌 비허용
        mWebSettings.builtInZoomControls = false //화면 확대 축소 비허용
        mWebSettings.layoutAlgorithm = WebSettings.LayoutAlgorithm.SINGLE_COLUMN //컨텐츠 사이즈 맞추기
        mWebSettings.cacheMode = WebSettings.LOAD_NO_CACHE //브라우저 캐시 비허용
        mWebSettings.domStorageEnabled = true //로컬저장소 허용

        binding.wv.loadUrl(cctv_url) //cctv stream 주소


    }


    //화면에 안보일 때 = 다른 탭으로 이동했을 때
     override fun onPause() {
        super.onPause()
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        _binding = FragmentTabCctvBinding.inflate(inflater, container, false)
        return binding.root
    }

}