# bd-proyecto-1
## Gramática del Parser SQL

La gramática del parser SQL está definida en formato BNF (Backus-Naur Form) como se muestra a continuación:

```
<statement> ::= <create_table> | <select> | <insert> | <delete>

<create_table> ::= "CREATE" "TABLE" <id> "FROM" "FILE" <value> "USING" "INDEX" "(" <index_type> ")" <id>
<index_type> ::= "AVL" | "ISAM" | "EXTENDIBLE"

<select> ::= "SELECT" "ALL" "FROM" <id> "WHERE" <condition>

<insert> ::= "INSERT" "INTO" <id> "VALUES" "(" <value_list> ")"
<value_list> ::= <value> | <value> ":" <value_list>

<delete> ::= "DELETE" "FROM" <id> "WHERE" <condition>

<condition> ::= <id> "=" <value> | <id> "BETWEEN" <value> "AND" <value>

<id> ::= /*identificador de la tabla, primary key. /
<value> ::= /*strings o numeros */
```

### Descripción de la gramática

- **statement**: Una declaración SQL puede ser de tipo `CREATE TABLE`, `SELECT`, `INSERT`, o `DELETE`.
- **create_table**: Define la estructura para crear una tabla especificando el nombre, el archivo de datos, y el tipo de índice.
- **index_type**: Enumera los tipos de índices disponibles (`AVL`, `ISAM`, `EXTENDIBLE`). 
- **select**: Construye una consulta de selección desde una tabla con una condición específica.
- **insert**: Inserta valores en una tabla especificada.
- **value_list**: Lista de valores a insertar, separados por `:`.
- **delete**: Elimina registros de una tabla basado en una condición dada.
- **condition**: Condición utilizada en consultas, que puede ser una comparación de igualdad o un rango.
- **id**, **value**: Representan identificadores y valores literales en las declaraciones SQL.
