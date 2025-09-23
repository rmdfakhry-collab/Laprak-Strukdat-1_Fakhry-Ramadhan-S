#include <iostream>
using namespace std;

#define MAX 20   // kapasitas maksimal antrian

class Antrian {
private:
    int data[MAX];
    int front, rear;

public:
    Antrian() {
        front = -1;
        rear = -1;
    }

    bool isEmpty() {
        return (front == -1 && rear == -1);
    }

    bool isFull() {
        return (rear == MAX - 1);
    }

    void enqueue(int nomor) {
        if (isFull()) {
            cout << "⚠️  Antrian penuh! Pasien nomor " << nomor << " tidak bisa masuk.\n";
            return;
        }
        if (isEmpty()) {
            front = rear = 0;
        } else {
            rear++;
        }
        data[rear] = nomor;
        cout << "✅ Pasien nomor " << nomor << " masuk antrian.\n";
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "⚠️  Antrian kosong! Tidak ada pasien yang dipanggil.\n";
            return;
        }
        cout << "🔔 Memanggil pasien nomor " << data[front] << endl;

        if (front == rear) {
            front = rear = -1;
        } else {
            front++;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "📭 Antrian kosong.\n";
            return;
        }
        cout << "📋 Antrian sekarang: ";
        for (int i = front; i <= rear; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    Antrian rs;
    int pilihan, nomor = 1;

    do {
        cout << "\n=== SISTEM ANTRIAN RUMAH SAKIT ===\n";
        cout << "1. Tambah pasien ke antrian\n";
        cout << "2. Panggil pasien berikutnya\n";
        cout << "3. Lihat antrian\n";
        cout << "4. Keluar\n";
        cout << "Pilih menu (1-4): ";
        cin >> pilihan;

        switch (pilihan) {
        case 1:
            rs.enqueue(nomor);
            nomor++;  // nomor pasien bertambah otomatis
            break;
        case 2:
            rs.dequeue();
            break;
        case 3:
            rs.display();
            break;
        case 4:
            cout << "👋 Terima kasih, program selesai.\n";
            break;
        default:
            cout << "⚠️  Pilihan tidak valid!\n";
        }
    } while (pilihan != 4);

    return 0;
}
