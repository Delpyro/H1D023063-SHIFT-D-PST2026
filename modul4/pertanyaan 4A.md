

# Jawaban Pertanyaan Praktikum 4A (ADC)

## Apa fungsi perintah analogRead() pada rangkaian praktikum ini?

Fungsi `analogRead()` digunakan untuk membaca nilai tegangan analog yang masuk melalui pin input analog (dalam hal ini pin yang terhubung ke potensiometer). Perintah ini mengubah sinyal analog kontinu menjadi data digital diskret dengan rentang nilai antara 0 hingga 1023, yang merepresentasikan besaran tegangan 0V hingga 5V.

## Mengapa diperlukan fungsi map() dalam program tersebut?

Fungsi `map()` diperlukan untuk melakukan penskalaan atau konversi rentang nilai data. Dalam praktikum ini, nilai input dari ADC memiliki rentang 0-1023, sedangkan motor servo bekerja dengan parameter sudut antara 0 hingga 180 derajat. Fungsi `map()` memetakan nilai ADC tersebut secara proporsional agar nilai input potensiometer dapat diterjemahkan menjadi derajat sudut yang sesuai untuk menggerakkan servo.

## Modifikasi program agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023.

**Kode Program:**

```cpp
#include <Servo.h> 

Servo motorServo; 

const int pinPotensio = A0;   
const int pinServo = 9;       

int posisiSudut = 0; 
int nilaiADC = 0;    

void setup() {
  motorServo.attach(pinServo);
  Serial.begin(9600);
}

void loop() {
  // 1. Membaca nilai dari potensiometer (0 - 1023)
  nilaiADC = analogRead(pinPotensio);

  // 2. Memetakan nilai ADC ke rentang sudut servo (30° - 150°)
  posisiSudut = map(nilaiADC, 0, 1023, 30, 150);

  // 3. Menggerakkan servo sesuai hasil pemetaan
  motorServo.write(posisiSudut);

  // 4. Menampilkan data ke Serial Monitor untuk pemantauan
  Serial.print("Nilai ADC: ");
  Serial.print(nilaiADC);
  Serial.print("\t|\tSudut Servo: ");
  Serial.println(posisiSudut);

  // 5. Jeda waktu agar pergerakan servo stabil
  delay(150);
}

```

**Penjelasan Modifikasi Program:**
Kunci dari modifikasi ini terletak pada fungsi `map()`. Meskipun sinyal input analog dari potensiometer tetap dikonversi menjadi rentang ADC utuh dari 0 hingga 1023, kita dapat membatasi batas bawah dan batas atas dari outputnya.

Dengan mengubah parameter pada baris `map(nilaiADC, 0, 1023, 30, 150);`, Arduino secara otomatis akan mengonversi saat nilai ADC berada di 0, motor servo hanya akan bergeser ke sudut minimum yaitu 30°. Sebaliknya, saat tuas potensiometer diputar penuh (nilai ADC 1023), servo hanya akan mencapai sudut maksimum 150°. Perubahan ini memungkinkan pergerakan aktuator yang lebih terkontrol sesuai kebutuhan sistem fisik tanpa perlu memotong sinyal dari sensor aslinya.