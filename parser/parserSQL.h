//
// Created by Paola Maguina aranda on 2024-05-12.
//

#ifndef PROYECTO1_PARSERSQL_H
#define PROYECTO1_PARSERSQL_H
#include <exception>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include "../tecnicas/AVL/AVL.h"
#include "../tecnicas/AVL/Record.h"
#include "tokensSQL.h"
#include <utility>
#include <vector>
using namespace std;

inline vector<Record> records;
inline string error_message = "";
inline vector<pair<string,string> > tablas(2);
inline pair<string, string> tabla;


class Parser {
    Scanner* scanner;
    Token* current, * previous;
    vector<string> atributos;
    vector<pair<string, string>> values;
    pair<string, string> value;
public:
    Parser(Scanner* sc):scanner(sc) {
        previous = current = NULL;
        return;
    }
    void parse(){
        current = scanner->nextToken();
        if(check(Token::ERR)){
            error_message = "\tCaracter invalido";
            return;
        }
        parseStatement();

        // if (current->type != Token::SEMICOLON)
        //   error_message = "Esperaba fin de input.";
        // else
        //   cout<< "Fin de la ejecucion" << endl;
        // if(current) delete current;
        return;
    }

private:
    void mostrar(){
        cout<<"Atributos: "<<endl;
        for (auto const &v: atributos){
            cout<<v<<", ";
        }
        cout<<endl;
    }

    void mostrarValues(){
        for(auto it : values){
            cout<<it.first<<": "<<it.second<<endl;
        }
    }

    void insertarValues(){
        try{
            if(tabla.second == "record1"){
                int fdc_id;
                char brand[50];
                char description[30];
                char ingredients[110];
                float servingsize;
                fdc_id = values[0].first;
                strncpy(brand, values[1].first.c_str(), sizeof(brand) - 1);
                brand[sizeof(brand) - 1 ]= '\0';
                strncpy(description, values[2].first.c_str(), sizeof(description) - 1);
                description[sizeof(description) - 1 ]= '\0';
                strncpy(ingredients, values[3].first.c_str(), sizeof(ingredients) - 1);
                ingredients[sizeof(ingredients) - 1 ]= '\0';

                Record record(fdc_id,brand,description,ingredients,servingsize);
                bool added = method->add(record);
                if(!added)
                    error_message = "\tYa existe este elemento";
                cout<<"Insertado"<<endl;
                error_message = "";
            }
            else{
            }
        } catch (const std::exception& e){
            error_message = "\tValores incorrectos para la tabla";
        }
    }

    bool match(Token::Type ttype) {
        if (check(ttype)) {
            advance();
            return true;
        }
        return false;
    }

    bool check(Token::Type ttype) {
        if (isAtEnd()) return false;
        return current->type == ttype;
    }


    bool advance() {
        if (!isAtEnd()) {
            Token* temp =current;
            if (previous) delete previous;
            current = scanner->nextToken();
            previous = temp;
            if (check(Token::ERR)) {
                error_message = "\tParse error, unrecognised character";
                return false;
            }
            return true;
        }
        return false;
    }

    bool isAtEnd() {
        return (current->type == Token::END);
    }


