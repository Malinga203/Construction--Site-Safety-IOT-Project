#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();

  // Check if reading is valid
  if (isnan(temp)) {
    Serial.println("Sensor Error!");
  } else {
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println(" C");
  }

  delay(2000);
}