This is a minimal Android app (Kotlin) to talk to the cab-ESP BLE service.

Open the `android-app` folder in Android Studio. The app scans for a device named `cab-ESP-BLE`, connects, discovers the custom service and characteristics, and provides a tiny UI to send commands and show incoming notifications.

Service & Characteristic UUIDs used by the ESP firmware (change both sides together):
- Service: b3a6f4d2-9c4e-4f2a-8a1e-6c3f2d9b1a77
- Write Characteristic: b3a6f4d2-9c4e-4f2a-8a1e-6c3f2d9b1a78
- Notify Characteristic: b3a6f4d2-9c4e-4f2a-8a1e-6c3f2d9b1a79

Note: This is a skeleton project. Open in Android Studio, let it sync Gradle, and run on a device (BLE is limited on emulators).