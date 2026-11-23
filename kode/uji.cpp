// uji.cpp - pengujian otomatis awal (Milestone 1-2: F1 dan F2)
// Kelompok 7 - Kelas E
// Pola fungsi cek() sesuai contoh PDF dosen. Dikompilasi TERPISAH dari main.cpp
// (hanya satu main() per executable).
#include <iostream>
#include "sipus.h"

using namespace std;

int lulus = 0, gagal = 0;

void cek(bool kondisi, const string& nama) {
    cout << (kondisi ? "[LULUS] " : "[GAGAL] ") << nama << endl;
    kondisi ? lulus++ : gagal++;
}

// Membuat buku contoh yang valid.
static Buku buatBuku(const string& kode, int kategori = 0, int stok = 3) {
    Buku b = {kode, "Judul " + kode, "Penulis", kategori, 2024, stok, 0};
    return b;
}

static Anggota buatAnggota(const string& id) {
    Anggota a = {id, "Nama " + id, "Teknologi Informasi", 0};
    return a;
}

static void ujiBukuTambahDanDuplikat() {
    cout << "\n-- F1: tambah buku --\n";
    DaftarBuku d;
    inisialisasi(d, 2);
    cek(d.n == 0 && d.kapasitas == 2 && d.data != nullptr, "inisialisasi buku kosong, kapasitas 2");

    cek(tambahBuku(d, {"BK-002", "Struktur Data", "Wati", 0, 2024, 3, 0}), "tambah buku berhasil");
    cek(d.n == 1 && d.data[0].kode == "BK-002", "data buku tersimpan");
    cek(d.data[0].totalDipinjam == 0, "totalDipinjam diinisialisasi 0");

    // Meskipun pemanggil mengisi totalDipinjam != 0, buku baru tetap mulai dari 0.
    Buku b = {"BK-001", "Algoritma", "Budi", 0, 2023, 1, 9};
    cek(tambahBuku(d, b) && d.data[1].totalDipinjam == 0, "buku baru selalu mulai totalDipinjam 0");

    Buku ganda = {"BK-001", "X", "Y", 0, 2020, 1, 0};
    cek(!tambahBuku(d, ganda), "kode buku ganda ditolak");
    cek(d.n == 2, "jumlah buku tetap setelah duplikat ditolak");

    cek(tambahBuku(d, {"BK-003", "Basis Data", "Sari", 1, 2022, 2, 0}), "tambah buku ke-3 (memicu pembesaran)");
    cek(d.n == 3 && d.kapasitas >= 3, "array buku membesar");
    cek(d.data[0].kode == "BK-002" && d.data[1].kode == "BK-001" && d.data[2].kode == "BK-003",
        "isi lama utuh setelah pembesaran");
    bebaskan(d);
}

static void ujiBukuPembesaranBerulang() {
    cout << "\n-- F1: pembesaran berulang --\n";
    DaftarBuku d;
    inisialisasi(d, 1);
    bool semuaBerhasil = true;
    for (int i = 1; i <= 20; i++) {
        string kode = (i < 10 ? "BK-00" : "BK-0") + to_string(i);
        if (!tambahBuku(d, buatBuku(kode))) semuaBerhasil = false;
    }
    cek(semuaBerhasil && d.n == 20, "20 buku ditambahkan dari kapasitas awal 1");
    cek(d.kapasitas >= 20, "kapasitas >= jumlah data");
    cek(d.data[0].kode == "BK-001" && d.data[19].kode == "BK-020", "urutan data terjaga setelah beberapa kali pembesaran");
    bebaskan(d);
}

