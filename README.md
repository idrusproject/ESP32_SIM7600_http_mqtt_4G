# ESP32 SIM7600 4G LTE - MQTT, HTTP Webhook & Internal GPS

Proyek ini adalah implementasi lengkap Internet of Things (IoT) menggunakan mikrokontroler **ESP32** dan modul 4G LTE **SIM7600**. Program ini mencakup koneksi GPRS, eksekusi HTTP Request (GET & POST) ke Webhook, koneksi ke MQTT Broker dengan autentikasi, serta pengambilan koordinat menggunakan GPS internal bawaan dari SIM7600.

Semua data diproses dan diubah menjadi format JSON menggunakan `ArduinoJson` (v7) sebelum dikirim ke server.

---

## 🌟 Fitur Utama
- **Koneksi Jaringan 4G/GPRS** menggunakan library TinyGSM.
- **HTTP GET & POST Request** dieksekusi sekali saat alat baru menyala (dikirim ke `webhook.site`).
- **MQTT Publish & Subscribe** untuk mengirim data telemetri secara berkala dan menerima perintah (Command) secara realtime.
- **Internal GNSS / GPS** membaca data lokasi, kecepatan, ketinggian, dan jumlah satelit langsung dari chip SIM7600 tanpa perlu modul GPS tambahan (seperti Neo-6M).
- **Multiplexing** memisahkan jalur komunikasi MQTT (Channel 0) dan HTTP (Channel 1) agar tidak saling bertabrakan.

---

## 🛠️ Persyaratan Perangkat Keras (Hardware)
1. **Board:** ESP32 Development Board (ESP-WROOM-32).
2. **Modul Seluler:** Modul SIM7600 (SIM7600E, SIM7600G, dll).
3. **Kartu SIM:** Kartu SIM dengan kuota data reguler aktif (Contoh di kode ini menggunakan **Tri / 3gprs**).
4. **Antena:** 
   - Antena MAIN / LTE (Wajib dipasang agar mendapat sinyal internet).
   - Antena GNSS / GPS Aktif (Wajib dipasang dan diletakkan di luar ruangan agar mendapat sinyal satelit).
5. **Power Supply Tambahan:** Modul SIM7600 membutuhkan arus hingga **2A** saat melakukan transmisi jaringan. Sangat disarankan memberi daya pada SIM7600 menggunakan adaptor 5V 2A terpisah, jangan hanya mengandalkan pin 5V (VIN) dari ESP32 saat terhubung ke USB komputer.

---

## 🔌 Konfigurasi Wiring (Kabel)

Hubungkan ESP32 dengan Modul SIM7600 sesuai tabel berikut:

| ESP32 Pin | SIM7600 Pin | Keterangan |
| :--- | :--- | :--- |
| `GND` | `GND` | Wajib dihubungkan agar tegangan referensi sama (Common Ground). |
| `GPIO 16` (RX2) | `TX` | Pin TX Modul masuk ke pin RX ESP32. |
| `GPIO 17` (TX2) | `RX` | Pin RX Modul masuk ke pin TX ESP32. |

> **⚠️ Catatan Tegangan (Logic Level):** Jika modul SIM7600 Anda tidak memiliki *Logic Level Converter* internal, pastikan Anda menggunakan pembagi tegangan (Voltage Divider) pada jalur TX ESP32 ke RX SIM7600, karena logic ESP32 adalah 3.3V dan beberapa chip SIM7600 murni menggunakan logic 1.8V/3.3V/5V tergantung *breakout board*-nya.

---

## 💻 Persyaratan Perangkat Lunak (Software)

Pastikan Anda menggunakan versi *Core* dan *Library* yang sesuai agar program berjalan tanpa kendala kompilasi.

*   **Board Manager:** `esp32 by Espressif Systems` versi **3.3.8**
*   **Library (Install via Library Manager):**
    *   **TinyGSM** by Volodymyr Shymanskyy - v`0.12.0`
    *   **PubSubClient** by Nick O'Leary - v`2.8`
    *   **ArduinoHttpClient** by Arduino - v`0.6.1`
    *   **ArduinoJson** by Benoit Blanchon - v`7.4.3`

---

## ⚙️ Panduan Penggunaan & Konfigurasi

Sebelum mengunggah (upload) kode ke ESP32, sesuaikan beberapa konfigurasi di bawah ini sesuai dengan jaringan dan server Anda:

### 1. Konfigurasi Jaringan (APN)
Sesuaikan APN dengan kartu SIM yang digunakan. Contoh di bawah menggunakan kartu Tri.
```cpp
const char apn[]  = "internet"; // Bisa menggunakan "3gprs" untuk kartu Tri
const char user[] = "";
const char pass[] = "";****
