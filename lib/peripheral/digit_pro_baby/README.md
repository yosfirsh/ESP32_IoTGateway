# Program BLE Client untuk Membaca Berat Badan (DP Baby)

Program ini dirancang untuk menghubungkan perangkat ESP32 dengan perangkat BLE (Bluetooth Low Energy) dan membaca data berat badan dari perangkat tersebut. Data berat badan yang diterima melalui BLE akan didekripsi dan dikonversi menjadi berat dalam kilogram.

## Fitur Program
- Koneksi dengan perangkat BLE menggunakan ESP32.
- Mendapatkan data dari perangkat BLE dan mengonversinya menjadi nilai desimal.
- Melakukan dekripsi data menggunakan XOR dengan kunci tertentu.
- Menampilkan hasil dalam format hexadecimal dan desimal.

## Penjelasan Program
### Library yang Digunakan
- **Arduino.h**: Library dasar untuk menggunakan fungsi Arduino.
- **BLEDevice.h**: Library untuk menggunakan fungsionalitas BLE pada ESP32.
- **bluetooth/configBT.h**: File konfigurasi untuk pengaturan Bluetooth (alamat dan UUID).
- **dp_baby.h**: Header untuk fungsi-fungsi terkait perangkat baby (berat badan).

### Variabel Global
- `pClient`: Pointer ke objek BLEClient untuk mengelola koneksi BLE.
- `pRemoteCharacteristic`: Pointer ke objek BLERemoteCharacteristic untuk berinteraksi dengan karakteristik perangkat BLE.
- `connected1`: Menandakan status koneksi perangkat pertama.
- `doReconnect1`: Menandakan jika perlu melakukan reconnect.

### Callback BLE
- **MyClientCallback**: Callback yang menangani event koneksi dan disconnection.
  - `onConnect()`: Tidak digunakan dalam program ini, tapi bisa dimodifikasi untuk tindakan setelah koneksi.
  - `onDisconnect()`: Menandakan bahwa perangkat terputus dan mencoba untuk reconnect.

### Callback Notifikasi BLE
- **notifyCallback1**: Callback yang menangani data yang diterima dari perangkat BLE dalam bentuk notifikasi.
  - Menerima data dalam format byte dan mencetaknya dalam format hexadecimal.
  - Mengonversi byte ke-5 dan ke-6 menjadi berat badan yang didekripsi dan mengonversinya ke kilogram.

### Fungsi Utama
- **connectToDevice1()**: Fungsi untuk menghubungkan ke perangkat BLE menggunakan alamat MAC dan UUID. Jika berhasil, akan mengaktifkan notifikasi untuk karakteristik tertentu.
- **setup_dp_baby()**: Fungsi untuk melakukan koneksi awal ke perangkat BLE. Jika koneksi gagal, fungsi akan mencoba lagi setelah beberapa detik.
- **dp_baby_handle()**: Fungsi untuk memeriksa status koneksi dan mencoba untuk reconnect jika koneksi terputus.

## Cara Penggunaan
1. **Koneksi**: Program akan secara otomatis mencoba untuk menghubungkan ke perangkat BLE yang telah ditentukan.
2. **Menerima Data**: Setelah koneksi berhasil, perangkat akan mengirimkan data berat badan melalui notifikasi BLE.
3. **Dekripsi dan Konversi**: Program akan mendekripsi data berat badan menggunakan XOR dan mengonversinya ke dalam satuan kilogram.
4. **Output**: Data berat badan akan dicetak dalam format hexadecimal dan desimal di serial monitor.

### Contoh Output:
```
Hex Berat : 0xEE71
Hasil Decrypt : 0x00BE
Berat Kg : 0.190 kg
Data (HEX): A700040501EC5BE8B4ED7A
```

## Troubleshooting
- **Koneksi Tidak Terhubung**:
  - Pastikan perangkat BLE dalam jangkauan dan alamat MAC serta UUID sudah benar.
  - Periksa kabel USB dan pastikan port yang dipilih di Arduino IDE sesuai dengan perangkat ESP32 yang digunakan.
  
- **Data Tidak Terbaca**:
  - Pastikan perangkat BLE mengirimkan data sesuai format yang diharapkan.
  - Pastikan notifikasi pada karakteristik BLE telah diaktifkan dengan benar.

