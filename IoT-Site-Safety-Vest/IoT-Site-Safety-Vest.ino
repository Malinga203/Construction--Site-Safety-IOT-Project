#include <ESP8266WiFi.h>

const char* ssid = "";
const char* password = "";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(100);

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
  Serial.println("Web server started");
}

void loop() {
  // put your main code here, to run repeatedly:

}
