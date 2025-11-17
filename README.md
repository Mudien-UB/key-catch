# Key-Catch

## Deskripsi

Aplikasi keyboard overlay untuk Windows yang menampilkan key yang sedang ditekan secara real-time di terminal. Aplikasi ini menggunakan low-level keyboard hook untuk menangkap semua keystroke termasuk kombinasi key (Ctrl, Alt, Shift) dan menampilkannya dalam format yang mudah dibaca.

### Fitur

- Menampilkan key yang sedang ditekan secara live
- Menangkap kombinasi key (Ctrl, Alt, Shift)
- Menampilkan history 3 key terakhir
- Beberapa Format key mengikuti style Vim

## Kebutuhan Build

- **Compiler**: MinGW-w64 (g++) atau MSVC
- **OS**: Windows (menggunakan Windows API)
- **C++ Standard**: C++11 atau lebih tinggi
- **Dependencies**:
  - `windows.h` (Windows API)
  - C++ Standard Library

## Cara Build

```bash
g++ -o main.exe main.cpp -std=c++11
```

## Cara Menjalankan

```bash
./main.exe
```

## Status

⚠️ **PROJECT MASIH DALAM TAHAP DEVELOPMENT** ⚠️

Aplikasi ini masih dalam pengembangan aktif dan mungkin memiliki bugs atau fitur yang belum lengkap.
