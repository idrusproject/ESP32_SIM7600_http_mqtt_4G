// ================= FUNGSI HTTP GET & POST =================
void sendWebhook() {
  // --- Proses HTTP GET ---
  Serial.println("\n--- Memulai HTTP GET ---");
  http.get(webhookResource);
  
  int getStatusCode = http.responseStatusCode();
  String getResponse = http.responseBody();
  
  Serial.print("GET Status Code: ");
  Serial.println(getStatusCode);
  Serial.print("GET Response: ");
  Serial.println(getResponse);
  
  http.stop();

  // --- Proses HTTP POST ---
  Serial.println("\n--- Memulai HTTP POST ---");
  String contentType = "application/json";
  
  JsonDocument docPost;
  docPost["event"] = "device_started";
  docPost["message"] = "Halo dari ESP32 SIM7600 via POST HTTP!";
  
  String postData;
  serializeJson(docPost, postData);
  
  http.post(webhookResource, contentType, postData);
  
  int postStatusCode = http.responseStatusCode();
  String postResponse = http.responseBody();
  
  Serial.print("POST Status Code: ");
  Serial.println(postStatusCode);
  Serial.print("POST Response: ");
  Serial.println(postResponse);
  
  http.stop();
}