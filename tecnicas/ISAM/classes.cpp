#include <iostream>
#include <fstream>
#include "record.h"
using namespace std;
const int TAM_PAGE = 2048;
const char nl = '\n';
const string ifile = "index.txt";
const string dfile = "data.txt";

long indexSZ = 0;
long dataSZ = 0;
long nd = -1;

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
    static const long MI = (TAM_PAGE-2*sizeof(long)-sizeof(pair<T,long>))/(sizeof(pair<T,long>));
    pair<T, long>keys[MI+1];
    long n = 0;
    long nextdel = -1;

    IndexPage(){
        ofstream file(ifile, ios::app | ios::binary | fstream::out);
        // chequear si el archivo de indexes está vacío
        file.seekp(0, ios::end);
        int end = file.tellp();
        file.seekp(0, ios::beg);
        if (file.tellp() == end){ // si esta vacío
            file.seekp(0, ios::beg);
            file.write(reinterpret_cast<const char*>(&indexSZ), sizeof(indexSZ));
            file.write(reinterpret_cast<const char*>(&nd), sizeof(nd));
            file.write(reinterpret_cast<const char*>(&nl), sizeof(nl));
        } else { // si no
            checkIndex();
        }
    };
    ~IndexPage(){};
};

struct DataPage{
    static const int MD = (TAM_PAGE-sizeof(long)-sizeof(long)-sizeof(long))/(sizeof(Record)+sizeof(long));
    Record records[MD];
    long n = 0;
    long nextPage = -1;
    long nd = -1;
    long nextdel[MD+1];

    DataPage(){
        ofstream file(dfile, ios::app | ios::binary | fstream::out);
        // chequear si el archivo de datos está vacío
        file.seekp(0, ios::end);
        int end = file.tellp();
        file.seekp(0, ios::beg);
        if (file.tellp() == end){ // si esta vacío
            file.seekp(0, ios::beg);
            file.write(reinterpret_cast<const char*>(&dataSZ), sizeof(dataSZ));
            file.write(reinterpret_cast<const char*>(&nd), sizeof(nd));
            file.write(reinterpret_cast<const char*>(&nl), sizeof(nl));
        } else { // si no
            checkIndex();
        }
    };
    ~DataPage(){};
}; 