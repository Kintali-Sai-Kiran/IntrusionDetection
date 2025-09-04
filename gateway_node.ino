#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi Credentials
const char* ssid = "ASHURA";
const char* password = "gnam9999";

// Web Server
ESP8266WebServer server(80);

// Buzzer Pin
const int buzzerPin = D6;

// Sensor Data
int pirState = 0;
int vibrationState = 0;

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);  // Ensure buzzer is OFF

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nGateway Node Connected!");

  // Define server routes
  server.on("/update", HTTP_GET, []() {
    if (server.hasArg("pir") && server.hasArg("vibration")) {
      pirState = server.arg("pir").toInt();
      vibrationState = server.arg("vibration").toInt();

      Serial.print("PIR: ");
      Serial.print(pirState);
      Serial.print(" | Vibration: ");
      Serial.println(vibrationState);

      // Buzzer Logic: Activate if PIR or both sensors detect movement
      if (pirState == 1 || vibrationState == 1) {
        digitalWrite(buzzerPin, HIGH);
        Serial.println("Buzzer ON");
      } else {
        digitalWrite(buzzerPin, LOW);
        Serial.println("Buzzer OFF");
      }

      server.send(200, "text/plain", "Data Received");
    } else {
      server.send(400, "text/plain", "Missing Parameters");
    }
  });

  // Start Server
  server.begin();
}

void loop() {
  server.handleClient();
}
