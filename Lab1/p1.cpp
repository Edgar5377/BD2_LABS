//
// Created by david on 19/08/2023.
//
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>


using namespace std;

struct Alumno{
    char codigo[5];
    char nombre[12];
    char apellidos[12];
    char carrera[15];
    int ciclo;
    float mensualidad;
    int nextDel=0;

    void setData(){
        cout<<"Codigo: ";cin>>codigo;
        cout<<"Nombre: ";cin>>nombre;
        cout<<"Apellidos: ";cin>>apellidos;
        cout<<"Carrera: ";cin>>carrera;
        cout<<"Ciclo: ";cin>>ciclo;
        cout<<"Mensualidad: "; cin>> mensualidad;
    }
    void showData(){
        cout<<"Codigo: "<<codigo<<endl;
        cout<<"Nombre: "<<nombre<<endl;
        cout<<"Apellidos: "<<apellidos<<endl;
        cout<<"Carrera: "<<carrera<<endl;
        cout<<"Ciclo: "<<ciclo<<endl;
        cout<<"Mensualidad: "<<mensualidad<<endl;
    }
    int get_nextDel(){return nextDel;}

};

class FixedRecordFile{
private:
    string filename;

public:
    explicit FixedRecordFile(string filename) : filename(std::move(filename)) {}

    vector<Alumno> load(){
        vector<Alumno> alumnos;
        Alumno record;

        ifstream file(filename, ios::binary);//abrir archivo
        if(!file.is_open()) throw ("No se pudo abrir el archivo");

        while(file.peek() != EOF){//realiza una revisión previa del siguiente registro
            record = Alumno();
            file.read(reinterpret_cast<char *>(&record), sizeof(Alumno));
            alumnos.push_back(record);
        }

        file.close();
        return alumnos;
    }

    void add(const Alumno &record) {

    }


    Alumno readRecord(int pos)
    {
        if(pos > size()-1) throw ("Posicion no valida");

        ifstream file(filename, ios::binary);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");
        file.seekg(pos * sizeof(Alumno) + sizeof(int), ios::beg);//me ubico en la posicion a leer
        Alumno record;
        file.read(reinterpret_cast<char *>(&record), sizeof(Alumno));
        file.close();
        return record;
    }

    bool deleteRecord(int pos){
        fstream file(this->filename, ios::binary | ios::in | ios::out);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");
        file.seekg(pos * sizeof(Alumno) + sizeof(int), ios::beg);//me ubico en la posicion a leer
        Alumno record;
        file.read(reinterpret_cast<char *>(&record), sizeof(Alumno));
        if(record.get_nextDel() == -1) return false;
        record.nextDel = -1;
        file.seekp(pos * sizeof(Alumno) + sizeof(int), ios::beg);//me ubico en la posicion a leer
        file.write(reinterpret_cast<const char *>(&record), sizeof(Alumno));
        file.close();
        return true;
    }

    int size(){
        ifstream file(this->filename, ios::binary);
        if(!file.is_open()) throw (std::out_of_range("No se pudo abrir el archivo"));
        file.seekg(0, ios::end);//ubicar cursos al final del archivo
        long total_bytes = file.tellg();//cantidad de bytes del archivo
        file.close();
        return total_bytes / sizeof(Alumno);
    }

    void add_prueba(const Alumno &record) {
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura
        int metadata;

        if(!file){
            file.close();

            std::ofstream outfile(filename, std::ios::binary | std::ios::app);  // Abre el archivo en modo adjunto
            metadata = -1; // metadata = - 1
            outfile.write(reinterpret_cast<const char*>(&metadata), sizeof(int));  // Escribe la metadata en el inicio
            outfile.close();

            file.open(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura
        } else{
            file.seekg(0, std::ios::beg); // Ubicar el puntero de lectura al inicio del archivo
            file.read(reinterpret_cast<char*>(&metadata), sizeof(int));
        }

        if (metadata == -1){
            file.seekp(0, std::ios::end); // Moverse al final del archivo
            file.write(reinterpret_cast<const char*>(&record), sizeof(Alumno));  // Escribe el nuevo registro al final del archivo
        }else {
            file.seekg(static_cast<std::streamoff>(metadata)  * sizeof(Alumno) + sizeof(int));  // Se ubica en la posición del registro eliminado
            file.seekp(static_cast<std::streamoff>(metadata) * sizeof(Alumno) + sizeof(int));  // Se ubica en la posición del registro eliminado
            Alumno tempAlumno;
            file.read(reinterpret_cast<char*>(&tempAlumno), sizeof(Alumno));  // Lee el registro eliminado
            metadata = tempAlumno.nextDel;  // Actualiza el valor de la metadata

            file.write(reinterpret_cast<const char*>(&record), sizeof(Alumno));  // Escribe el nuevo registro de Alumno
            file.seekg(0, std::ios::beg); // Ubicar el puntero de lectura al inicio del archivo
            file.write(reinterpret_cast<const char*>(&metadata), sizeof(int));  // Escribe el nuevo de la metada
        }


        file.close();
    }

//    void prueba_m1(){
//        fstream file(filename, std::ios::binary | std::ios::app);  // Abre el archivo en modo adjunto
//        int metadata = -1; // metadata = - 1
//        file.write(reinterpret_cast<const char*>(&metadata), sizeof(int));  // Escribe la metadata en el inicio
//        file.close();
//    }
//    void lectura_m1(){
//        fstream file(filename, std::ios::binary | std::ios::app);  // Abre el archivo en modo adjunto
//        int metadata;
//        file.seekg(0, std::ios::beg); // Ubicar el puntero de lectura al inicio del archivo
//        file.read(reinterpret_cast<char*>(&metadata), sizeof(int));
//        cout << metadata;
//        file.close();
//    }

    void prueba_m1() {
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura
        int metadata = -15; // metadata = -1
        file.write(reinterpret_cast<const char*>(&metadata), sizeof(int));  // Escribe la metadata en el inicio
        file.close();
    }

    void lectura_m1() {
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura
        int metadata;
        file.seekg(0, std::ios::beg); // Ubicar el puntero de lectura al inicio del archivo
        file.read(reinterpret_cast<char*>(&metadata), sizeof(int));
        cout << metadata;
        file.close();
    }


};

int main()
{
//Escritura
    FixedRecordFile file1("data.bin");
    Alumno Alumno1{"1234", "Edgar", "Chambilla", "CS", 5, 1250.0, 0};
    Alumno Alumno2{"5678", "María", "López", "Arquitectura", 3, 1500.0, 0};
    Alumno Alumno3{"9999", "Carlos", "Gómez", "Medicina", 2, 1800.0, 0};
    Alumno Alumno4{"9999", "Luis", "Torres", "CS", 6, 1500.0, 0};

    // file1.add_prueba(Alumno1);
    // file1.add_prueba(Alumno2);
    // file1.add_prueba(Alumno3);
    // file1.add_prueba(Alumno4);

    Alumno alumno = file1.readRecord(0);
    alumno.showData();

    alumno = file1.readRecord(1);
    alumno.showData();

    alumno = file1.readRecord(2);
    alumno.showData();


    alumno = file1.readRecord(3);
    alumno.showData();



    return 0;
}

