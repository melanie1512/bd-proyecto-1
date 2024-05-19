#ifndef PROYECTO1_RECORD_H
#define PROYECTO1_RECORD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Record
{
public:

    int fdc_id; //primary key
    char brand[50];
    char description[30];
    char ingredients[110];
    float servingsize;

    long left = -1, right = -1;
    long height = 0;

    // Constructor predeterminado
    Record() : fdc_id(0), servingsize(0.0) {
        strcpy(brand, "");
        strcpy(description, "");
        strcpy(ingredients, "");
    }

    // Constructor con parámetros
    Record(int id, const string& b, const string& d, const string& i, float s) {
        fdc_id = id;
        strncpy(brand, b.c_str(), sizeof(brand) - 1);
        strncpy(description, d.c_str(), sizeof(description) - 1);
        strncpy(ingredients, i.c_str(), sizeof(ingredients) - 1);
        brand[sizeof(brand) - 1] = '\0';
        description[sizeof(description) - 1] = '\0';
        ingredients[sizeof(ingredients) - 1] = '\0';
        servingsize = s;
    }

    std::string toString() const {
        return "Record(fdc_id: " + std::to_string(fdc_id) + ", brand: " + brand +
               ", description: " + description + ", ingredients: " + ingredients +
               ", servingsize: " + std::to_string(servingsize) + ")";
    }

    void setData()
    {
        cout << "fdc_id:";
        cin >> fdc_id;
        cout << "brand: ";
        cin >> brand;
        cout << "description: ";
        cin >> description;
        cout <<"ingredients:";
        cin >> ingredients;
        cout << "serving size: ";
        cin >> servingsize;
    }

    void showData()
    {
        cout << "\nfdc_id: " << fdc_id;
        cout << "\nbrand: " << brand;
        cout << "\ndescription: " << description;
        cout << "\ningredients: " << ingredients;
        cout << "\nserving size : " << servingsize <<endl;
    }
};
#endif //PROYECTO1_RECORD_H