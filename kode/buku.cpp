// buku.cpp - F1: kelola buku (tambah, ubah, hapus, tampilkan)
// Penanggung jawab : Pande Made Dhilla Aprilia Erista (NIM 260530911141)
// Kelompok 7 - Kelas E
//
// Struktur data: array of struct dinamis (Buku*) yang dialokasikan dengan
// new[], diperbesar dua kali lipat saat penuh, dan dibebaskan dengan delete[].
#include <iostream>
#include <iomanip>
#include <new>
#include "sipus.h"

using namespace std;

// Batas pengaman masukan stok pada antarmuka (ASUMSI kelompok).
static const int STOK_MAKS_INPUT = 1000000;

// ----------------------------------------------------------------------
// Array dinamis
// ----------------------------------------------------------------------

void inisialisasi(DaftarBuku& d, int kapasitasAwal) {
    if (kapasitasAwal < 1) kapasitasAwal = 1;
    d.data = new (nothrow) Buku[kapasitasAwal];
    d.n = 0;
    d.kapasitas = (d.data != nullptr) ? kapasitasAwal : 0;
}

void bebaskan(DaftarBuku& d) {
    delete[] d.data;
    d.data = nullptr;
    d.n = 0;
    d.kapasitas = 0;
}

// Menggandakan kapasitas: alokasi array baru, salin isi, bebaskan yang lama.
static bool perbesar(DaftarBuku& d) {
    int kapasitasBaru = (d.kapasitas < 1) ? 1 : d.kapasitas * 2;
    Buku* baru = new (nothrow) Buku[kapasitasBaru];
    if (baru == nullptr) return false;
    for (int i = 0; i < d.n; i++) {
        baru[i] = d.data[i];
    }
    delete[] d.data;
    d.data = baru;
    d.kapasitas = kapasitasBaru;
    return true;
}

// ----------------------------------------------------------------------
// Pencarian
// ----------------------------------------------------------------------

static int cariIndeks(const DaftarBuku& d, const string& kode) {
    for (int i = 0; i < d.n; i++) {
        if (d.data[i].kode == kode) return i;
    }
    return -1;
}

Buku* cariKode(DaftarBuku& d, const string& kode) {
    int i = cariIndeks(d, kode);
    return (i >= 0) ? &d.data[i] : nullptr;
}

const Buku* cariKode(const DaftarBuku& d, const string& kode) {
    int i = cariIndeks(d, kode);
    return (i >= 0) ? &d.data[i] : nullptr;
}

// ----------------------------------------------------------------------
// Validasi
// ----------------------------------------------------------------------

string validasiIsiBuku(const string& judul, const string& penulis,
                       int kategori, int tahun, int stok) {
    if (trim(judul).empty()) return "Judul tidak boleh kosong.";
    if (trim(penulis).empty()) return "Penulis tidak boleh kosong.";
    if (kategori < 0 || kategori >= JML_KATEGORI) {
        return "Kategori harus 0 sampai " + to_string(JML_KATEGORI - 1) + ".";
    }
    if (tahun < TAHUN_MIN || tahun > TAHUN_MAKS) {
        return "Tahun harus antara " + to_string(TAHUN_MIN) + " dan " +
               to_string(TAHUN_MAKS) + ".";
    }
    if (stok < 0) return "Stok tidak boleh negatif.";
    return "";
}

string validasiBuku(const DaftarBuku& d, const Buku& b) {
    if (!formatKodeBuku(b.kode)) return "Format kode harus BK-xxx (xxx = 3 digit).";
    if (cariIndeks(d, b.kode) >= 0) return "Kode " + b.kode + " sudah digunakan.";
    return validasiIsiBuku(b.judul, b.penulis, b.kategori, b.tahun, b.stok);
}

// ----------------------------------------------------------------------
// Tambah, ubah, hapus
// ----------------------------------------------------------------------

