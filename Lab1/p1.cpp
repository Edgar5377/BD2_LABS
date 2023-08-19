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

    }
    void showData(){
        cout<<"Codigo: "<<codigo<<endl;
        cout<<"Nombre: "<<nombre<<endl;
        cout<<"Apellidos: "<<apellidos<<endl;
        cout<<"Carrera: "<<carrera<<endl;
        cout<<"Ciclo: "<<ciclo<<endl;
        cout<<"Mensualidad: "<<mensualidad<<endl;
    }
    int get_netDel(){return nextDel;}
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

    void add(const Alumno &record){
        fstream file(filename, std::ios::binary | std::ios::app);
        //ifstream file(this->filename, ios::binary);
        if (file){
            file.seekp(0, std::ios::beg); // se ubica en el inicio del doc
            file.seekg(0, std::ios::beg);
            int value;                    // declaración de variable value
            file.read((char*) &value, sizeof(int)); // se guarda el valor de metadata a value

            if(value == -1){    // en caso no se tenga valores eliminados
                file.write(reinterpret_cast<const char *>(&record), sizeof(Alumno));
                file.close();
            } else {
                file.seekg(value * sizeof(Alumno) + sizeof(int), ios::beg);// me ubico en la posicion de metadata
                file.seekp(value * sizeof(Alumno) + sizeof(int), ios::beg);
                Alumno tempAlumno;  // creo un aluumno temporal
                file.read((char*) &tempAlumno, sizeof(Alumno)); // almaceno el alumno antiguo
                value = tempAlumno.get_netDel();    // guardo el nuevo valor de metadata

                file.write((char*) &record, sizeof(Alumno)); // escribo el nuevo valor de alumno
                file.seekp(0, std::ios::beg); // se ubica en el inicio del doc
                file.seekg(0, std::ios::beg);
                file.write((char*) &value, sizeof(int)); // se escribe la metadata
                file.close();
            }

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
    }

//    int size(){
//        ifstream file(this->file_name, ios::binary);
//        if(!file.is_open()) throw ("No se pudo abrir el archivo");
//        file.seekg(0, ios::end);//ubicar cursos al final del archivo
//        long total_bytes = file.tellg();//cantidad de bytes del archivo
//        file.close();
//        return total_bytes / sizeof(Alumno);
//    }
};

int main()
{
//Escritura
    FixedRecordFile file1("data.bin");
    Alumno record;
    record.setData();
    file1.writeRecord(record);
//Lectura
    FixedRecordFile file2("data.bin");
    vector<Alumno> alumnos = file2.load();
    for(Alumno r : alumnos){
        r.showData();
    }
    return 0;
}

