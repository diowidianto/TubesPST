#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "6tW7gXgOFe-uPT5OJ_aop48Ti2P3-gAS";
char ssid[] = "Yon";
char pass[] = "qwertyuiop";

#define DHTPIN 14      // Pin data sensor DHT22 terhubung ke D1 pada NodeMCU
#define DHTTYPE DHT22 // Jenis sensor DHT yang digunakan

#define RELAY_PIN_1 16  // Pin yang mengendalikan relay lampu
#define RELAY_PIN_2 4 // Pin yang mengendalikan relay lampu 2
#define FAN_RELAY_PIN 5 // Pin yang mengendalikan relay kipas (misalnya D5 pada NodeMCU)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW); // Awalnya matikan lampu
  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_2, LOW); // Awalnya matikan lampu 2
  pinMode(FAN_RELAY_PIN, OUTPUT);
  digitalWrite(FAN_RELAY_PIN, LOW); // Awalnya matikan relay kipas
  dht.begin();
}

void loop() {
  delay(2000); // Jeda antara pembacaan suhu dan kelembaban
  float suhu = dht.readTemperature(); // Baca suhu dari sensor
  float kelembaban = dht.readHumidity(); // Baca kelembaban dari sensor

  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Gagal membaca suhu atau kelembaban dari sensor DHT");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C\t");
    Serial.print("Kelembaban: ");
    Serial.print(kelembaban);
    Serial.println("%");

    Blynk.virtualWrite(V1, suhu);
    Blynk.virtualWrite(V2, kelembaban);

    if (suhu >= 39.0) {
      // Jika suhu di atas atau sama dengan 39 derajat, matikan lampu 1 dan lampu 2
      digitalWrite(RELAY_PIN_1, LOW);
      digitalWrite(RELAY_PIN_2, LOW);
      Serial.println("2 Lampu Mati");
    } else if (suhu < 37.0) {
      // Jika suhu di bawah 37 derajat, nyalakan lampu 1 dan lampu 2
      digitalWrite(RELAY_PIN_1, HIGH);
      digitalWrite(RELAY_PIN_2, HIGH);
      Serial.println("2 Lampu Menyala");
    } else {
      // Jika suhu di antara 37 dan 39 derajat, nyalakan satu lampu
      digitalWrite(RELAY_PIN_1, HIGH);
      digitalWrite(RELAY_PIN_2, LOW);
      Serial.println("1 Lampu Menyala");
    }

    if (kelembaban <= 60) {
      // Jika kelembaban kurang dari atau sama dengan 60%, nyalakan kipas
      digitalWrite(FAN_RELAY_PIN, HIGH);
      Serial.println("Kipas Menyala");
    } else {
      // Jika kelembaban lebih dari 60%, matikan kipas
      digitalWrite(FAN_RELAY_PIN, LOW);
      Serial.println("Kipas Mati");
    }
  }
  Blynk.run();
}
