SiPus - Sistem Peminjaman Perpustakaan Mini
============================================
Kelompok 7 - Kelas E
Mata Kuliah : Pemrograman dan Struktur Data
Program Studi Teknologi Informasi, Fakultas Teknik, Universitas Udayana, 2026

Anggota kelompok
  Pande Made Dhilla Aprilia Erista     NIM 260530911141   (Mahasiswa A)
  I Gusti Ayu Nandini Amara Santi      NIM 260530911060   (Mahasiswa B)

Repository
  https://github.com/ayunandiniamarasanti-ctrl/Kelompok7


1. DESKRIPSI PROGRAM
--------------------
SiPus adalah aplikasi konsol berbasis menu (C++17) untuk membantu petugas
perpustakaan mengelola data buku dan anggota. Seluruh data inti disimpan
dalam array of struct dinamis yang dibuat manual (new[] / delete[]) dan
kapasitasnya bertambah otomatis. std::vector, std::list, std::map, dan
std::sort TIDAK digunakan.


2. STATUS PENGERJAAN
--------------------
Dokumen ini mencerminkan Milestone 1 dan sebagian Milestone 2.

  SELESAI DAN DIUJI
    F1  Kelola buku    : tambah, ubah, hapus, tampilkan (tabel)
    F2  Kelola anggota : tambah, ubah, tampilkan (tabel)
    Array dinamis untuk Buku, Anggota, dan Transaksi (inisialisasi,
    pembesaran kapasitas, pembebasan memori)
    Pemeriksaan transaksi aktif untuk aturan "buku yang masih dipinjam
    tidak boleh dihapus"

  BELUM DIKERJAKAN (hanya prototipe di sipus.h, tidak ada menu aktifnya)
    F3  Peminjaman buku                       (Milestone 3)
    F4  Pengembalian dan denda                (Milestone 3)
    F5  Pencarian judul dan binary search     (Milestone 4)
    F6  Pengurutan buku                       (Milestone 4)
    F7  Laporan rekap kategori x bulan        (Milestone 3)
    F8  Total denda dan riwayat (rekursif)    (Milestone 3)
    Fitur tambahan (berkas, merge sort, antrean, undo)

Karena F3 belum ada, menu belum dapat membuat transaksi peminjaman.
Penolakan hapus buku yang sedang dipinjam sudah diterapkan di kode dan
diuji secara otomatis di uji.cpp dengan transaksi buatan.


3. FITUR F1 DAN F2
------------------
Menu utama:
  1. Kelola Buku       -> menu F1
  2. Kelola Anggota    -> menu F2
  3. Daftar Buku       -> menampilkan tabel buku
  0. Keluar

F1 - Kelola Buku
  Tambah  : kode (BK-xxx, unik), judul, penulis, kategori 0-4, tahun, stok.
            Validasi: format kode, kode unik, judul/penulis tidak kosong,
            kategori 0-4, tahun 1900-2026, stok >= 0. totalDipinjam = 0.
  Ubah    : cari berdasarkan kode, tampilkan data lama, ubah judul, penulis,
            kategori, tahun, stok (Enter = pertahankan). Kode dan
            totalDipinjam tidak pernah berubah.
  Hapus   : cari berdasarkan kode, tampilkan data, minta konfirmasi (y/n).
            Ditolak bila buku masih dipinjam (ada transaksi selesai == false).
  Tampil  : tabel rapi (<iomanip>), judul panjang dipotong dengan "...".
            Pesan khusus bila data kosong.

F2 - Kelola Anggota
  Tambah  : ID (AG-xxx, unik), nama, prodi. sedangPinjam = 0.
  Ubah    : cari berdasarkan ID, tampilkan data lama, ubah nama dan prodi
            (Enter = pertahankan). ID dan sedangPinjam tidak berubah.
  Tampil  : tabel ID, nama, prodi, sedang pinjam.

Penanganan masukan: semua masukan dibaca dengan getline() lalu divalidasi.
Masukan salah (huruf pada kolom angka, di luar rentang, kosong, format kode
salah, kode ganda) hanya menampilkan pesan dan meminta ulang; program tidak
berhenti. Pada kolom kode/ID, baris kosong berarti batal. Kode/ID huruf
kecil otomatis diubah menjadi huruf besar. Jika masukan terputus (EOF),
program keluar dengan rapi.


