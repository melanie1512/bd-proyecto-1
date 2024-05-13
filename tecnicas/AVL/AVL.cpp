//
// Created by Paola Maguina aranda on 2024-05-10.
//
#include "AVL.h"
#include "Record.h"
using namespace std;

AVLFile::AVLFile(std::string filename)
{
    this-> pos_root = -1;
    this->filename = filename;
};
void AVLFile::loadData(const string& csvFilename) {
    ifstream file(csvFilename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << csvFilename << endl;
        return;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string token;

        Record record;

        // Leer fdc_id
        if (!getline(ss, token, ';')) {
            cerr << "Error reading fdc_id" << endl;
            continue;
        }
        try {
            record.fdc_id = stoi(token);
        } catch (const invalid_argument& e) {
            cerr << "Invalid fdc_id value: " << token << endl;
            continue;
        }

        // Leer brand
        if (!getline(ss, token, ';')) {
            cerr << "Error reading brand" << endl;
            continue;
        }
        strncpy(record.brand, token.c_str(), sizeof(record.brand) - 1);
        record.brand[sizeof(record.brand) - 1] = '\0';  // Asegurarse de que termine en nulo

        // Leer description
        if (!getline(ss, token, ';')) {
            cerr << "Error reading description" << endl;
            continue;
        }
        strncpy(record.description, token.c_str(), sizeof(record.description) - 1);
        record.description[sizeof(record.description) - 1] = '\0';  // Asegurarse de que termine en nulo

        // Leer ingredients
        if (!getline(ss, token, ';')) {
            cerr << "Error reading ingredients" << endl;
            continue;
        }
        strncpy(record.ingredients, token.c_str(), sizeof(record.ingredients) - 1);
        record.ingredients[sizeof(record.ingredients) - 1] = '\0';  // Asegurarse de que termine en nulo

        // Leer servingsize
        if (!getline(ss, token, ';')) {
            cerr << "Error reading servingsize" << endl;
            continue;
        }
        try {
            record.servingsize = stof(token);
        } catch (const invalid_argument& e) {
            cerr << "Invalid servingsize value: " << token << endl;
            continue;
        }

        // Insertar el registro en el árbol AVL
        insert(record);
    }

    file.close();
}
AVLFile::~AVLFile()
{
    std::fstream file(filename, ios::binary | ios::in);
    file.write(reinterpret_cast<char *>(&pos_root), sizeof(long));
    file.close();
}

Record AVLFile::find(int key) {return find(pos_root, key);}

void AVLFile::insert(Record record) {insert(pos_root, record);}

vector<Record> AVLFile::inorder()
{
    vector<Record> result;
    std::fstream file(filename, ios::binary | ios::in);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for reading." << std::endl;
        return result;
    }
    inorder(pos_root, result, file);
    file.close();
    return result;
}

int AVLFile::height() {return height(this->pos_root);}

void AVLFile::remove(int key) {remove(pos_root, key);}

long AVLFile::minValueNode()
{
    if (pos_root == -1) {
        throw std::runtime_error("The tree is empty");
    }
    return minValueNode(pos_root);
}

void AVLFile::rangeSearch(int begin, int end, std::vector<Record> &results)
{
    rangeSearch(pos_root, begin, end, results);
}

void AVLFile::readRecord(Record &record, long pos) {
    std::fstream file(filename, ios::binary | ios::in | ios::out);
    file.seekg(pos* sizeof(Record), ios::beg);
    file.read(reinterpret_cast<char*>(&record), sizeof(Record));
}

bool AVLFile::recordExists(std::fstream &file, const Record& record, long pos_node) {
    if (pos_node == -1) return false;

    file.seekg(pos_node * sizeof(Record), std::ios::beg);
    Record node;
    file.read(reinterpret_cast<char*>(&node), sizeof(Record));

    if (node.fdc_id == record.fdc_id) {
        return true;
    } else if (record.fdc_id < node.fdc_id) {
        return recordExists(file, record, node.left);
    } else {
        return recordExists(file, record, node.right);
    }
}

void AVLFile::writeRecord(Record &record, long pos)
{
    std::fstream file(filename, ios::binary | ios::in | ios::out);
    file.seekp(pos* sizeof(Record), ios::beg);
    file.write(reinterpret_cast<char*>(&record), sizeof(Record));
}

