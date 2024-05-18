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
        int nextdel;

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
        vector <int> nextdel;

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

    template <typename T>
    long findpage(T key){
        ifstream file(ifile, ios::app | ios::binary | fstream::out);
        isam::IndexPage<T> ip; 
        
        int beg=0;
        int final=ip.n-1;

        file.seekg(4+1, ios::beg);
        file.read(sizeof(IndexPage), ip);

        //hago mi binary
        while(beg<=final) {
            int i=0;
            int mid=(beg+final)/2;
            
            if(key==ip.keys[mid]){
                return mid;
                file.close();
            }
            if(key<=ip.keys[mid]){
                final=mid-1;
            }
            else if(key>=ip.keys[i]){
                beg=mid+1;
            }
        }
        return ip.pages[beg];
        
    }
    
    template <typename T>
    vector<Record> search(T key){
        //obtengo mi ip2
        long p2=findpage(key); 
        //paso a mi segundo nivel
        file.seekg(p2*(sizeof(IndexPage)+1), ios::beg); 
        file.read(ip,sizeof(IndexPage))

        //aplico binary de mi 2ndo nivel
        while(beg<=final) {
            int i=0;
            int mid=(beg+final)/2;
            
            if(key==ip.keys[mid]){
                return mid;
                file.close();
            }
            if(key<=ip.keys[mid]){
                final=mid-1;
            }
            else if(key>=ip.keys[i]){
                beg=mid+1;
            }
        }
        long ip3=indexPage.pages[beg]; 

        //tengo q acceder a datafile de mi p2
        ifstream file(dfile, ios::app | ios::binary | fstream::out);
        
        isam::DataPage dataPage;
        file.seekg((4+1)+(1+sizeof(DataPage))*ip3, ios::beg);
        int beg=0;
        int final=file.n-1;

        
        //busco mi key en mi dp
        int i=0;
        while(dataPage.records[i]!=key){
            i++; 
        }

    }

    };
    template <typename T>
    void add(T r,T k){
        fstream file(ifile, ios::app | ios::binary | fstream::out);
        isam::IndexPage<T> ip0;  //mi primera page
        
        int beg=0;
        int final=ip0.n-1;

        file.seekg(4+1, ios::beg); //para ubicar la ip0
        if(ip0.n!=ip0.MI-1){ //no esta lleno
            isam::IndexPage<T> ipn;  //mi next ip
            ip0.pages[-1]=ipn; //mi ultimo page apunta al next level page
            //sort();
            //me ubico en ipn y escribo el record
            file.seekg(ipn*(sizeof(IndexPage)+1), ios::beg); 
            file.write(reinterpret_cast<const char*>(&k), sizeof(Record));

            //creo mi datapage para añadir mi record
            ifstream dfile(dfile, ios::app | ios::binary | fstream::out);
            isam::DataPage dpn;
            dfile.seekg((4+1)+(1+sizeof(DataPage))*ipn, ios::beg);
            dfile.write(reinterpret_cast<const char*>(&k), sizeof(Record));

        }
        else{
            file.seekg(4+1, ios::beg);
            file.read(ip0,sizeof(IndexPage));

            //hago mi binary para hallar el mayor key a k y obtener pos
            while(beg<=final) {
                int i=0;
                int mid=(beg+final)/2;
                
                if(k==ip0.keys[mid]){
                    return mid;
                    file.close();
                }
                if(k<=ip0.keys[mid]){
                    final=mid-1;
                }
                else if(k>=ip0.keys[mid]){
                    beg=mid+1;
                }
            }
            long x=ip0.pages[beg]; //hallo la pos para mi segundo nivel 
            //ipx a memoria
            file.seekg((4+1)+(x*(sizeof(IndexPage)+1)), ios::beg); 
            isam::IndexPage <T> ipx;
            if(ipx.n!=ipx.MI-1){
                isam::DataPage dpn; //creo mi datapage
                ipx.pages[x]=dpn;
            }

            else{
                fstream dfile(dfile, ios::app | ios::binary | fstream::out);
                file.seekg((4+1)+(1+sizeof(DataPage))*ipx, ios::beg);
                isam::DataPage dp; 
                
                int beg=0;
                int final=dp.n;
                //binary search para hallar el primer mayor a k y obtener pos de page
                while(beg<=final) {
                    int i=0;
                    int mid=(beg+final)/2;
                    
                    if(k==dp.records[mid]){
                        return mid;
                        dfile.close();
                    }
                    if(k<=dp.record[mid]){
                        final=mid-1;
                    }
                    else if(k>=dp.record[mid]){
                        beg=mid+1;
                    }
                }
                //me ubico en la pagina de mi dp
                long dpn=dp.record[beg]; 

                if(dp.n!=dp.MD-1){ //si no esta full
                    int pos=dp.nextdel; //uso free list para ver el next del y ubicarlo
                    dfile.seekg((4+1)+(1+sizeof(DataPage))*pos, ios::beg);
                    dfile.write(reinterpret_cast<const char*>(&k), sizeof(Record));  
                }
                else{
                    isam::DataPage dpe; 
                    isam::DataPage lastelem=dpn.records[-1]; //ultimo elemento
                    lastelem.nextPage=dpe; //su nextpage será el dpe
                    dpe.write(reinterpret_cast<const char*>(&k), sizeof(Record));  //escribo record
                }
            }
        }
    };

    /*
    add(r k):
        traer ip0 a memoria ram
        si ip0.n != ip0.MI - 1:
            crear ipx, donde x es la cantidad de index pages en el archivo de indexes
            añadir k al final de ip0.keys
            ip0.pages[at k] = x
            sort ip0.keys, manteniendo la relación de keys y nextpages
            ir a ipx // nivel 2
            añadir k a ipx.keys
            crear dpx, donde x es la cantidad de data pages en el archivo de data
            ipx.pages[at k] = x
            ir a dpx y añadir r // nivel 3
            escribir dpx
        else:
            busqueda binaria para encontrar primer mayor key a k, retorna x
            traer ipx a memoria ram
            si ipx.n != ipx.MI - 1:
                crear dpx, donde x es la cantidad de data pages en el archivo de data
                ipx.pages[at k] = x
            else:
                busqueda binaria para encontrar primer mayor key a k, retorna x
                traer dpx a memoria ram
                si dpx not full
                    añadir r a dpx (usando free list) // nivel 3
                    escribir dpx
                else:
                    crear dpe, donde e es la cantidad de data pages en el archivo de data
                    lastEle = el ultimo elemento en dpx
                    lastEle.nextP = dpe
                    añadir record a dpe
                    escribir dpe
        


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

