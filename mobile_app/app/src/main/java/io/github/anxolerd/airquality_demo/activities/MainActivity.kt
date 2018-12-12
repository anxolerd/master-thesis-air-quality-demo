package io.github.anxolerd.airquality_demo.activities

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.LinearLayoutManager
import android.support.v7.widget.RecyclerView
import android.util.Log
import android.widget.Button
import android.widget.Toast
import io.github.anxolerd.airquality_demo.MqttHelper
import io.github.anxolerd.airquality_demo.R
import io.github.anxolerd.airquality_demo.adapters.MeasurementAdapter
import io.github.anxolerd.airquality_demo.domain.Record

class MainActivity : AppCompatActivity(), MqttHelper.TemperatureCallback, MqttHelper.HumidityCallback,
    MqttHelper.HydrogenCallback {

    private val LOG_TAG = javaClass.name
    private var mqttHelper: MqttHelper? = null

    private lateinit var measurementsView: RecyclerView
    private lateinit var enableAirConditionBtn: Button
    private lateinit var disableAirConditionBtn: Button

    companion object {
        private val measurements = listOf(
            Record("Temperature", null, "°C", min = 18.0, max = 25.0),
            Record("Humidity", null, "%", min = 20.0, max = 50.0),
            Record("Hydrogen", null, "ppm", max = 900.0)
        )
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        measurementsView = findViewById(R.id.measurementsList)
        measurementsView.adapter = MeasurementAdapter(measurements)
        measurementsView.layoutManager = LinearLayoutManager(this)

        enableAirConditionBtn = findViewById(R.id.enable_air_condition_btn)
        enableAirConditionBtn.setOnClickListener {
            mqttHelper?.sendEnableAirConditionCmd()
            Toast.makeText(this, "Command sent", Toast.LENGTH_SHORT).show()
        }

        disableAirConditionBtn = findViewById(R.id.disable_air_condition_btn)
        disableAirConditionBtn.setOnClickListener {
            mqttHelper?.sendDisableAirConditionCmd()
            Toast.makeText(this, "Command sent", Toast.LENGTH_SHORT).show()
        }

        startMqtt()
    }

    fun startMqtt() {
        if (mqttHelper == null) {
            mqttHelper = MqttHelper(applicationContext)
            subscribe()
        }
    }

    override fun onResume() {
        super.onResume()
        subscribe()
    }

    override fun onPause() {
        unsubscribe()
        super.onPause()
    }

    private fun subscribe() {
        mqttHelper?.onTemperatureCallback = this
        mqttHelper?.onHumidityCallback = this
        mqttHelper?.onHydrogenCallback = this
    }

    private fun unsubscribe() {
        mqttHelper?.onTemperatureCallback = null
        mqttHelper?.onHumidityCallback = null
        mqttHelper?.onHydrogenCallback = null
    }

    override fun onTemperature(temperature: Double) {
        Log.d(LOG_TAG, "Received new temperature value: $temperature")
        measurements[0].value = temperature
        measurementsView.adapter?.notifyItemChanged(0)
    }

    override fun onHumidity(humidity: Double) {
        Log.d(LOG_TAG, "Received new humidity value: $humidity")
        measurements[1].value = humidity
        measurementsView.adapter?.notifyItemChanged(1)
    }

    override fun onHydrogen(hydrogen: Double) {
        Log.d(LOG_TAG, "Received new hydrogen value: $hydrogen")
        measurements[2].value = hydrogen
        measurementsView.adapter?.notifyItemChanged(2)
    }

}
