#define TINY_GSM_MODEM_SIM7600
#define TINY_GSM_RX_BUFFER 1024

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

// ================= PIN DEFINITIONS =================
#define MODEM_RX 16
#define MODEM_TX 17

// ================= NETWORK CONFIG (SIM CARD) =================
const char apn[]  = "internet"; // APN Tri
const char user[] = "";
const char pass[] = "";

// ================= MQTT CONFIG =================
const char* broker   = "mqtt.iotkita.com";
const int   port     = 1883;
const char* mqttUser = "iot_kita";
const char* mqttPass = "iot_kita_shared_mqtt";

// Definisikan topik untuk Publish dan Subscribe
const char* topicPub = "device/SL001/status";
const char* topicSub = "device/SL001/cmd"; 

// ================= HTTP WEBHOOK CONFIG =================
const char* webhookServer   = "webhook.site";
const int   webhookPort     = 80; // Ubah ke 80 (HTTP biasa, sangat stabil di modul GSM)
const char* webhookResource = "/1ceafbf8-e315-40d4-b685-f5803f25c9eb";

// ================= OBJECTS =================
HardwareSerial SerialAT(2);
TinyGsm modem(SerialAT);

// Client untuk MQTT (Biasa / Port 1883 - Menggunakan Channel 0)
TinyGsmClient gsmClient(modem, 0);
PubSubClient mqtt(gsmClient);

// Client untuk HTTP (Secure / Port 443 HTTPS)
// Client untuk HTTP (Menggunakan Channel 1 agar tidak bentrok dengan MQTT)
TinyGsmClient httpClient(modem, 1);
HttpClient http(httpClient, webhookServer, webhookPort);

unsigned long lastPublish = 0;

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  
  // Inisialisasi Serial untuk Modul SIM7600
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  Serial.println("Memulai modul SIM7600...");
  modem.restart();

  if (!connectGPRS()) {
    Serial.println("GPRS Connection Error");
  }else {
    // Jika GPRS Berhasil, Lakukan HTTP Request ke Webhook
    sendWebhook();
  }

  mqtt.setServer(broker, port);
  mqtt.setCallback(mqttCallback);

  if (!connectMQTT()) {
    Serial.println("MQTT Connection Error");
  }

  Serial.println("SYSTEM READY");
}

// ================= LOOP =================
void loop() {
  if (!mqtt.connected()) {
    if(connectGPRS()){
      connectMQTT();
    }
  }

  mqtt.loop();

  if (millis() - lastPublish > 5000) {
    // Variabel penampung data GPS
    float lat = 0, lon = 0, speed = 0, alt = 0, accuracy = 0;
    int vsat = 0, usat = 0, year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;

    // Ambil data GPS dari SIM7600 (Mengembalikan nilai true jika GPS sudah mendapat sinyal satelit / "Fix")
    bool gpsFix = modem.getGPS(&lat, &lon, &speed, &alt, &vsat, &usat, &accuracy, &year, &month, &day, &hour, &min, &sec);

    JsonDocument doc;
    
    // Karena GPS dihapus, kita kirim status alat sebagai contoh
    doc["status"] = "online";
    doc["uptime"] = millis() / 1000; // Mengirimkan waktu alat menyala (dalam detik)
    doc["sinyal"] = modem.getSignalQuality(); // Mengirim kekuatan sinyal (0-31)

    // Jika GPS berhasil mendapatkan koordinat (Lock/Fix)
    if (gpsFix && lat != 0.0 && lon != 0.0) {
      doc["lat"] = lat;
      doc["lng"] = lon;
      doc["speed"] = speed;     // Kecepatan dalam km/jam
      doc["sat"] = usat;        // Jumlah satelit yang digunakan
      doc["alt"] = alt;         // Ketinggian dari permukaan laut (meter)
      doc["gps_status"] = "Terhubung";
    } else {
      // Jika GPS belum mendapat sinyal (masih mencari)
      doc["lat"] = 0.0;
      doc["lng"] = 0.0;
      doc["gps_status"] = "Mencari Satelit...";
    }

    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);

    // Publish ke broker MQTT
    mqtt.publish(topicPub, jsonBuffer);

    Serial.println("Published Data:");
    Serial.println(jsonBuffer);

    lastPublish = millis();
  }
}