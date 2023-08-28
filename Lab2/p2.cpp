#include <iostream>
#include<fstream>
#include<cstdio>
#include <map>
using namespace std;

struct Record
{
    char nombre[12];
    char carrera[15];
    int ciclo;

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

    void showData_line(){
        cout << nombre << " " << carrera << " " << ciclo << endl;
    }

    string getKey(){
        return nombre;
    }
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
        this->indexName = _fileName + "_ind";
        readIndex();
    }

    ~RandomFile(){
        //writeIndex();
    }

    /*
    * leer el indice desde disco
    */
    void readIndex(){
        ifstream file(indexName, ios::binary | ios::in );  // Abre el archivo en modo lectura/escritura
        file.seekg(0,ios::beg); // ubica al inicio del archivo

        char key_value[12] = "";
        int value_value = 0 ;


        while(!file.eof()){
            file.read(reinterpret_cast<char *>(&key_value), sizeof(char[12]));
            file.read(reinterpret_cast<char *>(&value_value), sizeof(int));
            cout << key_value << " " << value_value << endl;
        }
//        while (file.read(reinterpret_cast<char *>(&key_value), sizeof(char[12])) &&
//               file.read(reinterpret_cast<char *>(&value_value), sizeof(int))) {
//            cout<<key_value<<endl;
//            index.insert(std::make_pair(key_value, value_value));
//        }

//        while(file.read(reinterpret_cast<char *>(&key[12],&value), sizeof(char[12])+sizeof(int))){
//            index.insert(std::make_pair(key,value));
//        }
    }

    /*
    * Regresa el indice al disco
    */
    void writeIndex(){
        ofstream file(indexName, ios::binary);
        for (auto it = index.begin(); it != index.end(); ++it) {
            file.write(reinterpret_cast<const char *>(&it->first), sizeof(it->first));
            file.write(reinterpret_cast<const char *>(&it->second), sizeof(it->second));
        }
    }

    /*
    * Escribe el registro al final del archivo de datos. Se actualiza el indice.
    */
    void write_record(Record record) {
        ofstream file(fileName, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura

        if(!file){
            file.close();
            std::ofstream outfile(fileName, std::ios::binary | std::ios::app);  // Abre el archivo en modo adjunto
            outfile.write(reinterpret_cast<const char*>(&record), sizeof(Record));  // Escribe la metadata en el inicio
            int position = outfile.tellp();
            cout <<"position : " <<position << endl;
            cout <<"nombre : " <<record.nombre << endl;
            index.insert(std::make_pair(record.nombre, position));
            outfile.close();
        } else{
            file.seekp(0, std::ios::end); // ubica al final del archivo
            file.write(reinterpret_cast<char*>(&record), sizeof(Record));
            int position = file.tellp();
            index.insert(std::make_pair(record.nombre, position));
        }
        file.close();
    }


    /*
    * Busca un registro que coincida con la key
    */
    Record* search(string key) {
        Record* result = nullptr;

        auto encontrado = index.find(key);
        long position = encontrado->second;

        if (encontrado == index.end()) {
            std::cout << "Key no encontrada." << std::endl;
            throw ("Key no encontrada.");
        }

        ifstream file(fileName, ios::binary | ios::in);  // Abre el archivo en modo lectura
        file.seekg(position, std::ios::beg);
        file.read(reinterpret_cast<char*>(&result), sizeof(Record));// Lee la metadata y la almacena en el objeto.
        file.close();

        return result;
    }

    /*
   * Muestra todos los registros de acuerdo como fueron insertados en el archivo de datos
   */
    void scanAll() {

    }

    /*
   * Muestra todos los registros de acuerdo a como estan ordenados en el indice
   */
    void scanAllByIndex() {

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

void readFile(string filename){
    RandomFile file(filename);
    cout<<"--------- show all data -----------\n";
    file.scanAll();
    cout<<"--------- show all sorted data -----------\n";
    file.scanAllByIndex();
}

int main(){


//    writeFile("data.dat");
    RandomFile file1("data4.bin");

    Record Alumno1{"Edgar", "CS", 5 };
    Record alumno2{"María", "Math", 20};
    Record alumno3{"Carlos", "Physics", 24};
//
    file1.write_record(Alumno1);
    file1.write_record(alumno2);
    file1.write_record(alumno3);

//    file1.readIndex();
//    readFile("index.dat");
    return 0;


}


