#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
};

class BST {
private:
    Node* root;
    
    Node* insert(Node* node, int nilai) {
        if (node == nullptr) {
            Node* newNode = new Node();
            newNode->data = nilai;
            newNode->left = nullptr;
            newNode->right = nullptr;
            return newNode;
        }
        
        if (nilai < node->data) {
            node->left = insert(node->left, nilai);
        } else if (nilai > node->data) {
            node->right = insert(node->right, nilai);
        }
        return node;
    }
    
    bool search(Node* node, int nilai) {
        if (node == nullptr) {
            return false;
        }
        
        if (nilai == node->data) {
            return true;
        } else if (nilai < node->data) {
            return search(node->left, nilai);
        } else {
            return search(node->right, nilai);
        }
    }
    
    void inorder(Node* node) {
        if (node == nullptr) return;
        
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }
    
public:
    BST() {
        root = nullptr;
    }
    
    void insert(int nilai) {
        root = insert(root, nilai);
    }
    
    bool search(int nilai) {
        return search(root, nilai);
    }
    
    void tampilkan() {
        if (root == nullptr) {
            cout << "BST kosong!\n";
        } else {
            cout << "Isi BST: ";
            inorder(root);
            cout << "\n";
        }
    }
};

int main() {
    BST tree;
    int pilihan, nilai;
    
    cout << "===== PROGRAM BINARY SEARCH TREE =====\n\n";
    
    do {
        cout << "MENU:\n";
        cout << "1. Tambah Data\n";
        cout << "2. Cari Data\n";
        cout << "3. Tampilkan Semua Data\n";
        cout << "4. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        
        switch (pilihan) {
            case 1:
                cout << "Masukkan data: ";
                cin >> nilai;
                tree.insert(nilai);
                cout << "Data " << nilai << " berhasil ditambahkan!\n\n";
                break;
                
            case 2:
                cout << "Masukkan data yang dicari: ";
                cin >> nilai;
                if (tree.search(nilai)) {
                    cout << "Data " << nilai << " DITEMUKAN!\n\n";
                } else {
                    cout << "Data " << nilai << " TIDAK DITEMUKAN!\n\n";
                }
                break;
                
            case 3:
                tree.tampilkan();
                cout << endl;
                break;
                
            case 4:
                cout << "Program selesai.\n";
                break;
                
            default:
                cout << "Pilihan tidak valid!\n\n";
        }
    } while (pilihan != 4);
    
    return 0;
}