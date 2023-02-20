package com.example.iotproject

import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.PowerManager
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.viewpager2.widget.ViewPager2
import com.example.iotproject.databinding.ActivityMainBinding
import com.google.android.material.tabs.TabLayoutMediator
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.activity_main.view.*
import android.provider.Settings;

class MainActivity : AppCompatActivity() {

    val binding by lazy { ActivityMainBinding.inflate(layoutInflater)}


    @RequiresApi(Build.VERSION_CODES.M)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(binding.root)

        val pm = getSystemService(Context.POWER_SERVICE) as PowerManager
        val packageName = packageName
        if (pm.isIgnoringBatteryOptimizations(packageName)) {
        } else {    // 메모리 최적화가 되어 있다면, 풀기 위해 설정 화면 띄움.
            val intent = Intent()
            intent.action = Settings.ACTION_REQUEST_IGNORE_BATTERY_OPTIMIZATIONS
            intent.data = Uri.parse("package:$packageName")
            startActivityForResult(intent, 0)
        }

        //Tab과 ViewPager2 연결
        initViewPager()

    }

    private fun initViewPager(){
        //ViewPager2 Adapter
        var viewPager2Adatper = ViewPager2Adapter(this)
        viewPager2Adatper.addFragment(TabHomeFragment())
        viewPager2Adatper.addFragment(TabLightFragment())
        viewPager2Adatper.addFragment(TabTvFragment())
        viewPager2Adatper.addFragment(TabCctvFragment())
        viewPager2Adatper.addFragment(TabLedFragment())

        binding.viewPager2.apply {
            adapter = viewPager2Adatper
            registerOnPageChangeCallback(object : ViewPager2.OnPageChangeCallback() {
                override fun onPageSelected(position: Int) {

                    super.onPageSelected(position)
                }

            })
        }


        //ViewPager, TabLayout 연결
        TabLayoutMediator(binding.tabLayout, binding.viewPager2) { tab, position ->
            //Log.e("YMC", "ViewPager position: ${position}")
            //tabLayout.selectTab(tabLayout.getTabAt(2))

            when (position) {
                0 -> {
                    tab.text = "홈"
                    tab.icon = getDrawable(R.drawable.ic_baseline_home_24)
                }
                1 -> {
                    tab.text = "무드등"
                    tab.icon = getDrawable(R.drawable.ic_baseline_lightbulb_24)
                }
                2 -> {
                    tab.text = "TV"
                    tab.icon = getDrawable(R.drawable.ic_baseline_tv_24)
                }
                3 -> {
                    tab.text = "CCTV"
                    tab.icon = getDrawable(R.drawable.ic_baseline_video_camera_front_24)
                }
                4 -> {
                    tab.text = "조명"
                    tab.icon = getDrawable(R.drawable.ic_baseline_light_24)
                }
            }

        }.attach()
    }

}