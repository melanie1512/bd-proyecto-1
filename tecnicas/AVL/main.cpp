
#include "AVL.h"
#include "Record.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "httplib.h"

using namespace std;
using json = nlohmann::json;
using namespace httplib;
//cambiar direccion al ejecutar!!!
const string DIR = "/Users/paolamaguinaaranda/Documents/2024-I/BD2/proyecto1/tecnicas";

//test manual de avl
void testAVL(string filename) {
    AVLFile file(filename);
    ifstream csvFile(DIR + "/data/data.csv");
    if (!csvFile.is_open()) {
        std::cerr << "Error opening CSV file!" << endl;
        return;
    } else {
        std::cout << "CSV file opened successfully!" << endl;
    }
    csvFile.close();

    file.loadData(DIR + "/data/data.csv");

    // Datos de prueba adicionales
    vector<Record> testData = {
            {1106920, "Richardson Oilseed Products (US) Limited", "WESSON Vegetable Oil 1 GAL", "Vegetable Oil", 15.0},
            {1106935, "CAMPBELL SOUP COMPANY", "SWANSON BROTH BEEF", "INGREDIENTS: BEEF STOCK, CONTAINS LESS THAN 2% OF: MIREPOIX (CARROTS, CELERY, ONIONS), SALT, NATURAL", 240.0},
            {1107939, "CAMPBELL SOUP COMPANY", "CAMPBELL'S SOUP BEAN AND HAM", "INGREDIENTS: WATER, PEA BEANS, CARROTS, COOKED HAM - WATER ADDED (PORK, WATER, SALT, SUGAR, SODIUM", 412.0}
    };
    //insertado de la data

    for (Record& rec : testData) {
        std::cout << "insert: " << rec.fdc_id << endl;
        file.insert(rec);
    }
    //muestra de datos ordenados

    cout << "--------- Showing all sorted data -----------\n";
    vector<Record> sortedData = file.inorder();
    for (Record& rec : sortedData) {
        rec.showData();
    }
    //muestra de elementos encontrados
    cout << "------Show data-----------\n";
    Record f2 = file.find(1106935);
    f2.showData();

    cout << "----Eliminacion -----\n";
    file.remove(1107939);
    //muestra la data despues de la eliminación
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
//backend de interfaz
/*AVLFile* file = nullptr;

void handle_insert(const Request& req, Response& res) {
    try {
        json record_json = json::parse(req.body);
        Record newRecord(
                record_json["fdc_id"],
                record_json["brand"],
                record_json["description"],
                record_json["ingredients"],
                record_json["servingsize"]
        );

        file->insert(newRecord);
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("Record inserted", "text/plain");
    } catch (const std::exception& e) {
        res.status = 500;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(e.what(), "text/plain");
        // Agregar registros detallados
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Request body: " << req.body << std::endl;
    }
}


void handle_traversal(const Request& req, Response& res) {
    vector<Record> result = file->inorder();
    json response = json::array();
    for (const auto& rec : result) {
        response.push_back({
                                   {"fdc_id", rec.fdc_id},
                                   {"brand", rec.brand},
                                   {"description", rec.description},
                                   {"ingredients", rec.ingredients},
                                   {"servingsize", rec.servingsize}
                           });
    }
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(response.dump(), "application/json");
}


void handle_delete(const Request& req, Response& res) {
    try {
        json record_json = json::parse(req.body);

        if (!record_json.contains("fdc_id")) {
            throw std::runtime_error("JSON data is missing required field: fdc_id.");
        }

        int fdc_id = record_json["fdc_id"];
        file->remove(fdc_id);
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("Record deleted", "text/plain");
    } catch (const std::exception& e) {
        res.status = 500;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(e.what(), "text/plain");
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Request body: " << req.body << std::endl;
    }
}

void loadDataIfEmpty(AVLFile &file) {
    fstream binFile(DIR + "/bin/avl_data.bin", ios::in | ios::binary);
    binFile.seekg(0, ios::end);
    if (binFile.tellg() == 0) {
        cout << "Binary file is empty, loading data from CSV." << endl;
        binFile.close();
        file.loadData(DIR + "/data/data.csv");
    } else {
        cout << "Binary file is not empty." << endl;
        binFile.close();

        vector<Record> result = file.inorder();
        cout << "Records loaded from binary file: " << result.size() << endl;
        for (const auto& rec : result) {
            cout << "Record: " << rec.fdc_id << ", " << rec.brand << ", " << rec.description << ", " << rec.ingredients << ", " << rec.servingsize << endl;
        }
    }
}
void handle_range_search(const Request& req, Response& res) {
    try {
        json record_json = json::parse(req.body);

        if (!record_json.contains("start") || !record_json.contains("end")) {
            throw std::runtime_error("JSON data is missing required fields: start or end.");
        }

        int start = record_json["start"];
        int end = record_json["end"];
        vector<Record> result;
        file->rangeSearch(start, end,result);
        json response = json::array();
        for (const auto& rec : result) {
            response.push_back({
                                       {"fdc_id", rec.fdc_id},
                                       {"brand", rec.brand},
                                       {"description", rec.description},
                                       {"ingredients", rec.ingredients},
                                       {"servingsize", rec.servingsize}
                               });
        }
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(response.dump(), "application/json");
    } catch (const std::exception& e) {
        res.status = 500;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(e.what(), "text/plain");
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Request body: " << req.body << std::endl;
    }
}
void handle_create_table(const Request& req, Response& res) {
    try {
        json record_json = json::parse(req.body);
        if (!record_json.contains("file_path") || !record_json.contains("index_field")) {
            throw std::runtime_error("JSON data is missing required fields: file_path or index_field.");
        }
        std::string file_path = record_json["file_path"];
        std::string index_field = record_json["index_field"];
        // Cargar datos desde el archivo CSV
        file->loadData(file_path); // Llama a loadData directamente
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("Table created and data loaded from CSV", "text/plain");
    } catch (const std::exception& e) {
        res.status = 500;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(e.what(), "text/plain");
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Request body: " << req.body << std::endl;
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

    file = new AVLFile(DIR + "/bin/avl_data.bin");
    loadDataIfEmpty(*file);

    Server svr;

    svr.Post("/insert", handle_insert);
    svr.Get("/traversal", handle_traversal);
    svr.Post("/delete", handle_delete);
    svr.Post("/range_search", handle_range_search);
    //svr.Post("/create_table", handle_create_table);

    svr.Options(R"(/\w*)", [](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    svr.set_pre_routing_handler([](const Request& req, Response& res) {
        if (req.method == "OPTIONS") {
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
            res.status = 204;
            return Server::HandlerResponse::Handled;
        }
        return Server::HandlerResponse::Unhandled;
    });

    std::cout << "Server started at http://localhost:8080" << std::endl;
    svr.listen("localhost", 8080);

    delete file;
    return 0;
}*/
