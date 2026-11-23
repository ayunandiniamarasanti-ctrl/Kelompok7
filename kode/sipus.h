// sipus.h - definisi tipe data dan prototipe fungsi
// SiPus: Sistem Peminjaman Perpustakaan Mini
// Kelompok 7 - Kelas E - Pemrograman dan Struktur Data
// Program Studi Teknologi Informasi, Fakultas Teknik, Universitas Udayana, 2026
//
// Mahasiswa A : Pande Made Dhilla Aprilia Erista    (NIM 260530911141)
// Mahasiswa B : I Gusti Ayu Nandini Amara Santi     (NIM 260530911060)
// Repository  : https://github.com/ayunandiniamarasanti-ctrl/Kelompok7
//
// Versi awal (Milestone 1 dan sebagian Milestone 2): F1 dan F2.
// Prototipe fungsi F3-F8 sudah dicantumkan sesuai PDF dosen, tetapi
// BELUM diimplementasikan dan tidak boleh dipanggil pada milestone ini.
#ifndef SIPUS_H
#define SIPUS_H

#include <string>
using namespace std;

// ======================================================================
// Konstanta (sesuai PDF dosen)
// ======================================================================
const int JML_KATEGORI = 5;    // Ukuran baris rekap
const int JML_BULAN = 12;      // Ukuran kolom rekap
const int MAKS_PINJAM = 3;     // Batas buku/anggota
const int DENDA_HARIAN = 1000; // Rupiah per hari

// ----------------------------------------------------------------------
// Konstanta tambahan kelompok (ASUMSI, tidak ada di PDF dosen)
// ----------------------------------------------------------------------
const int KAPASITAS_AWAL = 4;  // PDF: kapasitas awal array dinamis maksimal 4
const int TAHUN_MIN = 1900;    // Batas bawah tahun terbit yang dianggap valid
const int TAHUN_MAKS = 2026;   // Batas atas tahun terbit (tahun proyek)

// ======================================================================
// Definisi struct (sesuai PDF dosen, atribut tidak diubah)
// ======================================================================
struct Tanggal {
    int hari, bulan, tahun;
};

struct Buku {
    string kode;        // Format "BK-001"
    string judul;
    string penulis;
    int kategori;       // 0..JML_KATEGORI-1
    int tahun;
    int stok;           // Eksemplar tersedia
    int totalDipinjam;  // Untuk statistik
};

struct Anggota {
    string id;          // Format "AG-001"
    string nama;
    string prodi;
    int sedangPinjam;   // Maksimum MAKS_PINJAM
};

struct Transaksi {
    int nomor;
    string idAnggota;
    string kodeBuku;
    Tanggal tglPinjam;
    Tanggal tglKembali; // Diisi saat buku kembali
    bool selesai;
    int denda;
};

// Pembungkus array dinamis: alamat, isi, kapasitas
struct DaftarBuku {
    Buku* data; int n, kapasitas;
};
struct DaftarAnggota {
    Anggota* data; int n, kapasitas;
};
struct DaftarTransaksi {
    Transaksi* data; int n, kapasitas;
};

// ======================================================================
// Tipe bantu (tambahan kelompok)
// ======================================================================

// Hasil penghapusan buku dengan pemeriksaan transaksi aktif.
enum StatusHapus {
    HAPUS_OK = 0,
    HAPUS_TIDAK_ADA,       // kode buku tidak ditemukan
    HAPUS_MASIH_DIPINJAM   // masih ada transaksi aktif (selesai == false)
};

// Hasil pembacaan masukan dari papan ketik.
enum HasilBaca {
    BACA_OK = 0,           // masukan valid terbaca
    BACA_KOSONG,           // baris kosong (hanya jika diizinkan)
    BACA_EOF               // masukan berakhir (Ctrl+Z / Ctrl+D / pipa habis)
};

// ======================================================================
// Modul Buku (Mahasiswa A) - F1 : IMPLEMENTASI di buku.cpp
// ======================================================================

// ---- Prototipe wajib PDF dosen yang termasuk F1 ----
void inisialisasi(DaftarBuku& d, int kapasitasAwal);
void bebaskan(DaftarBuku& d);
bool tambahBuku(DaftarBuku& d, const Buku& b);
bool hapusBuku(DaftarBuku& d, const string& kode);
Buku* cariKode(DaftarBuku& d, const string& kode);

// ---- Prototipe tambahan kelompok untuk melengkapi F1 ----
const Buku* cariKode(const DaftarBuku& d, const string& kode);
// Mengembalikan pesan kesalahan, atau string kosong jika data valid.
string validasiBuku(const DaftarBuku& d, const Buku& b);
string validasiIsiBuku(const string& judul, const string& penulis,
                       int kategori, int tahun, int stok);
