#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

template<typename T>
struct FoodC {
    // 5 columnas aparte del key
    T key; // fdc_id
    char brand_owner[31]; 
    char description[151];
    char ingredients[41];
    char serving_size_unit[6];
    char branded_food_category[31];

    FoodC(){};
    
    FoodC(T key, const std::string& brand_owner, const std::string& description, const std::string& ingredients, const std::string& serving_size_unit, const std::string& branded_food_category) :
            key(key) {
        strncpy(this->brand_owner, brand_owner.c_str(), sizeof(this->brand_owner));
        strncpy(this->description, description.c_str(), sizeof(this->description));
        strncpy(this->ingredients, ingredients.c_str(), sizeof(this->ingredients));
        strncpy(this->serving_size_unit, serving_size_unit.c_str(), sizeof(this->serving_size_unit));
        strncpy(this->branded_food_category, branded_food_category.c_str(), sizeof(this->branded_food_category));
    }

    friend std::ostream& operator<<(std::ostream& out, const FoodC<T>& food) {
        out.write(reinterpret_cast<const char*>(&food.key), sizeof(food.key));
        out.write(food.brand_owner, sizeof(food.brand_owner));
        out.write(food.description, sizeof(food.description));
        out.write(food.ingredients, sizeof(food.ingredients));
        out.write(food.serving_size_unit, sizeof(food.serving_size_unit));
        out.write(food.branded_food_category, sizeof(food.branded_food_category));
        return out;
    }

    
    friend std::istream& operator>>(std::istream& in, FoodC<T>& food) {
        in.read(reinterpret_cast<char*>(&food.key), sizeof(food.key));
        in.read(food.brand_owner, sizeof(food.brand_owner));
        in.read(food.description, sizeof(food.description));
        in.read(food.ingredients, sizeof(food.ingredients));
        in.read(food.serving_size_unit, sizeof(food.serving_size_unit));
        in.read(food.branded_food_category, sizeof(food.branded_food_category));
        return in;
    }
};

int main() {
    FoodC<int> food1{1, "Brand1", "Description1", "Ingredient1", "Unit1", "Category1"};
    std::ofstream outFile("food_data.dat", std::ios::binary);
    if (outFile.is_open()) {
        outFile << food1;
        outFile.close();
        std::cout << "Datos escritos correctamente en el archivo." << std::endl;
    } else {
        std::cerr << "Error al abrir el archivo para escritura." << std::endl;
    }

  
    FoodC<int> food2;
    std::ifstream inFile("food_data.dat", std::ios::binary);
    if (inFile.is_open()) {
        inFile >> food2;
        inFile.close();
        std::cout << "Datos leidos correctamente desde el archivo:" << std::endl;
        
        std::cout << "Key: " << food2.key << std::endl;
        std::cout << "Brand Owner: " << food2.brand_owner << std::endl;
        std::cout << "Description: " << food2.description << std::endl;
        std::cout << "Ingredients: " << food2.ingredients << std::endl;
        std::cout << "Serving Size Unit: " << food2.serving_size_unit << std::endl;
        std::cout << "Branded Food Category: " << food2.branded_food_category << std::endl;
    } else {
        std::cerr << "Error al abrir el archivo para lectura." << std::endl;
    }

    return 0;
}
