

# Jawaban Pertanyaan Praktikum 4B (DAC/PWM)

## Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!

LED dapat diatur kecerahannya karena fungsi `analogWrite()` menerapkan teknik Pulse Width Modulation (PWM). Teknik ini mengatur lebar pulsa (*duty cycle*) dalam satu periode waktu tertentu, sehingga mikrokontroler dapat menghasilkan tegangan rata-rata efektif yang berbeda-beda. Perubahan tegangan rata-rata inilah yang dipersepsikan oleh mata manusia sebagai perubahan intensitas cahaya atau kecerahan pada LED.

## Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?

Hubungan antara keduanya adalah hubungan pemetaan linear atau skalasi. Nilai ADC merupakan representasi digital dari sinyal analog input dengan resolusi 10-bit (0-1023). Sementara itu, nilai PWM pada Arduino merupakan sinyal output dengan resolusi 8-bit (0-255). Agar nilai input dapat mengontrol output secara proporsional, diperlukan konversi nilai dari rentang 10-bit ke 8-bit, yang biasanya dilakukan menggunakan fungsi `map()`.

## Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200.

**Kode Program:**

```cpp
#include <Arduino.h> 

const int potPin = A0;   // Pin analog untuk input potensiometer
const int ledPin = 9;    // Pin PWM untuk output LED

int nilaiADC = 0;  
int pwm = 0;       

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // 1. Membaca nilai analog dari potensiometer
  nilaiADC = analogRead(potPin);

  // 2. Mengubah nilai ADC (0-1023) menjadi resolusi PWM (0-255)
  pwm = map(nilaiADC, 0, 1023, 0, 255);

  // 3. Logika untuk membatasi rentang nyala LED (50 - 200)
  if (pwm >= 50 && pwm <= 200) {
    analogWrite(ledPin, pwm); // LED menyala sesuai nilai PWM
  } else {
    analogWrite(ledPin, 0);   // LED mati jika di luar rentang
  }

  // 4. Menampilkan data ke Serial Monitor
  Serial.print("Nilai ADC: ");
  Serial.print(nilaiADC);
  Serial.print("\t|\tNilai PWM: ");
  Serial.println(pwm);

  delay(50);
}

```

**Penjelasan Modifikasi Program:**
Modifikasi utama pada program ini terdapat pada penambahan blok struktur kendali `if-else` setelah proses konversi dengan fungsi `map()`.

Potensiometer tetap memetakan nilai dari 0 hingga 255 secara internal, tetapi sinyal tersebut tidak langsung dikirim ke pin LED. Sistem akan mengevaluasi kondisi `if (pwm >= 50 && pwm <= 200)`. Jika kondisi ini terpenuhi (nilai PWM di antara 50 dan 200 inklusif), baris `analogWrite(ledPin, pwm)` dieksekusi dan LED menyala dengan kecerahan tersebut. Jika nilai PWM kurang dari 50 atau lebih besar dari 200, maka kondisi `else` dieksekusi, yang mana instruksi `analogWrite(ledPin, 0)` akan memaksa tegangan keluaran menjadi 0V dan LED akan mati total.