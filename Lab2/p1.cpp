//
// Created by david on 24/08/2023.
//

#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

struct Record{
    int cod;
    char nombre[12];
    int ciclo;
    long left = -1, right = -1;

    void showData_line(){
        cout << cod << " " << nombre << " " << ciclo << " " << left << " " << right << endl;
    }
};


class AVLFile{
private:
    string filename;
    long pos_root;
public:
    AVLFile(string filename){
        this->pos_root = -1;
        this->filename = filename;
    }

    Record find(int key){
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura
        if(file){
            pos_root=0;
        }
        file.close();
        return find(pos_root, key);
    }

    void insert(Record record){
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura
        if(file){
            pos_root=0;
        }
        file.close();
        insert(pos_root, record);
    }

    vector<Record> inorder(){
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura
        if(file){
            pos_root=0;
        }
        file.close();
        return inorder(pos_root);
    }

    long get_pos_root(){return pos_root;}

    void print_file(){
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
        Record current_record;
        file.seekg(0);
        while(file.read(reinterpret_cast<char*>(&current_record), sizeof(Record))){
            current_record.showData_line();
        }
        file.close();
    }

private:
    Record find(long pos_node, int key){
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura
        Record current_record;
        Record return_record;


        file.seekg(pos_node*sizeof(Record)); // Ubicar el puntero de lectura al inicio del archivo
        file.read(reinterpret_cast<char*>(&current_record), sizeof(Record));//obtengo el record de la primera posicion y lo guardo en actual_recor
        if(current_record.cod == key){
            return_record = current_record;
            file.close();
            return return_record;
        }
        else if(current_record.cod > key){
            if(current_record.left != -1){
                return_record = find(current_record.left-1, key);
            }
            else{
                file.close();
                throw std::out_of_range("No existe el registro con la clave indicada.");
            }
        }
        else if(current_record.cod < key){
            if(current_record.right != -1){
                return_record = find(current_record.right-1, key);
            }
            else{
                file.close();
                throw std::out_of_range("No existe el registro con la clave indicada.");
            }
        }
        file.close();
        return return_record;
    }

    void insert(long pos_node, Record record){

        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);  // Abre el archivo en modo lectura/escritura
        Record current_record;

        if(!file){
            file.close();
            std::ofstream outfile(filename, std::ios::binary | std::ios::app);  // Abre el archivo en modo adjunto
//            current_record = record;
            outfile.write(reinterpret_cast<const char*>(&record), sizeof(Record));  // Escribe la metadata en el inicio
            outfile.close();
//            cout << pos_node << endl;
            return;
        }
//        cout << pos_node << endl;

        file.seekg(pos_node* sizeof(Record)); // Ubicar el puntero de lectura al inicio del archivo, es decir la raiz
        file.read(reinterpret_cast<char*>(&current_record), sizeof(Record));//obtengo el record de la posicion


        Record insert_record;
        insert_record =  record;


        //comparar los valores de codigo;
        if(insert_record.cod > current_record.cod){
            if(current_record.right == -1 ){

                file.seekp(0, std::ios::end); // Moverse al final del archivo
                file.write(reinterpret_cast<const char*>(&record), sizeof(Record));  // Escribe el nuevo registro al final del archivo
                long position_nuevo = file.tellp()/sizeof(Record);  // calculo del valor del valor a insertar
                file.seekp(pos_node* sizeof(Record)); // Moverse a la posicion a actualizar
                current_record.right = position_nuevo; // Actualizamos el valor de right
                file.write(reinterpret_cast<const char*>(&current_record), sizeof(Record));  // Actualizamos el valor de nuestro Record
                file.close();
            }else{
                file.close();
                if(current_record.right != -1 )
                {
                    insert(current_record.right-1, insert_record);
                }
            }

        }
        else if(insert_record.cod < current_record.cod) {
            if (current_record.left == -1) {
                file.seekp(0, std::ios::end); // Moverse al final del archivo
                file.write(reinterpret_cast<const char*>(&record), sizeof(Record));  // Escribe el nuevo registro al final del archivo
                long position_nuevo = file.tellp()/sizeof(Record);  // calculo del valor del valor a insertar
                file.seekp(pos_node* sizeof(Record)); // Moverse a la posicion a actualizar
                current_record.left = position_nuevo; // Actualizamos el valor de left
                file.write(reinterpret_cast<const char*>(&current_record), sizeof(Record));  // Actualizamos el valor de nuestro Record
                file.close();
            } else {
                file.close();
                if (current_record.left != -1)
                {
                    insert(current_record.left-1, record);
                }
            }
        }

        file.close();
        /*
        if (node == nullptr)
            node = new NodeBT<T>(value);
        else if (value < node->data)
            insert(node->left, value);
        else
            insert(node->right, value);
        */
    }

    vector<Record> inorder(long pos_node){
        fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
        vector<Record> result;
        if(!file){
            throw std::out_of_range("No existe el archivo, no se puede cargar info.");
        }
        Record current_record;
        file.seekg(pos_node*sizeof(Record)); // Ubicar el puntero de lectura al inicio del archivo
        file.read(reinterpret_cast<char*>(&current_record), sizeof(Record));//obtengo el record de la primera posicion y lo guardo en actual_recor
        if(current_record.left != -1){
            vector<Record> left_result = inorder(current_record.left-1);
            result.insert(result.end(), left_result.begin(), left_result.end());
        }
        result.push_back(current_record);
        if(current_record.right != -1){
            vector<Record> right_result = inorder(current_record.right-1);
            result.insert(result.end(), right_result.begin(), right_result.end());
        }
        file.close();


        return result;
    }



};


int main() {
    AVLFile avlFile("data.dat");

    // Insertar registros
    Record record1 = {271, "Josimar", 5};
    Record record2 = {255, "Manuel", 8};
    Record record3 = {362, "Cinthya", 3};
    Record record4 = {224, "Andrea", 2};
    Record record5 = {887, "Benjamin", 9};

     avlFile.insert(record1);
     avlFile.insert(record2);
     avlFile.insert(record3);
     avlFile.insert(record4);
     avlFile.insert(record5);
     avlFile.print_file();

//    // Realizar recorrido inorder
    vector<Record> inorder_result = avlFile.inorder();
    cout << "Recorrido In-Order:" << endl;
    for (const Record& record : inorder_result) {
        cout << "Cod: " << record.cod << ", Nombre: " << record.nombre << ", Ciclo: " << record.ciclo << endl;
    }

//     Buscar un registro
    int key_to_find = 271;
    Record found_record = avlFile.find(key_to_find);
    cout << "Registro encontrado con clave " << key_to_find << ": " << endl;
    cout << "Cod: " << found_record.cod << ", Nombre: " << found_record.nombre << ", Ciclo: " << found_record.ciclo << endl;
//    cout << avlFile.get_pos_root() << endl;
    return 0;
}

