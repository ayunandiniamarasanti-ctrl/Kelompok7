// transaksi.cpp - pendukung array dinamis DaftarTransaksi
// Penanggung jawab : I Gusti Ayu Nandini Amara Santi (NIM 260530911060)
// Kelompok 7 - Kelas E
//
// BERKAS INI BUKAN IMPLEMENTASI F3/F4.
// Isinya hanya fondasi yang dibutuhkan F1 pada Milestone 1-2:
//   - inisialisasi/bebaskan/tambahTransaksi untuk array dinamis transaksi
//   - bukuMasihDipinjam(): pemeriksaan transaksi aktif untuk hapusBuku()
// pinjamBuku() dan kembalikanBuku() (F3/F4) baru dikerjakan pada Milestone 3.
#include <new>
#include "sipus.h"

using namespace std;

void inisialisasi(DaftarTransaksi& d, int kapasitasAwal) {
    if (kapasitasAwal < 1) kapasitasAwal = 1;
    d.data = new (nothrow) Transaksi[kapasitasAwal];
    d.n = 0;
    d.kapasitas = (d.data != nullptr) ? kapasitasAwal : 0;
}

void bebaskan(DaftarTransaksi& d) {
    delete[] d.data;
    d.data = nullptr;
    d.n = 0;
    d.kapasitas = 0;
}

static bool perbesar(DaftarTransaksi& d) {
    int kapasitasBaru = (d.kapasitas < 1) ? 1 : d.kapasitas * 2;
    Transaksi* baru = new (nothrow) Transaksi[kapasitasBaru];
    if (baru == nullptr) return false;
    for (int i = 0; i < d.n; i++) {
        baru[i] = d.data[i];
    }
    delete[] d.data;
    d.data = baru;
    d.kapasitas = kapasitasBaru;
    return true;
}

// Menambahkan record apa adanya. Aturan bisnis (stok, batas 3 buku, dsb.)
// akan diterapkan oleh pinjamBuku() pada Milestone 3, bukan di sini.
bool tambahTransaksi(DaftarTransaksi& d, const Transaksi& t) {
    if (d.n >= d.kapasitas) {
        if (!perbesar(d)) return false;
    }
    d.data[d.n] = t;
    d.n++;
    return true;
}

// Sebuah buku dianggap "masih dipinjam" bila ada transaksi untuk kode itu
// yang belum selesai. F3 kelak membuat transaksi dengan selesai == false dan
// F4 menggantinya menjadi true, sehingga fungsi ini tidak perlu diubah.
bool bukuMasihDipinjam(const DaftarTransaksi& t, const string& kode) {
    for (int i = 0; i < t.n; i++) {
        if (t.data[i].kodeBuku == kode && !t.data[i].selesai) return true;
    }
    return false;
}
