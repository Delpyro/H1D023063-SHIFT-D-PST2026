
# Jawaban Pertanyaan Praktikum 3A

## Jelaskan proses dari input keyboard hingga LED menyala/mati!

Proses komunikasi dari input komputer ke Arduino terjadi melalui beberapa tahapan berikut:

* 
**Input Keyboard:** Karakter diketik pada antarmuka Serial Monitor dan dikirim.


* 
**Transmisi UART:** Komputer mengirimkan karakter dalam bentuk bit biner melalui kabel USB menuju Arduino.


* 
**Penerimaan di Buffer:** Chip USB-to-Serial pada Arduino menerima data bit tersebut dan menyimpannya sementara di memori yang disebut serial buffer.


* 
**Pengecekan dan Pembacaan:** Fungsi `Serial.available()` mengecek apakah ada data di buffer. Jika ada, `Serial.read()` akan mengambil satu karakter dari buffer.


* 
**Eksekusi Logika:** Karakter dievaluasi menggunakan `if-else`; jika karakter '1', perintah `digitalWrite(PIN_LED, HIGH)` dieksekusi sehingga LED menyala. Jika karakter '0', dieksekusi perintah `LOW` sehingga aliran arus terputus dan LED mati.



## Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?

`Serial.available()` berfungsi untuk memeriksa apakah ada data serial yang masuk dan tersedia untuk dibaca di dalam buffer. Hal ini memastikan Arduino hanya membaca instruksi ketika memang ada instruksi baru.

Jika dihilangkan, Arduino akan terus-menerus mencoba membaca port serial pada setiap siklus `loop()`. Jika tidak ada data, `Serial.read()` akan mengembalikan nilai -1 (karakter kosong/sampah). Hal ini sangat memboroskan siklus pemrosesan CPU dan dapat menyebabkan program error jika nilai tersebut tidak sengaja tereksekusi oleh logika program.

## Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

**Kode Program:**

```cpp
const int PIN_LED = 8;
char inputKarakter = '0';
unsigned long waktuSebelumnya = 0;
const long intervalBlink = 500;
bool statusLED = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    inputKarakter = Serial.read();
  }

  if (inputKarakter == '1') {
    digitalWrite(PIN_LED, HIGH);
  } 
  else if (inputKarakter == '0') {
    digitalWrite(PIN_LED, LOW);
  } 
  else if (inputKarakter == '2') {
    unsigned long waktuSekarang = millis();
    if (waktuSekarang - waktuSebelumnya >= intervalBlink) {
      waktuSebelumnya = waktuSekarang;
      statusLED = !statusLED;
      digitalWrite(PIN_LED, statusLED);
    }
  }
}

```

**Penjelasan Baris Kode:**

* `const int PIN_LED = 8;` : Menetapkan pin digital 8 untuk LED.
* `char inputKarakter = '0';` : Menyimpan karakter input dari Serial Monitor (default '0').
* `unsigned long waktuSebelumnya = 0;` : Menyimpan jejak waktu terakhir LED berubah status (untuk logika millis).
* `const long intervalBlink = 500;` : Menetapkan jeda waktu berkedip selama 500 milidetik.
* `bool statusLED = LOW;` : Melacak kondisi LED saat ini (menyala/mati).
* `Serial.begin(9600);` : Memulai komunikasi serial dengan *baud rate* 9600.
* `pinMode(PIN_LED, OUTPUT);` : Mengatur pin LED sebagai output.
* `if (Serial.available() > 0)` : Mengecek apakah ada instruksi baru dari Serial Monitor.
* `inputKarakter = Serial.read();` : Membaca dan menyimpan instruksi tersebut untuk dieksekusi terus-menerus.
* `if (inputKarakter == '1') ... else if (inputKarakter == '0')` : Menyalakan LED jika input '1' dan mematikan secara statis jika input '0'.
* `else if (inputKarakter == '2')` : Mengeksekusi logika non-blocking *blink* jika input yang terakhir diterima adalah '2'.
* `unsigned long waktuSekarang = millis();` : Mengambil waktu berjalannya Arduino saat ini.
* `if (waktuSekarang - waktuSebelumnya >= intervalBlink)` : Mengecek apakah selisih waktu sekarang dan sebelumnya sudah melewati 500ms.
* `waktuSebelumnya = waktuSekarang;` : Memperbarui catatan waktu untuk siklus kedipan berikutnya.
* `statusLED = !statusLED;` : Membalik status LED dari HIGH ke LOW atau sebaliknya.
* `digitalWrite(PIN_LED, statusLED);` : Menerapkan perubahan status tersebut ke perangkat fisik LED.

## Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap sistem

Wajib menggunakan `millis()`. Fungsi `delay()` bersifat *blocking* yang membuat mikrokontroler "berhenti berpikir" total. Akibatnya, instruksi baru dari Serial Monitor tidak akan langsung terbaca dan sistem akan terasa tidak responsif (*ngelag*).

Sebaliknya, `millis()` bersifat *non-blocking* dan bekerja seperti *stopwatch* latar belakang. Dengan `millis()`, sistem komunikasi UART tetap *real-time* dan responsif menerima perintah kapan saja meskipun LED sedang sibuk berkedip.