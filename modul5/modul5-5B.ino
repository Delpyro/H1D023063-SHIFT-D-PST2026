#include <Arduino_FreeRTOS.h>

// Deklarasi fungsi (Task Prototypes)
void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);

void setup() {
  // Inisialisasi komunikasi serial pada 9600 bps
  Serial.begin(9600);

  // Pembuatan task (Task Creation)
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL);

  // Memulai scheduler RTOS
  vTaskStartScheduler();
}

void loop() {
  // Dibiarkan kosong karena eksekusi diambil alih oleh scheduler FreeRTOS
}

// Definisi Task 1: Mengontrol LED pada Pin 8
void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);

  while (1) {
    Serial.println("Task1");
    digitalWrite(8, HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    digitalWrite(8, LOW);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// Definisi Task 2: Mengontrol LED pada Pin 7
void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);

  while (1) {
    Serial.println("Task2");
    digitalWrite(7, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(7, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// Definisi Task 3: Mencetak counter ke Serial Monitor
void Taskprint(void *pvParameters) {
  int counter = 0;

  while (1) {
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
