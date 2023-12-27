#ifndef BTREEINDEX_BTREEINDEX_H
#define BTREEINDEX_BTREEINDEX_H


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <sstream>
using namespace std;

struct BTreeNode {
public:
    int isLeaf;
    int place;
    vector<pair<int,int>>  node;
    vector<BTreeNode> children;
};




class BTreeIndex{


    const  string BTreeFileName = "BTreeIndex.txt";
    int numberOfRecords;
    fstream BTreeFile;
    int m;
public:

    void CreateIndexFile(const char* filename, int numberOfRecords, int m);
    int InsertNewRecordAtIndex(const char* filename, int RecordID, int Reference);
    void DeleteRecordFromIndex(const char* filename, int RecordID);
    void DisplayIndexFileContent(const char* filename);
    int SearchARecord(const char* filename, int RecordID);
    void run();

//////////////////////////////////////Functions for searching//////////////////////////////////////

    bool record_valid(int recordNumber) const;


    int read_val(int rowIndex, int columnIndex);

    bool isEmpty(int recordNumber);

    bool isLeaf(int recordNumber);

    vector<pair<int, int>> read_node_values(int recordNumber);
};


#endif //BTREEINDEX_BTREEINDEX_H
