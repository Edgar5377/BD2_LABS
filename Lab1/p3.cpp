//
// Created by david on 24/08/2023.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

struct Matricula{
    string Codigo;
    int Ciclo;
    float Mensualidad;
    string Observaciones;

    size_t string_with_delimiter_size(const string& str){
        return sizeof(size_t)+str.size();
    }

    size_t size_of(){
        return string_with_delimiter_size(Codigo) +
               sizeof(Ciclo) +
               sizeof(Mensualidad) +
               sizeof(Observaciones);
    }
    void concat(char*& buffer, const char* str) {
        size_t len = strlen(str);
        memcpy(buffer, &len, sizeof(len));//Concatenamos el tamaño del string al buffer, el tercer parámetro es el tamaño del dato que estamos copiando.
        buffer += sizeof(len);//Nos movemos en el buffer, para que el siguiente dato que concatenemos no se sobreescriba con el anterior.
        memcpy(buffer, str, len);//Concatenamos el string al buffer, el tercer parámetro es el tamaño del dato que estamos copiando.
        buffer += len;//Nos movemos en el buffer, para que el siguiente dato que concatenemos no se sobreescriba con el anterior.
    }

    void concat(char*& buffer, const float& value) {
        memcpy(buffer, &value, sizeof(value));
        buffer+=sizeof(value);
    }
    void concat(char*& buffer, const int& value) {
        memcpy(buffer, &value, sizeof(value));
        buffer+=sizeof(value);
    }

    char* empaquetar(){
        char* buffer = new char[size_of()];
        concat(buffer, Codigo.c_str());
        concat(buffer, Ciclo);
        concat(buffer, Mensualidad);
        concat(buffer, Observaciones.c_str());
        return buffer;
    }

    void desempaquetar(char* buffer, int n){
        size_t tam = 0;
        memcpy(&tam, buffer, sizeof(size_t));//Copiamos la información del buffer a tam, el tercer parámetro es el tamaño del dato que estamos copiando.
        buffer += sizeof(size_t);//Nos movemos en el buffer, para identificar el siguiente dato que queremos copiar.
        Codigo.assign(buffer, tam);//Copiamos la información del buffer a Codigo, el tercer parámetro es el tamaño del dato que estamos copiando.
        buffer += tam;//Nos movemos en el buffer, para identificar el siguiente dato que queremos copiar.

        memcpy(&Ciclo, buffer, sizeof(Ciclo));//Copiamos la información de Ciclo al buffer, el tercer parámetro es el tamaño del dato que estamos copiando.
        buffer += sizeof(Ciclo);//Nos movemos en el buffer, para identificar el siguiente dato que queremos copiar.

        memcpy(&Mensualidad, buffer, sizeof(Mensualidad));//Copiamos la información de Mensualidad al buffer
        buffer += sizeof(Mensualidad);//Nos movemos en el buffer

        memcpy(&tam, buffer, sizeof(size_t));//Obtenemos el tamaño del siguiente string.
        buffer += sizeof(size_t);//Nos movemos en el buffer, para identificar el siguiente dato que queremos copiar.
        Observaciones.assign(buffer, tam);//Copiamos la información del buffer a Observaciones, el tercer parámetro es el tamaño del dato que estamos copiando.

    }
};

class VariableRecordFile{
private:
    string filename;
public:
    explicit VariableRecordFile(string filename){
        this->filename = filename;
    }

    void write(Matricula alumno){
//        ofstream file(filename, ios::binary | ios::app);
//        file.write(alumno.Nombre.c_str(), alumno.Nombre.size());
//        file.write(alumno.Apellidos.c_str(), alumno.Apellidos.size());
//        file.write(alumno.Carrera.c_str(), alumno.Carrera.size());
//        file.write((char*)&alumno.mensualidad, sizeof(alumno.mensualidad));
//        file.close();
    }

    vector<Matricula> read(){
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

    Matricula readRecord(int pos){
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

        Matricula record;
        file.read((char*)&record, tam_reg);//Leemos el registro que queremos leer.
        file.close();//Cerramos el archivo
        //¿Cuántos bytes le corresponde a cada campo?¿Cómo lo sabe el programa?
        //Debemos usar separadores, que nos indique, más que nada, los pesos de cada string, porque de int/float/double ya sabemos.



        return record;
    }

};

int main(){
    VariableRecordFile file1("data_variable.bin");
    Matricula a1;
//    a1.Nombre = "David";
//    a1.Apellidos = "Gonzalez";
//    a1.Carrera = "Ing. Sistemas";
//    a1.mensualidad = 1000.0;
//    cout<<a1.size_of()<<endl;


    return 0;
}