Record AVLFile::find(long pos_node, int key)
{
    if (pos_node == -1) {
        // si el código no se encuentra, retorna registro invalido
        return Record(-1, "NOT FOUND", "NOT FOUND", "NOT FOUND", 0.0);
    }
    std::fstream file(filename, ios::binary | ios::in | ios::out);
    //error al abrir archivo
    if (!file) {
        cerr << "Error opening file." << endl;
        return Record(-1, "FILE ERROR", "FILE ERROR", "FILE ERROR", 0.0);
    }
    file.seekg(pos_node * sizeof(Record), ios::beg);
    Record node;
    file.read(reinterpret_cast<char*>(&node), sizeof(Record));
    file.close();

    if(node.fdc_id==key)   {
        return node;
    }
    else if( key<node.fdc_id){
        return find(node.left,key); //busca a la izquierda
    }
    else
        return find(node.right,key); //busca a la derecha
}

void AVLFile::insert(long &pos_node, Record record)
{
    std::fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    if (recordExists(file, record,pos_node)) {
        std::cerr << "Record already exists." << std::endl;
        file.close();
        return;
    }
    if (pos_node == -1) {
        // Si no hay nodo en esta posición, crea uno nuevo al final del archivo.
        file.seekp(0, ios::end);
        pos_node = file.tellp() / sizeof(Record);
        //file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
        writeRecord(record, pos_node);

    } else {
        // Si hay nodo, lee el nodo actual.
        file.seekg(pos_node * sizeof(Record), ios::beg);
        Record node;
        file.read(reinterpret_cast<char*>(&node), sizeof(Record));

        // Decide si insertar a la izquierda o derecha.
        if (record.fdc_id < node.fdc_id) {
            if (node.left == -1) {
                file.seekp(0, ios::end);
                node.left = file.tellp() / sizeof(Record);
                file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
                //writeRecord(record, node.left);
            } else {
                insert(node.left, record);
            }
        } else if (record.fdc_id > node.fdc_id) {
            if (node.right == -1) {
                file.seekp(0, ios::end);
                node.right = file.tellp() / sizeof(Record);
                file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
                //writeRecord(record, node.right);
            } else {
                insert(node.right, record);
            }
        }
        // Actualiza con los nuevos valores de left y right
        file.seekp(pos_node * sizeof(Record));
        file.write(reinterpret_cast<const char*>(&node), sizeof(Record));
        updateHeight(node);
        balance(pos_node);
        // Escribir el nodo actualizado
        writeRecord(node, pos_node);
    }
    file.close();
}

void AVLFile::remove(long &pos_node, int key) {
    std::fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    if (pos_node == -1)
        return; // Nodo no encontrado, terminar.

    Record node;
    file.seekg(pos_node * sizeof(Record), ios::beg);
    file.read(reinterpret_cast<char*>(&node), sizeof(Record));

    if (!file) {
        std::cerr << "Failed to read from file at position " << pos_node << std::endl;
        file.close();
        return;
    }

    if (key < node.fdc_id) {
        remove(node.left, key); // Llama recursivamente para la izquierda.
    } else if (key > node.fdc_id) {
        remove(node.right, key); // Llama recursivamente para la derecha.
    } else {
        // Nodo encontrado, se procedemos a eliminar.
        if (node.left == -1 || node.right == -1) {
            long tempPos = (node.left != -1) ? node.left : node.right;

            // Nodo con un solo hijo
            if (tempPos == -1) {
                pos_node = -1; // No hay hijos elimina el nodo.
            } else {
                //Reemplaza el nodo con el hijo.
                Record tempNode;
                file.seekg(tempPos * sizeof(Record), ios::beg);
                file.read(reinterpret_cast<char*>(&tempNode), sizeof(Record));
                if (!file) {
                    std::cerr << "Failed to read from file at position " << tempPos << std::endl;
                    file.close();
                    return;
                }
                node = tempNode;
                pos_node = tempPos;
            }
        } else {
            // Nodo con dos hijos: Obtener el sucesor in-order (mínimo en el subárbol derecho).
            long succPos = minValueNode(node.right);
            Record succNode;
            file.seekg(succPos * sizeof(Record), ios::beg);
            file.read(reinterpret_cast<char*>(&succNode), sizeof(Record));
            if (!file) {
                std::cerr << "Failed to read from file at position " << succPos << std::endl;
                file.close();
                return;
            }
            node.fdc_id = succNode.fdc_id; // Copia el sucesor aquí.
            strcpy(node.brand, succNode.brand);
            strcpy(node.description, succNode.description);
            strcpy(node.ingredients, succNode.ingredients);
            node.servingsize = succNode.servingsize;

            // Elimina el sucesor.
            remove(node.right, succNode.fdc_id);
        }
    }

    if (pos_node != -1) {
        updateHeight(node);
        balance(pos_node);

        file.seekp(pos_node * sizeof(Record), ios::beg);
        file.write(reinterpret_cast<char*>(&node), sizeof(Record));
        if (!file) {
            std::cerr << "Failed to write to file at position " << pos_node << std::endl;
        }
    }

    file.close();
}



