# Dokumentasi Program

## Deskripsi Program

Program ini adalah implementasi komunikasi Bluetooth Low Energy (BLE) pada perangkat berbasis ESP32 untuk membaca data berat badan anak dari perangkat BLE lain. Program ini menghubungkan ESP32 sebagai BLE Client ke perangkat BLE Peripheral, membaca data tertentu, dan mengonversi data tersebut menjadi berat badan dalam kilogram. Program ini juga mencakup logika untuk mengelola koneksi ulang secara otomatis jika koneksi terputus.

## Fitur Utama

1. Koneksi BLE Client

    - Inisialisasi BLE Client menggunakan pustaka ``` BLEDevice ```.

    - Koneksi ke perangkat BLE berdasarkan alamat MAC dan UUID karakteristik.

2. Notifikasi Data

    - Menerima data dari perangkat BLE melalui callback.

    - Menangani data notifikasi yang diterima dan memprosesnya menjadi informasi berat badan.

3. Pengolahan Data Berat Badan

    - Mengambil byte tertentu dari data yang diterima (byte ke-15 hingga ke-18).

    - Mengonversi byte menjadi string ASCII.

    - Mengonversi string ASCII menjadi nilai desimal untuk berat badan dalam kilogram.

4. Manajemen Koneksi Ulang

    - Otomatis mencoba untuk terhubung kembali ke perangkat BLE jika koneksi terputus.

# Struktur Program
## Header File yang Digunakan

- ``` Arduino.h ```: Digunakan untuk fungsi dasar Arduino.
- ``` BLEDevice.h ```: Library untuk mengelola komunikasi BLE.

- ``` bluetooth/configBT.h ```: File konfigurasi BLE yang mendefinisikan alamat MAC dan UUID.

- ``` dp_ida.h ```: Header file untuk fungsi khusus.

## Variabel Global

- ``` BLEClient ```: Objek untuk mengelola koneksi BLE.
- ``` BLERemoteCharacteristic ```: Karakteristik BLE untuk membaca data.
- ``` connected2 ```: Menyimpan status koneksi.
- ``` doReconnect2 ```: Menyimpan status kebutuhan untuk melakukan koneksi ulang.

## Fungsi Utama

1. ```MyClientCallback2```

    Callback untuk menangani perubahan status koneksi BLE.

2. ```notifyCallback2```

    Fungsi callback yang menangani data notifikasi dari BLE Peripheral.

    - Data yang diterima diperiksa pada byte ke-19.

    - Byte ke-15 hingga ke-18 diambil dan diolah menjadi:

        - String ASCII.

        - Nilai Hexadecimal.

        - Nilai berat badan dalam kilogram.

3. ```connectToDevice2```

    Fungsi untuk menghubungkan BLE Client ke BLE Peripheral menggunakan alamat MAC dan UUID tertentu.

4. ```setup_dp_ida```

    Fungsi inisialisasi untuk mengatur koneksi BLE saat program dimulai.

5. ```dp_ida_handle```

    Fungsi untuk memantau status koneksi BLE dan mencoba menyambung ulang jika terputus.

# Alur Kerja Program

1. Program memulai koneksi ke perangkat BLE menggunakan ```setup_dp_ida```.

2. Jika koneksi berhasil, notifikasi dari perangkat BLE diterima oleh notifyCallback2.

3. Callback memproses data dengan langkah berikut:

    - Mengambil byte ke-15 hingga ke-18 dari data yang diterima.

    - Mengubah byte menjadi string ASCII.

    - Mengubah string menjadi nilai desimal.

    - Mengonversi nilai desimal menjadi berat badan dalam kilogram.

4. Jika koneksi terputus, program mencoba untuk menyambung ulang secara otomatis.

# Contoh Output Serial


## Koneksi Berhasil

```
Perangkat terhubung
```

## Data Notifikasi Diterima

```
Data (HEX): 54 2B 41 44 56 44 41 54 41 3D 43 30 34 35 30 32 35 44 30 30
String ASCII dari byte 15-18: 025D
Nilai Hexadecimal: 25D
Berat Anak: 6.05 kg
```

## Koneksi Gagal

```
Perangkat tidak terhubung
```

# Konfigurasi yang Dibutuhkan

## File ```configBT.h```

Pastikan file ```configBT.h``` memiliki konfigurasi seperti berikut:

```
#define mac_address2 "AA:BB:CC:DD:EE:FF"
#define serviceUUIDStr2 "0000181D-0000-1000-8000-00805F9B34FB"
#define charUUIDStr2 "00002A9C-0000-1000-8000-00805F9B34FB"
```

- ```mac_address2```: Alamat MAC perangkat BLE Peripheral.

- ```serviceUUIDStr2```: UUID service BLE Peripheral.

- ```charUUIDStr2```: UUID karakteristik BLE Peripheral.

# Cara Penggunaan

1. Pastikan perangkat ESP32 dan perangkat BLE Peripheral tersedia.

2. Konfigurasikan ```configBT.h``` dengan alamat MAC dan UUID yang sesuai.

3. Unggah kode ke ESP32 menggunakan Arduino IDE.

4. Pantau hasil melalui Serial Monitor untuk melihat data berat badan yang diterima.

# Catatan

- Pastikan perangkat BLE Peripheral mendukung layanan dan karakteristik yang sesuai.

- Jarak antara ESP32 dan perangkat BLE harus dalam jangkauan untuk koneksi stabil.

