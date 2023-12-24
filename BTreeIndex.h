#ifndef BTREEINDEX_BTREEINDEX_H
#define BTREEINDEX_BTREEINDEX_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

using namespace std;
struct BTreeNode
{
    int isLeaf;
    int keys[10];
    int references[10];
    int parent[11];
};

class BTreeIndex
{
private:
    const string BTreeFileName = "BTreeIndex.txt";
    void DeleteCase1(int RecordID); // delete without any effect
    void DeleteCase2(int RecordID); // delete the largest key
    void DeleteCase3(int RecordID); // Deleting a key from a node causing an underflow while siblings have more than the minimum number of keys
    void DeleteCase4(int RecordID); // Deleting a key from a node causing an underflow while siblings have the minimum number of keys public : void CreateIndexFile(const char *filename, int numberOfRecords, int m);

public:
    void CreateIndexFile(const char *filename, int numberOfRecords, int m);
    int InsertNewRecordAtIndex(const char *filename, int RecordID, int Reference);
    void DeleteRecordFromIndex(const char *filename, int RecordID);
    void DisplayIndexFileContent(const char *filename);
    int SearchARecord(const char *filename, int RecordID);
    void run();
};

#endif // BTREEINDEX_BTREEINDEX_H
