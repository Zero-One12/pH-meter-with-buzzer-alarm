#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define buzzerPin 9
#define pHMeterPin A0
#define SerialMonitor 9600

LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C untuk LCD
//created by mgk012/Zero_One

void setup() {
  pinMode(buzzerPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" pH Meter Alarm");
  lcd.setCursor(0, 1);
  lcd.print("Zero_One");
  delay(5000);
  lcd.clear();
  Serial.begin(9600);

}

void loop() {
  float voltage = analogRead(pHMeterPin) * (5.0 / 1023 / 7); // Konversi nilai ADC ke tegangan
  float pHValue = 4 - (voltage - 2.5); // Menghitung nilai pH berdasarkan karakteristik sensor Anda

  lcd.setCursor(0, 0);
  lcd.print(" pH: ");
  lcd.print(pHValue, 2);
  Serial.println(pHValue);


  if (pHValue < 5.19) {
    lcd.setCursor(  0, 1);
    lcd.print("Harus Di Reject");
    Serial.println(pHValue);
    triggerAlarm(1); // Memicu alarm 
  } else if (pHValue >= 5.20 && pHValue < 5.35) {
    lcd.setCursor(0, 1);
    lcd.print(" pH Layak (Kritis!)");
    Serial.println(pHValue);
    triggerAlarm(2); // Memicu alarm
  } else if (pHValue >= 5.36 && pHValue < 5.99 ) {
    lcd.setCursor(0, 1);
    lcd.print(" pH Baik");
    Serial.println(pHValue);
    //triggerAlarm(); // Memicu alarm
  } else if (pHValue >= 6.00 && pHValue < 7.00) {
    lcd.setCursor(  0, 1);
    lcd.print(" pH Tinggi");
    Serial.println(pHValue);
    triggerAlarm(3); // Memicu alarm pH Tinggi
  }

  delay(100); // Delay 1 detik sebelum membaca ulang pH
}

void triggerAlarm(int alarmType) {
  if (alarmType == 1) {
    tone(buzzerPin, 2000);
    delay(10000);
    noTone(buzzerPin); // Frekuensi buzzer untuk pH < 5.19
  } else if (alarmType == 2) {
    tone(buzzerPin, 3000);
    delay(200); // Frekuensi buzzer untuk pH >= 5.20 - < 5.35
    noTone(buzzerPin);
  } else if (alarmType == 3) {
    tone(buzzerPin, 3000);
    delay(500); // Frekuensi buzzer untuk pH >= 6.00 - < 7.00
    noTone(buzzerPin);
  }
}

void stopAlarm() {
  noTone(buzzerPin);
}
