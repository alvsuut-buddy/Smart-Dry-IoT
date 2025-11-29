#define BLYNK_TEMPLATE_ID "TMPL6o80RvRFT"
#define BLYNK_TEMPLATE_NAME "SmartDry"
#define BLYNK_AUTH_TOKEN "Nhr3zY0Jupew4qve9LWkVlCDWElTqlRk"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Stepper.h> // Library Stepper Motor

// Blynk dan WiFi
char auth[] = "TOKEN_BLYNK";
char ssid[] = "SSID_WIFI"; 
char pass[] = "PASSWORD_WIFI";        

// Sensor dan Stepper Motor
#define RAIN_SENSOR_PIN 34 // Pin digital untuk sensor hujan
const int stepsPerRevolution = 512; // Jumlah langkah per putaran motor
Stepper myStepper(stepsPerRevolution, 13, 14, 12, 27);

// Variabel untuk mengendalikan motor dan status jemuran
int currentRevolutions = 0;      // Jumlah putaran saat ini
bool movingBackward = false;     // Status pergerakan motor (mundur)
bool rainDetected = false;       // Status deteksi hujan
bool jemuranDiLuar = false;      // Status awal jemuran (false = di dalam)
bool manualMode = false;         // Status mode manual (true = manual aktif)

void setup() {
  Serial.begin(115200);

  // Menghubungkan ke jaringan WiFi dan Blynk
  Blynk.begin(auth, ssid, pass);

  // Konfigurasi pin sensor dan kecepatan motor
  pinMode(RAIN_SENSOR_PIN, INPUT);
  myStepper.setSpeed(40); // Sesuaikan nilai kecepatan motor (dalam RPM)

  Serial.println("Sistem siap.");

  // Tentukan posisi awal jemuran
  if (currentRevolutions == 0) {
    jemuranDiLuar = false; // Default: jemuran di dalam
    Blynk.virtualWrite(V2, "Jemuran di dalam");
    Blynk.virtualWrite(V4, "Mode Otomatis");
  } else {
    jemuranDiLuar = true; // Default jika posisi motor tidak di awal
    Blynk.virtualWrite(V2, "Jemuran di luar");
    Blynk.virtualWrite(V4, "Mode Otomatis");
  }
}

// Fungsi untuk membaca status switch manual dari Blynk
BLYNK_WRITE(V3) {
  manualMode = param.asInt(); // Baca nilai dari Blynk switch (0 = off, 1 = on)
  if (manualMode) {
    Serial.println("Mode manual diaktifkan: Memasukkan jemuran.");
    Blynk.virtualWrite(V4, "Mode Manual");
    if (jemuranDiLuar) { // Jika jemuran sedang di luar, masukkan jemuran
      for (int i = 0; i < 10; i++) {
        myStepper.step(-stepsPerRevolution);
        currentRevolutions--;
        Serial.println("Berputar mundur, jumlah putaran: " + String(currentRevolutions));
        delay(1000);
      }
      jemuranDiLuar = false;
      Blynk.virtualWrite(V2, "Jemuran di dalam");
      Serial.println("Jemuran sekarang di dalam (mode manual).");
    }
  } else {
    Serial.println("Mode manual dimatikan: Kembali ke mode otomatis.");
    Blynk.virtualWrite(V4, "Mode Otomatis");
  }
}

void loop() {
  Blynk.run();

  // Membaca nilai dari sensor hujan
  int rainStatus = digitalRead(RAIN_SENSOR_PIN);

  // Cek status hujan dan kirimkan ke Blynk
  if (rainStatus == LOW) { // LOW berarti hujan terdeteksi
    Blynk.virtualWrite(V1, "Hujan Terdeteksi");
    Serial.println("Hujan terdeteksi!");
    rainDetected = true;
  } else {
    Blynk.virtualWrite(V1, "Tidak ada hujan");
    Serial.println("Tidak ada hujan.");
    rainDetected = false;
  }

  // Jika kontrol manual aktif, jemuran tetap di dalam dan tidak mengikuti logika otomatis
  if (manualMode) {
    Serial.println("Mode manual aktif. Jemuran tetap di dalam.");
    return; // Menghentikan eksekusi lebih lanjut agar tidak memicu kontrol otomatis
  }

  // Jika kontrol manual tidak aktif, kembali ke kontrol otomatis
  else {
    // LOGIKA 1: Jika hujan terdeteksi dan jemuran masih di luar, motor bergerak mundur
    if (rainDetected && jemuranDiLuar) {
      Serial.println("Motor berputar mundur untuk memasukkan jemuran (mode otomatis).");

      // Motor berputar mundur sejumlah putaran
      for (int i = 0; i < 10; i++) { // 10 kali putaran mundur
        myStepper.step(-stepsPerRevolution);
        currentRevolutions--;
        Serial.println("Berputar mundur, jumlah putaran: " + String(currentRevolutions));
        delay(1000); // Delay 1 detik antar putaran
      }

      jemuranDiLuar = false; // Jemuran sudah di dalam
      Blynk.virtualWrite(V2, "Jemuran di dalam");
      Serial.println("Jemuran sekarang di dalam (mode otomatis).");
    }

    // LOGIKA 2: Jika tidak hujan dan jemuran masih di dalam, motor bergerak maju
    if (!rainDetected && !jemuranDiLuar) {
      Serial.println("Tidak ada hujan, motor berputar maju untuk mengeluarkan jemuran (mode otomatis).");

      // Motor berputar maju sejumlah putaran
      for (int i = 0; i < 10; i++) { // 10 kali putaran maju
        myStepper.step(stepsPerRevolution);
        currentRevolutions++;
        Serial.println("Berputar maju, jumlah putaran: " + String(currentRevolutions));
        delay(1000); // Delay 1 detik antar putaran
      }

      jemuranDiLuar = true; // Jemuran sudah di luar
      Blynk.virtualWrite(V2, "Jemuran di luar");
      Serial.println("Jemuran sekarang di luar (mode otomatis).");
    }
  }

  delay(1000); // Delay 1 detik sebelum membaca sensor lagi
}