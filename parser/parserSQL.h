//
// Created by Paola Maguina aranda on 2024-05-12.
//
// parserSQL.h
#ifndef PROYECTO1_PARSERSQL_H
#define PROYECTO1_PARSERSQL_H

#include <iostream>
#include <vector>
#include <string>
#include "tokensSQL.h"
#include "AVL.h"
//#include "ISAM.h"
//#include "Extendible.h"

using namespace std;

struct Table {
    string name;
    string file;
    string index;
};

struct Condition {
    string field;
    string op;
    string value1;
    string value2;
};

class Parser {
public:
    Parser(Scanner* scanner, AVLFile* avlFile);
    void parse();

private:
    Scanner* scanner;
    Token* currentToken;
    vector<Table> tables;
    AVLFile* avlFile;
    ISAMFile* isamFile;
    Extendible* extendibleFile;

    void parseStatement();
    void parseCreateTable();
    void parseSelect();
    void parseInsert();
    void parseDelete();
    Condition parseCondition();
    vector<string> parseValues();
    Token* expect(Token::Type type);
    Token* expectOneOf(const std::initializer_list<Token::Type>& types);
    void error(const string& message);
};

#endif //PROYECTO1_PARSERSQL_H