    void parseStatement(){
        if(match(Token::SELECT)){
            if(match(Token::ID)){
                parseExpression();
            }
            else if(match(Token::ALL))
                parseTable(nullptr, "");
            else
                error_message =  "\tError esperaba los atributos";
            return;
        }
        else if(match(Token::CREATE)){
            if(match(Token::TABLE)){
                Token* temp = previous;
                if(match(Token::ID)){
                    parseTable(temp, previous->lexema);
                    return;
                }
                error_message = "\tSe esperaba un nombre de tabla";
                return;
            }
            error_message = "\tError de sintaxis";
            return;
        }
        else if(match(Token::INSERT)){
            if(match(Token::INTO)){
                if(match(Token::ID)){
                    string tabla_nombre = previous->lexema;
                    if(!match_tabla(tabla_nombre)){
                        error_message = "\tNo existe esa tabla";
                        return;
                    }
                    if(match(Token::VALUES)){
                        bool res = parseValuesSentence(tabla_nombre);
                        return;
                    }
                    error_message = "\tEsperaba la sentencia VALUES";
                    return;
                }
                error_message = "\tEsperaba el nombre de la tabla";
                return;
            }
            error_message = "\tEspera INTO en despues del INSERT.";
            return;
        }
        else if(match(Token::DELETE)){
            parseTableDelete();
            return;
        }
        error_message = "\tError esperaba una sentencia SQL.";
    }
    bool parseValuesSentence(string nombre_tabla){
        if(match(Token::LPARENT)){
            bool res = parseValuesList();
            if(!res) return false;
            if(match(Token::RPARENT)){
                if(match(Token::SEMICOLON)){
                    insertarValues();
                    return true;
                }
                error_message = "\tEsperaba ;";
                return false;
            }
            error_message = "\tEsperaba )";
            return false;
        }
        error_message = "\tSe esperaba (.";
        return false;
    }

    bool parseValuesList(){
        bool res = parseValues();
        while(match(Token::COLON)){
            res = parseValues();
        }
        return res;
    }

    bool parseValues(){
        if(match(Token::QUOTE)){
            if(match(Token::ID)){
                string value = previous->lexema;
                if(match(Token::QUOTE)){
                    values.push_back(make_pair(value, "string"));
                    return true;
                }
                error_message = "\tEsperaba \"";
                return false;
            }
            error_message = "\tSe esperaba un dato";
            return false;
        }
        else if(match(Token::NUM)){
            values.push_back(make_pair(previous->lexema, "int"));
            return true;
        }
        else if(match(Token::FLOAT)){
            values.push_back(make_pair(previous->lexema, "float"));
            return true;
        }
        else if(match(Token::TRUE)){
            values.push_back(make_pair("true", "bool"));
            return true;
        }
        else if(match(Token::FALSE)){
            values.push_back(make_pair("false", "bool"));
            return true;
        }
        error_message = "\tSintaxis incorrecta";
        return false;
    }


    void parseExpression(){
        atributos.push_back(previous->lexema);
        if(match(Token::COLON)){
            advance();
            parseExpression();
        }
        else
            parseTable(nullptr, "");
    }

    void parseTable(Token* temp, string nombre_tabla){
        if(match(Token::FROM)){
            if(match(Token::ID)){
                nombre_tabla = previous->lexema;
                if(match(Token::SEMICOLON)){
                    if(!match_tabla(nombre_tabla)){
                        error_message = "\tNo existe la tabla " + nombre_tabla;
                        return;
                    }
                    cout<<tabla.second<<endl;

                    if(tabla.second == "record1")
                        records = method->load();
                    else
                        records = method1->load();
                    atributos.clear();
                    error_message = "";
                    return;
                }
                if(nombre_tabla != tabla.first){
                    error_message = "\tNo existe la tabla " + nombre_tabla;
                    return;
                }
                parseCondition(nombre_tabla);
                return;
            }
            else if(temp!=nullptr && match(Token::FILE)){
                parseFile(nombre_tabla);
                return;
            }
            error_message = "\tError de sintaxis.";
            return;
        }
        error_message = "\tError de sintaxis";
    }
    void parseTableDelete(){
        if(match(Token::FROM)){
            if(match(Token::ID)){
                string nombre_table = previous->lexema;
                if(!match_tabla(nombre_table)){
                    error_message = "\tNo existe la tabla "+ nombre_table;
                    return;
                }
                bool r = parseConditionDelete(nombre_table);
                return;
            }
            error_message  = "\tSe esperaba el nombre de la tabla.";
            return;
        }
        error_message = "\tSe esperaba la sentencia FROM, se obtuvo ";
    }

    bool parseConditionDelete(string nombre_tabla){
        if (match(Token::WHERE)){
            if(match(Token::ID)){
                if(match(Token::EQUAL)){
                    bool r = parseEqual();
                    if(r){
                        if(match(Token::SEMICOLON)){
                            cout<<" Eliminacion"<<endl;
                            deleteKey(nombre_tabla);
                            error_message = "";
                            return true;
                        }
                        error_message = "\tSintaxis incorrecta, se esperaba ;";
                        return false;
                    }
                    return false;
                }
                error_message = "\tSe esperaba la un '=' se encontro ";
            }
            error_message = "\tSe esperaba un id" ;
            return false;
        }
        error_message = "\tSe esperaba un WHERE";
        return false;
    }

