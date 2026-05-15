
```markdown
# Pertanyaan Komunikasi Task (Queue)

1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
2. Apakah program ini berpotensi mengalami race condition? Jelaskan!
3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file README.md.

---

# Jawaban

## Apakah kedua task berjalan secara bersamaan atau bergantian?
Sama seperti pada percobaan sebelumnya, kedua *task* (`read_data` dan `display`) berjalan secara **bergantian**.

Mekanismenya: Karena Arduino Uno menggunakan CPU *single-core*, eksekusi diatur oleh *scheduler* FreeRTOS. Pada program ini, *task* `display` mengeksekusi `xQueueReceive` dengan parameter `portMAX_DELAY`, yang artinya *task* ini akan masuk ke mode *Blocked* (menunggu) selama antrean (*queue*) kosong. Saat *task* `display` terblokir, *scheduler* memberikan CPU kepada *task* `read_data` untuk memproses data dan mengirimkannya ke antrean menggunakan `xQueueSend`. Begitu data masuk ke antrean, *task* `display` otomatis terbangun (*Ready*) untuk mengambil dan mencetak data tersebut, sementara `read_data` diistirahatkan sejenak oleh fungsi `vTaskDelay`.

## Apakah program ini berpotensi mengalami race condition?
**Tidak.** Program ini tidak berpotensi mengalami *race condition*. 

*Race condition* biasanya terjadi jika dua *task* atau lebih mencoba membaca dan mengubah satu variabel global di memori yang sama pada waktu yang bersamaan tanpa adanya penguncian (*locking*). Pada program ini, komunikasi antar-*task* tidak menggunakan variabel global secara langsung, melainkan menggunakan mekanisme **Queue** dari FreeRTOS. Fungsi antrean pada FreeRTOS (`xQueueSend` dan `xQueueReceive`) sudah dirancang secara bawaan agar bersifat *thread-safe*, sehingga sinkronisasi dan perlindungan memori saat proses transfer data dikelola secara otomatis oleh sistem operasi.

## Modifikasi program menggunakan sensor DHT 

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h> // Library untuk sensor DHT

// Definisi pin dan tipe sensor DHT
#define DHTPIN 2       // Pin data sensor DHT dihubungkan ke Pin 2 digital
#define DHTTYPE DHT11  // Gunakan DHT11 atau DHT22 sesuai sensor fisik yang dipakai

DHT dht(DHTPIN, DHTTYPE);

// Struktur data antrean
struct readings {
  int temp;
  int h;
};

QueueHandle_t my_queue;

void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  Serial.begin(9600);
  
  // Inisialisasi sensor DHT
  dht.begin(); 
  
  // Membuat Queue
  my_queue = xQueueCreate(1, sizeof(struct readings));
  
  xTaskCreate(read_data, "read sensors", 128, NULL, 1, NULL);
  xTaskCreate(display, "display", 128, NULL, 1, NULL);
  
  vTaskStartScheduler();
}

void loop() {}

void read_data(void *pvParameters) {
  struct readings x;
  
  for(;;) {
    // Membaca data aktual dari sensor DHT
    x.temp = dht.readTemperature(); // Suhu dalam Celcius
    x.h = dht.readHumidity();       // Kelembapan dalam persen
    
    // Mengirim data ke antrean
    xQueueSend(my_queue, &x, portMAX_DELAY);
    
    // Delay 2000ms karena sensor DHT membutuhkan waktu untuk membaca ulang dengan stabil
    vTaskDelay(2000 / portTICK_PERIOD_MS); 
  }
}

void display(void *pvParameters) {
  struct readings x;
  
  for(;;) {
    // Menerima data dari antrean
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.println("--- Data Sensor Masuk ---");
      Serial.print("Suhu       : ");
      Serial.print(x.temp);
      Serial.println(" C");
      
      Serial.print("Kelembapan : ");
      Serial.print(x.h);
      Serial.println(" %");
      Serial.println("-------------------------");
    }
  }
}

```

**Bagaimana hasilnya?**
Hasil keluaran pada Serial Monitor tidak lagi menampilkan angka statis (54 dan 30). Informasi yang dicetak sekarang berubah secara dinamis merespons kondisi lingkungan nyata yang ditangkap oleh sensor fisik DHT. Selain itu, karena sensor DHT umumnya membutuhkan waktu jeda pembacaan sekitar 2 detik, penggunaan *delay* pada *task* `read_data` juga disesuaikan menjadi 2000 milidetik agar data terbaca dengan stabil tanpa mengganggu kinerja *task* penerima.

```

```