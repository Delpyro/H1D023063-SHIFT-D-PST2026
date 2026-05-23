

# Jawaban Pertanyaan Praktikum 3B

## Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!

Pada rangkaian ini, Arduino bertindak sebagai Master dan LCD bertindak sebagai Slave. Komunikasi berjalan menggunakan hanya dua jalur kabel, yaitu SDA (Serial Data) yang terhubung ke pin A4 untuk mentransfer bit data, dan SCL (Serial Clock) yang terhubung ke pin A5 untuk mengatur sinkronisasi waktu (clock) pengiriman.

Agar Arduino bisa mengirim data secara spesifik ke LCD (dan bukan ke modul lain yang mungkin terpasang di jalur yang sama), Arduino memanggil alamat I2C unik milik LCD tersebut (biasanya 0x27 atau 0x3F). Setelah alamat cocok, Arduino mengirimkan instruksi teks dan karakter bar untuk ditampilkan di layar.

## Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!

Tidak harus mutlak seperti itu. Syarat utamanya hanyalah kaki tengah (wiper) harus terhubung ke pin analog (A0) untuk membaca output tegangan.

Jika pin kiri (GND) dan pin kanan (5V) tertukar posisinya, rangkaian tidak akan rusak, yang terjadi hanyalah arah pembacaannya yang terbalik. Misalnya, jika pada konfigurasi awal memutar kenop ke kanan membuat nilai ADC naik (dari 0 ke 1023), maka setelah kabelnya ditukar, memutar kenop ke kanan justru akan membuat nilai ADC turun (dari 1023 ke 0).

## Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output)

**Kode Program:**

```cpp
#include <Wire.h>                  // Library komunikasi I2C
#include <LiquidCrystal_I2C.h>     // Library LCD I2C
#include <Arduino.h>               // Library dasar Arduino

// Inisialisasi LCD (alamat 0x27, 16 kolom, 2 baris)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin potensiometer
const int pinPot = A0;

void setup() {
  Serial.begin(9600);      // Memulai komunikasi UART (Serial Monitor)
  lcd.init();              // Inisialisasi LCD
  lcd.backlight();         // Menyalakan backlight LCD

  // Header tabel di Serial Monitor
  Serial.println("ADC\tVolt(V)\tPersen(%)");
}

void loop() {
  int nilai = analogRead(pinPot);   // Membaca nilai ADC (0–1023)

  // Konversi ke tegangan (0–5V)
  float volt = nilai * (5.0 / 1023.0);

  // Konversi ke persen (0–100%)
  float persen = nilai * (100.0 / 1023.0);

  // Mapping ke bar (0–16 karakter)
  int panjangBar = map(nilai, 0, 1023, 0, 16);

  // ================= UART (Serial Monitor) =================
  Serial.print(nilai);     // Tampilkan nilai ADC
  Serial.print("\t");
  Serial.print(volt, 2);   // Tampilkan volt (2 angka desimal)
  Serial.print("\t");
  Serial.println(persen, 2); // Tampilkan persen

  // ================= LCD (I2C) =================

  // Baris 1: Persentase
  lcd.setCursor(0, 0);         
  lcd.print("ADC: ");
  lcd.print(persen, 1);        
  lcd.print("%   ");             // Clear sisa karakter

  // Baris 2: Bar level
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255);    // Blok penuh
    } else {
      lcd.print(" ");          // Kosong
    }
  }

  delay(200);  // Delay agar tidak terlalu cepat update
}

```

**Penjelasan Per Bagian Kode:**

**1. Library**

* `#include <Wire.h>`: Library untuk mengaktifkan protokol komunikasi I2C.
* `#include <LiquidCrystal_I2C.h>`: Library khusus untuk mengontrol modul LCD via I2C.
* `#include <Arduino.h>`: Library fungsi dasar mikrokontroler Arduino.

**2. Inisialisasi LCD**

* `LiquidCrystal_I2C lcd(0x27, 16, 2);`: Mengatur alamat I2C LCD pada `0x27` dengan spesifikasi layar 16 kolom dan 2 baris.

**3. Setup**

* `Serial.begin(9600);`: Memulai komunikasi UART ke komputer dengan baudrate 9600.
* `lcd.init();` & `lcd.backlight();`: Menyiapkan modul LCD dan menyalakan lampu latarnya.
* `Serial.println(...);`: Mencetak header tabel (ADC, Volt, Persen) satu kali di Serial Monitor.

**4. Pemrosesan Data (Loop)**

* `int nilai = analogRead(pinPot);`: Membaca sinyal analog dari potensiometer dan mengubahnya ke nilai digital (0–1023).
* `float volt = nilai * (5.0 / 1023.0);`: Konversi nilai matematis dari ADC menjadi satuan tegangan aktual (0–5V).
* `float persen = nilai * (100.0 / 1023.0);`: Konversi persentase putaran.
* `int panjangBar = map(nilai, 0, 1023, 0, 16);`: Memetakan rentang ADC (0-1023) menjadi jumlah blok karakter (0-16) untuk tampilan *bar level*.

**5. Output UART (Serial Monitor)**

* Kumpulan fungsi `Serial.print` dan `Serial.println` digunakan untuk mencetak nilai ADC, tegangan (2 desimal), dan persentase secara berurutan membentuk baris data di bawah header tabel.

**6. Output LCD (I2C)**

* **Baris 1 (`lcd.setCursor(0, 0);`):** Mencetak teks "ADC: " diikuti nilai persentasenya. Spasi kosong `"%   "` ditambahkan untuk menghapus sisa karakter jika angka sebelumnya lebih panjang.
* **Baris 2 (`lcd.setCursor(0, 1);`):** Menjalankan perulangan `for` dari 0 hingga 15. Jika indeks lebih kecil dari `panjangBar`, LCD mencetak karakter blok penuh `(char)255`. Jika lebih besar, mencetak spasi kosong `" "`.
* `delay(200);`: Memberikan jeda 200 milidetik pada setiap siklus agar pembacaan Serial Monitor dan LCD lebih stabil di mata.

## Lengkapi tabel berikut berdasarkan pengamatan pada Serial Monitor

Tabel di bawah ini disesuaikan dengan data hasil pengamatan yang tercatat pada laporan praktikum:

| ADC | Volt (V) | Persen (%) |
| --- | --- | --- |
| 1 | 0.00V | 0% |
| 21 | 0.10V | 2% |
| 49 | 0.24V | 4% |
| 74 | 0.36V | 7% |
| 96 | 0.47V | 9% |