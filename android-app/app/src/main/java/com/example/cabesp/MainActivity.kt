package com.example.cabesp

import android.Manifest
import android.app.Activity
import android.bluetooth.*
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanResult
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.inputmethod.EditorInfo
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

private const val TAG = "CabEspBLE"
// Unique UUIDs for Cab-ESP BLE service and characteristics. Keep these in sync with the
// firmware on your ESP devices (src/ble_handler.cpp).
// Updated to use the user's base UUID with 1/2/3 variants for service/write/notify
private const val SERVICE_UUID = "edc01af1-5f53-4ee0-83df-e6888b252f6e"
private const val CHAR_UUID_WRITE = "edc01af2-5f53-4ee0-83df-e6888b252f6e"
private const val CHAR_UUID_NOTIFY = "edc01af3-5f53-4ee0-83df-e6888b252f6e"

class MainActivity : AppCompatActivity() {
    private lateinit var tvStatus: TextView
    private lateinit var etCommand: EditText
    private lateinit var btnSend: Button
    private lateinit var btnScan: Button
    private lateinit var tvLog: TextView

    private var bluetoothGatt: BluetoothGatt? = null
    private var writeChar: BluetoothGattCharacteristic? = null

    private val handler = Handler(Looper.getMainLooper())

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        tvStatus = findViewById(R.id.tvStatus)
        etCommand = findViewById(R.id.etCommand)
        btnSend = findViewById(R.id.btnSend)
        btnScan = findViewById(R.id.btnScan)
        tvLog = findViewById(R.id.tvLog)

        btnScan.setOnClickListener {
            requestPermissionsAndScan()
        }

        btnSend.setOnClickListener {
            sendCommand()
        }

