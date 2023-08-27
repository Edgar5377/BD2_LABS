//
// Created by david on 24/08/2023.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Alumno
{
    string Nombre;
    string Apellidos;
    string Carrera;
    float mensualidad;

    void print_data(){
        cout << Nombre << " " << Apellidos << " " << Carrera << " " << mensualidad << endl;
    }};

class VariableRecord{
    string filename;
    char field_delim = '|';
    char record_delim = '\n';


    void get_first_line(std::ifstream& file){
        string first_line;
        getline(file,first_line);
    }

    void parse_line_record(std::stringstream& ss,Alumno& alumno){
        std::getline(ss, alumno.Nombre, field_delim);
        std::getline(ss, alumno.Apellidos, field_delim);
        std::getline(ss, alumno.Carrera, field_delim);
        ss >> alumno.mensualidad;
    }
public:
    explicit VariableRecord(string filename): filename(filename){}

    vector<Alumno> load(){
        std::ifstream archivo(filename,std::ios::in);
        vector<Alumno> result;
        string data_field;

        get_first_line(archivo); // Leemos la primera linea que nombra los campos de los registros


        if (archivo.is_open()) {
            std::string linea;

            while (std::getline(archivo, linea)) {

                std::stringstream ss(linea);
                Alumno alumno;

                parse_line_record(ss,alumno);
                result.push_back(alumno);

            }

            archivo.close();
        } else {
            throw std::runtime_error("No existe el archivo.");
        }


        return result;
    }

    void add(Alumno record){

        std::fstream archivo(filename ,std::ios::out | std::ios::app);

        if (archivo.is_open()) {
            archivo << std::endl <<  record.Nombre
            << field_delim <<  record.Apellidos
            << field_delim << record.Carrera
            << field_delim << record.mensualidad ;
            archivo.close(); // Cerrar el archivo
        } else {
            throw std::runtime_error("No se pudo abrir el archivo.");
        }
    }

    Alumno readRecord(int pos) {
        ifstream file(filename, std::ios::in);
        Alumno record;

        if (!file) {
            throw std::runtime_error("Could not open file.");
        }
        if (pos < 0) {
            file.close();
            throw std::invalid_argument("Posicion invalida, no puede ser negativo.");
        }
        std::string linea;
        int contador = 0;
        get_first_line(file); // Leemos la primera linea que contiene los nombres de cada campo del registro

        while (std::getline(file, linea)) {
            if (contador == pos) {
                Alumno alumno;
                std::stringstream ss(linea);
                parse_line_record(ss, alumno);

                file.close();
                return alumno;
            }
            contador++;
        }

        file.close();
        throw std::out_of_range("Position fuera de rango, no valido.");
    }
};



int main() {
    VariableRecord file("p2_texto.txt");

    vector<Alumno> records_loaded;
    Alumno nuevoAlumno1 = {"Juan", "Perez", "Ingenieria", 1500.0};
    Alumno nuevoAlumno2 = {"Maria", "Lopez", "Medicina", 1800.0};
    Alumno nuevoAlumno3 = {"Carlos", "Gomez", "Derecho", 1300.0};
    Alumno nuevoAlumno4 = {"Ana", "Martinez", "Psicologia", 1600.0};
    Alumno nuevoAlumno5 = {"Luis", "Rodriguez", "Arquitectura", 1700.0};

//    file.add(nuevoAlumno1);
//    file.add(nuevoAlumno2);
//    file.add(nuevoAlumno3);
//    file.add(nuevoAlumno4);
//    file.add(nuevoAlumno5);

    std::cout << "Carga de datos del archivo: " << endl;
    records_loaded = file.load();

    for(auto &record: records_loaded){
        record.print_data();
    }

    std::cout << endl;
    std::cout << "Buscar por posicion en el archivo: " << endl;
    int pos = 3;
    std::cout << "Record a leer pos " << pos << ":" << std::endl;
    Alumno alumno = file.readRecord(pos);
    alumno.print_data();

    pos = 4;
    std::cout << "Record a leer pos " << pos << ":" << std::endl;
    alumno = file.readRecord(pos);
    alumno.print_data();

    return 0;
}
