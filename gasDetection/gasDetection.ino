#include <ESP8266WiFi.h>

const char* ssid = "";
const char* password = "";

#define GAS_PIN A0
#define BUZZER_PIN D3
#define LED_PIN D2

int gasValue = 0;
int threshold = 500; //safety limit

WiFiClient client;
const char* host = ""; //server
String apiKey = "";   

void setup() {

  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  connectWiFi();
}

void loop() {

  readGasSensor();
  checkGasLevel();
  sendDataWiFi();

  delay(15000);
}

void connectWiFi() {

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void readGasSensor() {

  gasValue = analogRead(GAS_PIN);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);
}

void checkGasLevel() {

  if (gasValue > threshold) {

    Serial.println("DANGER: Gas Detected");

    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  }
  else {

    Serial.println("SAFE");

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }
}
//pending
void sendDataWiFi() {

  if (!client.connect(host, 80)) {
    Serial.println("Connection Failed");
    return;
  }

  String url = "/update?api_key=" + apiKey +
               "&field1=" + String(gasValue);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Data Sent to Cloud");
}