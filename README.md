# KTP Digital

Program konsol C++ sederhana untuk mencatat, menampilkan, mencari, dan menghapus data. Proyek ini dibuat sebagai latihan bahasa C++ dengan pengelolaan data interaktif di terminal.

## Fitur

- Menambahkan data KTP melalui terminal.
- Memvalidasi NIK agar terdiri dari 16 digit angka.
- Membaca tanggal lahir dan jenis kelamin dari format NIK.
- Menampilkan seluruh data yang tersimpan.
- Mencari data berdasarkan NIK.
- Menghapus seluruh data tersimpan.

## Kebutuhan

- Compiler C++ yang mendukung C++11 atau lebih baru, misalnya `g++` (MinGW-w64).
- Windows untuk dukungan tampilan UTF-8 dan warna terminal yang digunakan program.

## Cara menjalankan

1. Clone repository ini.

   ```bash
   git clone https://github.com/Rafstar14/KTP-Digital.git
   cd KTP-Digital
   ```

2. Kompilasi program utama.

   ```bash
   g++ -std=c++11 hello.cpp -o ktp-digital.exe
   ```

3. Jalankan program.

   ```bash
   .\ktp-digital.exe
   ```
4. Atau kalian bisa copy file "hello.cpp" lalu tempelkan di vs code tanpa perlu pusing atur atur 
   
## Penyimpanan data

Data yang dimasukkan program disimpan secara lokal pada `data_ktp.txt`. File tersebut sengaja tercantum dalam `.gitignore`, sehingga tidak ikut diunggah ke GitHub.

## Struktur file

| File | Keterangan |
| --- | --- |
| `hello.cpp` | Program utama dengan menu interaktif. |
| `ktp.cpp` | Contoh tampilan kartu KTP dengan data fiktif. |
| `.gitignore` | Mengecualikan data lokal dan hasil kompilasi dari Git. |

## Lisensi

Belum ada lisensi yang ditetapkan untuk proyek ini.
