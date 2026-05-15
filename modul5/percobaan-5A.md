
```markdown
# Pertanyaan Multitasking

1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!
3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan program pada file README.md.

---

# Jawaban

## Apakah ketiga task berjalan secara bersamaan atau bergantian?
Secara teknis, ketiga *task* berjalan secara **bergantian** dengan sangat cepat, bukan benar-benar bersamaan. 

Mekanismenya: Mikrokontroler Arduino Uno (ATmega328P) memiliki CPU *single-core* yang hanya bisa memproses satu instruksi pada satu waktu. Agar seolah-olah berjalan bersamaan, FreeRTOS menggunakan *scheduler* untuk melakukan *context switching* (*time-slicing*). *Scheduler* ini yang mengatur pembagian jatah waktu eksekusi CPU untuk masing-masing *task* secara bergantian dalam hitungan milidetik.

## Bagaimana cara menambahkan task keempat?
Untuk menambahkan *task* keempat, diperlukan tiga langkah pada kode program:

1. **Deklarasi Prototipe Fungsi:**
```cpp
void TaskKeempat(void *pvParameters);

```

2. **Registrasi Task di dalam `setup()`:**

```cpp
xTaskCreate(TaskKeempat, "task4", 128, NULL, 1, NULL); 

```

3. **Implementasi Fungsi Task:**

```cpp
void TaskKeempat(void *pvParameters) {
  while(1) {
    Serial.println("Task 4 aktif");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

```

## Modifikasi program dengan potensiometer untuk kontrol kecepatan LED

```cpp
#include <Arduino_FreeRTOS.h>

int potValue;
int dynamicDelay;

void TaskBlinkMerah(void *pvParameters);
void TaskBlinkKuning(void *pvParameters);
void TaskReadPot(void *pvParameters);

void setup() {
  Serial.begin(9600);

  // Membuat task
  xTaskCreate(TaskBlinkMerah, "Blink1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlinkKuning, "Blink2", 128, NULL, 1, NULL);
  
  // Memberikan prioritas lebih tinggi (2) agar pembacaan sensor selalu responsif
  xTaskCreate(TaskReadPot, "Pot", 128, NULL, 2, NULL);

  vTaskStartScheduler();
}

void loop() {}

void TaskReadPot(void *pvParameters) {
  while(1) {
    potValue = analogRead(A0);

    // Konversi nilai ADC (0-1023) menjadi nilai delay (50 - 1000 milidetik)
    dynamicDelay = map(potValue, 0, 1023, 50, 1000);

    Serial.print("Nilai Pot: ");
    Serial.print(potValue);
    Serial.print(" | Delay: ");
    Serial.println(dynamicDelay);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void TaskBlinkMerah(void *pvParameters) {
  pinMode(10, OUTPUT); // LED Merah pada Pin 10

  while(1) {
    digitalWrite(10, HIGH);
    vTaskDelay(dynamicDelay / portTICK_PERIOD_MS);
    digitalWrite(10, LOW);
    vTaskDelay(dynamicDelay / portTICK_PERIOD_MS);
  }
}

void TaskBlinkKuning(void *pvParameters) {
  pinMode(8, OUTPUT); // LED Kuning pada Pin 8

  while(1) {
    digitalWrite(8, HIGH);
    // Dibuat setengah dari dynamicDelay agar LED kuning berkedip 2x lebih cepat dari LED merah
    vTaskDelay((dynamicDelay/2) / portTICK_PERIOD_MS); 
    digitalWrite(8, LOW);
    vTaskDelay((dynamicDelay/2) / portTICK_PERIOD_MS);
  }
}

```

**Bagaimana hasilnya?** Kecepatan kedipan kedua LED dapat dikontrol secara *real-time* saat kenop potensiometer diputar, tanpa memblokir satu sama lain.

* **Jika nilai potensiometer kecil (mendekati 0):** Waktu *delay* menjadi sangat singkat, sehingga LED berkedip dengan sangat cepat.
* **Jika nilai potensiometer besar (mendekati 1023):** Waktu *delay* memanjang, sehingga LED berkedip dengan lebih lambat.

```

```