4. STRUKTUR FILE
----------------
  kode/
    sipus.h        Konstanta, struct, dan prototipe fungsi
    buku.cpp       F1 (Mahasiswa A) + array dinamis DaftarBuku
    anggota.cpp    F2 (Mahasiswa B) + array dinamis DaftarAnggota
    transaksi.cpp  Array dinamis DaftarTransaksi + bukuMasihDipinjam()
                   (hanya fondasi; BUKAN F3/F4)
    util.cpp       Pembaca masukan, validasi format, pemotong teks
    main.cpp       Menu utama
    uji.cpp        Pengujian otomatis (punya main() sendiri)
    README.txt     Berkas ini
  dokumen/
    Rancangan-SiPus-Kelompok07.docx
  bukti/
    (diisi tangkapan layar hasil pengujian)

main.cpp dan uji.cpp masing-masing memiliki main(), jadi keduanya
dikompilasi TERPISAH. Jangan menyertakan keduanya dalam satu perintah.


5. CARA KOMPILASI (GCC / MinGW, dari folder kode/)
--------------------------------------------------
Program utama:
  g++ -std=c++17 -Wall -Wextra -pedantic main.cpp buku.cpp anggota.cpp transaksi.cpp util.cpp -o sipus

Program pengujian:
  g++ -std=c++17 -Wall -Wextra -pedantic uji.cpp buku.cpp anggota.cpp transaksi.cpp util.cpp -o uji

Dengan AddressSanitizer (Linux/macOS, atau MinGW yang mendukung):
  g++ -std=c++17 -Wall -Wextra -pedantic -g -fsanitize=address,undefined main.cpp buku.cpp anggota.cpp transaksi.cpp util.cpp -o sipus_asan
  g++ -std=c++17 -Wall -Wextra -pedantic -g -fsanitize=address,undefined uji.cpp buku.cpp anggota.cpp transaksi.cpp util.cpp -o uji_asan

Catatan Windows: AddressSanitizer pada MinGW sering tidak tersedia. Jalankan
pemeriksaan memori di Linux/WSL, atau gunakan MSVC (/fsanitize=address).


6. CARA MENJALANKAN
-------------------
  Windows : sipus.exe        (atau .\sipus di PowerShell)
  Linux   : ./sipus


7. CARA PENGUJIAN
-----------------
  Windows : uji.exe
  Linux   : ./uji
Setiap kasus mencetak [LULUS] atau [GAGAL], diakhiri ringkasan
"N lulus, M gagal". Kode keluar 0 bila semua lulus, 1 bila ada yang gagal.
Pengujian mencakup: tambah, duplikat, validasi, pembesaran array, ubah,
hapus, pemeriksaan transaksi aktif, anggota, dan pembebasan memori.


8. PEMBAGIAN TUGAS
------------------
  Mahasiswa A - Pande Made Dhilla Aprilia Erista (260530911141)
    F1 (buku.cpp), rancangan F5 dan F6, array dinamis DaftarBuku
  Mahasiswa B - I Gusti Ayu Nandini Amara Santi (260530911060)
    F2 (anggota.cpp), rancangan F3, F4, F7, F8, array dinamis
    DaftarAnggota dan DaftarTransaksi (transaksi.cpp)
  Bersama
    Rancangan struct, sipus.h, menu main.cpp, util.cpp, integrasi, dan
    pengujian memori.


9. ASUMSI (tidak tertulis di PDF dosen)
---------------------------------------
  - Tahun terbit valid: 1900 sampai 2026 (TAHUN_MIN, TAHUN_MAKS di sipus.h).
  - Nama kategori 0-4 mengikuti contoh laporan di PDF: Pemrograman,
    Basis Data, Jaringan, Matematika, Umum.
  - Kode/ID harus tepat "BK-" atau "AG-" diikuti 3 digit.
  - Buku/anggota baru selalu memulai totalDipinjam / sedangPinjam dari 0.
  - Stok pada menu dibatasi 0 sampai 1.000.000 sebagai pengaman masukan.
  - Stok yang diubah lewat menu Ubah Buku dianggap koreksi jumlah
    eksemplar tersedia oleh petugas.
