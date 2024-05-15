#include "parserSQL.h"

Parser::Parser(Scanner* scanner, AVLFile* avlFile) : scanner(scanner), avlFile(avlFile) {
    currentToken = scanner->nextToken();
}
//cambiar de acuerdo a lo que implementaron
/*Parser::Parser(Scanner *scanner, ISAMFile* isamFile): scanner(scanner), isamFile(isamFile) {
    currentToken = scanner->nextToken();
}
Parser::Parser(Scanner *scanner, ExtendibleFile* extendibleFile): scanner(scanner), extendibleFile(extendibleFile) {
    currentToken = scanner->nextToken();
}*/

void Parser::parse() {
    while (currentToken->type != Token::END) {
        parseStatement();
        if (currentToken->type == Token::SEMICOLON) {
            currentToken = scanner->nextToken();
        } else if (currentToken->type != Token::END) {
            error("Expected token: SEMICOLON but got: " + string(Token::token_names[currentToken->type]));
        }
    }
}

void Parser::parseStatement() {
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

void Parser::parseCreateTable() {
    expect(Token::CREATE);
    expect(Token::TABLE);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::FROM);
    expect(Token::FILE);
    string fileName = expect(Token::VALUE)->lexema;
    expect(Token::USING);
    expect(Token::INDEX);

    Token* indexType = expectOneOf({Token::AVL, Token::ISAM, Token::EXTENDIBLE});
    expect(Token::LPARENT);
    string indexField = expect(Token::VALUE)->lexema;
    expect(Token::RPARENT);

    Table table = {tableName, fileName, Token::token_names[indexType->type]};
    tables.push_back(table);

    if (indexType->type == Token::AVL) {
        avlFile->loadData(fileName);
    }
    vector<Record> sortedData = avlFile->inorder();
    for (Record& rec : sortedData) {
        rec.showData();
    }
    /*
     * CAMBIAR LAS FUNCIONES PARA IMPRIMIR VALORES DEL ARCHIVO
    if (indexType->type == Token::ISAM) {
        isamFile->loadData(fileName);
    }
    vector<Record> sortedData = isamFile->display();
    for (Record& rec : sortedData) {
        rec.showData();
    }
    if (indexType->type == Token::EXTENDIBLE) {
        isamFile->loadData(fileName);
    }
    vector<Record> sortedData = isamFile->display();
    for (Record& rec : sortedData) {
        rec.showData();
    }*/


    cout << "---- Created table " << tableName << " from file " << fileName << " using index " << indexField << " (" << Token::token_names[indexType->type] << ")" << endl;
}

void Parser::parseSelect() {
    expect(Token::SELECT);
    expect(Token::ALL);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();
    if (condition.op == "=") {
        Record result = avlFile->find(stoi(condition.value1));
        cout << "---- Selecting from " << tableName << " where " << condition.field << " = " << condition.value1 << ": " << result.toString() << endl;
    } else if (condition.op == "between") {
        vector<Record> results;
        avlFile->rangeSearch(stoi(condition.value1), stoi(condition.value2), results);
        cout << "-----Selecting from " << tableName << " where " << condition.field << " between " << condition.value1 << " and " << condition.value2 << ": ";
        for (Record& rec : results) {
            rec.showData();
        }
    }
}


void Parser::parseInsert() {
    expect(Token::INSERT);
    expect(Token::INTO);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::VALUES);
    expect(Token::LPARENT);
    vector<string> values = parseValues();
    expect(Token::RPARENT);

    Record record;
    record.fdc_id = stoi(values[0]);
    strncpy(record.brand, values[1].c_str(), sizeof(record.brand) - 1);
    strncpy(record.description, values[2].c_str(), sizeof(record.description) - 1);
    strncpy(record.ingredients, values[3].c_str(), sizeof(record.ingredients) - 1);
    record.servingsize = stof(values[4]);
    avlFile->insert(record);
    cout << "---- Inserting into " << tableName << " values: ";
    for (const string& value : values) {
        cout << value << " ";
    }
    cout << endl;
    //opcional
    cout << "--- Ver lemento insertado, en la data ---- \n"<<endl;
    vector<Record> sortedData = avlFile->inorder();
    for (Record& rec : sortedData) {
        rec.showData();
    }
}

void Parser::parseDelete() {
    expect(Token::DELETE);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();

    avlFile->remove(stoi(condition.value1));

    cout << "Deleting from " << tableName << " where " << condition.field << " = " << condition.value1;
    if (!condition.value2.empty()) {
        cout << " and " << condition.value2;
    }
    cout << endl;
    //opcional
    cout <<"---- Ver registro eliminado de la tabla -----"<<endl;
    vector<Record> sortedData = avlFile->inorder();
    for (Record& rec : sortedData) {
        rec.showData();
    }

}

Condition Parser::parseCondition() {
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

vector<string> Parser::parseValues() {
    vector<string> values;
    values.push_back(expect(Token::VALUE)->lexema);
    while (currentToken->type == Token::COLON) {
        expect(Token::COLON);
        values.push_back(expect(Token::VALUE)->lexema);
    }
    return values;
}

Token* Parser::expect(Token::Type type) {
    if (currentToken->type == type) {
        Token* token = currentToken;
        currentToken = scanner->nextToken();
        return token;
    } else {
        error("Expected token: " + string(Token::token_names[type]) + " but got: " + string(Token::token_names[currentToken->type]));
        return nullptr;
    }
}

Token* Parser::expectOneOf(const std::initializer_list<Token::Type>& types) {
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

void Parser::error(const string& message) {
    cerr << "Error: " << message << endl;
    exit(1);
}
