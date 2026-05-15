#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Struktur data untuk menyimpan nilai parameter lingkungan
struct readings {
  int temp;
  int h;
};

// Deklarasi variabel global untuk handle Queue
QueueHandle_t my_queue;

// Deklarasi prototipe fungsi task
void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  // Inisialisasi komunikasi serial pada 9600 bps
  Serial.begin(9600);

  // Membuat Queue dengan panjang antrean 1 dan ukuran memori sebesar struct readings
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Registrasi task ke scheduler FreeRTOS
  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
}

void loop() {
  // Dibiarkan kosong karena eksekusi ditangani oleh task FreeRTOS
}

// Definisi Task 1: Menyiapkan data sensor dan mengirimkannya ke Queue
void read_data(void *pvParameters) {
  struct readings x;
  
  for (;;) {
    // Simulasi pembacaan data sensor (nilai statis)
    x.temp = 54;
    x.h = 30;
    
    // Mengirim struktur data ke antrean
    xQueueSend(my_queue, &x, portMAX_DELAY);
    
    // Jeda waktu 100 tick sebelum iterasi berikutnya
    vTaskDelay(100); 
  }
}

// Definisi Task 2: Menerima data dari Queue dan menampilkannya
void display(void *pvParameters) {
  struct readings x;
  
  for (;;) {
    // Mengecek apakah ada data yang berhasil diterima dari antrean
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.print("temp = ");
      Serial.println(x.temp);
      
      Serial.print("humidity = ");
      Serial.println(x.h);
    }
  }
}