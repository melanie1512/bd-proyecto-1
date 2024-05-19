#include <iostream>
#include "tokensSQL.h"
#include "parserSQL.h"

#include "AVL.h"

using namespace std;

int main() {
    const char* input =
            "CREATE TABLE FoodCentral FROM FILE '/Users/paolamaguinaaranda/Documents/2024-I/BD2/proyecto1/tecnicas/data/data.csv' USING INDEX AVL('fdc_id');"
            "INSERT INTO FoodCentral VALUES ('12345678', 'BrandC', 'DescriptionC', 'IngredientsC', '300.0');"
            "SELECT * FROM FoodCentral WHERE fdc_id = '12345678';"
            "SELECT * FROM FoodCentral WHERE fdc_id BETWEEN '1105905' AND '1105908';"
            "DELETE FROM FoodCentral WHERE fdc_id='12345678';";

    // Crear el escáner
    Scanner scanner(input);
    // Crear la instancia de AVLFile
    AVLFile avlFile("/Users/paolamaguinaaranda/Documents/2024-I/BD2/proyecto1/tecnicas/bin/avl_data.bin");

    // Crear el parser con ambos argumentos
    Parser parser(&scanner, &avlFile);
    parser.parse();
    /*Token* tk = scanner.nextToken();
   while (tk->type != Token::END) {
       cout << "next token " << tk << endl;
       delete tk;
       tk = scanner.nextToken();
   }
   cout << "last token " << tk << endl;
   delete tk;*/

    return 0;
}
