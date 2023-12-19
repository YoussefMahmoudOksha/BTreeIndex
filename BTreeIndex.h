#ifndef BTREEINDEX_BTREEINDEX_H
#define BTREEINDEX_BTREEINDEX_H

struct BTreeNode {
    int isLeaf;
    int keys[10];
    int references[10];
    int children[11];
};

void CreateIndexFile(const char* filename, int numberOfRecords, int m);
int InsertNewRecordAtIndex(const char* filename, int RecordID, int Reference);
void DeleteRecordFromIndex(const char* filename, int RecordID);
void DisplayIndexFileContent(const char* filename);
int SearchARecord(const char* filename, int RecordID);

#endif //BTREEINDEX_BTREEINDEX_H