    bool match_tabla(string nombre_tabla){
        cout<<tablas[0].first<<endl;
        cout<<tablas[1].first<<endl;
        if(nombre_tabla == tablas[0].first){
            tabla = tablas[0];
            return true;
        }
        else if(nombre_tabla == tablas[1].first){
            tabla = tablas[1];
            return true;
        }
        return false;
    }

    void deleteKey(string nombre_tabla){
        try {
            bool deleted;
            if(tabla.second == "record2"){
                char key[20];
                strncpy(key, value.first.c_str(), sizeof(key) - 1);
                key[sizeof(key) - 1 ]= '\0';
                deleted = method1->remove(key);
            }else if (tabla.second == "record1"){
                int key = stoi(value.first);
                deleted = method->remove(key);
            }
            if(!deleted){
                error_message = "\tNo existe este valor.";
                return;
            }
        }catch(const std::exception &e){
            error_message = "\tError al eliminar.";
        }
    }

    void parseFile(string nombre_tabla){
        string filename = "";
        if(match(Token::QUOTE)){
            if(match(Token::FILENAME)){
                filename = previous->lexema;
                if(match(Token::QUOTE)){
                    parseIndex(filename, nombre_tabla);
                    return;
                }
                error_message = "\tSe esperaba \"";
                return;
            }
            error_message = "\tSe esperaba el nombre del archivo";
            return;
        }
        error_message = "\tSe esperaba '";
        return;
    }

    void parseIndex(string filename, string nombre_tabla){
        if(match(Token::USING)){
            if(match(Token::INDEX)){
                parseIndexType(filename, nombre_tabla);
                return;
            }
            error_message = "\tSintaxis incorrecta";
            return;
        }
        error_message = "\tSintaxis incorrecta";
        return;
    }

    void parseIndexType(string filename, string nombre_tabla){
        string archivo = "";
        int i = filename.length()-1;
        while (filename[i] != '/'){
            archivo = filename[i] + archivo;
            i--;
        }
        cout<<"archivo: "<<archivo<<endl;
        if(match(Token::BPLUS)){
            if(match(Token::SEMICOLON)){
                if(archivo == "datos.csv"){
                    remove("bplus_datos.dat");
                    remove("bplus_index.dat");
                    method = new BPlusFile<Record,int, sizeof(int)>();
                    if (nombre_tabla == tablas[1].first) {
                        tablas[1].first = "";
                    }
                    tablas[0] = make_pair(nombre_tabla, "record1");
                    tabla = tablas[0];
                    insertCsv(filename);
                }
                else if(archivo == "datos2.csv"){
                    remove("bplus_datos.dat");
                    remove("bplus_index.dat");
                    method1 = new BPlusFile<Record2,char*, 20>();
                    if (nombre_tabla == tablas[0].first) {
                        tablas[0].first = "";
                    }
                    tablas[1] = make_pair(nombre_tabla, "record2");
                    tabla = tablas[1];
                    insertCsv2(filename);
                }

                error_message = "";
                cout<<"Se crea tabla con index bplus"<<endl;

                return;
            }
            error_message = "\tSe esperaba un ;";
            return;
        }
        else if(match(Token::AVL)){
            if(match(Token::SEMICOLON)){
                if(archivo == "datos.csv"){
                    remove("AVLdata.dat");
                    method = new AVLFile<Record,int>();
                    if (nombre_tabla == tablas[1].first) {
                        tablas[1].first = "";
                    }
                    tablas[0] = make_pair(nombre_tabla, "record1");
                    tabla = tablas[0];
                    insertCsv(filename);
                }
                else if(archivo == "datos2.csv"){
                    remove("AVLdata.dat");
                    method1 = new AVLFile<Record2,char*>();
                    if (nombre_tabla == tablas[0].first) {
                        tablas[0].first = "";
                    }
                    tablas[1] = make_pair(nombre_tabla, "record2");

                    tabla = tablas[1];
                    insertCsv2(filename);
                }

                cout<<"Se crea tabla con index avl"<<endl;
                error_message = "";
                return;
            }
            error_message ="\tSe esperaba un ;";
            return;
        }
        else if(match(Token::SEQUENTIAL)){
            if(match(Token::SEMICOLON)){
                if(archivo == "datos.csv"){
                    remove("sequential_datos.dat");
                    remove("sequential_aux.dat");
                    method = new SequentialFile<Record,int>();
                    if (nombre_tabla == tablas[1].first) {
                        tablas[1].first = "";
                    }
                    tablas[0] = make_pair(nombre_tabla, "record1");
                    tabla = tablas[0];
                    insertCsv(filename);
                }
                else if(archivo == "datos2.csv"){
                    remove("sequential_datos.dat");
                    remove("sequential_aux.dat");
                    method1 = new SequentialFile<Record2,char*>();
                    if (nombre_tabla == tablas[0].first) {
                        tablas[0].first = "";
                    }
                    tablas[1] = make_pair(nombre_tabla, "record2");
                    tabla = tablas[1];
                    insertCsv2(filename);
                }

                cout<<"Se crea tabla con index sequential"<<endl;
                error_message = "";
                return;
            }
            error_message = "\tSe esperaba un ;";
            return;
        }
        error_message = "\tSe esperaba index para hashear";
        return;
    }

