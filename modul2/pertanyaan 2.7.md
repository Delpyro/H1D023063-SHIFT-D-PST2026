

# Jawaban Pertanyaan Praktikum 2.7

Bagian ini merangkum analisis hasil percobaan serta prinsip kerja komponen yang digunakan pada Modul 2: Pemrograman GPIO.

## 1. Uraian Hasil Tugas Praktikum
* **Percobaan 2A (Seven Segment Display):** Sistem berhasil menampilkan urutan karakter heksadesimal 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, b, C, d, E, F secara otomatis. Setiap karakter muncul dengan jeda waktu 1 detik yang diatur menggunakan fungsi `delay(1000)`. Hal ini membuktikan bahwa pengiriman sinyal digital secara sekuensial melalui pin Arduino dapat membentuk pola visual yang terstruktur pada *Seven Segment*.
* **Percobaan 2B (Kontrol Counter dengan Push Button):** Sistem berhasil mengimplementasikan kontrol manual terhadap *Seven Segment*. Angka hanya akan bertambah (*increment*) jika *push button* ditekan. Penggunaan logika *edge detection* pada kode program sangat krusial di sini, karena memastikan angka hanya bertambah satu kali per tekanan tombol meskipun tombol ditahan dalam waktu lama.

## 2. Prinsip Kerja Seven Segment Display
*Seven Segment Display* bekerja dengan prinsip menyalakan kombinasi dari tujuh LED (segmen a hingga g) untuk membentuk angka atau karakter tertentu. 
* **Jenis yang Digunakan:** Berdasarkan logika program yang memberikan nilai `1` (HIGH) untuk menyalakan segmen, maka *Seven Segment* yang digunakan adalah jenis **Common Cathode**.
* **Mekanisme:** Seluruh kaki katoda dari ketujuh LED dihubungkan menjadi satu ke jalur *Ground*. Untuk menyalakan segmen tertentu, pin anoda yang bersesuaian harus menerima tegangan (sinyal HIGH) dari pin digital Arduino melalui resistor pembatas arus.

## 3. Prinsip Kerja Sistem Counter pada Program
Sistem *counter* pada praktikum ini bekerja melalui tiga tahapan utama:
1.  **Penyimpanan Nilai (Variable Storage):** Program menyimpan nilai numerik dalam sebuah variabel (misalnya `currentDigit`). Nilai ini akan berubah (bertambah atau berkurang) berdasarkan pemicu tertentu (waktu atau penekanan tombol).
2.  **Pemetaan Pola (Bit Mapping):** Nilai dari variabel *counter* digunakan sebagai indeks untuk mengakses matriks atau *array* dua dimensi `digitPattern`. Matriks ini berisi kombinasi logika HIGH (1) dan LOW (0) yang merepresentasikan bentuk visual karakter tersebut.
3.  **Eksekusi Output:** Fungsi `displayDigit()` mengambil data pola dari *array* dan mengirimkannya secara simultan ke pin-pin GPIO Arduino menggunakan perintah `digitalWrite()`, sehingga LED pada *Seven Segment* menyala sesuai pola yang diinginkan.

---
