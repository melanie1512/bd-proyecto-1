#include <iostream>
using namespace std;

const int TAM_PAGE = 2048;

struct Record{
    char codigo [5];
    char nombre [12];
    char apellidos[20];
    float mensualidad;
};

template <typename T>
struct IndexPage{
    int MI = (TAM_PAGE-sizeof(int)-sizeof(long))/(sizeof(T)+sizeof(long));
	T keys[MI];
	long pages[MI+1];
	int n;
};

// MI*sizeof(T) + (MI+1)*sizeof(long) + sizeof(int) = TAM_PAGE;

struct DataPage{
    static const int MD = (TAM_PAGE-sizeof(int)-sizeof(long))/sizeof(Record);
    Record records[MD];
    int n;
    long nextPage;
};

// MD*sizeof(Record) + sizeof(int) + sizeof(long) = TAM_PAGE;