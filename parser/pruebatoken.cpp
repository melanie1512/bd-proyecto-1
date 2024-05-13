#include <iostream>
#include "tokensSQL.h"
#include "parserSQL.h"
using namespace std;
int main() {
    const char* input =
            "CREATE TABLE Customer FROM FILE 'C:\\data.csv' USING INDEX AVL('DNI');"
            "SELECT * FROM Customer WHERE DNI = '12345678';"
            "SELECT * FROM Customer WHERE DNI BETWEEN '12345678' AND '87654321';"
            "INSERT INTO Customer VALUES ('John', 'Doe', '12345678');"
            "DELETE FROM Customer WHERE DNI = '12345678';";

    Scanner scanner(input);

    /*Token* tk = scanner.nextToken();
    while (tk->type != Token::END) {
        cout << "next token " << tk << endl;
        delete tk;
        tk = scanner.nextToken();
    }
    cout << "last token " << tk << endl;
    delete tk;*/
    Parser parser(&scanner);
    parser.parse();

    return 0;
}