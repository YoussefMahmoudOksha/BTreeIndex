#ifndef BTREEINDEX_BTREEINDEX_H
#define BTREEINDEX_BTREEINDEX_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <utility>
#include <sstream>
#include <algorithm>
#include <tuple>
using namespace std;

struct BTreeNode
{
public:
    int isLeaf;
    // update
    int count = 0;
    int place;
    vector<pair<int, int>> node;
    vector<BTreeNode> children;
};

class BTreeIndex
{

    const char *const BTreeFileName = "BTreeIndex.txt";
    int numberOfRecords;
    int m;
    int head{};
    fstream BTreeFile;
    void DeleteCase2(const char *filename, vector<BTreeNode> &bTree, BTreeNode &find, int RecordID, int &count, int &temp);
    void DeleteCase1(const char *filename, vector<BTreeNode> &bTree, BTreeNode &find, int RecordID);


public:
    void CreateIndexFile(const char *filename, int numberOfRecords, int m);
    int InsertNewRecordAtIndex(int RecordID, int Reference);
    void DeleteRecordFromIndex(const char *filename, int RecordID, int m);
    void DisplayIndexFileContent(const char *filename);
    int SearchARecord(const char *filename, int RecordID);
    void run();

    //////////////////////////////////////Functions for searching//////////////////////////////////////
    bool record_valid(int recordNumber) const;
    int read_val(int rowIndex, int columnIndex);
    bool isEmpty(int recordNumber);
    bool isLeaf(int recordNumber);
    vector<pair<int, int>> read_node_values(int recordNumber);

    vector<BTreeNode> readFile(const char *filename);
    void savefile(const char *filename, vector<BTreeNode> bTree, int m);


    /////////////////////////////functions for insert////////////////////////////////////////////
    int split(int i,vector<BTreeNode> bTree);
    bool split_root(vector<BTreeNode> bTree);
    pair<vector<pair<int, int>>, vector<pair<int, int>>> splitOriginalNode(const vector<pair<int, int>>& originalNode);
    int updateAfterInsert(int parentRecordNumber, int newChildRecordNumber);
};

#endif // BTREEINDEX_BTREEINDEX_H
