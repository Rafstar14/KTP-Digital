#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
// ============================================================
// Struktur data KTP
// ============================================================
struct KTPData
{
    string nik;
    string nama;
    string tempat_lahir;
    string tanggal_lahir; // hasil decode otomatis dari NIK
    string jenis_kelamin; // hasil decode otomatis dari NIK
    string alamat;
    string rt_rw;
    string kel_desa;
    string kecamatan;
    string agama;
    string status_perkawinan;
    string pekerjaan;
    string kewarganegaraan;
    string berlaku_hingga;
};

const string FILE_NAME = "data_ktp.txt";

void clearAllData(vector<KTPData> &daftarKTP);

// ============================================================
// dasar
// ============================================================
void enableConsoleSupport()
{
#ifdef _WIN32
    // Supaya karakter & warna nya tampil benar di cmd/PowerShell Windows
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hOut, &mode))
    {
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

string trim(const string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos)
        return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool isDigitsOnly(const string &s)
{
    if (s.empty())
        return false;
    for (char c : s)
        if (!isdigit((unsigned char)c))
            return false;
    return true;
}

string readLineTrimmed(const string &label)
{
    string s;
    cout << label;
    getline(cin, s);
    return trim(s);
}

string readRequired(const string &label)
{
    string s;
    do
    {
        s = readLineTrimmed(label);
        if (s.empty())
            cout << "  -> Data tidak boleh kosong, silakan isi ulang.\n";
    } while (s.empty());
    return s;
}

string readNIK()
{
    string nik;
    while (true)
    {
        nik = readLineTrimmed("NIK (16 digit)       : ");
        if (!isDigitsOnly(nik) || nik.length() != 16)
        {
            cout << "  -> NIK harus tepat 16 digit angka. Coba lagi.\n";
            continue;
        }
        break;
    }
    return nik;
}
// ============================================================
// Decode NIK -> tanggal lahir & jenis kelamin
// Format resmi NIK: [kode wilayah 6 digit][DD][MM][YY][nomor urut 4 digit]
// Untuk perempuan, digit tanggal (DD) ditambah 40.
// ============================================================
void decodeNIK(const string &nik, string &tanggalLahir, string &jenisKelamin)
{
    int tgl = stoi(nik.substr(6, 2));
    int bln = stoi(nik.substr(8, 2));
    int thn2 = stoi(nik.substr(10, 2));

    if (tgl > 40)
    {
        jenisKelamin = "PEREMPUAN";
        tgl -= 40;
    }
    else
    {
        jenisKelamin = "LAKI-LAKI";
    }

    if (tgl < 1 || tgl > 31 || bln < 1 || bln > 12)
    {
        tanggalLahir = "(format NIK tidak dikenali)";
        jenisKelamin = "(format NIK tidak dikenali)";
        return;
    }

    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    int currentYear2Digit = (ltm->tm_year + 1900) % 100;
    // Heuristik abad: kalau dua digit tahun lebih besar dari tahun sekarang,
    // asumsikan kelahiran abad lalu (19xx), bukan masa depan (20xx).
    int fullYear = (thn2 <= currentYear2Digit) ? (2000 + thn2) : (1900 + thn2);

    ostringstream oss;
    oss << setfill('0') << setw(2) << tgl << "-"
        << setfill('0') << setw(2) << bln << "-"
        << fullYear;
    tanggalLahir = oss.str();
}

// ============================================================
// Input data satu KTP
// ============================================================
KTPData inputKTPData()
{
    KTPData d;

    cout << "\n============================================\n";
    cout << "          INPUT DATA KTP DIGITAL            \n";
    cout << "============================================\n\n";

    d.nik = readNIK();
    decodeNIK(d.nik, d.tanggal_lahir, d.jenis_kelamin);
    cout << "  -> Terbaca otomatis dari NIK: " << d.tanggal_lahir
         << " / " << d.jenis_kelamin << "\n\n";

    d.nama = readRequired("Nama                 : ");
    d.tempat_lahir = readRequired("Tempat Lahir         : ");
    d.alamat = readRequired("Alamat               : ");
    d.rt_rw = readLineTrimmed("RT/RW                : ");
    d.kel_desa = readLineTrimmed("Kel/Desa             : ");
    d.kecamatan = readLineTrimmed("Kecamatan            : ");
    d.agama = readLineTrimmed("Agama                : ");
    d.status_perkawinan = readLineTrimmed("Status Perkawinan    : ");
    d.pekerjaan = readLineTrimmed("Pekerjaan            : ");

    d.kewarganegaraan = readLineTrimmed("Kewarganegaraan:     ");
    if (d.kewarganegaraan.empty())
        d.kewarganegaraan = "WNI";

    d.berlaku_hingga = readLineTrimmed("Berlaku Hingga:        ");
    if (d.berlaku_hingga.empty())
        d.berlaku_hingga = "SEUMUR HIDUP";

    return d;
}
// ============================================================
// Tampilan kartu KTP
// ============================================================
void displayKTP(const KTPData &d)
{
    cout << "\n";
    cout << "+---------------------------------------------------+\n";
    cout << "|               PROVINSI DKI JAKARTA                 |\n";
    cout << "|                   JAKARTA TIMUR                    |\n";
    cout << "+---------------------------------------------------+\n";

    cout << left;
    cout << setw(23) << "NIK" << ": " << d.nik << "\n";
    cout << setw(23) << "Nama" << ": " << d.nama << "\n";
    cout << setw(23) << "Tempat/Tgl Lahir" << ": " << d.tempat_lahir << ", " << d.tanggal_lahir << "\n";
    cout << setw(23) << "Jenis Kelamin" << ": " << d.jenis_kelamin << "\n";
    cout << setw(23) << "Alamat" << ": " << d.alamat << "\n";
    cout << "    " << setw(19) << "RT/RW" << ": " << d.rt_rw << "\n";
    cout << "    " << setw(19) << "Kel/Desa" << ": " << d.kel_desa << "\n";
    cout << "    " << setw(19) << "Kecamatan" << ": " << d.kecamatan << "\n";
    cout << setw(23) << "Agama" << ": " << d.agama << "\n";
    cout << setw(23) << "Status Perkawinan" << ": " << d.status_perkawinan << "\n";
    cout << setw(23) << "Pekerjaan" << ": " << d.pekerjaan << "\n";
    cout << setw(23) << "Kewarganegaraan" << ": " << d.kewarganegaraan << "\n";
    cout << setw(23) << "Berlaku Hingga" << ": " << d.berlaku_hingga << "\n";

    cout << "+---------------------------------------------------+\n";
    cout << "|             DATA IDENTITAS DIGITAL                 |\n";
    cout << "+---------------------------------------------------+\n";
}

// ============================================================
// Simpan / muat dari file, supaya data tidak hilang tiap program ditutup
// ============================================================
void saveAllToFile(const vector<KTPData> &list)
{
    ofstream fout(FILE_NAME);
    if (!fout)
    {
        cout << "Gagal membuka file untuk menyimpan.\n";
        return;
    }
    for (const auto &d : list)
    {
        fout << d.nik << "|" << d.nama << "|" << d.tempat_lahir << "|" << d.tanggal_lahir << "|"
             << d.jenis_kelamin << "|" << d.alamat << "|" << d.rt_rw << "|" << d.kel_desa << "|"
             << d.kecamatan << "|" << d.agama << "|" << d.status_perkawinan << "|" << d.pekerjaan << "|"
             << d.kewarganegaraan << "|" << d.berlaku_hingga << "\n";
    }
    cout << "Data tersimpan ke " << FILE_NAME << " (" << list.size() << " entri).\n";
}

vector<string> splitPipe(const string &line)
{
    vector<string> parts;
    stringstream ss(line);
    string token;
    while (getline(ss, token, '|'))
        parts.push_back(token);
    return parts;
}

vector<KTPData> loadAllFromFile()
{
    vector<KTPData> list;
    ifstream fin(FILE_NAME);
    if (!fin)
        return list;

    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;
        vector<string> p = splitPipe(line);
        if (p.size() < 14)
            continue;
        KTPData d;
        d.nik = p[0];
        d.nama = p[1];
        d.tempat_lahir = p[2];
        d.tanggal_lahir = p[3];
        d.jenis_kelamin = p[4];
        d.alamat = p[5];
        d.rt_rw = p[6];
        d.kel_desa = p[7];
        d.kecamatan = p[8];
        d.agama = p[9];
        d.status_perkawinan = p[10];
        d.pekerjaan = p[11];
        d.kewarganegaraan = p[12];
        d.berlaku_hingga = p[13];
        list.push_back(d);
    }
    return list;
}
// ============================================================
// Menu
// ============================================================
int readMenuChoice()
{
    string s;
    getline(cin, s);
    s = trim(s);
    try
    {
        return stoi(s);
    }
    catch (...)
    {
        return -1;
    }
}

