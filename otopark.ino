#include <Servo.h>

const int trigPin = 7;
const int echoPin = 6;
const int yesilLED = 8;       // Yesil LED
const int kirmiziLED = 13;    // Kirmizi LED
const int sifirlaButonu = 2;  // Sifirla butonu
const int servoPin = 9;       // Servo pinini tanimla

Servo benimServo;            // Servo motor nesnesi olustur
int mesafeCm = 0;            // Baslangicta mesafeyi 0 olarak ayarla
int sayac = 0;               // Sayac baslangic degeri 0
bool servoAktif = false;     // Servo motorunun calisip calismadigini kontrol etmek icin

void setup() {
  pinMode(trigPin, OUTPUT);        // trigPin'i cikis olarak ayarla
  pinMode(echoPin, INPUT);         // echoPin'i giris olarak ayarla
  pinMode(yesilLED, OUTPUT);       // Yesil LED'i cikis olarak ayarla
  pinMode(kirmiziLED, OUTPUT);     // Kirmizi LED'i cikis olarak ayarla
  pinMode(sifirlaButonu, INPUT_PULLUP); // Sifirlama butonunu giris olarak ayarla
  benimServo.attach(servoPin);    // Servo'yu bagla
  benimServo.write(0);            // Servo baslangic pozisyonu (asagida)
  Serial.begin(9600);             // Seri iletisim baslat
}

void loop() {
  // Sifirlama butonunu kontrol et
  if (digitalRead(sifirlaButonu) == LOW) {
    sayac = 0;                    // Sayaci sifirla
    digitalWrite(kirmiziLED, LOW);  // Kirmizi LED kapali
    digitalWrite(yesilLED, HIGH);   // Yesil LED acik
    delay(500);                    // Buton basimini algilamak icin kisa bir gecikme
    servoAktif = false;            // Servo motorunu sifirla, yeniden calismasin
  }

  // Mesafeyi olc
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long sure = pulseIn(echoPin, HIGH, 30000); // Sensorden donus sureyi olc (30 ms timeout)
  
  if (sure == 0) {
    // Eger pulseIn() 0 donerse sensor yanlis olcum yapmis olabilir
    Serial.println("Mesafe olculemedi.");
    mesafeCm = 999;  // Mesafeyi buyuk bir deger olarak ayarla
  } else {
    mesafeCm = sure * 0.034 / 2;  // Mesafeyi cm cinsinden hesapla
  }

  // Eger bir cisim varsa servo motor hareket etsin
  if (!servoAktif && mesafeCm > 0 && mesafeCm < 15) { // 15 cm'den yakin bir cisim varsa ve servo motoru aktif degilse
    benimServo.write(90);         // Servo'yu yukari pozisyona getir
    digitalWrite(yesilLED, HIGH); // Yesil LED'i yak
    delay(4000);                  // 4 saniye bekle
    benimServo.write(0);          // Servo'yu asagi pozisyona getir
    sayac++;                      // Sayaci artir

    if (sayac >= 20) {            // Sayac 20'yi gecerse
      digitalWrite(yesilLED, LOW); // Yesil LED'i kapat
      digitalWrite(kirmiziLED, HIGH); // Kirmizi LED'i yak
      benimServo.detach();        // Servo motorunu durdur
      servoAktif = true;          // Servo motoru artik aktif
    }
  }

  // Sayaci seri porta yazdirma
  Serial.print("Sayac: ");
  Serial.println(sayac);

  Serial.print("Mesafe: ");
  Serial.print(mesafeCm);
  Serial.println(" cm");

  delay(100);  // Dongu icin kisa gecikme
}