bool tambahBuku(DaftarBuku& d, const Buku& b) {
    if (!validasiBuku(d, b).empty()) return false;
    if (d.n >= d.kapasitas) {
        if (!perbesar(d)) return false;
    }
    Buku baru = b;
    baru.judul = trim(b.judul);
    baru.penulis = trim(b.penulis);
    baru.totalDipinjam = 0;          // buku baru belum pernah dipinjam
    d.data[d.n] = baru;
    d.n++;
    return true;
}

bool ubahBuku(DaftarBuku& d, const string& kode, const string& judul,
              const string& penulis, int kategori, int tahun, int stok) {
    Buku* p = cariKode(d, kode);
    if (p == nullptr) return false;
    if (!validasiIsiBuku(judul, penulis, kategori, tahun, stok).empty()) return false;
    // Hanya lima field ini yang berubah. kode dan totalDipinjam sengaja dibiarkan.
    p->judul = trim(judul);
    p->penulis = trim(penulis);
    p->kategori = kategori;
    p->tahun = tahun;
    p->stok = stok;
    return true;
}

// Versi dasar dari PDF: menghapus tanpa memeriksa transaksi.
// Pemanggil bertanggung jawab memeriksa bukuMasihDipinjam() terlebih dahulu,
// atau gunakan versi di bawahnya yang sudah memeriksa.
bool hapusBuku(DaftarBuku& d, const string& kode) {
    int pos = cariIndeks(d, kode);
    if (pos < 0) return false;
    for (int i = pos; i < d.n - 1; i++) {
        d.data[i] = d.data[i + 1];   // geser ke kiri, urutan tetap terjaga
    }
    d.n--;
    return true;
}

// Versi aman: menerapkan aturan "buku yang masih dipinjam tidak boleh dihapus".
// Daftar transaksi dibaca saja (const). Pada Milestone 3 cukup F3 yang
// menambah transaksi dengan selesai == false; fungsi ini tidak perlu diubah.
StatusHapus hapusBuku(DaftarBuku& d, const DaftarTransaksi& t,
                      const string& kode) {
    if (cariIndeks(d, kode) < 0) return HAPUS_TIDAK_ADA;
    if (bukuMasihDipinjam(t, kode)) return HAPUS_MASIH_DIPINJAM;
    hapusBuku(d, kode);
    return HAPUS_OK;
}

// ----------------------------------------------------------------------
// Tampilan tabel
// ----------------------------------------------------------------------

void tampilBuku(const DaftarBuku& d) {
    cout << "\n=== DAFTAR BUKU ===\n";
    if (d.n == 0) {
        cout << "(Belum ada data buku.)\n";
        return;
    }
    const int W_NO = 3, W_KODE = 6, W_JUDUL = 21, W_PENULIS = 15;
    const int W_KAT = 11, W_TAHUN = 5, W_STOK = 4, W_PINJAM = 6;
    const int LEBAR = W_NO + W_KODE + W_JUDUL + W_PENULIS + W_KAT +
                      W_TAHUN + W_STOK + W_PINJAM + 7;

    cout << left;
    cout << garis(LEBAR) << '\n'
         << setw(W_NO) << "No" << ' ' << setw(W_KODE) << "Kode" << ' '
         << setw(W_JUDUL) << "Judul" << ' ' << setw(W_PENULIS) << "Penulis" << ' '
         << setw(W_KAT) << "Kategori" << ' ' << setw(W_TAHUN) << "Tahun" << ' '
         << setw(W_STOK) << "Stok" << ' ' << setw(W_PINJAM) << "Pinjam" << '\n'
         << garis(LEBAR) << '\n';
    for (int i = 0; i < d.n; i++) {
        const Buku& b = d.data[i];
        cout << setw(W_NO) << (i + 1) << ' ' << setw(W_KODE) << b.kode << ' '
             << setw(W_JUDUL) << potong(b.judul, W_JUDUL) << ' '
             << setw(W_PENULIS) << potong(b.penulis, W_PENULIS) << ' '
             << setw(W_KAT) << labelKategori(b.kategori) << ' '
             << setw(W_TAHUN) << b.tahun << ' ' << setw(W_STOK) << b.stok << ' '
             << setw(W_PINJAM) << b.totalDipinjam << '\n';
    }
    cout << garis(LEBAR) << '\n';
    cout << right;
    cout << "Total: " << d.n << " judul buku.  "
         << "(Pinjam = total riwayat dipinjam, hanya statistik)\n";
}

