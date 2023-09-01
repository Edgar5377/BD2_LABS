#include <iostream>
#include <fstream>
#include <map>

using namespace std;

struct Record
{
    char nombre[30];
    char carrera[20];
    int ciclo;
    int nextDel=-2;
    void setData() {
        cout << "Alumno:";
        cin >> nombre;
        cout << "Carrera: ";
        cin >> carrera;
        cout << "Ciclo: ";
        cin >> ciclo;
    }
    void showData() {
        cout << "\nNombre: " << nombre;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo;
    }
    string getKey(){
        return nombre;
    }
};

class RandomFile {
private:
    string fileName;
    string indexName;
    map<string, long> index;

public:
    RandomFile(string _fileName) {
        this->fileName = _fileName;
        this->indexName = _fileName + ".idx";
        readIndex();
    }

    ~RandomFile() {
        writeIndex();
    }

    void readIndex() {
        ifstream indexFile(indexName, ios::binary);
        if (indexFile.is_open()) {
            while (!indexFile.eof()) {
                string key;
                long position;
                indexFile.read(reinterpret_cast<char*>(&position), sizeof(long));
                getline(indexFile, key);
                if (!key.empty()) {
                    index[key] = position;
                }
            }
            indexFile.close();
        }
    }

    void writeIndex() {
        ofstream indexFile(indexName, ios::binary);
        for (const auto& entry : index) {
            indexFile.write(reinterpret_cast<const char*>(&entry.second), sizeof(long));
            indexFile << entry.first << endl;
        }
        indexFile.close();
    }

    void write_record(Record record) {
        ofstream dataFile(fileName, ios::binary | ios::app);
        long position = dataFile.tellp();
        dataFile.write(reinterpret_cast<const char*>(&record), sizeof(Record));
        index[record.getKey()] = position;
        dataFile.close();
    }

    Record* search(string key) {
        if (index.find(key) != index.end()) {
            ifstream dataFile(fileName, ios::binary);
            dataFile.seekg(index[key]);
            Record* foundRecord = new Record();
            dataFile.read(reinterpret_cast<char*>(foundRecord), sizeof(Record));
            dataFile.close();
            return foundRecord;
        }
        return nullptr;
    }

    void scanAll() {
        ifstream dataFile(fileName, ios::binary);
        Record record;
        while (dataFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
            record.showData();
        }
        dataFile.close();
    }

    void scanAllByIndex() {
        for (const auto& entry : index) {
            ifstream dataFile(fileName, ios::binary);
            dataFile.seekg(entry.second);
            Record record;
            dataFile.read(reinterpret_cast<char*>(&record), sizeof(Record));
            record.showData();
            dataFile.close();
        }
    }
};

void writeFile(string filename) {
    RandomFile file(filename);
    Record record;
    for (int i = 0; i < 4; i++) {
        record.setData();
        file.write_record(record);
    }
}

void readFile(string filename) {
    RandomFile file(filename);
    cout << "--------- show all data -----------\n";
    file.scanAll();
    cout << "--------- show all sorted data -----------\n";
    file.scanAllByIndex();
}

int main() {
    writeFile("data.dat");
    readFile("data.dat");
    return 0;
}
