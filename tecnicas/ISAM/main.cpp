#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> 
using namespace std;
const char nl = '\n';

const int TAM_PAGE = 2048;

struct Record{
    char codigo [5];
    char nombre [12];
    char apellidos[20];
    float mensualidad;
};

namespace isam{
    string ifile = "index.txt";
    string dfile = "data.txt";
    int indexSZ = 0;
    int dataSZ = 0;
    void checkIndex(){
        ifstream file(ifile, ios::app | ios::binary);
        file.seekg(0, ios::beg);
        file.read(reinterpret_cast<char*>(&indexSZ), sizeof(indexSZ));
        file.close();
    }
    void checkData(){
        ifstream file(dfile, ios::app | ios::binary);
        file.seekg(0, ios::beg);
        file.read(reinterpret_cast<char*>(&dataSZ), sizeof(dataSZ));
        file.close();
    }
    template <typename T>
    struct IndexPage{
        static const int MI = (TAM_PAGE-sizeof(int)-sizeof(long))/(sizeof(T)+sizeof(long));
        T keys[MI];
        long pages[MI+1];
        int n;

        IndexPage(){
            ofstream file(ifile, ios::app | ios::binary | fstream::out);
            // chequear si el archivo de indexes está vacío
            file.seekp(0, ios::end);
            int end = file.tellp();
            file.seekp(0, ios::beg);
            if (file.tellp() == end){ // si esta vacío
                file.seekp(0, ios::beg);
                file.write(reinterpret_cast<const char*>(&indexSZ), sizeof(indexSZ));
            } else { // si no
                checkIndex();
            }
            file.seekp(indexSZ*sizeof(IndexPage) + sizeof(int), ios::beg);
            file.write((char*) this, sizeof(IndexPage));
            file.write(reinterpret_cast<const char*>(&nl), sizeof(char));
            file.close();
            indexSZ++;           
        };
        ~IndexPage(){};
    }; // MI*sizeof(T) + (MI+1)*sizeof(long) + sizeof(int) = TAM_PAGE;

    struct DataPage{
        static const int MD = (TAM_PAGE-sizeof(int)-sizeof(long))/sizeof(Record);
        Record records[MD];
        int n;
        long nextPage;

        DataPage(){
            ofstream file(dfile, ios::app | ios::binary | fstream::out);
            // chequear si el archivo de datos está vacío
            file.seekp(0, ios::end);
            int end = file.tellp();
            file.seekp(0, ios::beg);
            if (file.tellp() == end){ // si esta vacío
                file.seekp(0, ios::beg);
                file.write(reinterpret_cast<const char*>(&indexSZ), sizeof(indexSZ));
            } else { // si no
                checkIndex();
            }
            file.seekp(dataSZ*sizeof(DataPage) + sizeof(int), ios::beg);
            file.write((char*) this, sizeof(DataPage));
            file.write(reinterpret_cast<const char*>(&nl), sizeof(char));
            file.close();
            dataSZ++;  
        };
        ~DataPage(){};
    }; // MD*sizeof(Record) + sizeof(int) + sizeof(long) = TAM_PAGE;

    int findpage( int beg, int end, char * key){
        ifstream file(ifile, ios::app | ios::binary | fstream::out);
        file.seekg(0, ios::beg);
        int beg=0;
        int final= 10; //debería ser indexPage.n -1

        while(beg<=final) {
            Record r;
            
            int mid=(beg+final)/2;
            int ant=mid;
            file.seekg(mid);
            file.read((char*)&r, sizeof(Record));
            if(key==r.codigo){
                return mid;
                file.close();
            }
            if(key<=r.codigo){
                end=mid+1;
                
            }
            else if(key>=r.codigo){
                findpage(mid+1, end, key);
            }
        }
        //return indexPage.pages[beg];
        
    }
    
    template <typename T>
    vector<Record> search(T key){
        

    }

    };
    /*
    void add(Record record) {
        std::ifstream file(dfile, ios::app | ios::binary | fstream::out);
        file.seekg(0, ios::end);
        int end = file.tellp();
        if(file.tellp()==end){ //esta vacio solo añado
            file.write(reinterpret_cast<const char*>(&nl), sizeof(char));
        }
        Record tmp,prev;
        /*
        while ((file.read((char*)&tmp, sizeof(Record))) && (tmp->nextpointer!=NULL &&tmp.codigo<record.codigo)) { //leo otros registos
             prev=tmp;
             tmp=tmp->nextpointer;
        }
        record->nextpointer=prev->nextpointer;
        prev->nextpointer=record->nextpointer;
        
        file.close();
    }
    */


int main(){
    isam::IndexPage<int> n;

    cout << n.MI << endl;
    cout << isam::indexSZ << endl;

}

/*
Busqueda(key):
- Abrir el index1 
- Leer la entrada relacionada a key (pagina del siguiente index)
- Abrir el index2
- Leer la entrada relacionada a key (pagina del siguiente index)
- Abrir el index3
- Leer la entrada en el archivo de datos(pagina del archivo de datos)
- Leer la pagina de datos y ubicas el registro con la key
- Buscar en el encadenamiento de paginas
:: O(L+1) + O(encadenamiento),  L total de niveles

Insertar(record, key):
- Abrir el index1 y localizamos la entrada  *
- Abrir el index2 y localizamos la entrada  *
- Abrir el index3 y localizamos la entrada  *
- Leer la pagina de datos se lleva a la RAM *
- Caso 1: pagina no llena
  - Insertar el registro en dicha pagina
  - Escribir la pagina en la misma posicion *
- Caso 2: pagina si esta llena
  - Crear nueva pagina de datos
  - Agregar el nuevo registro a dicha pagina (o aplicar split)
  - La pagina actual se enlaza con la pagina recien creada
  - Escribir pagina actual y pagina enlazada *
O(L) + O(encadenamiento)
*/

