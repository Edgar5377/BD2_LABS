//
// Created by david on 19/08/2023.
//
#include <iostream>
#include <fstream>
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
        cout<<"Codigo";cin>>codigo;
        cout<<"Nombre";cin>>nombre;
        cout<<"Apellidos";cin>>apellidos;
        cout<<"Carrera";cin>>carrera;
        cout<<"Ciclo";cin>>ciclo;
        cout<<"Mensualidad"; cin>> mensualidad;

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
    FixedRecordFile(string filename) : filename(filename) {}

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

//    void add(const Alumno &record){
//        fstream file(filename, std::ios::binary | std::ios::app);
//        //ifstream file(this->filename, ios::binary);
//        if (file){
//            file.seekp(0, std::ios::beg); // se ubica en el inicio del doc
//            file.seekg(0, std::ios::beg);
//            int value;                    // declaración de variable value
//            file.read((char*) &value, sizeof(int)); // se guarda el valor de metadata a value
//
//            if(value == -1){    // en caso no se tenga valores eliminados
//                file.write(reinterpret_cast<const char *>(&record), sizeof(Alumno));
//                file.close();
//            } else {
//                file.seekg(value * sizeof(Alumno) + sizeof(int), ios::beg);// me ubico en la posicion de metadata
//                file.seekp(value * sizeof(Alumno) + sizeof(int), ios::beg);
//                Alumno tempAlumno;  // creo un aluumno temporal
//                file.read((char*) &tempAlumno, sizeof(Alumno)); // almaceno el alumno antiguo
//                value = tempAlumno.get_netDel();    // guardo el nuevo valor de metadata
//
//                file.write((char*) &record, sizeof(Alumno)); // escribo el nuevo valor de alumno
//                file.seekp(0, std::ios::beg); // se ubica en el inicio del doc
//                file.seekg(0, std::ios::beg);
//                file.write((char*) &value, sizeof(int)); // se escribe la metadata
//                file.close();
//            }
//
//        }
//        file.close();
//    }

    void add(const Alumno &record) {
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo para lectura y escritura

        if (file) {
            int metadata;
            file.read(reinterpret_cast<char*>(&metadata), sizeof(int));  // Lee el valor de la metadata

            if (metadata == -1) {
                file.seekp(0, std::ios::end);  // Se ubica al final del archivo
                file.write(reinterpret_cast<const char*>(&record), sizeof(Alumno));
            } else {
                file.seekg(metadata * sizeof(Alumno) + sizeof(int));  // Se ubica en la posición del registro eliminado
                Alumno tempAlumno;
                file.read(reinterpret_cast<char*>(&tempAlumno), sizeof(Alumno));  // Lee el registro eliminado
                metadata = tempAlumno.get_nextDel();  // Actualiza el valor de la metadata

                file.seekp(metadata * sizeof(Alumno) + sizeof(int));  // Se ubica en la posición del registro eliminado
                file.write(reinterpret_cast<const char*>(&record), sizeof(Alumno));  // Escribe el nuevo registro
            }

            file.seekp(0, std::ios::beg);  // Se ubica al principio del archivo
            file.write(reinterpret_cast<const char*>(&metadata), sizeof(int));  // Escribe la nueva metadata
        }

        file.close();
    }


    Alumno readRecord(int pos)
    {
        Alumno record;
        std::ifstream file(filename, std::ios::binary);
        if (file)
        {
            file.seekg(pos * sizeof(Alumno), std::ios::beg);
            file.read(reinterpret_cast<char *>(&record), sizeof(Alumno));
            file.close();
        }
        return record;
    }

    bool deleteRecord(int pos){
        fstream file(filename, std::ios::binary | std::ios::app);
        //ifstream file(this->filename, ios::binary);
        if (file){
            file.seekp(0, std::ios::beg); // se ubica en el inicio del doc
            int value;                    // decalracion de variable value
            file.read((char*) &value, sizeof(int)); // se guarda el valor de metadata a value

            file.seekg(pos * sizeof(Alumno) + sizeof(int), ios::beg);// me ubico en la posicion a eliminar
            file.seekp(pos * sizeof(Alumno) + sizeof(int), ios::beg);// me ubico en la posicion a eliminar
            Alumno tempAlumno;  // creo un alumno temporal
            file.read((char*) &tempAlumno, sizeof(Alumno)); // almaceno el alumno antiguo
            tempAlumno.nextDel = value;
            file.write((char*) &tempAlumno, sizeof(Alumno)); // escribo el alumno alterando el nextDel

            int new_metadata =  static_cast<int>(file.tellg()) - sizeof(int) / sizeof(Alumno);

            file.seekp(0, std::ios::beg); // se ubica en el inicio del doc
            file.seekg(0, std::ios::beg); // se ubica en el inicio del doc
            file.write((char*) &new_metadata, sizeof(int)); // escribo el nuevo valor de metaData
        }
        file.close();
        return true ;
    }

