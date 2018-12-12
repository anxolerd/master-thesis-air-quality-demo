package io.github.anxolerd.airquality_demo

import android.content.Context
import android.util.Log
import org.eclipse.paho.android.service.MqttAndroidClient
import org.eclipse.paho.client.mqttv3.*


class MqttHelper(context: Context) : MqttCallbackExtended {
    companion object {
        private val LOG_TAG = ::MqttHelper.javaClass.name

        const val TEMPERATURE_TOPIC = "anxolerd/feeds/temperature"
        const val HUMIDITY_TOPIC = "anxolerd/feeds/humidity"
        const val HYDROGEN_TOPC = "anxolerd/feeds/hydrogen"
        const val COMMANDS_TOPIC = "anxolerd/feeds/cmd"

        internal val MQTT_SERVER_URL = "ssl://localhost:8883"

        // Credentials
        // TODO: move outside the code
        internal val CLIENT_ID = "air-quality-android-app"
        internal val MQTT_USER = "username"
        internal val MQTT_PASS = "password"
    }

    var mqttClient: MqttAndroidClient

    var onTemperatureCallback: TemperatureCallback? = null
    var onHumidityCallback: HumidityCallback? = null
    var onHydrogenCallback: HydrogenCallback? = null


    init {
        mqttClient = MqttAndroidClient(context, MQTT_SERVER_URL, CLIENT_ID)
        mqttClient.setCallback(this)
        connect()
    }

    fun sendEnableAirConditionCmd() {
        mqttClient.publish(COMMANDS_TOPIC, "enableAirCondition".toByteArray(), 2, false)
    }

    fun sendDisableAirConditionCmd() {
        mqttClient.publish(COMMANDS_TOPIC, "disableAirCondition".toByteArray(), 2, false)
    }

    private fun connect() {
        val mqttConnectOptions = MqttConnectOptions()
        mqttConnectOptions.mqttVersion = MqttConnectOptions.MQTT_VERSION_3_1_1
        mqttConnectOptions.isAutomaticReconnect = true
        mqttConnectOptions.isCleanSession = true
        mqttConnectOptions.userName = MQTT_USER
        mqttConnectOptions.password = MQTT_PASS.toCharArray()

        try {
            mqttClient.connect(mqttConnectOptions, null, object : IMqttActionListener {
                override fun onSuccess(asyncActionToken: IMqttToken) {

                    val disconnectedBufferOptions = DisconnectedBufferOptions()
                    disconnectedBufferOptions.isBufferEnabled = true
                    disconnectedBufferOptions.bufferSize = 100
                    disconnectedBufferOptions.isPersistBuffer = false
                    disconnectedBufferOptions.isDeleteOldestMessages = false
                    mqttClient.setBufferOpts(disconnectedBufferOptions)
                    subscribe()
                }

                override fun onFailure(asyncActionToken: IMqttToken, exception: Throwable) {
                    Log.w(LOG_TAG, "Failed to connect to: " + MQTT_SERVER_URL + exception.toString())
                    exception.printStackTrace()
                }
            })


        } catch (ex: MqttException) {
            Log.e(LOG_TAG, "Something bad happened", ex)
        }

    }


    private fun subscribe() {
        sequenceOf(TEMPERATURE_TOPIC, HUMIDITY_TOPIC, HYDROGEN_TOPC).forEach {
            try {
                mqttClient.subscribe(it, 0)
            } catch (e: MqttException) {
                Log.e(LOG_TAG, "Exception while subscribing to $it", e)
            }
        }
    }

    override fun connectComplete(reconnect: Boolean, serverURI: String?) {
        val operation: String = if (reconnect) "reconnected" else "connected"
        Log.i(LOG_TAG, "Successfully $operation to $serverURI")
    }

    override fun messageArrived(topic: String?, message: MqttMessage?) {
        when (topic) {
            TEMPERATURE_TOPIC -> onTemperatureCallback?.onTemperature(message.toString().toDouble())
            HUMIDITY_TOPIC -> onHumidityCallback?.onHumidity(message.toString().toDouble())
            HYDROGEN_TOPC -> onHydrogenCallback?.onHydrogen(message.toString().toDouble())
            else -> Log.d(LOG_TAG, "Received message to '$topic'")
        }
    }

    override fun connectionLost(cause: Throwable?) {
        Log.e(LOG_TAG, "Connection lost!", cause)
    }

    override fun deliveryComplete(token: IMqttDeliveryToken?) {
        val message = token?.message?.toString()
        Log.d(LOG_TAG, "Message has been sent: $message")
    }


    interface TemperatureCallback {
        fun onTemperature(temperature: Double)
    }

    interface HumidityCallback {
        fun onHumidity(humidity: Double)
    }

    interface HydrogenCallback {
        fun onHydrogen(hydrogen: Double)
    }
}