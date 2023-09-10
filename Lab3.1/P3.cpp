#include <iostream>
#include <list>
#include<vector>
using namespace std;

const int TABLE_SIZE = 6;  // Tamaño de la tabla hash
const int PAGE_SIZE = 5;    // Tamaño de página (número de elementos por página)

class bucket{
public:
    int position;
    vector<int> numbers;
    bucket* punt = nullptr;
    int estado = -1;//-1: con_espacio //-2: lleno

    bucket(int position) : position(position), punt(nullptr), estado(-1){}
};

class HashTable {
private:
    vector<bucket*> table;
    vector<bucket*> overflow;

public:
    HashTable() : table(TABLE_SIZE, nullptr) {
        for(int i=0;i<TABLE_SIZE;i++)
            table[i] = new bucket(i);
    }

    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }
    //AGORITMO DE INSERCIÓN
    void insertRecursive(bucket* record, int value) {
        if (record->estado == -1) {
            record->numbers.push_back(value);
            if (record->numbers.size() >= PAGE_SIZE) {
                record->estado = -2;
            }
        } else if (record->estado == -2) {
            if (record->punt == nullptr) {
                record->punt = new bucket(PAGE_SIZE + overflow.size() + 1);
                overflow.push_back(record->punt);
                record->punt->estado = -1;
            }
            insertRecursive(record->punt, value);
        }
    }

    void insert(int key, int value) {
        int index = hashFunction(key);
        bucket* record = table[index];
        insertRecursive(record, value);
    }

    //ALGORITMO DE BUSQUEDA
    int searchRecursive(bucket* record, int value) {
        for (int num : record->numbers) {
            if (num == value) {
                return record->position; // Devuelve la posición (key) del registro
            }
        }

        if (record->estado == -2 && record->punt != nullptr) {
            return searchRecursive(record->punt, value);
        }

        return -1; // Valor no encontrado
    }

    int search(int value) {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            bucket* record = table[i];
            int position = searchRecursive(record, value);
            if (position != -1) {
                return position;
            }
        }

        for (int k = 0; k < overflow.size(); ++k) {
            bucket* record = overflow[k];
            int position = searchRecursive(record, value);
            if (position != -1) {
                return position + TABLE_SIZE; // La posición en el overflow se ajusta
            }
        }

        return -1; // Valor no encontrado
    }

    void printTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            bucket* record = table[i];
            if(record->numbers.size()>0){
                cout << "Bucket " << i << ", Page ID " << record->position << ": ";
                for(int j=0;j<record->numbers.size();j++){
                    cout << record->numbers[j] << " ";
                }
                cout<<endl;
            }
        }
        for (int k = 0; k < overflow.size(); ++k) {
            bucket* record = overflow[k];
            if(record->numbers.size()>0){
                cout << "Bucket_overflow " << k << ", Page ID " << record->position << ": ";
                for(int l=0;l<record->numbers.size();l++){
                    cout << record->numbers[l] << " ";
                }
                cout<<endl;
            }
        }
    }
};
int main() {
    HashTable hashTable;

    hashTable.insert(1, 10);
    hashTable.insert(1, 11);
    hashTable.insert(1, 12);
    hashTable.insert(1, 13);
    hashTable.insert(1, 14);
    hashTable.insert(1, 15);
    hashTable.insert(1, 16);
    hashTable.insert(9, 17);
    hashTable.insert(9, 18);

    cout << "Tabla Hash con desbordamiento encadenado y paginas:" << endl;
    hashTable.printTable();
    cout<<endl;
    int search = 15;
    cout<<"El registro "<< search<< " encuentra en la posicion "<< hashTable.search(search)<<" del hash table." <<endl;

    return 0;
}