//
// Created by Paola Maguina aranda on 2024-05-12.
//

#ifndef PROYECTO1_PARSERSQL_H
#define PROYECTO1_PARSERSQL_H

#include <iostream>
#include <vector>
#include <string>
#include "tokensSQL.h"

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
    Parser(Scanner* scanner) : scanner(scanner) {
        currentToken = scanner->nextToken();
    }

    void parse() {
        while (currentToken->type != Token::END) {
            parseStatement();
            if (currentToken->type == Token::SEMICOLON) {
                currentToken = scanner->nextToken();
            } else if (currentToken->type != Token::END) {
                error("Expected token: SEMICOLON but got: " + string(Token::token_names[currentToken->type]));
            }
        }
    }

private:
    Scanner* scanner;
    Token* currentToken;
    vector<Table> tables;

    void parseStatement() {
        if (currentToken->type == Token::CREATE) {
            parseCreateTable();
        } else if (currentToken->type == Token::SELECT) {
            parseSelect();
        } else if (currentToken->type == Token::INSERT) {
            parseInsert();
        } else if (currentToken->type == Token::DELETE) {
            parseDelete();
        } else {
            error("Unexpected token: " + string(Token::token_names[currentToken->type]));
        }
    }

    void parseCreateTable() {
        expect(Token::CREATE);
        expect(Token::TABLE);
        string tableName = expect(Token::ID)->lexema;
        expect(Token::FROM);
        expect(Token::FILE);
        string fileName = expect(Token::VALUE)->lexema;
        expect(Token::USING);
        expect(Token::INDEX);

        // Aceptar AVL, ISAM, o EXTENDIBLE
        Token* indexType = expectOneOf({Token::AVL, Token::ISAM, Token::EXTENDIBLE});

        expect(Token::LPARENT);
        string indexField = expect(Token::VALUE)->lexema;
        expect(Token::RPARENT);
        Table table = {tableName, fileName, Token::token_names[indexType->type]};
        tables.push_back(table);
        cout << "Created table " << tableName << " from file " << fileName << " using index " << indexField << " (" << Token::token_names[indexType->type] << ")" << endl;
    }

    void parseSelect() {
        expect(Token::SELECT);
        expect(Token::ALL);
        expect(Token::FROM);
        string tableName = expect(Token::ID)->lexema;
        expect(Token::WHERE);
        Condition condition = parseCondition();
        cout << "Selecting from " << tableName << " where " << condition.field << " " << condition.op << " " << condition.value1;
        if (!condition.value2.empty()) {
            cout << " and " << condition.value2;
        }
        cout << endl;
    }

    void parseInsert() {
        expect(Token::INSERT);
        expect(Token::INTO);
        string tableName = expect(Token::ID)->lexema;
        expect(Token::VALUES);
        expect(Token::LPARENT);
        vector<string> values = parseValues();
        expect(Token::RPARENT);
        cout << "Inserting into " << tableName << " values: ";
        for (const string& value : values) {
            cout << value << " ";
        }
        cout << endl;
    }

    void parseDelete() {
        expect(Token::DELETE);
        expect(Token::FROM);
        string tableName = expect(Token::ID)->lexema;
        expect(Token::WHERE);
        Condition condition = parseCondition();
        cout << "Deleting from " << tableName << " where " << condition.field << " " << condition.op << " " << condition.value1;
        if (!condition.value2.empty()) {
            cout << " and " << condition.value2;
        }
        cout << endl;
    }

    Condition parseCondition() {
        string field = expect(Token::ID)->lexema;
        Token* opToken = currentToken;
        if (opToken->type == Token::EQUAL) {
            expect(Token::EQUAL);
            string value = expect(Token::VALUE)->lexema;
            return {field, "=", value, ""};
        } else if (opToken->type == Token::BETWEEN) {
            expect(Token::BETWEEN);
            string value1 = expect(Token::VALUE)->lexema;
            expect(Token::AND);
            string value2 = expect(Token::VALUE)->lexema;
            return {field, "between", value1, value2};
        } else {
            error("Expected = or between");
            return {};
        }
    }

    vector<string> parseValues() {
        vector<string> values;
        values.push_back(expect(Token::VALUE)->lexema);
        while (currentToken->type == Token::COLON) {
            expect(Token::COLON);
            values.push_back(expect(Token::VALUE)->lexema);
        }
        return values;
    }

    Token* expect(Token::Type type) {
        if (currentToken->type == type) {
            Token* token = currentToken;
            currentToken = scanner->nextToken();
            return token;
        } else {
            error("Expected token: " + string(Token::token_names[type]) + " but got: " + string(Token::token_names[currentToken->type]));
            return nullptr;
        }
    }

    Token* expectOneOf(const vector<Token::Type>& types) {
        for (Token::Type type : types) {
            if (currentToken->type == type) {
                Token* token = currentToken;
                currentToken = scanner->nextToken();
                return token;
            }
        }
        string expected = "";
        for (Token::Type type : types) {
            if (!expected.empty()) expected += ", ";
            expected += Token::token_names[type];
        }
        error("Expected one of: " + expected + " but got: " + string(Token::token_names[currentToken->type]));
        return nullptr;
    }

    void error(const string& message) {
        cerr << "Error: " << message << endl;
        exit(1);
    }
};
#endif //PROYECTO1_PARSERSQL_H
