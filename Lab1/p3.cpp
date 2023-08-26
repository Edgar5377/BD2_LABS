#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

struct Matricula{
    string Codigo;
    int Ciclo=0;
    float Mensualidad=0;
    string Observaciones;

    static size_t string_with_delimiter_size(const string& str){
        return sizeof(size_t)+str.size();
    }

    size_t size_of(){
        return string_with_delimiter_size(Codigo) +
               sizeof(Ciclo) +
               sizeof(Mensualidad) +
               string_with_delimiter_size(Observaciones);
    }

    static void concat(char*& buffer, const char* str) {
        size_t len = strlen(str);
        memcpy(buffer, &len, sizeof(len));//Concatenamos el tamaño del string al buffer, el tercer parámetro es el tamaño del dato que estamos copiando.
        buffer += sizeof(len);//Nos movemos en el buffer, para que el siguiente dato que concatenemos no se sobreescriba con el anterior.
        memcpy(buffer, str, len);//Concatenamos el string al buffer, el tercer parámetro es el tamaño del dato que estamos copiando.
        buffer += len;//Nos movemos en el buffer, para que el siguiente dato que concatenemos no se sobreescriba con el anterior.
    }

    static void concat(char*& buffer, const float& value) {
        memcpy(buffer, &value, sizeof(value));
        buffer+=sizeof(value);
    }

    static void concat(char*& buffer, const int& value) {
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
    void desempaquetar(char* buffer) {
        size_t strLen;

        // Extraer Codigo
        memcpy(&strLen, buffer, sizeof(size_t));
        buffer += sizeof(size_t);
        char* codigoBuffer = new char[strLen];
        memcpy(codigoBuffer, buffer, strLen);
        buffer += strLen;
        Codigo = string(codigoBuffer, strLen);
        delete[] codigoBuffer;

        // Extraer Ciclo
        memcpy(&Ciclo, buffer, sizeof(int));
        buffer += sizeof(int);

        // Extraer Mensualidad
        memcpy(&Mensualidad, buffer, sizeof(float));
        buffer += sizeof(float);

        // Extraer Observaciones
        memcpy(&strLen, buffer, sizeof(size_t));
        buffer += sizeof(size_t);
        char* observacionesBuffer = new char[strLen];
        memcpy(observacionesBuffer, buffer, strLen);
        buffer += strLen;
        Observaciones = string(observacionesBuffer, strLen);
        delete[] observacionesBuffer;
    }

    void showData(){
        cout<<"Codigo: "<<Codigo<<endl;
        cout<<"Ciclo: "<<Ciclo<<endl;
        cout<<"Mensualidad: "<<Mensualidad<<endl;
        cout<<"Observciones: "<<Observaciones<<endl;
    }
};

class VariableRecordFile{
private:
    string filename;

public:
    explicit VariableRecordFile(string filename) : filename(std::move(filename)) {}

    void add(Matricula matricula) {
        ofstream file(filename, ios::app | ios::binary );//abro el archivo en app para que el puntero se posicione al final del archivo
        file.seekp(0,ios::end);
        long pos_fisica = file.tellp();//obtengo la posición física del archivo

        file.write(matricula.empaquetar(), matricula.size_of());//escribimos el alumno en el archivo
        file.close();//cerramos el archivo

        //LLENANDO METADATA:
        ofstream fm("cabacera.bin",ios::app | ios::binary );//abro el archivo, que será nuestra metadata, en donde almacenaré la posición y el tamaño de cada registro.
        size_t tam_Reg = matricula.size_of();//obtenemos el tamaño del registro que acabamos de escribir.
        cout<<pos_fisica<<" "<<tam_Reg<<endl;
        fm.write(reinterpret_cast<char*>(&pos_fisica), sizeof(long));//escribimos la posición física del registro que acabamos de escribir.
        fm.write(reinterpret_cast<char*>(&tam_Reg), sizeof(int));//escribimos el tamaño del registro que acabamos de escribir.
        fm.close();//cerramos el archivo
    }

//    vector<Matricula> read(){
//        vector<Matricula> alumnos;
//
//        return alumnos;
//    }

    Matricula readRecord(int pos){
        //Leyendo metadata
        ifstream fm("cabacera.bin", ios::binary);//abro el archivo, que será nuestra metadata, en donde almacenaré la posición y el tamaño de cada registro.
        fm.seekg(pos*sizeof(int)*2);//Cada registro tiene 2 enteros, pos y tamaño. Nos movemos a la posición del registro que queremos leer. NOLINT(*-narrowing-conversions)

        //Obtenemos las características del record[pos]
        long pos_fisica;
        int tam_reg;
        fm.read(reinterpret_cast<char*>(&pos_fisica), sizeof(long));//Leemos la posición física del registro que queremos leer.
        fm.read(reinterpret_cast<char*>(&tam_reg), sizeof(int));//Leemos el tamaño del registro que queremos leer.
        cout<<pos_fisica<<" "<<tam_reg<<endl;
        fm.close();//Es todo lo que quería de la metadata

        //Procedemos a trabajar con el archivo de registros
        ifstream file(filename, ios::binary);//abro el archivo
        file.seekg(pos_fisica);//Nos movemos a la posición del registro que queremos leer.

        //Ubicado el cursor en el registro pos:
        char* buffer = new char[tam_reg];//Creamos un buffer del tamaño del registro que vamos a leer.
        file.read(buffer, tam_reg);//Leemos el registro que queremos leer.
        file.close();//Cerramos el archivo

        Matricula record;
        record.desempaquetar(buffer);//Desempaquetamos el buffer, asignamos los valores al record.
        delete[] buffer;

        return record;
    }

};

int main(){
    VariableRecordFile file1("data.bin");

    Matricula Matricula1{"201910111",1,100,"Ninguna"};
    Matricula Matricula2{"201910112",2,200,"Ninguna"};
    Matricula Matricula3{"20191011",3,300,"Ninguna"};
    Matricula Matricula4{"201910114",4,400,"Ninguna"};
    Matricula Matricula5{"201910115",5,500,"Ninguna"};

//    file1.add(Matricula1);
//    file1.add(Matricula2);
//    file1.add(Matricula3);
//    file1.add(Matricula4);
//    file1.add(Matricula5);

    Matricula record = file1.readRecord(2);
//    record.showData();
//    ifstream file("cabecera.bin", ios::binary);
//    char c;
//    while (file.get(c)) {
//        cout << c;
//    }
//    file.close();
    return 0;
}

