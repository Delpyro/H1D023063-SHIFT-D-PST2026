

# Percobaan 2A: Seven Segment Display



## 1. Rangkaian Schematic
[cite_start]Rangkaian terdiri dari sebuah Arduino Uno yang dihubungkan ke *Seven Segment Display* (Common Cathode)[cite: 109, 201]. [cite_start]Setiap segmen (a, b, c, d, e, f, g, dan dp) dihubungkan ke pin digital Arduino melalui resistor pembatas arus sebesar 220 Ohm[cite: 103, 193]. [cite_start]Konfigurasi pin yang digunakan adalah sebagai berikut[cite: 195]:
* [cite_start]**Segmen a**: Pin 7 [cite: 195]
* [cite_start]**Segmen b**: Pin 6 [cite: 195]
* [cite_start]**Segmen c**: Pin 5 [cite: 195]
* [cite_start]**Segmen d**: Pin 11 [cite: 195]
* [cite_start]**Segmen e**: Pin 10 [cite: 195]
* [cite_start]**Segmen f**: Pin 8 [cite: 195]
* [cite_start]**Segmen g**: Pin 9 [cite: 195]
* [cite_start]**Segmen dp**: Pin 4 [cite: 195]

## 2. Analisis Nilai `num` > 15
[cite_start]Jika variabel `num` memiliki nilai lebih dari 15, maka program akan mencoba mengakses indeks di luar batas *array* `digitPattern[16][8]`[cite: 153]. Hal ini dapat menyebabkan:
* [cite_start]**Tampilan Error**: *Seven Segment* akan menampilkan karakter yang tidak beraturan atau acak karena program mengambil data dari alamat memori yang tidak ditentukan untuk pola karakter[cite: 197].
* **Instabilitas Program**: Mengakses indeks di luar jangkauan *array* dapat menyebabkan perilaku program yang tidak terduga (*undefined behavior*).

## 3. Jenis Seven Segment (Common Cathode/Anode)
[cite_start]Program ini menggunakan jenis **Common Cathode**[cite: 252].
* [cite_start]**Alasan**: Di dalam kode program, pola bit untuk menyalakan segmen menggunakan angka `1` (HIGH)[cite: 153, 252]. [cite_start]Pada sistem *Common Cathode*, seluruh katoda dihubungkan ke *Ground*, sehingga setiap segmen akan menyala ketika menerima sinyal HIGH dari pin Arduino[cite: 109, 201].

## 4. Modifikasi Program (F ke 0)
Untuk mengubah urutan tampilan dari F ke 0, bagian perulangan pada fungsi `loop()` dimodifikasi sebagai berikut:

```cpp
#include <Arduino.h>

// 7-Segment Display (Efficient Version)
// Modifikasi: Menampilkan urutan mundur dari F ke 0

// Pin mapping segment berdasarkan Tabel 1.4 dan rangkaian pada modul
// a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4}; [cite: 138, 195]

// Segment pattern untuk Common Cathode (1 = ON, 0 = OFF) [cite: 252]
// Indeks 0-9 untuk angka 0-9, Indeks 10-15 untuk A-F
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // 0 [cite: 156]
  {0,1,1,0,0,0,0,0}, // 1 [cite: 158]
  {1,1,0,1,1,0,1,0}, // 2 [cite: 160]
  {1,1,1,1,0,0,1,0}, // 3 [cite: 162]
  {0,1,1,0,0,1,1,0}, // 4 [cite: 164]
  {1,0,1,1,0,1,1,0}, // 5 [cite: 167]
  {1,0,1,1,1,1,1,0}, // 6 [cite: 169]
  {1,1,1,0,0,0,0,0}, // 7 [cite: 169]
  {1,1,1,1,1,1,1,1}, // 8 [cite: 169]
  {1,1,1,1,0,1,1,0}, // 9 [cite: 170]
  {1,1,1,0,1,1,1,0}, // A [cite: 171]
  {0,0,1,1,1,1,1,0}, // b [cite: 174]
  {1,0,0,1,1,1,0,0}, // C [cite: 175]
  {0,1,1,1,1,0,1,0}, // d [cite: 176]
  {1,0,0,1,1,1,1,0}, // E [cite: 177]
  {1,0,0,0,1,1,1,0}  // F [cite: 178]
};

// Fungsi untuk menampilkan digit pada Seven Segment
void displayDigit(int num) {
  for(int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], digitPattern[num][i]); [cite: 126]
  }
}

void setup() {
  // Mengatur semua pin segmen sebagai OUTPUT 
  for(int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT); [cite: 151]
  }
}

void loop() {
  // Jawaban No. 4: Modifikasi perulangan agar berjalan dari F (15) ke 0 (0)
  for(int i = 15; i >= 0; i--) {
    displayDigit(i); // Memanggil fungsi display [cite: 189]
    delay(1000);      // Jeda 1 detik sesuai spesifikasi 
  }
}
```

### **Penjelasan Baris Kode (untuk README.md):**
* [cite_start]**`const int segmentPins[8]`**: Mendefinisikan *array* pin digital yang terhubung ke segmen a sampai dp untuk mempermudah kontrol dalam perulangan[cite: 138, 195].
* **`byte digitPattern[16][8]`**: Matriks yang menyimpan logika biner (0 dan 1) untuk membentuk karakter 0-F. [cite_start]Angka 1 digunakan untuk menyalakan segmen karena menggunakan jenis *Common Cathode*[cite: 153, 252].
* [cite_start]**`void displayDigit(int num)`**: Fungsi bantu yang mengambil data dari baris ke-`num` pada matriks pola dan mengirimkannya ke pin fisik Arduino menggunakan `digitalWrite`[cite: 124, 126].
* [cite_start]**`pinMode(segmentPins[i], OUTPUT)`**: Mengonfigurasi pin agar dapat mengirimkan tegangan ke LED *Seven Segment*[cite: 41, 151].
* **`for(int i = 15; i >= 0; i--)`**: Inti dari modifikasi nomor 4; perulangan dimulai dari indeks tertinggi (15 melambangkan 'F') dan terus berkurang hingga indeks 0 (melambangkan '0').
* [cite_start]**`delay(1000)`**: Memberikan waktu tunggu selama 1000 milidetik agar karakter yang ditampilkan dapat terlihat jelas oleh praktikan sebelum berganti[cite: 190].