//    int size(){
//        ifstream file(this->file_name, ios::binary);
//        if(!file.is_open()) throw ("No se pudo abrir el archivo");
//        file.seekg(0, ios::end);//ubicar cursos al final del archivo
//        long total_bytes = file.tellg();//cantidad de bytes del archivo
//        file.close();
//        return total_bytes / sizeof(Alumno);
//    }

    void add_prueba(const Alumno &record) {
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo adjunto

        file.seekg(0, std::ios::end); // Moverse al final del archivo
        std::streampos fileSize = file.tellg(); // Obtener la posición actual

        if (fileSize == 0) {
            int metadata = -1; // metadata = - 1
            file.write(reinterpret_cast<const char*>(&metadata), sizeof(int));  // Escribe la metadata en el inicio
            file.seekp(0, std::ios::end); // Moverse al final del archivo
            file.write(reinterpret_cast<const char*>(&record), sizeof(Alumno));  // Escribe el nuevo registro al final del archivo
        }

        int metadata;
        file.seekg(0, std::ios::beg); // Ubicar el puntero de lectura al inicio del archivo
        file.read(reinterpret_cast<char*>(&metadata), sizeof(int));
        cout << metadata;

//        metadata = 2;
//        file.seekg(static_cast<std::streamoff>(metadata)  * sizeof(Alumno) + sizeof(int));  // Se ubica en la posición del registro eliminado
//        file.write(reinterpret_cast<const char*>(&record), sizeof(Alumno));  // Escribe el nuevo registro al final del archivo

        if (metadata == -1){
            file.seekp(0, std::ios::end); // Moverse al final del archivo
            file.write(reinterpret_cast<const char*>(&record), sizeof(Alumno));  // Escribe el nuevo registro al final del archivo
        }else {
            file.seekg(static_cast<std::streamoff>(metadata)  * sizeof(Alumno) + sizeof(int));  // Se ubica en la posición del registro eliminado
            file.seekp(static_cast<std::streamoff>(metadata) * sizeof(Alumno) + sizeof(int));  // Se ubica en la posición del registro eliminado
            Alumno tempAlumno;
            file.read(reinterpret_cast<char*>(&tempAlumno), sizeof(Alumno));  // Lee el registro eliminado
            metadata = tempAlumno.nextDel;  // Actualiza el valor de la metadata
//
            file.write(reinterpret_cast<const char*>(&record), sizeof(Alumno));  // Escribe el nuevo registro de Alumno
            file.seekg(0, std::ios::beg); // Ubicar el puntero de lectura al inicio del archivo
            file.write(reinterpret_cast<const char*>(&metadata), sizeof(int));  // Escribe el nuevo registro de Alumno
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
//    Alumno Alumno1{"1234", "Edgar", "Chambilla", "CS", 5, 1250.0, 0};
    Alumno Alumno3{"9999", "Carlos", "Gómez", "Medicina", 2, 1800.0, 0};
//    Alumno Alumno2{"5678", "María", "López", "Arquitectura", 3, 1500.0, 0};
    Alumno Alumno4{"9999", "Luis", "Torres", "CS", 6, 1500.0, 0};


//    file1.add_prueba(Alumno4);
//    file1.add_prueba(Alumno3);
//    file1.deleteRecord(2);
//    file1.prueba_m1();
    file1.lectura_m1();


////Lectura
//    FixedRecordFile file2("data.bin");
//    vector<Alumno> alumnos = file2.load();
//    for(Alumno r : alumnos){
//        r.showData();
//    }
//    return 0;
//cout << "Holas";
}

