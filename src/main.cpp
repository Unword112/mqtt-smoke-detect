#include <Arduino.h>

#define MQ2_SMOKE_PIN 34  // Potentiometer 1 (ตรวจจับควัน)
#define MQ2_FILTER_PIN 2 // Potentiometer 2 (ตรวจสอบไส้กรอง)
#define RELAY_PIN 5       // รีเลย์ (ควบคุมพัดลม Active Low)
#define FILTER_LED 18     // LED แจ้งเตือนไส้กรอง

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // ตั้งค่าเริ่มต้นให้รีเลย์ปิด (Active Low)

  pinMode(FILTER_LED, OUTPUT);
  digitalWrite(FILTER_LED, LOW);  // เริ่มต้นปิด LED แจ้งเตือนไส้กรอง
}

void loop() {
  int smokeValue = analogRead(MQ2_SMOKE_PIN);
  int filterValue = analogRead(MQ2_FILTER_PIN);

  Serial.print("Smoke Level: ");
  Serial.print(smokeValue);
  Serial.print(" | Filter Level: ");
  Serial.println(filterValue);

  // ตรวจจับควัน → เปิดพัดลม
  if (smokeValue > 2000) {  
    digitalWrite(RELAY_PIN, HIGH);  // เปิดรีเลย์ (พัดลมทำงาน)
    Serial.println("🔥 ควันเยอะ! เปิดพัดลม");
  } else {
    digitalWrite(RELAY_PIN, LOW); // ปิดพัดลม
    Serial.println("✅ อากาศปกติ");
  }

  // ตรวจสอบไส้กรอง → แจ้งเตือน LED ถ้าต้องเปลี่ยน
  if (filterValue > 2500) {  
    digitalWrite(FILTER_LED, HIGH);  // เปิดไฟแจ้งเตือน
    Serial.println("⚠️ ไส้กรองต้องเปลี่ยน!");
  } else {
    digitalWrite(FILTER_LED, LOW);   // ปิดไฟแจ้งเตือน
    Serial.println("✅ ไส้กรองปกติ");
  }

  delay(1000);
}
