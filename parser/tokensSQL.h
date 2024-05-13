//
// Created by Paola Maguina aranda on 2024-05-12.
//

#ifndef PROYECTO1_TOKENSSQL_H
#define PROYECTO1_TOKENSSQL_H
#include <cctype>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include <map>

using namespace std;

class Token {
public:
    enum Type { SELECT, CREATE, TABLE, FROM, ALL, WHERE, DELETE, EQUAL, BETWEEN, AND, INSERT, INTO, VALUES, FILE, LPARENT, RPARENT, INDEX, USING, AVL, END, ERR, SEMICOLON, COLON, ID, EOL, NUM, VALUE, QUOTE, FILENAME, TRUE, FALSE, FLOAT};
    inline static const char* token_names[32]= {"SELECT", "CREATE", "TABLE", "FROM", "ALL", "WHERE", "DELETE", "EQUAL", "BETWEEN", "AND", "INSERT", "INTO", "VALUES", "FILE", "LPARENT", "RPARENT", "INDEX", "USING",  "AVL", "END", "ERR", "SEMICOLON", "COLON", "ID", "EOL", "NUM", "VALUE", "QUOTE", "FILENAME", "TRUE", "FALSE", "FLOAT"};;
    Type type;
    string lexema;
    Token(Type type):type(type) { lexema = ""; }
    Token(Type, char c);
    Token(Type type, const string source):type(type) {
        lexema = source;
    }

};

inline std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    if (tok.lexema.empty())
        return outs << Token::token_names[tok.type];
    else
        return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

inline std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}

class ReservedWords {
    map<string, Token::Type> palabras;

public:
    ReservedWords(){

        palabras.insert(make_pair("SELECT", Token::SELECT));
        palabras.insert(make_pair("CREATE", Token::CREATE));
        palabras.insert(make_pair("TABLE", Token::TABLE));
        palabras.insert(make_pair("FROM", Token::FROM));
        palabras.insert(make_pair("WHERE", Token::WHERE));
        palabras.insert(make_pair("DELETE", Token::DELETE));
        palabras.insert(make_pair("EQUAL", Token::EQUAL));
        palabras.insert(make_pair("BETWEEN", Token::BETWEEN));
        palabras.insert(make_pair("AND", Token::AND));
        palabras.insert(make_pair("INSERT", Token::INSERT));
        palabras.insert(make_pair("INTO", Token::INTO));
        palabras.insert(make_pair("VALUES", Token::VALUES));
        palabras.insert(make_pair("FILE", Token::FILE));
        palabras.insert(make_pair("INDEX", Token::INDEX));
        palabras.insert(make_pair("USING", Token::USING));
        palabras.insert(make_pair("AVL", Token::AVL));
        palabras.insert(make_pair("TRUE", Token::TRUE));
        palabras.insert(make_pair("FALSE", Token::FALSE));
    }

    Token::Type search(string lexema){
        auto it = palabras.find(lexema);
        if(it != palabras.end()) return it->second;
        //cout << lexema << endl;
        return Token::VALUE;
    }
};


class Scanner {
public:
    Scanner(string _input) :input(_input),first(0),current(0) { };
    Token* nextToken() {

        Token* token;
        char c;
        c = nextChar();
        while (c == ' ' || c == '\t' ) c = nextChar();
        if(c == '\n'){
            c = nextChar();
            while(c == '\n')
                c = nextChar();
        }
        // : \\ / .
        startLexema();
        if (isdigit(c) || isalpha(c)) {
            c = nextChar();
            while (isalpha(c) || isdigit(c)) {
                c = nextChar();
            }

            if (c == ':') {
                c = nextChar();
                while (strchr("\\.:/-_",c) || isalpha(c) || isdigit(c)) {
                    c = nextChar();
                }
                rollBack();
                string a = getLexema();
                return new Token(Token::FILENAME,a);
            }
            else if(isNumber(getLexema().substr(0,getLexema().length()-1)) && c == '.'){
                c = nextChar();
                while (isdigit(c)){
                    c = nextChar();
                }
                if(isalpha(c))
                    return new Token(Token::ERR, getLexema());
                rollBack();
                string a = getLexema();
                return new Token(Token::FLOAT, getLexema());
            }
            rollBack();
            string lexema = getLexema();
            if(current == input.length()){
                current-=1;
                lexema = getLexema();
                current++;
            }
            Token::Type ktype = palabras.search(lexema);
            if(ktype != Token::VALUE){
                return new Token(ktype);
            }

            if(isNumber(getLexema()))
                return new Token(Token::NUM, getLexema());

            return new Token(Token::ID, getLexema());
        }
        else if (c == '\0') {
            return new Token(Token::END);
        }
        else if (c == '/' || c == '~'){
            c = nextChar();
            while (strchr("~/.-_",c) || isalpha(c) || isdigit(c)) {
                c = nextChar();
            }
            rollBack();
            string a = getLexema();
            return new Token(Token::FILENAME,a);
        }
        else if (strchr("()=,;*'",c)){
            switch (c) {
                case '(': return new Token(Token::LPARENT);
                case ')': return new Token(Token::RPARENT);
                case '=': return new Token(Token::EQUAL);
                case ',': return new Token(Token::COLON);
                case ';': return new Token(Token::SEMICOLON);
                case '*': return new Token(Token::ALL);
                case '\'': return new Token(Token::QUOTE);
                default: cout << "No deberia llegar aca" << endl;
            }
        }
        else {
            return new Token(Token::ERR,getLexema());
        }
        return NULL;
    };
    ~Scanner(){};
private:
    string input;
    ReservedWords palabras;
    int first, current;
    int state;
    bool isNumber(const string& s) {
        for (char c : s) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    };
    char nextChar() {
        int c = input[current];
        if (c != '\0') current++;
        return c;
    };
    void rollBack() { // retract
        if (input[current] != '\0')
            current--;
    };
    void startLexema() {
        first = current-1;
        return;
    };
    void incrStartLexema() {
        first++;
    };
    string getLexema() {
        return input.substr(first,current-first);
    };
};
#endif //PROYECTO1_TOKENSSQL_H
