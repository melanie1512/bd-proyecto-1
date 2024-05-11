//
// Created by Paola Maguina Aranda on 2024-05-10.
//

#ifndef PROYECTO1_AVL_H
#define PROYECTO1_AVL_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Record.h"


using namespace std;

class AVLFile
{
private:
    string filename;
    long pos_root;
public:
    AVLFile(string filename);
    void loadData(const std::string& csvFilename);
    ~AVLFile();
    Record find(int key);
    void insert(Record record);
    vector<Record> inorder();
    int height();
    void remove(int key);
    long minValueNode();
    void rangeSearch(int begin, int end, std::vector<Record> &results);

private:
    void readRecord(Record& record, long pos);
    bool recordExists(std::fstream &file, const Record& record, long pos_node);
    void writeRecord(Record& record, long pos);
    Record find(long pos_node, int key);
    void insert(long& pos_node, Record record);
    void remove(long& pos_node, int key);
    long minValueNode(long pos);
    void inorder(long pos_node, vector<Record> &result, std::fstream &file);
    int height(long nodePos);
    int balancingFactor(long nodePos);
    void updateHeight(Record& record);
    void right_rota(long& rootPos);
    void left_rota(long& rootPos);
    void balance(long& pos);
    void rangeSearch(long pos_node, int lower, int upper, std::vector<Record> &results);
};


#endif //PROYECTO1_AVL_H