int main()
{
    enableConsoleSupport();

    vector<KTPData> daftarKTP = loadAllFromFile();
    if (!daftarKTP.empty())
    {
        cout << "(" << daftarKTP.size() << " data KTP dimuat dari " << FILE_NAME << ")\n";
    }

    bool running = true;
    while (running)
    {
        cout << "\n===================== MENU =====================\n";
        cout << " 1. Input data KTP baru\n";
        cout << " 2. Tampilkan semua data KTP\n";
        cout << " 3. Cari data KTP berdasarkan NIK\n";
        cout << " 4. Keluar\n";
        cout << " 5. Hapus semua data KTP\n";
        cout << "=================================================\n";
        cout << "Pilihan: ";

        int pilihan = readMenuChoice();

        switch (pilihan)
        {
        case 1:
        {
            KTPData baru = inputKTPData();
            daftarKTP.push_back(baru);
            saveAllToFile(daftarKTP);
            displayKTP(baru);
            break;
        }
        case 2:
        {
            if (daftarKTP.empty())
            {
                cout << "\nBelum ada data. Silakan input data terlebih dahulu.\n";
            }
            else
            {
                for (const auto &d : daftarKTP)
                    displayKTP(d);
            }
            break;
        }
        case 3:
        {
            string cari = readLineTrimmed("Masukkan NIK yang dicari: ");
            bool found = false;
            for (const auto &d : daftarKTP)
            {
                if (d.nik == cari)
                {
                    displayKTP(d);
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << "\nData dengan NIK tersebut tidak ditemukan.\n";
            break;
        }
        case 4:
            running = false;
            cout << "\nTerima kasih, program ditutup.\n";
            break;

        case 5:
            clearAllData(daftarKTP);
            break;

        default:
            cout << "\nPilihan tidak valid, coba lagi.\n";
        }
    }

    return 0;
}

void clearAllData(vector<KTPData> &daftarKTP)
{
    daftarKTP.clear();

    ofstream fout(FILE_NAME, ios::trunc);
    if (!fout)
    {
        cout << "Gagal menghapus data KTP.\n";
        return;
    }
    fout.close();
    cout << "Semua data KTP berhasil dihapus.\n";
}