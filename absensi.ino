#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

const char *ssid = "Nama-Wifi";
const char *password = "Password-Wifi";

#define SS_PIN 15
#define RST_PIN 13
int buzzer = 12;

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_PCF8574 lcd(0x27);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  lcd.begin(20, 4);
  lcd.setBacklight(255);
  lcd.setCursor(0, 0);
  lcd.print("Memulai...");

  pinMode(buzzer, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);

  lcd.setCursor(0, 1);
  lcd.print("Menghubungkan...");

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    lcd.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Failed");
    delay(2000);
    ESP.restart();
  }

  SPI.begin();
  mfrc522.PCD_Init();
  tapCard();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reconnecting...");

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
      delay(500);
      retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WiFi Restored");
      lcd.setCursor(0, 1);
      lcd.print(WiFi.localIP());
      delay(1000);
      tapCard();
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Reconn. Failed");
      delay(2000);
      ESP.restart();
    }
  }

  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      String uid = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += String(mfrc522.uid.uidByte[i], HEX);
      }
      uid.toUpperCase();

      bool success = kirim_data(uid);
      if (success) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tap Berhasil");
        lcd.setCursor(0, 1);
        lcd.print("Absen Masuk!");
        succesTone();
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tap Gagal");
        lcd.setCursor(0, 1);
        lcd.print("Absen Gagal!");
        failedTone();
      }

      delay(1000);
      tapCard();
      mfrc522.PICC_HaltA();
    }
  }
}

bool kirim_data(String uid) {
  HTTPClient http;
  String url = "Website-URL" + uid;
  http.begin(url.c_str());
  http.setTimeout(5000);

  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    http.end();
    return httpCode == 200;
  } else {
    http.end();
    return false;
  }
}

void succesTone() {
  tone(buzzer, 880, 100);
  delay(150);
  tone(buzzer, 988, 100);
  delay(150);
  tone(buzzer, 1046, 100);
  delay(150);
  noTone(buzzer);
}

void failedTone() {
  tone(buzzer, 440, 150);
  delay(200);
  tone(buzzer, 330, 150);
  delay(200);
  tone(buzzer, 220, 150);
  delay(200);
  noTone(buzzer);
}

void tapCard() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tap Your Card");
  lcd.setCursor(10, 3);
  lcd.print("BY: MCROBO");
}