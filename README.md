
# Dokumentasi IoT Gateway

Halaman ini akan membantumu untuk memahami program IoT Gateway menggunakan ESP32 dengan framework Arduino




# IoT Gateway

IoT gateway adalah perangkat atau sistem untuk mengelola komunikasi data antara perangkat kecil yang tidak dapat terhubung langsung ke internet dan jaringan yang lebih besar sehingga memungkinkan perangkat kecil untuk berfungsi secara efektif dalam ekosistem IoT




## Direktori

Direktori ini ditujukan untuk memahami pustaka khusus untuk lebih mudah mengatur dan memahami struktur program

```
|--lib
|  |--config
|  |  |--bluetooth
|  |  |  |-configBT.h //Konfigurasi BLE
|  |  |-config.h // Konfigurasi umum (wifi dan server)
|  |--peripheral //Untuk menambahkan perangkat disini
|  |  |--digit_pro_baby
|  |  |  |-dp_baby.cpp
|  |  |  |-dp_baby.h
|  |  |--digit_pro_ida
|  |  |  |-dp_ida.cpp
|  |  |  |-dp_ida.h
|--src
|  |-main.cpp //Program Utama
```