static void ujiBukuValidasi() {
    cout << "\n-- F1: validasi masukan --\n";
    DaftarBuku d;
    inisialisasi(d, 4);
    cek(!tambahBuku(d, {"BK-1", "J", "P", 0, 2024, 1, 0}), "kode terlalu pendek ditolak");
    cek(!tambahBuku(d, {"bk-001", "J", "P", 0, 2024, 1, 0}), "kode huruf kecil ditolak");
    cek(!tambahBuku(d, {"BK-0012", "J", "P", 0, 2024, 1, 0}), "kode terlalu panjang ditolak");
    cek(!tambahBuku(d, {"XX-001", "J", "P", 0, 2024, 1, 0}), "awalan kode salah ditolak");
    cek(!tambahBuku(d, {"BK-0A1", "J", "P", 0, 2024, 1, 0}), "kode non-digit ditolak");
    cek(!tambahBuku(d, {"BK-001", "", "P", 0, 2024, 1, 0}), "judul kosong ditolak");
    cek(!tambahBuku(d, {"BK-001", "   ", "P", 0, 2024, 1, 0}), "judul spasi saja ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "", 0, 2024, 1, 0}), "penulis kosong ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", -1, 2024, 1, 0}), "kategori -1 ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", JML_KATEGORI, 2024, 1, 0}), "kategori 5 ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", 0, 2024, -1, 0}), "stok negatif ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", 0, 0, 1, 0}), "tahun 0 ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", 0, TAHUN_MAKS + 1, 1, 0}), "tahun melebihi batas ditolak");
    cek(d.n == 0, "tidak ada data masuk dari masukan tidak valid");
    cek(tambahBuku(d, {"BK-001", "J", "P", 0, 2024, 0, 0}), "stok 0 diperbolehkan");
    cek(tambahBuku(d, {"BK-002", "J", "P", JML_KATEGORI - 1, TAHUN_MIN, 1, 0}), "nilai batas kategori dan tahun diterima");
    bebaskan(d);
}

static void ujiBukuCariUbahHapus() {
    cout << "\n-- F1: cari, ubah, hapus --\n";
    DaftarBuku d;
    inisialisasi(d, 2);
    tambahBuku(d, buatBuku("BK-001"));
    tambahBuku(d, buatBuku("BK-002"));
    tambahBuku(d, buatBuku("BK-003"));

    Buku* p = cariKode(d, "BK-002");
    cek(p != nullptr && p->kode == "BK-002", "cariKode menemukan buku");
    cek(cariKode(d, "BK-999") == nullptr, "cariKode kode tidak ada mengembalikan nullptr");

    p->totalDipinjam = 7;   // simulasi statistik dari F3 (belum ada)
    cek(ubahBuku(d, "BK-002", "Judul Baru", "Penulis Baru", 3, 2020, 9), "ubah buku berhasil");
    cek(d.data[1].judul == "Judul Baru" && d.data[1].penulis == "Penulis Baru" &&
        d.data[1].kategori == 3 && d.data[1].tahun == 2020 && d.data[1].stok == 9,
        "semua field yang diubah tersimpan");
    cek(d.data[1].kode == "BK-002", "kode tetap setelah diubah");
    cek(d.data[1].totalDipinjam == 7, "totalDipinjam tidak berubah saat ubah buku");
    cek(!ubahBuku(d, "BK-999", "J", "P", 0, 2024, 1), "ubah buku tidak ada ditolak");
    cek(!ubahBuku(d, "BK-002", "", "P", 0, 2024, 1), "ubah dengan judul kosong ditolak");
    cek(!ubahBuku(d, "BK-002", "J", "P", 9, 2024, 1), "ubah dengan kategori tidak valid ditolak");
    cek(!ubahBuku(d, "BK-002", "J", "P", 0, 2024, -5), "ubah dengan stok negatif ditolak");
    cek(d.data[1].judul == "Judul Baru" && d.data[1].stok == 9, "data tidak rusak setelah ubah ditolak");

    // Hapus tanpa transaksi
    DaftarTransaksi t;
    inisialisasi(t, 2);
    cek(hapusBuku(d, t, "BK-001") == HAPUS_OK, "hapus buku (tidak dipinjam) berhasil");
    cek(d.n == 2 && d.data[0].kode == "BK-002" && d.data[1].kode == "BK-003",
        "sisa buku bergeser dengan urutan benar");
    cek(cariKode(d, "BK-001") == nullptr, "buku terhapus tidak ditemukan lagi");
    cek(hapusBuku(d, t, "BK-001") == HAPUS_TIDAK_ADA, "hapus buku tidak ada dilaporkan");

    // Hapus dengan transaksi aktif
    Transaksi aktif = {1, "AG-001", "BK-002", {3, 11, 2026}, {0, 0, 0}, false, 0};
    cek(tambahTransaksi(t, aktif), "tambahTransaksi (fondasi array) berhasil");
    cek(bukuMasihDipinjam(t, "BK-002"), "bukuMasihDipinjam true untuk transaksi aktif");
    cek(!bukuMasihDipinjam(t, "BK-003"), "bukuMasihDipinjam false untuk buku lain");
    cek(hapusBuku(d, t, "BK-002") == HAPUS_MASIH_DIPINJAM, "hapus buku yang masih dipinjam ditolak");
    cek(d.n == 2 && cariKode(d, "BK-002") != nullptr, "buku yang dipinjam tetap ada");

    t.data[0].selesai = true;   // simulasi pengembalian (F4 belum ada)
    cek(!bukuMasihDipinjam(t, "BK-002"), "bukuMasihDipinjam false setelah transaksi selesai");
    cek(hapusBuku(d, t, "BK-002") == HAPUS_OK, "hapus buku berhasil setelah dikembalikan");

    bebaskan(t);
    bebaskan(d);
}

static void ujiAnggota() {
    cout << "\n-- F2: anggota --\n";
    DaftarAnggota d;
    inisialisasi(d, 2);
    cek(d.n == 0 && d.kapasitas == 2 && d.data != nullptr, "inisialisasi anggota kosong, kapasitas 2");

    cek(tambahAnggota(d, {"AG-001", "Made", "Teknologi Informasi", 0}), "tambah anggota berhasil");
    cek(d.n == 1 && d.data[0].id == "AG-001" && d.data[0].nama == "Made", "data anggota tersimpan");
    cek(tambahAnggota(d, {"AG-002", "Ayu", "Sistem Informasi", 2}) && d.data[1].sedangPinjam == 0,
        "sedangPinjam anggota baru selalu 0");
    cek(!tambahAnggota(d, {"AG-001", "Lain", "Prodi Lain", 0}), "ID anggota ganda ditolak");
    cek(d.n == 2, "jumlah anggota tetap setelah duplikat ditolak");

    cek(!tambahAnggota(d, {"AG-1", "N", "P", 0}), "ID terlalu pendek ditolak");
    cek(!tambahAnggota(d, {"ag-003", "N", "P", 0}), "ID huruf kecil ditolak");
    cek(!tambahAnggota(d, {"BK-003", "N", "P", 0}), "awalan ID salah ditolak");
    cek(!tambahAnggota(d, {"AG-003", "", "P", 0}), "nama kosong ditolak");
    cek(!tambahAnggota(d, {"AG-003", "N", "  ", 0}), "prodi kosong ditolak");
    cek(d.n == 2, "tidak ada data masuk dari masukan tidak valid");

    cek(tambahAnggota(d, {"AG-003", "Komang", "Teknologi Informasi", 0}), "tambah anggota ke-3 (memicu pembesaran)");
    cek(d.n == 3 && d.kapasitas >= 3, "array anggota membesar");
    cek(d.data[0].id == "AG-001" && d.data[2].id == "AG-003", "isi lama utuh setelah pembesaran");

    for (int i = 4; i <= 15; i++) {
        string id = (i < 10 ? "AG-00" : "AG-0") + to_string(i);
        tambahAnggota(d, buatAnggota(id));
    }
    cek(d.n == 15 && d.kapasitas >= 15, "anggota bertambah hingga 15 (pembesaran berulang)");

    Anggota* p = cariAnggota(d, "AG-002");
    cek(p != nullptr && p->nama == "Ayu", "cariAnggota menemukan anggota");
    cek(cariAnggota(d, "AG-999") == nullptr, "cariAnggota ID tidak ada mengembalikan nullptr");

    p->sedangPinjam = 2;   // simulasi dari F3 (belum ada)
    cek(ubahAnggota(d, "AG-002", "Ayu Baru", "Prodi Baru"), "ubah anggota berhasil");
    cek(p->nama == "Ayu Baru" && p->prodi == "Prodi Baru", "nama dan prodi berubah");
    cek(p->id == "AG-002", "ID tetap setelah diubah");
    cek(p->sedangPinjam == 2, "sedangPinjam tidak berubah saat ubah anggota");
    cek(!ubahAnggota(d, "AG-999", "N", "P"), "ubah anggota tidak ada ditolak");
    cek(!ubahAnggota(d, "AG-002", "", "P"), "ubah dengan nama kosong ditolak");
    cek(!ubahAnggota(d, "AG-002", "N", ""), "ubah dengan prodi kosong ditolak");
    cek(p->nama == "Ayu Baru" && p->prodi == "Prodi Baru", "data tidak rusak setelah ubah ditolak");

    bebaskan(d);
    cek(d.data == nullptr && d.n == 0 && d.kapasitas == 0, "memori anggota dibebaskan");
}

static void ujiTransaksiDanMemori() {
    cout << "\n-- Array dinamis transaksi dan pembebasan memori --\n";
    DaftarTransaksi t;
    inisialisasi(t, 2);
    bool ok = true;
    for (int i = 1; i <= 9; i++) {
        Transaksi tr = {i, "AG-001", "BK-001", {1, 1, 2026}, {0, 0, 0}, false, 0};
        if (!tambahTransaksi(t, tr)) ok = false;
    }
    cek(ok && t.n == 9 && t.kapasitas >= 9, "array transaksi membesar");
    cek(t.data[0].nomor == 1 && t.data[8].nomor == 9, "isi transaksi utuh setelah pembesaran");
    bebaskan(t);
    cek(t.data == nullptr && t.n == 0 && t.kapasitas == 0, "memori transaksi dibebaskan");

    DaftarBuku d;
    inisialisasi(d, 4);
    tambahBuku(d, buatBuku("BK-001"));
    bebaskan(d);
    cek(d.data == nullptr, "memori dibebaskan");
    cek(d.n == 0 && d.kapasitas == 0, "n dan kapasitas direset setelah dibebaskan");
    bebaskan(d);
    cek(d.data == nullptr, "bebaskan dua kali aman");
    cek(tambahBuku(d, buatBuku("BK-005")) && d.n == 1, "daftar dapat dipakai lagi setelah dibebaskan");
    bebaskan(d);
}

static void ujiValidatorFormat() {
    cout << "\n-- Utilitas validasi format --\n";
    cek(formatKodeBuku("BK-001") && !formatKodeBuku("BK-01") && !formatKodeBuku(""), "formatKodeBuku");
    cek(formatIdAnggota("AG-123") && !formatIdAnggota("AG-12a") && !formatIdAnggota("AG 123"), "formatIdAnggota");
    int x = 0;
    cek(parseInt("42", x) && x == 42, "parseInt bilangan positif");
    cek(parseInt(" -7 ", x) && x == -7, "parseInt bilangan negatif dengan spasi");
    cek(!parseInt("", x) && !parseInt("abc", x) && !parseInt("12a", x) && !parseInt("1234567890", x),
        "parseInt menolak masukan salah");
    cek(trim("  halo  ") == "halo" && hurufBesar("bk-1") == "BK-1", "trim dan hurufBesar");
}

int main() {
    ujiBukuTambahDanDuplikat();
    ujiBukuPembesaranBerulang();
    ujiBukuValidasi();
    ujiBukuCariUbahHapus();
    ujiAnggota();
    ujiTransaksiDanMemori();
    ujiValidatorFormat();

    cout << "\n" << lulus << " lulus, " << gagal << " gagal\n";
    return gagal == 0 ? 0 : 1;
}
