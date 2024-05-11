#include "AVL.cpp"
#include "AVL.h"
#include "Record.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
//cambiar direccion al ejecutar!!!
const string DIR = "/Users/paolamaguinaaranda/Documents/2024-I/BD2/proyecto1/tecnicas";

void testAVL(string filename) {
    AVLFile file(filename);
    ifstream csvFile(DIR + "/data/data.csv");
    if (!csvFile.is_open()) {
        cerr << "Error opening CSV file!" << endl;
        return;
    } else {
        cout << "CSV file opened successfully!" << endl;
    }
    csvFile.close();

    file.loadData(DIR + "/data/data.csv");

    // Datos de prueba adicionales
    vector<Record> testData = {
            {1105930, "Richardson Oilseed Products (US) Limited", "WESSON Vegetable Oil 1 GAL", "Vegetable Oil", 15.0},
            {1105935, "CAMPBELL SOUP COMPANY", "SWANSON BROTH BEEF", "INGREDIENTS: BEEF STOCK, CONTAINS LESS THAN 2% OF: MIREPOIX (CARROTS, CELERY, ONIONS), SALT, NATURAL", 240.0},
            {1105939, "CAMPBELL SOUP COMPANY", "CAMPBELL'S SOUP BEAN AND HAM", "INGREDIENTS: WATER, PEA BEANS, CARROTS, COOKED HAM - WATER ADDED (PORK, WATER, SALT, SUGAR, SODIUM", 412.0}
    };

    for (Record& rec : testData) {
        cout << "insert: " << rec.fdc_id << endl;
        file.insert(rec);
    }

    cout << "--------- Showing all sorted data -----------\n";
    vector<Record> sortedData = file.inorder();
    for (Record& rec : sortedData) {
        rec.showData();
    }

    cout << "------Show data-----------\n";
    Record f2 = file.find(1105905);
    f2.showData();

    cout << "----Eliminacion -----\n";
    file.remove(1105904);
    vector<Record> sortedData1 = file.inorder();
    for (Record& rec : sortedData1) {
        rec.showData();
    }

    cout << "---Busqueda por rango ---- \n";
    vector<Record> results;
    int begin = 1105904;
    int end = 1105910;
    file.rangeSearch(begin, end, results);

    // Mostrar resultados de la búsqueda por rango
    cout << "Records in range [" << begin << ", " << end << "]:" << endl;
    for (Record& rec : results) {
        rec.showData();
    }
}

int main() {
    fstream binFile(DIR + "/bin/avl_data.bin", ios::in | ios::out | ios::binary);
    if (!binFile.is_open()) {
        cerr << "Error opening binary file for writing!" << endl;
        binFile.open(DIR + "/bin/avl_data.bin", ios::out | ios::binary);
        if (!binFile.is_open()) {
            cerr << "Failed to create binary file!" << endl;
            return 1;
        } else {
            cout << "Binary file created successfully!" << endl;
        }
        binFile.close();

        binFile.open(DIR + "/bin/avl_data.bin", ios::in | ios::out | ios::binary);
        if (!binFile.is_open()) {
            cerr << "Failed to reopen binary file!" << endl;
            return 1;
        }
    } else {
        cout << "Binary file opened successfully for writing!" << endl;
    }
    binFile.close();

    testAVL( DIR + "/bin/avl_data.bin");
    return 0;
}
