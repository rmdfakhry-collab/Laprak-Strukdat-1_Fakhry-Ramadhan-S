#include <iostream>
using namespace std;

int main() {
    int n, arr[100];
    int pilihan;

    cout << "Masukkan jumlah orang: ";
    cin >> n;

    cout << "Masukkan berat badan:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];

    cout << "\nPilih metode pengurutan:\n";
    cout << "1. Ascending (kecil -> besar)\n";
    cout << "2. Descending (besar -> kecil)\n";
    cout << "Pilihan: ";
    cin >> pilihan;

    // Proses Bubble Sort
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if ((pilihan == 1 && arr[j] > arr[j + 1]) || 
                (pilihan == 2 && arr[j] < arr[j + 1])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }

    // Output array setelah diurutkan
    cout << "\nBerat badan setelah diurutkan:\n";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;

    return 0;
}