    void parseCondition(string nombre_tabla){
        if (match(Token::WHERE)){
            if(match(Token::ID)){
                parseCondition2(nombre_tabla);
                return;
            }
            error_message = "\tSe esperaba un id";
            return;
        }
        error_message = "\tSe esperaba un WHERE";
        return;
    }

    void parseCondition2(string nombre_tabla){
        if(match(Token::EQUAL)){
            bool v = parseEqual();
            if(v){
                cout << previous->lexema << endl;
                if(match(Token::SEMICOLON)){

                    bool s = search_key(nombre_tabla);

                    if(!s){
                        error_message = "                                                                   No existe un elemento con la llave buscada";
                        return;
                    }
                    atributos.clear();
                    error_message = "";
                    return;
                }
                error_message = "\tSintaxis incorrecta";
                return;
            }

            return;
        }
        else if(match(Token::BETWEEN)){
            cout<<"Between"<<endl;
            parse_range(nombre_tabla);
        }
        else
            error_message = "\tSintaxis incorrecta";
        return;
    }


    bool search_key(string nombre_tabla){
        try {

            if(tabla.second == "record2"){
                records1.clear();
                char key[20];
                strncpy(key, value.first.c_str(), sizeof(key) - 1);
                key[sizeof(key) - 1 ]= '\0';
                pair<Record2,bool> result = method1->search(key);
                if(!result.second){
                    error_message = "\tNo existe este valor.";
                    return false;
                }else{
                    records1.push_back(result.first);
                    return true;
                }
            }else if (tabla.second == "record1"){
                records.clear();
                int key = stoi(value.first);
                pair<Record,bool> result = method->search(key);
                cout <<"Rpta: "<< result.second << endl;
                if(!result.second){
                    error_message = "\tNo existe este valor.";
                    return false;
                }else{
                    records.push_back(result.first);
                    return true;
                }
            }

        }catch(const std::exception &e){
            error_message = "\tError al eliminar.";
        }
    }

    bool parseEqual(){
        if(match(Token::NUM)){
            value = make_pair(previous->lexema, "int");
            return true;
        }
        else if(match(Token::QUOTE)){
            if(match(Token::ID)){
                string dato = previous->lexema;
                if(!match(Token::QUOTE)){
                    error_message = "\tError de sintaxis, esperaba un \"";
                    return false;
                }
                value = make_pair(dato, "char");
                return true;
            }
            error_message = "\tSintaxis incorrecta";
            return false;
        }
        error_message  = "\tSintaxis incorrecta";
        return false;
    }

