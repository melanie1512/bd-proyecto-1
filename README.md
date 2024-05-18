# Proyecto 1: Organización de Archivos

## Introducción 

Entender y aplicar los algoritmos de almacenamiento de archivos físicos y acceso concurrente.
A través de crear y manipular un DataSet usando técnicas de organización de archivos.

### Objetivos

1. **Comprender Técnicas de Organización de Archivos**:
   - Estudiar archivos secuenciales/AVL, ISAM y hashing extensible.

2. **Implementar Operaciones Básicas**:
   - Desarrollar algoritmos de inserción, búsqueda y eliminación para cada técnica.


### Organización de Archivos

Las técnicas usadas para este proyecto son las siguientes:

- [ISAM-Sparse Index](poner link)
- [Extendible Hashing](poner link)
- [AVL](poner link)

### Funciones implementadas

Para cada técnica, se debe implementó las siguientes funciones:

- vector<Registro> search(T key)
- vector<Registro> rangeSearch(T begin-key, T end-key)
- bool add(Registro registro)
- bool remove(T key)

### DataSet
Hemos usado un dataset de [FoodData Central](https://www.kaggle.com/datasets/pranavkarnani/fooddata-central). Esta elección fue debida a los siguientes factores:

- **Complejidad de los datos**: Ofrece una variedad de información sobre alimentos, como ID, propietario de la marca, descripción, ingredientes, código GTIN/UPC y tamaño de la porción.

- **Tamaño adecuado**: El conjunto de datos es lo suficientemente grande para pruebas exhaustivas y evaluaciones de rendimiento.

- **Relaciones entre datos**: Permite explorar relaciones entre diferentes alimentos y marcas, lo que es útil para entender las estructuras de datos.

- **Variedad de consultas**: Con ingredientes, códigos de barras y descripciones, se pueden realizar diversas consultas para probar las funciones implementadas en las técnicas de organización de archivos.

```c++
struct FoodRecord {
    int fdc_id; //primary key
    char brand[50];
    char description[30];
    char ingredients[110];
    float servingsize;}
```

|       Campo       |                         Descripción                          |
|:-----------------:|:------------------------------------------------------------:|
|   ```fdc_id```    |                   Id único de cada registro.                 |
|    ```brand```    |        Marca del producto, puede ser algún corporativo       |
| ```description``` |                   Descripción del producto                   |
| ```ingredients``` |                   Ingredientes del producto                  |
| ```servingsize``` |               Tamaño de la porción del producto              |

## Técnicas Utilizadas

### AVL File
##### Insert
##### Search
##### Seacrh for Range
##### Delete

### ISAM-Sparse Index
##### Insert
##### Search
##### Seacrh for Range
##### Delete

### Extendible Hash
##### Insert
##### Search
##### Seacrh for Range
##### Delete

## Experimentación

## Integrantes
|                    **Bihonda Epiquien Rodas**                   |                          **Paola Maguiña**                          |                         **Camila Acosta**                          |                         **Melanie Cortez**                         |                       **Sofía Herrera**                       |
|:---------------------------------------------------------------:|:-------------------------------------------------------------------:|:-------------------------------------------------------------------:|:------------------------------------------------------------------:|:-------------------------------------------------------------:|
| <a href="https://github.com/bihoepiro" target="_blank">`github.com/bihonda`</a> | <a href="https://github.com/paolamag" target="_blank">`github.com/paolamag`</a> | <a href="https://github.com/camilaacosta" target="_blank">`github.com/camilaacosta`</a> | <a href="https://github.com/melaniecortes" target="_blank">`github.com/melanie1512`</a> | <a href="https://github.com/sofiyin" target="_blank">`github.com/sofiyin`</a> |
