package com.tutorials.bluetoothjoystick

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.os.Bundle
import android.os.Handler
import android.util.DisplayMetrics
import android.util.Log
import android.view.MotionEvent
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import java.io.IOException
import java.io.InputStream
import java.io.OutputStream
import java.util.*

class MainActivity : AppCompatActivity() {

    private var xDelta = 0
    private var yDelta = 0

    private var thumbCenterX = 0
    private var thumbCenterY = 0

    private var joyXPos = 0
    private var joyYPos = 0

    var btConnected = false

    private lateinit var imageView: ImageView
    private lateinit var tvXPos: TextView
    private lateinit var tvYPos: TextView
    private lateinit var rlJoystickContainer: RelativeLayout
    private lateinit var btnBTToggle: Button

    private lateinit var bluetoothAdapter: BluetoothAdapter
    private lateinit var btSocket: BluetoothSocket
    private lateinit var relayOutputStream: OutputStream
    private lateinit var relayInputStream: InputStream

    private lateinit var commandHandler: Handler
    private val commandInterval: Long = 100 // Milliseconds

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        imageView = findViewById(R.id.imageView)
        tvXPos = findViewById(R.id.tvXPOS)
        tvYPos = findViewById(R.id.tvYPOS)
        rlJoystickContainer = findViewById(R.id.rlJoystickContainer)
        btnBTToggle = findViewById(R.id.btnConnectBluetooth)

        commandHandler = Handler()

        val bluetoothManager: BluetoothManager = getSystemService(BluetoothManager::class.java)
        bluetoothAdapter = bluetoothManager.adapter

        //setup layout params
        val layoutParams = RelativeLayout.LayoutParams(200, 200)
        imageView.layoutParams = layoutParams
        //setup touch listener
        imageView.setOnTouchListener(CustomTouchListener())
        setThumbCenter()
        centerThumb()

