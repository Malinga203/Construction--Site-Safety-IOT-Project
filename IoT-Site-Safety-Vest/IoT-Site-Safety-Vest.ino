#include <ESP8266WiFi.h>
#include "DHT.h"

/* WiFi Config */
const char* ssid = "Dialog 4G 091";
const char* password = "nazeef123";

WiFiServer server(80);

/* MQ2 Sensor */
#define MQ2_A0 A0

/* DHT22 Sensor */
#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(100);

  /* Start Sensors */
  dht.begin();

  /* WiFi Connect */
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

/* ---------- LOOP ---------- */
void loop() {

  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("New Client Connected");

  while (!client.available()) {
    delay(1);
  }

  client.readStringUntil('\r');

  /* -------- Sensor Readings -------- */

  int gasLevel = analogRead(MQ2_A0);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    humidity = 0;
    temperature = 0;
  }

  /* Gas Status Logic */
  String gasStatus;

  if (gasLevel < 29)
    gasStatus = "Clean Air";
  else if (gasLevel < 30)
    gasStatus = "Possible Alcohol/Smoke";
  else if (gasLevel < 50)
    gasStatus = "Possible LPG or Methane";
  else
    gasStatus = "Heavy Gas Concentration Detected!";

  /* -------- Serial Monitor -------- */

  Serial.print("Gas: ");
  Serial.print(gasLevel);
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  Serial.print("C | Humidity: ");
  Serial.println(humidity);

  /* -------- JSON Response -------- */

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Connection: close");
  client.println();

  client.print("{");
  client.print("\"gasLevel\":");
  client.print(gasLevel);
  client.print(",");

  client.print("\"temperature\":");
  client.print(temperature);
  client.print(",");

  client.print("\"humidity\":");
  client.print(humidity);
  client.print(",");

  client.print("\"status\":\"");
  client.print(gasStatus);
  client.print("\"");

  client.print("}");

  delay(1);
}
