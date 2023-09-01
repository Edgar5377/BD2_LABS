#include <iostream>
<<<<<<< HEAD
#include <fstream>
#include <map>

=======
#include<fstream>
#include<cstdio>
#include <map>
#include <string>
#include <cstring>
//#include <c
>>>>>>> 9c38137ee88c70a3c3444cf898f63523c54a7acb
using namespace std;

struct Record
{
<<<<<<< HEAD
    char nombre[30];
    char carrera[20];
    int ciclo;
    int nextDel=-2;
=======
    char nombre[12];
    char carrera[15];
    int ciclo;

>>>>>>> 9c38137ee88c70a3c3444cf898f63523c54a7acb
    void setData() {
        cout << "Alumno:";
        cin >> nombre;
        cout << "Carrera: ";
        cin >> carrera;
        cout << "Ciclo: ";
        cin >> ciclo;
    }
<<<<<<< HEAD
=======

>>>>>>> 9c38137ee88c70a3c3444cf898f63523c54a7acb
    void showData() {
        cout << "\nNombre: " << nombre;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo;
    }
<<<<<<< HEAD
=======

    void showData_line(){
        cout << nombre << " " << carrera << " " << ciclo << endl;
    }

>>>>>>> 9c38137ee88c70a3c3444cf898f63523c54a7acb
    string getKey(){
        return nombre;
    }
};
<<<<<<< HEAD
=======

struct RandomIndex{
    char key[12];
    int address;
};

class RandomFile {
public:
    string fileName;
    string indexName;
    //map: mantiene ordenado las entradas
    map<string, long> index;

public:
    RandomFile(string _fileName) {
        this->fileName = _fileName;
        this->indexName = "indexfile.bin";
        readIndex();
    }

    ~RandomFile(){
        writeIndex();
    }

    /*
    * leer el indice desde disco
    */
    void readIndex(){
        ifstream file(indexName, ios::binary | ios::in );  // Abre el archivo en modo lectura/escritura
        file.seekg(0,ios::beg); // ubica al inicio del archivo

//        string key_value = "";
//        int value_value = 0 ;
        RandomIndex index_read;
//        while(!file.eof()){
//            file.read(reinterpret_cast<char *>(&key_value), sizeof(string));
//            file.read(reinterpret_cast<char *>(&value_value), sizeof(int));
//            cout << key_value << " " << value_value << endl;
        while (file.read(reinterpret_cast<char *>(&index_read), sizeof(RandomIndex))){
            index.insert(std::make_pair((string)(index_read.key), index_read.address));
        }
//        }

//        while(file.read(reinterpret_cast<char *>(&key[12],&value), sizeof(char[12])+sizeof(int))){
//            index.insert(std::make_pair(key,value));
//        }
        file.close();
    }

    /*
    * Regresa el indice al disco
    */
    void writeIndex(){

//        for (const auto& par : index) {
//            std::cout << "Clave: " << par.first << ", Valor: " << par.second << std::endl;
//        }

        RandomIndex index_write;
        ofstream file(indexName, ios::binary | ios::app);
//        const char* cString = nullptr;
//        for (const auto& par : index) {
//            std::cout << "Clave: " << par.first << ", Valor: " << par.second << std::endl;
//            file.write(reinterpret_cast<const char *>(&par.first), sizeof(string));
//            file.write(reinterpret_cast<const char *>(&par.second), sizeof(int));
//        }

        for (auto it = index.begin(); it != index.end(); ++it){
            strcpy(index_write.key, it->first.c_str());
            index_write.address = it->second;
//            cout << index_write.key << endl;
            file.write(reinterpret_cast<const char *>(&index_write), sizeof(RandomIndex));
        }

        file.close();
    }

    /*
    * Escribe el registro al final del archivo de datos. Se actualiza el indice.
    */
    void write_record(Record record) {
        ofstream file(fileName, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura

        if(!file){
            file.close();
            std::ofstream outfile(fileName, std::ios::binary | std::ios::app);
            int position = outfile.tellp();
            outfile.write(reinterpret_cast<const char*>(&record), sizeof(Record));
//            cout <<"position : " <<position << endl;
//            cout <<"nombre : " <<record.nombre << endl;
            index.insert(std::make_pair(record.nombre, position));
            outfile.close();
        } else{
            file.seekp(0, std::ios::end); // ubica al final del archivo
            int position = file.tellp();
            file.write(reinterpret_cast<char*>(&record), sizeof(Record));
//            cout <<"position : " <<position << endl;
//            cout <<"nombre : " <<record.nombre << endl;
            index.insert(std::make_pair(record.nombre, position));
        }
        file.close();
    }


    /*
    * Busca un registro que coincida con la key
    */
    Record* search(string key) {
        Record* result_ptr = nullptr;
        Record result{};

        auto encontrado = index.find(key);

        if (encontrado == index.end()) {
            std::cout << "Key no encontrada." << std::endl;
            return result_ptr; // Return nullptr if key is not found
        }

        int position = encontrado->second;

        ifstream file(fileName, ios::binary | ios::in);  // Abre el archivo en modo lectura
        file.seekg(position, std::ios::beg);
        file.read(reinterpret_cast<char*>(&result), sizeof(Record));
//        cout <<"Si lo leyo " <<result.nombre  << endl;
        result_ptr = new Record(result);
        file.close();

        return result_ptr;

    }



    void print_data_index(){
        ifstream indexFile(indexName, ios::binary | ios::in);  // Abre el archivo en modo lectura
        RandomIndex index;
        indexFile.seekg(0, std::ios::beg); // Ubicar el puntero de lectura al inicio del archivo
//        std::cout << "YOU CALL ME! "<< endl;
//        std::cout << indexName<< endl;

        while (indexFile.read(reinterpret_cast<char *>(&index), sizeof(RandomIndex))) {
//            cout << "aaaa";
            std::cout << "Clave: " << index.key << ", Valor: " << index.address << std::endl;
        }
        indexFile.close();
    }

};

void writeFile(string filename){
    RandomFile file(filename);
    Record record;
    for (int i = 0; i < 2; i++){
        record.setData();
        file.write_record(record);
    }
}



int main(){
>>>>>>> 9c38137ee88c70a3c3444cf898f63523c54a7acb

class RandomFile {
private:
    string fileName;
    string indexName;
    map<string, long> index;

<<<<<<< HEAD
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
=======
//    writeFile("data.dat");
    RandomFile file1("data4.bin");

    Record Alumno1{"Edgar", "CS", 5 };
    Record alumno2{"Maria", "IngIndustrial", 20};
    Record alumno3{"Carlos", "Matematica", 24};
////

//    file1.write_record(Alumno1);
//    file1.write_record(alumno2);
//    file1.write_record(alumno3);
//
    Record* puntero = file1.search("Edgar");
    cout << "Nombre del puntero: "<< puntero->nombre << endl;

//    file1.print_data_index();
//    file1.readIndex();
//    readFile("index.dat");
>>>>>>> 9c38137ee88c70a3c3444cf898f63523c54a7acb
    return 0;


}


