# SmartDry: Jemuran Otomatis Cerdas Berbasis IoT 🌦️👕

**SmartDry** adalah sistem jemuran otomatis yang dirancang untuk mengatasi masalah cuaca tak menentu di Indonesia. Alat ini secara otomatis menarik pakaian ke tempat teduh saat hujan turun dan mengeluarkannya kembali saat cerah, serta dapat dipantau melalui aplikasi Blynk.

## 🚀 Fitur Utama
* [cite_start]**Deteksi Hujan Otomatis:** Menggunakan sensor hujan untuk menarik jemuran masuk secara otomatis[cite: 82, 83].
* [cite_start]**Monitoring Real-time:** Memantau status cuaca dan posisi jemuran melalui smartphone[cite: 111].
* [cite_start]**Kontrol Manual:** Mode manual via aplikasi Blynk untuk mengontrol jemuran tanpa menunggu sensor[cite: 113].
* [cite_start]**Notifikasi:** Mengirim notifikasi ke pengguna saat terjadi perubahan status[cite: 112].

## 🛠️ Komponen Hardware
[cite_start]Project ini dibangun menggunakan komponen berikut[cite: 104, 127]:
* **Mikrokontroler:** ESP32 DOIT DEVKIT V1
* **Sensor:** Modul Sensor Hujan (Raindrops Module)
* **Aktuator:** Motor Stepper + Driver ULN2003
* **Software:** Arduino IDE & Blynk IoT Platform

## 🔌 Peta Pin (Wiring)
[cite_start]Berikut adalah konfigurasi pin yang digunakan pada ESP32[cite: 120, 121]:
| Komponen | Pin ESP32 |
| :--- | :--- |
| Motor Stepper IN1 | D13 |
| Motor Stepper IN2 | D12 |
| Motor Stepper IN3 | D14 |
| Motor Stepper IN4 | D27 |
| Sensor Hujan (DO) | D34 |

![Wiring Diagram](docs/peta pin.png)
*(Note: Upload gambar Peta Pin dari laporan Anda ke folder docs dan link di sini)*

## 💻 Cara Kerja Sistem
1.  **Sensor** mendeteksi kondisi air/hujan.
2.  **ESP32** memproses data. [cite_start]Jika `rainStatus == LOW` (Hujan), motor stepper berputar mundur untuk menarik jemuran[cite: 204, 209].
3.  [cite_start]**Blynk** menerima update status "Hujan Terdeteksi" dan "Jemuran di Dalam"[cite: 204, 211].