#include <iostream>
#include <string>
using namespace std;

struct Node {
    string teks;
    Node* next;
};

class Stack {
    Node* top;
public:
    Stack() : top(nullptr) {}
    void push(string t) { top = new Node{t, top}; }
    void pop() { if (top) { Node* h=top; top=top->next; delete h; } }
    string peek() { return top ? top->teks : ""; }
    bool empty() { return !top; }
};

int main() {
    Stack undo, redo;
    int pilih;
    string teks;

    do {
        cout << "\n1. Ketik teks\n2. Undo\n3. Redo\n4. Lihat teks terakhir\n5. Keluar\nPilih: ";
        cin >> pilih; cin.ignore();
        switch (pilih) {
            case 1:
                cout << "Masukkan kata: "; getline(cin, teks);
                undo.push(teks);
                cout << teks << " ditambahkan ke dokumen.\n";
                break;
            case 2:
                if (undo.empty()) cout << "Tidak ada yang bisa di-undo!\n";
                else {
                    teks = undo.peek(); undo.pop(); redo.push(teks);
                    cout << "Undo: " << teks << " dihapus.\n";
                }
                break;
            case 3:
                if (redo.empty()) cout << "Tidak ada yang bisa di-redo!\n";
                else {
                    teks = redo.peek(); redo.pop(); undo.push(teks);
                    cout << "Redo: " << teks << " dikembalikan.\n";
                }
                break;
            case 4:
                cout << "Teks terakhir: " << (undo.empty() ? "(kosong)" : undo.peek()) << endl;
        }
    } while (pilih != 5);
}