// Mengubah judul, penulis, kategori, tahun, stok. Kode dan totalDipinjam tetap.
bool ubahBuku(DaftarBuku& d, const string& kode, const string& judul,
              const string& penulis, int kategori, int tahun, int stok);
// Hapus dengan pemeriksaan transaksi aktif (aturan bisnis).
StatusHapus hapusBuku(DaftarBuku& d, const DaftarTransaksi& t,
                      const string& kode);
void tampilBuku(const DaftarBuku& d);

// Antarmuka interaktif F1
void prosesTambahBuku(DaftarBuku& d);
void prosesUbahBuku(DaftarBuku& d);
void prosesHapusBuku(DaftarBuku& d, const DaftarTransaksi& t);
void menuBuku(DaftarBuku& d, const DaftarTransaksi& t);

// ======================================================================
// Modul Anggota (Mahasiswa B) - F2 : IMPLEMENTASI di anggota.cpp
// ======================================================================
void inisialisasi(DaftarAnggota& d, int kapasitasAwal);
void bebaskan(DaftarAnggota& d);
bool tambahAnggota(DaftarAnggota& d, const Anggota& a);
Anggota* cariAnggota(DaftarAnggota& d, const string& id);
const Anggota* cariAnggota(const DaftarAnggota& d, const string& id);
string validasiAnggota(const DaftarAnggota& d, const Anggota& a);
string validasiNamaProdi(const string& nama, const string& prodi);
// Mengubah nama dan prodi. ID dan sedangPinjam tetap.
bool ubahAnggota(DaftarAnggota& d, const string& id, const string& nama,
                 const string& prodi);
void tampilAnggota(const DaftarAnggota& d);

// Antarmuka interaktif F2
void prosesTambahAnggota(DaftarAnggota& d);
void prosesUbahAnggota(DaftarAnggota& d);
void menuAnggota(DaftarAnggota& d);

// ======================================================================
// Pendukung array dinamis transaksi (Mahasiswa B) : transaksi.cpp
// Hanya fondasi memori dan pemeriksaan transaksi aktif. BUKAN F3/F4.
// ======================================================================
void inisialisasi(DaftarTransaksi& d, int kapasitasAwal);
void bebaskan(DaftarTransaksi& d);
// Menambah record apa adanya (tanpa aturan bisnis peminjaman).
bool tambahTransaksi(DaftarTransaksi& d, const Transaksi& t);
// true bila ada transaksi dengan kodeBuku == kode dan selesai == false.
bool bukuMasihDipinjam(const DaftarTransaksi& t, const string& kode);

// ======================================================================
// Utilitas masukan dan tampilan : util.cpp (kerja bersama)
// ======================================================================
string trim(const string& s);
string hurufBesar(const string& s);
bool formatKodeBuku(const string& kode);   // BK-xxx
bool formatIdAnggota(const string& id);    // AG-xxx
bool parseInt(const string& s, int& hasil);
HasilBaca bacaBaris(const string& prompt, string& hasil);
HasilBaca bacaTeks(const string& prompt, string& hasil, bool bolehKosong);
HasilBaca bacaInt(const string& prompt, int minimum, int maksimum,
                  int& hasil, bool bolehKosong);
string labelKategori(int kategori);
string potong(const string& s, int lebar);
string garis(int panjang);

// ======================================================================
// BELUM DIIMPLEMENTASIKAN - rencana milestone berikutnya (PDF dosen)
// Jangan dipanggil pada Milestone 1/2. Hanya prototipe.
// ======================================================================
// ---- Modul Buku (Mahasiswa A) : F5, F6 - Milestone 4 ----
void cariJudul(const DaftarBuku& d, const string& kata);
void urutkanKode(DaftarBuku& d);
int binarySearchKode(const Buku arr[], int kiri, int kanan, const string& kode);

// ---- Modul Anggota & Transaksi (Mahasiswa B) : F3, F4, F8 - Milestone 3 ----
bool pinjamBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a,
                const string& idAg, const string& kodeBk, const Tanggal& tgl);
bool kembalikanBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a,
                    int nomor, const Tanggal& tgl);
int selisihHari(const Tanggal& awal, const Tanggal& akhir);
int totalDendaRekursif(const Transaksi arr[], int n);

// ---- Modul Laporan (Mahasiswa B) : F7 - Milestone 3 ----
void isiRekap(const DaftarTransaksi& t, const DaftarBuku& b,
              int rekap[][JML_BULAN]);
void tampilRekap(const int rekap[][JML_BULAN], const string namaKategori[]);

#endif
