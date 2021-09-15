#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
Servo servo;

// Deklarasi Pin dan Nilai Awal
#define sensorPin A0
#define servoPin 2
#define relayPin 3
int posisiServo = 75; // dalam derajat
float sensorValue = 0;
float batasKelembaban = 60; //dalam persen
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

// Inisialisasi Awal
void setup() {
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(posisiServo);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  lcd.begin(16,2)
  lcd.backlight();
}

// Prosedur Pembacaan Nilai Kelembaban
void sensorKelembaban(){
  for (int i = 0; i <= 100; i++) {
    sensorValue = sensorValue + analogRead(sensorPin);
    delay(5);
  }
  sensorValue = sensorValue/100.0;
  sensorValue = map(sensorValue,740,190,0,100);
  Serial.print(sensorValue); Serial.println("%");
  lcd.setCursor(0, 1); lcd.print(sensorValue);
  lcd.setCursor(5,1); lcd.print("%");
}

// Program Utama
void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Kelembaban Tanah");
  sensorKelembaban();
  
  // Kalo Nilai Kelembaban Dibawah Batas, Air Mengalir
  while (sensorValue <= batasKelembaban) {
    
    // Mengaktifkan DC Water Pump
    digitalWrite(relayPin, LOW);
    
    // Menjalankan Servo
    if (posisiServo == 75) {
      for (int i = 1; i <= 30; i++) {
        posisiServo++;
        servo.write(posisiServo);
        delay(15);
      }
    }
    else if (posisiServo == 105) {
      for (int i = 1; i <= 30; i++) {
        posisiServo--;
        servo.write(posisiServo);
        delay(15);
      }
    }
    sensorKelembaban();
  }
  
  // Menonaktifkan DC Water Pump
  digitalWrite(relayPin, HIGH);
}
