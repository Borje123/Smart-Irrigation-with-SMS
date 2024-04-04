#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "irrigation-mob-app-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "7CxAoV3I0S25iQK2rHJ8nXD1eXUR7FtTF5DMt0n3"

#define WIFI_SSID "ZTE_2.4G_xs6YrY"              //ZTE_2.4G_xs6YrY, PLDTHOMEFIBR272c0, TCL 306, Trojan Horse
#define WIFI_PASSWORD "yyinathh"  //yyinathh, PLDTwifi0708),kkkkkkkk, admin123

// WATER SENSOR PIN
int waterSens = A0;

void setup() {
  Serial.begin(9600);

  // Connect to WiFi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected to Wi-Fi");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  if (Firebase.failed()) {
    Serial.print("Firebase connection failed: ");
    Serial.println(Firebase.error());
  } else {
    Serial.println("Firebase connected successfully");
  }
}

void loop() {
  int sensorValue = analogRead(waterSens);

  String recipientData = Firebase.getString("NotifyList");
  int firstCommaIndex = recipientData.indexOf(',');
  String sensorData = String(sensorValue) + recipientData.substring(firstCommaIndex);

  Firebase.setString("NotifyList", sensorData);

  Firebase.setInt("SensorValue", sensorValue);

  if (sensorValue == 0) {
    Firebase.setString("SensorLevel", "Low");
  } else if (sensorValue > 1 && sensorValue < 389) {
    Firebase.setString("SensorLevel", "Low");
  } else if (sensorValue > 390 && sensorValue < 510) {
    Firebase.setString("SensorLevel", "Medium");
  } else if (sensorValue > 511) {
    Firebase.setString("SensorLevel", "High");
  } else {
    Serial.println(".");
  }

  Serial.println(sensorData);
  delay(1000);  // Adjust the delay as needed
}