// ----------------------------------------------------------------------
// Antarmuka interaktif F1
// ----------------------------------------------------------------------

static void tampilDaftarKategori() {
    cout << "Kategori:";
    for (int k = 0; k < JML_KATEGORI; k++) {
        cout << (k == 0 ? " " : ", ") << k << "=" << labelKategori(k);
    }
    cout << '\n';
}

static void tampilDetailBuku(const Buku& b) {
    cout << "  Kode      : " << b.kode << '\n'
         << "  Judul     : " << b.judul << '\n'
         << "  Penulis   : " << b.penulis << '\n'
         << "  Kategori  : " << b.kategori << " (" << labelKategori(b.kategori) << ")\n"
         << "  Tahun     : " << b.tahun << '\n'
         << "  Stok      : " << b.stok << '\n'
         << "  Total dipinjam : " << b.totalDipinjam << '\n';
}

void prosesTambahBuku(DaftarBuku& d) {
    cout << "\n--- Tambah Buku ---\n";
    Buku b;
    string kode;
    while (true) {
        if (bacaTeks("Kode buku (BK-xxx, kosongkan untuk batal): ", kode, true) != BACA_OK) {
            cout << ">> Dibatalkan.\n";
            return;
        }
        kode = hurufBesar(kode);
        if (!formatKodeBuku(kode)) {
            cout << ">> Format kode harus BK-xxx (xxx = 3 digit), contoh BK-001.\n";
        } else if (cariKode(d, kode) != nullptr) {
            cout << ">> Kode " << kode << " sudah digunakan.\n";
        } else {
            break;
        }
    }
    b.kode = kode;
    if (bacaTeks("Judul   : ", b.judul, false) != BACA_OK) return;
    if (bacaTeks("Penulis : ", b.penulis, false) != BACA_OK) return;
    tampilDaftarKategori();
    if (bacaInt("Kategori (0-" + to_string(JML_KATEGORI - 1) + "): ",
                0, JML_KATEGORI - 1, b.kategori, false) != BACA_OK) return;
    if (bacaInt("Tahun terbit: ", TAHUN_MIN, TAHUN_MAKS, b.tahun, false) != BACA_OK) return;
    if (bacaInt("Stok (eksemplar): ", 0, STOK_MAKS_INPUT, b.stok, false) != BACA_OK) return;
    b.totalDipinjam = 0;

    if (tambahBuku(d, b)) {
        cout << ">> Buku " << b.kode << " berhasil ditambahkan.\n";
    } else {
        string pesan = validasiBuku(d, b);
        cout << ">> Gagal menambah buku: "
             << (pesan.empty() ? "memori tidak cukup." : pesan) << '\n';
    }
}