        etCommand.setOnEditorActionListener { v, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_DONE) {
                sendCommand()
                true
            } else false
        }
    }

    private fun appendLog(s: String) {
        handler.post {
            tvLog.append("\n" + s)
        }
    }

    private fun requestPermissionsAndScan() {
        val required = mutableListOf<String>()
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED) {
            required.add(Manifest.permission.BLUETOOTH_SCAN)
        }
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            required.add(Manifest.permission.BLUETOOTH_CONNECT)
        }
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S) {
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                required.add(Manifest.permission.ACCESS_FINE_LOCATION)
            }
        }
        if (required.isNotEmpty()) {
            ActivityCompat.requestPermissions(this, required.toTypedArray(), 1)
            return
        }
        startScan()
    }

    private fun startScan() {
        appendLog("Starting scan...")
        val btAdapter = (getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager).adapter
        val scanner = btAdapter.bluetoothLeScanner
        // Lint requires an explicit permission check before starting BLE scan
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED) {
            appendLog("Missing BLUETOOTH_SCAN permission")
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.BLUETOOTH_SCAN), 1)
            return
        }
        scanner.startScan(scanCallback)
        handler.postDelayed({ scanner.stopScan(scanCallback); appendLog("Scan stopped") }, 10000)
    }

    private val scanCallback = object : ScanCallback() {
        override fun onScanResult(callbackType: Int, result: ScanResult) {
            // ensure we have permission to read scan results
            if (ContextCompat.checkSelfPermission(this@MainActivity, Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED) {
                appendLog("Missing BLUETOOTH_SCAN permission in scan callback")
                ActivityCompat.requestPermissions(this@MainActivity, arrayOf(Manifest.permission.BLUETOOTH_SCAN), 1)
                return
            }
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S) {
                if (ContextCompat.checkSelfPermission(this@MainActivity, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                    appendLog("Missing ACCESS_FINE_LOCATION permission in scan callback")
                    ActivityCompat.requestPermissions(this@MainActivity, arrayOf(Manifest.permission.ACCESS_FINE_LOCATION), 1)
                    return
                }
            }

            val name = result.device.name ?: result.scanRecord?.deviceName
            if (name == "cab-ESP-BLE") {
                appendLog("Found cab-ESP-BLE, connecting...")
                (getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager).adapter.bluetoothLeScanner.stopScan(this)
                connectToDevice(result.device)
            }
        }
    }

    private fun connectToDevice(device: BluetoothDevice) {
        tvStatus.text = "Connecting..."
        // Ensure BLUETOOTH_CONNECT permission is granted before connecting
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            appendLog("Missing BLUETOOTH_CONNECT permission")
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), 1)
            return
        }
        bluetoothGatt = device.connectGatt(this, false, gattCallback)
    }

    private val gattCallback = object : BluetoothGattCallback() {
        override fun onConnectionStateChange(gatt: BluetoothGatt, status: Int, newState: Int) {
            super.onConnectionStateChange(gatt, status, newState)
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                appendLog("Connected, discovering services...")
                tvStatus.post { tvStatus.text = "Connected" }
                // ensure we have BLUETOOTH_CONNECT permission before calling GATT operations
                if (ContextCompat.checkSelfPermission(this@MainActivity, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                    appendLog("Missing BLUETOOTH_CONNECT permission before discoverServices")
                    ActivityCompat.requestPermissions(this@MainActivity, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), 1)
                    return
                }
                gatt.discoverServices()
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                appendLog("Disconnected")
                tvStatus.post { tvStatus.text = "Disconnected" }
                writeChar = null
                bluetoothGatt = null
            }
        }

        override fun onServicesDiscovered(gatt: BluetoothGatt, status: Int) {
            super.onServicesDiscovered(gatt, status)
            // ensure we have BLUETOOTH_CONNECT permission before accessing services/descriptors
            if (ContextCompat.checkSelfPermission(this@MainActivity, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                appendLog("Missing BLUETOOTH_CONNECT permission in onServicesDiscovered")
                ActivityCompat.requestPermissions(this@MainActivity, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), 1)
                return
            }
            appendLog("Services discovered")
            val service = gatt.services.firstOrNull { it.uuid.toString().equals(SERVICE_UUID, true) }
            if (service != null) {
                val write = service.characteristics.firstOrNull { it.uuid.toString().equals(CHAR_UUID_WRITE, true) }
                val notify = service.characteristics.firstOrNull { it.uuid.toString().equals(CHAR_UUID_NOTIFY, true) }
                if (write != null) {
                    writeChar = write
                    appendLog("Found write characteristic")
                }
                if (notify != null) {
                    gatt.setCharacteristicNotification(notify, true)
                    // try to enable CCC descriptor if present
                    val desc = notify.getDescriptor(java.util.UUID.fromString("00002902-0000-1000-8000-00805f9b34fb"))
                    desc?.let {
                        it.value = BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE
                        // writeDescriptor requires BLUETOOTH_CONNECT permission
                        if (ContextCompat.checkSelfPermission(this@MainActivity, Manifest.permission.BLUETOOTH_CONNECT) == PackageManager.PERMISSION_GRANTED) {
                            gatt.writeDescriptor(it)
                            appendLog("Enabled notifications")
                        } else {
                            appendLog("Missing BLUETOOTH_CONNECT permission before writeDescriptor")
                            ActivityCompat.requestPermissions(this@MainActivity, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), 1)
                        }
                    }
                }
            } else {
                appendLog("Service not found")
            }
        }

        override fun onCharacteristicChanged(gatt: BluetoothGatt, characteristic: BluetoothGattCharacteristic) {
            super.onCharacteristicChanged(gatt, characteristic)
            if (ContextCompat.checkSelfPermission(this@MainActivity, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                appendLog("Missing BLUETOOTH_CONNECT permission in onCharacteristicChanged")
                ActivityCompat.requestPermissions(this@MainActivity, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), 1)
                return
            }
            val s = characteristic.getStringValue(0) ?: ""
            appendLog("Notify: $s")
        }

        override fun onCharacteristicWrite(gatt: BluetoothGatt, characteristic: BluetoothGattCharacteristic, status: Int) {
            super.onCharacteristicWrite(gatt, characteristic, status)
            if (ContextCompat.checkSelfPermission(this@MainActivity, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                appendLog("Missing BLUETOOTH_CONNECT permission in onCharacteristicWrite")
                ActivityCompat.requestPermissions(this@MainActivity, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), 1)
                return
            }
            appendLog("Write status: $status")
        }
    }

    private fun sendCommand() {
        val cmd = etCommand.text.toString()
        if (writeChar == null || bluetoothGatt == null) {
            appendLog("Not connected")
            return
        }
        // ensure BLUETOOTH_CONNECT permission before writing characteristic
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            appendLog("Missing BLUETOOTH_CONNECT permission before writeCharacteristic")
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), 1)
            return
        }
        // send an actual newline terminator (not the literal string "\\n")
        val toSend = if (cmd.endsWith("\n")) cmd else cmd + "\n"
        writeChar?.setValue(toSend)
        writeChar?.writeType = BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT
        bluetoothGatt?.writeCharacteristic(writeChar)
        appendLog("Sent: $toSend")
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        startScan()
    }

    override fun onDestroy() {
        super.onDestroy()
        // ensure BLUETOOTH_CONNECT permission before closing the GATT connection
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) == PackageManager.PERMISSION_GRANTED) {
            bluetoothGatt?.close()
        } else {
            appendLog("Missing BLUETOOTH_CONNECT permission before close")
        }
    }
}
