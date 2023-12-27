#include <vector>
#include <sstream>
#include "BTreeIndex.h"

using namespace std;



void BTreeIndex::CreateIndexFile(const char *filename, int numberOfRecords, int m) {
    BTreeFile.open(BTreeFileName, ios::in | ios::out | ios::binary);
    this->numberOfRecords = numberOfRecords;
    this->m = m;
}

int BTreeIndex::InsertNewRecordAtIndex(const char *filename, int RecordID, int Reference) {
    // Implementation to insert a new record into the index file
    // Return -1 if there is no place to insert the record or the index of the node where the new record is inserted
    // ...
    return -1;
}

void BTreeIndex::DeleteRecordFromIndex(const char *filename, int RecordID) {
    // Implementation to delete a record from the index file
    // ...
}

void BTreeIndex::DisplayIndexFileContent(const char *filename) {
    // Implementation to display the content of the index file
    // ...
}

//////////////////////////////////////Functions for searching//////////////////////////////////////


bool BTreeIndex::isEmpty(int recordNumber) {
    return read_val(recordNumber, 0) == -1;
}

bool BTreeIndex::isLeaf(int recordNumber) {
    return read_val(recordNumber, 0) == 0;
}

bool BTreeIndex::record_valid(int recordNumber) const {
    if (recordNumber <= 0 || recordNumber > numberOfRecords)
        return false;

    return true;
}


int BTreeIndex::read_val(int rowIndex, int columnIndex) {
    BTreeFile.seekg(0, std::ios::beg);
    int x ;
    for ( int i = 0 ; i < rowIndex * (2 * m + 1) + columnIndex + 1 ; i++){
        BTreeFile >> x ;
    }

    return x;

}

vector<pair<int, int>> BTreeIndex::read_node_values(int recordNumber) {
    if(record_valid(recordNumber)) {

        vector<pair<int, int>> theNode;

        for (int i = 1; i <= 10; i += 2) {
            int key = read_val(recordNumber, i);
            int value = read_val(recordNumber, i + 1);
            theNode.emplace_back(key, value);
        }
        return theNode;
    }
    else{
        return {};
    }

}

int BTreeIndex::SearchARecord(const char *filename, int RecordID) {
    if (isEmpty(1)) return -1;
    std::vector<std::pair<int, int>> current;

    int i = 1;
    bool found;
    while (!isLeaf(i)) {
        current = read_node_values(i);
        found = false;
        for (auto p: current) {
            if (p.first >= RecordID) {
                i = p.second;
                found = true;
                break;
            }
        }

        if (!found) return -1;
    }

    current = read_node_values(i);

    for (auto pair: current)
        if (pair.first == RecordID)
            return pair.second;

    return -1;
}


void BTreeIndex::run() {

}