void prosesUbahBuku(DaftarBuku& d) {
    cout << "\n--- Ubah Buku ---\n";
    if (d.n == 0) {
        cout << ">> Belum ada data buku.\n";
        return;
    }
    string kode;
    if (bacaTeks("Kode buku yang diubah (kosongkan untuk batal): ", kode, true) != BACA_OK) {
        cout << ">> Dibatalkan.\n";
        return;
    }
    kode = hurufBesar(kode);
    const Buku* lama = cariKode(d, kode);
    if (lama == nullptr) {
        cout << ">> Buku dengan kode " << kode << " tidak ditemukan.\n";
        return;
    }
    cout << "Data saat ini:\n";
    tampilDetailBuku(*lama);
    cout << "(Tekan Enter tanpa mengetik untuk mempertahankan nilai lama)\n";

    string judul = lama->judul;
    string penulis = lama->penulis;
    int kategori = lama->kategori;
    int tahun = lama->tahun;
    int stok = lama->stok;
    string teks;

    HasilBaca r = bacaTeks("Judul baru   : ", teks, true);
    if (r == BACA_EOF) return;
    if (r == BACA_OK) judul = teks;
    r = bacaTeks("Penulis baru : ", teks, true);
    if (r == BACA_EOF) return;
    if (r == BACA_OK) penulis = teks;
    tampilDaftarKategori();
    r = bacaInt("Kategori baru (0-" + to_string(JML_KATEGORI - 1) + "): ",
                0, JML_KATEGORI - 1, kategori, true);
    if (r == BACA_EOF) return;
    r = bacaInt("Tahun baru    : ", TAHUN_MIN, TAHUN_MAKS, tahun, true);
    if (r == BACA_EOF) return;
    r = bacaInt("Stok baru     : ", 0, STOK_MAKS_INPUT, stok, true);
    if (r == BACA_EOF) return;

    if (ubahBuku(d, kode, judul, penulis, kategori, tahun, stok)) {
        cout << ">> Data buku " << kode << " berhasil diubah.\n";
    } else {
        string pesan = validasiIsiBuku(judul, penulis, kategori, tahun, stok);
        cout << ">> Gagal mengubah buku: " << (pesan.empty() ? "data tidak ditemukan." : pesan) << '\n';
    }
}

void prosesHapusBuku(DaftarBuku& d, const DaftarTransaksi& t) {
    cout << "\n--- Hapus Buku ---\n";
    if (d.n == 0) {
        cout << ">> Belum ada data buku.\n";
        return;
    }
    string kode;
    if (bacaTeks("Kode buku yang dihapus (kosongkan untuk batal): ", kode, true) != BACA_OK) {
        cout << ">> Dibatalkan.\n";
        return;
    }
    kode = hurufBesar(kode);
    const Buku* p = cariKode(d, kode);
    if (p == nullptr) {
        cout << ">> Buku dengan kode " << kode << " tidak ditemukan.\n";
        return;
    }
    if (bukuMasihDipinjam(t, kode)) {
        cout << ">> Buku " << kode << " masih dipinjam, tidak boleh dihapus.\n";
        return;
    }
    cout << "Buku yang akan dihapus:\n";
    tampilDetailBuku(*p);

    string jawab;
    while (true) {
        if (bacaBaris("Yakin menghapus? (y/n): ", jawab) == BACA_EOF) return;
        jawab = hurufBesar(trim(jawab));
        if (jawab == "Y" || jawab == "N") break;
        cout << ">> Ketik y atau n.\n";
    }
    if (jawab == "N") {
        cout << ">> Penghapusan dibatalkan.\n";
        return;
    }
    switch (hapusBuku(d, t, kode)) {
        case HAPUS_OK:
            cout << ">> Buku " << kode << " berhasil dihapus.\n";
            break;
        case HAPUS_MASIH_DIPINJAM:
            cout << ">> Buku " << kode << " masih dipinjam, tidak boleh dihapus.\n";
            break;
        case HAPUS_TIDAK_ADA:
            cout << ">> Buku dengan kode " << kode << " tidak ditemukan.\n";
            break;
    }
}

void menuBuku(DaftarBuku& d, const DaftarTransaksi& t) {
    int pilihan = 0;
    do {
        cout << "\n--------- KELOLA BUKU ---------\n"
             << "1. Tambah Buku\n"
             << "2. Ubah Buku\n"
             << "3. Hapus Buku\n"
             << "4. Tampilkan Buku\n"
             << "0. Kembali\n"
             << "-------------------------------\n";
        if (bacaInt("Masukkan pilihan: ", 0, 4, pilihan, false) != BACA_OK) return;
        switch (pilihan) {
            case 1: prosesTambahBuku(d); break;
            case 2: prosesUbahBuku(d); break;
            case 3: prosesHapusBuku(d, t); break;
            case 4: tampilBuku(d); break;
            default: break;
        }
    } while (pilihan != 0);
}
