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

int main(){
    Alumno a1;
    a1.Nombre = "David";
    a1.Apellidos = "Gonzalez";
    a1.Carrera = "Ing. Sistemas";
    a1.mensualidad = 1000.0;
    cout<<a1.size()<<endl;


    return 0;
}
