// main.cpp - menu utama SiPus (Milestone 1-2: F1 dan F2)
// Kelompok 7 - Kelas E
// Mahasiswa A : Pande Made Dhilla Aprilia Erista (260530911141)
// Mahasiswa B : I Gusti Ayu Nandini Amara Santi  (260530911060)
//
// Menu F3-F8 belum aktif; baru direncanakan untuk milestone berikutnya.
#include <iostream>
#include "sipus.h"

using namespace std;

static void tampilMenuUtama() {
    cout << "\n========================================\n"
         << "      SiPus - PERPUSTAKAAN MINI\n"
         << "========================================\n"
         << "        KELOMPOK 7 - KELAS E\n"
         << "----------------------------------------\n"
         << "1. Kelola Buku\n"
         << "2. Kelola Anggota\n"
         << "3. Daftar Buku\n"
         << "0. Keluar\n"
         << "----------------------------------------\n";
}

int main() {
    DaftarBuku buku;
    DaftarAnggota anggota;
    DaftarTransaksi transaksi;

    // Kapasitas awal kecil (4) agar mekanisme pembesaran mudah teruji.
    inisialisasi(buku, KAPASITAS_AWAL);
    inisialisasi(anggota, KAPASITAS_AWAL);
    inisialisasi(transaksi, KAPASITAS_AWAL);

    int pilihan = 0;
    do {
        tampilMenuUtama();
        if (bacaInt("Masukkan pilihan: ", 0, 3, pilihan, false) != BACA_OK) {
            cout << "\n>> Masukan berakhir. Program ditutup.\n";
            pilihan = 0;
        }
        switch (pilihan) {
            case 1: menuBuku(buku, transaksi); break;
            case 2: menuAnggota(anggota); break;
            case 3: tampilBuku(buku); break;
            case 0: cout << "\nTerima kasih telah menggunakan SiPus.\n"; break;
            default: break;
        }
    } while (pilihan != 0);

    // Wajib: seluruh memori dinamis dibebaskan sebelum keluar.
    bebaskan(buku);
    bebaskan(anggota);
    bebaskan(transaksi);
    return 0;
}