        btnBTToggle.setOnClickListener {
            if(btConnected == true){
                btSocket.close()
                btConnected = false
                stopCommandTask()
            } else {
                val relayAddress = getBluetoothAddress("ESP32_RELAY")
                if(relayAddress != null){
                    Log.d("BLUETOOTH", "Relay found: $relayAddress")
                    val p: ConnectThread = ConnectThread(bluetoothAdapter.getRemoteDevice(relayAddress))
                    p.start()
                } else {
                    Toast.makeText(this, "Could not find relay", Toast.LENGTH_SHORT).show()
                }
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        stopCommandTask()
    }

    val commandTask: Runnable = object : Runnable {
        override fun run() {
            try {
                sendCommand() //this function can change value of mInterval.
            } finally {
                // 100% guarantee that this always happens, even if
                // your update method throws an exception
                commandHandler.postDelayed(this, commandInterval)
            }
        }
    }

    private inner class CustomTouchListener : View.OnTouchListener {
        override fun onTouch(v: View, event: MotionEvent): Boolean {
            val x = event.rawX.toInt()
            val y = event.rawY.toInt()
            when (event.action and MotionEvent.ACTION_MASK) {
                MotionEvent.ACTION_DOWN -> {
                    val lParams = v.layoutParams as RelativeLayout.LayoutParams
                    xDelta = x - lParams.leftMargin
                    yDelta = y - lParams.topMargin
                }
                MotionEvent.ACTION_UP -> {
                    Toast.makeText(this@MainActivity, "Image is on new Location!", Toast.LENGTH_SHORT).show()
                    centerThumb()
                }
                MotionEvent.ACTION_POINTER_DOWN -> {
                }
                MotionEvent.ACTION_POINTER_UP -> {
                }
                MotionEvent.ACTION_MOVE -> {
                    val layoutParams = v.layoutParams as RelativeLayout.LayoutParams
                    layoutParams.leftMargin = x - xDelta
                    layoutParams.topMargin = y - yDelta
                    layoutParams.rightMargin = 0
                    layoutParams.bottomMargin = 0
                    v.layoutParams = layoutParams

                    joyXPos = x-xDelta-thumbCenterX
                    joyYPos = y-yDelta-thumbCenterY
                    tvXPos.text = "X: $joyXPos"
                    tvYPos.text = "Y: $joyYPos"
                }
            }
            rlJoystickContainer.invalidate()
            return true
        }
    }

    private inner class ConnectThread(device: BluetoothDevice): Thread(){
        private val mmSocket: BluetoothSocket? by lazy(LazyThreadSafetyMode.NONE){
            val uuid: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
            device.createRfcommSocketToServiceRecord(uuid)
        }

        public override fun run(){
            mmSocket?.let { socket ->
                socket.connect()
                Log.d("BLUETOOTH", "Relay Connection Successful")
                btSocket = socket
                relayInputStream = socket.inputStream
                relayOutputStream = socket.outputStream
                btConnected = true
                startCommandTask()
            }
        }

        fun cancel(){
            try{
                mmSocket?.close()
            } catch (e: IOException){
                Log.e("BLUETOOTH", "Could not close the client socket", e)
            }
        }
    }

    private fun setThumbCenter(){
        val displayMetrics = DisplayMetrics()
        windowManager.defaultDisplay.getMetrics(displayMetrics)

        val screenHeight = displayMetrics.heightPixels
        val screenWidth = displayMetrics.widthPixels

        val imageLayoutParams = imageView.layoutParams as RelativeLayout.LayoutParams
        val imageHeight = imageLayoutParams.height
        val imageWidth = imageLayoutParams.width

        thumbCenterX = (screenWidth / 2) - (imageWidth / 2)
        thumbCenterY = (screenHeight / 2) - (imageHeight / 2)

    }

    private fun centerThumb(){
        val imageLayoutParams = imageView.layoutParams as RelativeLayout.LayoutParams
        imageLayoutParams.leftMargin = thumbCenterX
        imageLayoutParams.topMargin = thumbCenterY
        imageLayoutParams.rightMargin = 0
        imageLayoutParams.bottomMargin = 0
        imageView.layoutParams = imageLayoutParams

        joyXPos = 0
        joyYPos = 0
        tvXPos.text = "X:0"
        tvYPos.text = "Y:0"

    }

    private fun getBluetoothAddress(deviceName: String): String? {
        val pairedDevices: Set<BluetoothDevice> = bluetoothAdapter.bondedDevices
        pairedDevices?.forEach{device ->
            if(device.name == deviceName){
                return device.address
            }
        }
        return null
    }

    private fun startCommandTask(){
        commandTask.run()

        /*
        val msgToSend = "Hello world!"
        Log.d("BLUETOOTH", "Sending message '$msgToSend'")
        val outputStream: OutputStream = btSocket.outputStream
        outputStream.write("$msgToSend\n".toByteArray())
        outputStream.flush()

        btSocket.close()
        btConnected = false
        */
    }

    private fun stopCommandTask(){
        commandHandler.removeCallbacks(commandTask)

        Log.d("COMMAND", "Stopping command task")
    }

    private fun sendCommand(){
        val boundedXValue = joyXPos.coerceIn(-255..255)
        val boundedYValue = joyYPos.coerceIn(-255..255) * -1
        Log.d("COMMAND", "Raw values: THR $boundedYValue STR $boundedXValue")

        val throttleValue: Double = ((boundedYValue + 255) / 2).toDouble() / 255
        val steeringValue: Double = ((boundedXValue + 255) / 2).toDouble() / 255

        Log.d("COMMAND", "Normalized values: THR $throttleValue STR $steeringValue")

        //val commandString = "THR $throttleValue STR $steeringValue"
        val commandString = String.format("THR %.2f STR %.2f", throttleValue, steeringValue)
        relayOutputStream.write(commandString.toByteArray())
        relayOutputStream.flush()
        Log.d("COMMAND", "Send command: $commandString")
    }
}