#include <iostream>
#include <string>
using namespace std;

int main() {
    const int n = 6;
    string rakBuku[n] = {"Matematika", "Fisika", "Biologi", "Kimia", "Sejarah", "Geografi"};
    string target;
    int i = 0;
    bool ketemu = false;

    cout << "Masukkan judul buku yang dicari: ";
    getline(cin, target);  // langsung gunakan getline, tidak perlu cin.ignore() jika ini input pertama

    while (i < n) {
        if (rakBuku[i] == target) {
            ketemu = true;
            break;
        }
        i++;
    }

    if (ketemu) {
        cout << "Buku ditemukan di posisi ke-" << i + 1 << endl;
    } else {
        cout << "Buku tidak ditemukan" << endl;
    }

    return 0;
}