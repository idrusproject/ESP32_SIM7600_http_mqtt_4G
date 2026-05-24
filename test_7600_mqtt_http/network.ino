// ================= CONNECT GPRS =================
bool connectGPRS() {
  Serial.println("Connecting to Network...");
  if (!modem.waitForNetwork()) {
    Serial.println("Network Failed");
    return false;
  }

  Serial.println("Connecting to GPRS...");
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println("GPRS Failed");
    return false;
  }

  Serial.println("GPRS Connected");
  return true;
}

// ================= CONNECT MQTT =================
bool connectMQTT() {
  String clientID = "ESP32_SIM7600_" + String(random(10000));
  
  Serial.print("Menghubungkan ke MQTT Broker: ");
  Serial.println(broker);

  // Koneksi MQTT dengan menyertakan Username dan Password
  if (mqtt.connect(clientID.c_str(), mqttUser, mqttPass)) {
    Serial.println("MQTT Connected");
    
    // Subscribe ke topik setelah berhasil connect
    mqtt.subscribe(topicSub);
    Serial.print("Subscribed ke: ");
    Serial.println(topicSub);
    
    return true;
  }

  Serial.print("MQTT Failed, status code: ");
  Serial.println(mqtt.state());
  return false;
}