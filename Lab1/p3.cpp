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
               string_with_delimiter_size(Observaciones);
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

    void write(Matricula matricula) {
        ofstream file(filename, ios::binary | ios::app);//abro el archivo en app para que el puntero se posicione al final del archivo
        long pos_fisica = file.tellp();//obtengo la posición física del archivo
        file.write(matricula.empaquetar(), matricula.size_of());//escribimos el alumno en el archivo
        file.close();//cerramos el archivo

        ofstream fm("cabacera.bin",
                    ios::binary);//abro el archivo, que será nuestra metadata, en donde almacenaré la posición y el tamaño de cada registro.
        fm.write((char*)pos_fisica, sizeof(long));//escribimos la posición física del registro que acabamos de escribir.
        fm.write((char*)matricula.size_of(), sizeof(int));//escribimos el tamaño del registro que acabamos de escribir.
        fm.close();//cerramos el archivo
    }

    vector<Matricula> read(){
        vector<Matricula> alumnos;

        return alumnos;
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
        //Ubicado el cursor en el registro pos:
        char* buffer = new char[tam_reg];//Creamos un buffer del tamaño del registro que vamos a leer.
        file.read(buffer, tam_reg);//Leemos el registro que queremos leer.
        file.close();//Cerramos el archivo

        Matricula record;
        record.desempaquetar(buffer, tam_reg);//Desempaquetamos el buffer, asignamos los valores al record.
        file.close();//Cerramos el archivo

        return record;
    }

};

int main(){
    VariableRecordFile file1("data.bin");
    Matricula a1;
    a1.Ciclo=1;
    a1.Mensualidad=100;
    a1.Codigo="201910111";
    a1.Observaciones="Ninguna";
    file1.write(a1);


    return 0;
}