    void parse_range(string nombre_tabla){
        if(match(Token::NUM)){
            int begin = stoi(previous->lexema);
            // cout<"<"numero: "<<previous->lexema<<endl;
            if(match(Token::AND)){
                if(match(Token::NUM)){
                    int end = stoi(previous->lexema);
                    if(match(Token::SEMICOLON)){
                        records.clear();
                        cout<<"Busqueda por rango"<<endl;
                        error_message = "";
                        records = method->rangeSearch(begin, end);
                        return;
                    }
                    error_message = "\tSintaxis Incorrecta";
                    return;
                }
                error_message = "\tSintaxis Incorrecta";
                return;
            }
            error_message = "\tSintaxis Incorrecta";
            return;
        }
        error_message = "\tSintaxis Incorrecta";
        return;
    }

    void insertCsv(string ruta){
        cout << ruta << endl;
        ifstream archivo(ruta);
        cout<<"ruta: "<<ruta<<endl;
        if (!archivo.is_open()) {
            error_message = "\tLa ruta del archivo es incorrecta.";
            return;
        }

        string linea;
        vector<string> campos;
        istringstream lineaStream(linea);
        int counter = 0;
        while (getline(archivo, linea)) {
            istringstream lineaStream(linea);
            string campo;

            while (getline(lineaStream, campo, ',')) {
                campos.push_back(campo);
            }

            //char key[20];
            int key;
            char nombre[20];
            char producto[20];
            char marca[20];
            float precio;
            int cantidad;

            key = stoi(campos[0]);

            //strncpy(key, campos[0].c_str(), sizeof(key) - 1);
            //nombre[sizeof(key) - 1 ]= '\0';

            strncpy(nombre, campos[1].c_str(), sizeof(nombre) - 1);
            nombre[sizeof(nombre) - 1 ]= '\0';


            strncpy(producto, campos[2].c_str(), sizeof(producto) - 1);
            producto[sizeof(producto) - 1 ]= '\0';

            strncpy(marca, campos[3].c_str(), sizeof(marca) - 1);
            marca[sizeof(marca) - 1 ]= '\0';

            // cout << campos[4] << endl;
            precio = stof(campos[4]);
            cantidad = stoi(campos[5]);
            // cout << "--------" << endl;
            Record record(key,nombre,producto,marca,precio,cantidad);
            bool asd__ = method->add(record);
            // cout << "---*----" << endl;
            // method->display_all();
            if(counter == 1000)
                break;
            counter ++;
            campos.clear();
        }
        // method->display_all();
    }

    void insertCsv2(string ruta){
        cout << ruta << endl;
        ifstream archivo(ruta);
        cout<<"ruta: "<<ruta<<endl;
        if (!archivo.is_open()) {
            error_message = "\tLa ruta del archivo es incorrecta.";
            return;
        }

        string linea;
        vector<string> campos;
        istringstream lineaStream(linea);
        int counter = 0;
        while (getline(archivo, linea)) {
            istringstream lineaStream(linea);
            string campo;

            while (getline(lineaStream, campo, ',')) {
                campos.push_back(campo);
            }

            char key[20];
            char genero[20];
            char profesion[20];
            int edad;
            float sueldo;


            strncpy(key, campos[0].c_str(), sizeof(key) - 1);
            key[sizeof(key) - 1 ]= '\0';

            strncpy(genero, campos[1].c_str(), sizeof(genero) - 1);
            genero[sizeof(genero) - 1 ]= '\0';

            strncpy(profesion, campos[2].c_str(), sizeof(profesion) - 1);
            profesion[sizeof(profesion) - 1 ]= '\0';

            // cout << campos[4] << endl;
            edad = stoi(campos[3]);
            sueldo = stof(campos[4]);

            // cout << "--------" << endl;
            Record2 record(key,genero,profesion,edad,sueldo);
            bool asd__ = method1->add(record);
            // cout << "---*----" << endl;
            // method->display_all();
            if(counter == 100000)
                break;
            counter ++;
            campos.clear();
        }
        // method->display_all();
    }
};
#endif //PROYECTO1_PARSERSQL_H
