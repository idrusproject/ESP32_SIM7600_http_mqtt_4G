// ================= MQTT CALLBACK (SUBSCRIBE) =================
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan masuk di topik: ");
  Serial.println(topic);

  // Parse payload JSON yang masuk
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("Gagal membaca JSON: ");
    Serial.println(error.c_str());
    
    // Jika bukan JSON, print sebagai teks biasa
    Serial.print("Pesan teks: ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    return;
  }

  // Contoh membaca perintah dari JSON
  const char* perintah = doc["perintah"];
  int nilai = doc["nilai"];

  Serial.print("Perintah diterima: ");
  Serial.println(perintah ? perintah : "Tidak ada kunci 'perintah'");
  Serial.print("Nilai: ");
  Serial.println(nilai);
}