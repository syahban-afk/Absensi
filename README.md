
# 📌 Catatan Penting

**Jangan lupa untuk mengganti:**

```cpp
const char *ssid = "Nama-Wifi";       // Line 8
const char *password = "Password-Wifi"; // Line 9
String url = "Website-URL" + uid;     // Line 126
```

**Contoh URL yang digunakan:**

```
http://absensi.com/card/scan?id=
```

---

# 📖 Penjelasan Program

Program ini merupakan sistem **absensi berbasis RFID** menggunakan **ESP32** yang berkomunikasi dengan server melalui koneksi **WiFi** dan protokol **HTTP GET**. Perangkat dilengkapi dengan **LCD I2C** sebagai tampilan antarmuka dan **buzzer** sebagai pemberi notifikasi suara.

---

## 🔧 Fitur Utama

### ✅ Koneksi WiFi Otomatis
- Terhubung otomatis ke jaringan WiFi saat perangkat menyala.
- Jika koneksi terputus, sistem mencoba menyambung ulang dan akan restart jika gagal.

### ✅ Pembacaan Kartu RFID
- Menggunakan **modul MFRC522** untuk membaca UID dari kartu RFID.
- UID dikonversi ke bentuk **string heksadesimal**.

### ✅ Pengiriman Data ke Server
- UID dikirim ke server melalui HTTP GET.
- Keberhasilan absensi ditentukan dari **kode HTTP response (200)**.

### ✅ Tampilan LCD I2C 20x4
- Menampilkan status sistem: koneksi WiFi, IP address, perintah "Tap Your Card", dan hasil absensi seperti "Absen Masuk" atau "Absen Gagal".

### ✅ Feedback Buzzer
- **Nada naik** → absensi berhasil.
- **Nada turun** → absensi gagal.

---

## ⚙️ Cara Kerja

### 1. Startup
- Inisialisasi LCD, WiFi, SPI, RFID, dan buzzer.
- Tampilkan status koneksi WiFi pada LCD.

### 2. Menunggu Tap
- LCD menampilkan pesan **"Tap Your Card"**.
- Sistem terus memantau keberadaan kartu baru.

### 3. Pembacaan Kartu
- Jika kartu terbaca, UID dikirim ke server melalui URL yang telah ditentukan.

### 4. Respons Server
- Jika server membalas dengan kode 200 → absensi berhasil.
- Jika gagal atau tidak merespons → tampilkan pesan gagal, sistem kembali ke awal.

---

## 🧠 Cocok Untuk

- Sistem absensi **sekolah**, **kampus**, atau **kantor**.
- Proyek pembelajaran **IoT** dan **Web Backend**.
- Implementasi nyata dari integrasi **RFID + HTTP + LCD + Buzzer** dengan ESP32.
