//
// Created by david on 24/08/2023.
//
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct Alumno{
    string Nombre;
    string Apellidos;
    string Carrera;
    float mensualidad;

    size_t size(){
        return Nombre.size() + Apellidos.size() + Carrera.size() + sizeof(mensualidad);
    }
};

class VariableRecordFile{
private:
    string filename;
public:
    explicit VariableRecordFile(string filename){
        this->filename = filename;
    }

    void write(Alumno alumno){
//        ofstream file(filename, ios::binary | ios::app);
//        file.write(alumno.Nombre.c_str(), alumno.Nombre.size());
//        file.write(alumno.Apellidos.c_str(), alumno.Apellidos.size());
//        file.write(alumno.Carrera.c_str(), alumno.Carrera.size());
//        file.write((char*)&alumno.mensualidad, sizeof(alumno.mensualidad));
//        file.close();
    }

    vector<Alumno> read(){
//        vector<Alumno> alumnos;
//        ifstream file(filename, ios::binary);
//        while(!file.eof()){
//            Alumno alumno;
//            char buffer[100];
//            file.read(buffer, 100);
//            alumno.Nombre = buffer;
//            file.read(buffer, 100);
//            alumno.Apellidos = buffer;
//            file.read(buffer, 100);
//            alumno.Carrera = buffer;
//            file.read((char*)&alumno.mensualidad, sizeof(alumno.mensualidad));
//            alumnos.push_back(alumno);
//        }
//        file.close();
//        return alumnos;
    }

    Alumno readRecord(int pos){
        ifstream fm("cabacera.bin", ios::binary);//abro el archivo, que será nuestra metadata, en donde almacenaré la posición y el tamaño de cada registro.
        fm.seekg(pos*sizeof(int)*2);//Cada registro tiene 2 enteros, pos y tamaño. Nos movemos a la posición del registro que queremos leer.

        //Obtenemos las características del record[pos]
        int pos_fisica, tam_reg;
        fm.read((char*)&pos_fisica, sizeof(int));//Leemos la posición física del registro que queremos leer.
        fm.read((char*)&tam_reg, sizeof(int));//Leemos el tamaño del registro que queremos leer.

        fm.close();//Es todo lo que quería de la metadata

        //Procedemos a trabajar con el archivo de registros
        ifstream file(filename, ios::binary);//abro el archivo
        file.seekg(pos_fisica);//Nos movemos a la posición del registro que queremos leer.

        Alumno record;
        file.read((char*)&record, tam_reg);//Leemos el registro que queremos leer.
        file.close();//Cerramos el archivo

        return record;
    }

};

int main(){
    VariableRecordFile file1("data_variable.bin");
    Alumno a1;
    a1.Nombre = "David";
    a1.Apellidos = "Gonzalez";
    a1.Carrera = "Ing. Sistemas";
    a1.mensualidad = 1000.0;
    cout<<a1.size()<<endl;


    return 0;
}
