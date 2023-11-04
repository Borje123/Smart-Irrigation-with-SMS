#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

#define WIFI_SSID "" 
#define WIFI_PASSWORD ""    

// WATER SENSOR PIN
int waterSens = A0;//set water sensor to A0

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

  int sensorValue = analogRead(waterSens);//read the water sensor valu
  
  Firebase.setInt("sensor", sensorValue);
  delay(1000); // Send data every 5 seconds (adjust as needed)


  if (sensorValue == 0) {

   
   Firebase.setString("status", "Low");
    
  } 
  
  else if (sensorValue > 1 && sensorValue < 300) {

    Firebase.setString("status", "Low");
    
    
  } 
  
  else if (sensorValue > 301 && sensorValue < 400) {
    Firebase.setString("status", "Medium");
    
  } 
  
  else if (sensorValue > 401){
    Firebase.setString("status", "High");
  }
  else {
    
    Serial.println(".");
  }

  String number = Firebase.getString("num");

  Serial.println(String(number));

   // Send data to Arduino over serial communication
  Serial.println(sensorValue);
  delay(1000); // Adjust the delay as needed


}
