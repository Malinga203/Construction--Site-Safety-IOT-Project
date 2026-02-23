#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D4
#define RST_PIN D3

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();             
  mfrc522.PCD_Init();      

  Serial.println("Scan an RFID Card...");
}

void loop() {

  // Check if card is present
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Read the card
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card UID: ");

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }

  Serial.println();

  mfrc522.PICC_HaltA();
}