long AVLFile::minValueNode(long pos) {
    std::fstream file(filename, ios::binary | ios::in);
    Record node;
    long currentPos = pos;
    file.seekg(pos * sizeof(Record), ios::beg);
    file.read(reinterpret_cast<char*>(&node), sizeof(Record));

    // Busca el nodo más a la izquierda (el menor).
    while (node.left != -1) {
        currentPos = node.left;
        file.seekg(node.left * sizeof(Record), ios::beg);
        file.read(reinterpret_cast<char*>(&node), sizeof(Record));
    }
    file.close();
    return currentPos;
}

void AVLFile::inorder(long pos_node, vector<Record> &result, std::fstream &file)
{
    if (pos_node == -1) return;

    Record node;
    file.seekg(pos_node * sizeof(Record), ios::beg);
    file.read(reinterpret_cast<char*>(&node), sizeof(Record));
    //verifica si se lee correctamente
    if (!file) {
        std::cerr << "Failed to read from file at position " << pos_node << std::endl;
        return;
    }

    if (node.left != -1) {
        inorder(node.left, result, file);
    }
    result.push_back(node);
    if (node.right != -1) {
        inorder(node.right, result, file);
    }
}

int AVLFile::height(long nodePos)
{
    if (nodePos == -1) return -1;
    Record record;
    readRecord(record, nodePos);
    return record.height;
}

int AVLFile::balancingFactor(long nodePos)
{
    if (nodePos == -1) return 0;
    Record record;
    readRecord(record, nodePos);
    return height(record.left) - height(record.right);
}

void AVLFile::updateHeight(Record &record)
{
    int leftHeight = height(record.left);
    int rightHeight = height(record.right);
    record.height = 1 + max(leftHeight, rightHeight);
    // node->height = max(height(node->left), height(node->right)) + 1;

}

void AVLFile::right_rota(long &rootPos)
{
    Record root, leftChild;
    readRecord(root, rootPos);
    readRecord(leftChild, root.left);

    root.left = leftChild.right;
    leftChild.right = rootPos;
    updateHeight(root);
    updateHeight(leftChild);

    writeRecord(root, rootPos);
    long leftChildPos = root.left;
    writeRecord(leftChild, leftChildPos);

    rootPos = leftChildPos;  // Actualiza root con el nuevo root después de la rotación
}


void AVLFile::left_rota(long &rootPos)
{
    Record root, rightChild;
    readRecord(root, rootPos);
    readRecord(rightChild, root.right);

    root.right = rightChild.left;
    rightChild.left = rootPos;
    updateHeight(root);
    updateHeight(rightChild);

    writeRecord(root, rootPos);
    long rightChildPos = root.right;
    writeRecord(rightChild, rightChildPos);

    rootPos = rightChildPos; // Actualiza root con el nuevo root después de la rotación
}

void AVLFile::balance(long &pos)
{
    if (pos == -1) return;

    Record node;
    readRecord(node, pos);

    int hb = balancingFactor(pos);
    if (hb > 1) { //esta cargado a la izquierda
        if (balancingFactor(node.left) < 0)
            left_rota(node.left);
        right_rota(pos);
    } else if (hb < -1) { //esta cargado a la derecha
        if (balancingFactor(node.right) > 0)
            right_rota(node.right);
        left_rota(pos);
    }
    updateHeight(node);
    writeRecord(node, pos);
}
void AVLFile::rangeSearch(long pos_node, int lower, int upper, std::vector<Record> &results)
{
    std::fstream file(filename, ios::binary | ios::in);
    if (pos_node == -1) return;

    Record node;
    readRecord(node, pos_node);

    // Recorrer el subárbol izquierdo si es posible
    if (node.left != -1 && lower <= node.fdc_id) {
        rangeSearch(node.left, lower, upper, results);
    }

    // Si el nodo actual está en el rango, agregarlo a los resultados
    if (node.fdc_id >= lower && node.fdc_id <= upper) {
        results.push_back(node);
    }

    // Recorrer el subárbol derecho si es posible
    if (node.right != -1 && upper >= node.fdc_id) {
        rangeSearch(node.right, lower, upper, results);
    }
}
