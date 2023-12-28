#include "BTreeIndex.h"

using namespace std;



void BTreeIndex::CreateIndexFile(const char *filename, int numberOfRecords, int m) {
    BTreeFile.open(BTreeFileName, ios::in | ios::out | ios::binary);
    this->numberOfRecords = numberOfRecords;
    this->m = m;
    /////////////////////////////////////////////////////////
    ofstream outfile(filename, ios::binary);
    outfile.clear();
    int i= 1;
    for (int j = 0; j < numberOfRecords; ++j) {
        if(i == numberOfRecords){
            i = -1;
        }
        outfile << -1 << "  " << i << "  ";
        for (int k = 0; k < numberOfRecords -1; ++k) {
            outfile << "-1" << "  ";
        }
        i++;
        outfile << "\n";
    }
    readFile(filename);
    outfile.close();
    ///////////////////////////////////////////////////////////////
}

int BTreeIndex::InsertNewRecordAtIndex(const char *filename, int RecordID, int Reference) {
    // Implementation to insert a new record into the index file
    // Return -1 if there is no place to insert the record or the index of the node where the new record is inserted
    // ...
    return -1;
}

void BTreeIndex::DeleteRecordFromIndex(const char *filename, int RecordID, int m) {
    vector<BTreeNode> bTree;
    bTree = readFile(filename);
    int balance = m/2;
    int count = 0;
    int temp;
    BTreeNode find; // to find the node
    for (auto & i : bTree) {
        if (i.isLeaf == 0){
            for (int k = 0; k < i.node.size(); ++k) {
                if (i.node[k].first == RecordID){
                    find = i;
                    break;
                }
            }
        }
    }
    if (find.place == 1){// if the node is the root
        for (int i = 0; i <  find.node.size(); ++i) {
            if (find.node[i].first == RecordID){
                find.node[i].first = -1;
                find.node[i].second = -1;
            }
            sort(find.node.begin(), find.node.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                if (a.first != -1 && b.first != -1) {
                    return a.first < b.first;
                }
                return a.first != -1;
            });
        }
        if (find.node[0].first == -1){
            find.node[0].first = head;
            head = find.place;
            find.isLeaf = -1;
        }
        bTree[(find.place -1)].node =find.node;
        bTree[(find.place -1)].isLeaf =find.isLeaf;
        savefile(filename,bTree, m);
    }
    for (auto & i : find.node) {// get the node balance
        if (i.first != -1){
            count++;
        }
    }
    if ((count -1) >= balance){
        if (find.node[count -1].first == RecordID){//check the case 1 or 2
            for (int i = 0; i <  find.node.size(); ++i) { // case 2
                if (find.node[i].first == RecordID){
                    find.node[i].first = -1;
                    find.node[i].second = -1;
                }
                sort(find.node.begin(), find.node.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                    if (a.first != -1 && b.first != -1) {
                        return a.first < b.first;
                    }
                    return a.first != -1;
                });
            }
            temp = find.node[count-2].first;
            bTree[(find.place -1)].node =find.node;
            for (auto & i : bTree) {
                for (auto & k : i.node) {
                    if (k.first == RecordID){
                        k.first = temp;
                    }
                }
            }
            savefile(filename,bTree, m);
        }else{
            for (int i = 0; i <  find.node.size(); ++i) { // case 1
                if (find.node[i].first == RecordID){
                    find.node[i].first = -1;
                    find.node[i].second = -1;
                }
                sort(find.node.begin(), find.node.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                    if (a.first != -1 && b.first != -1) {
                        return a.first < b.first;
                    }
                    return a.first != -1;
                });
            }
            bTree[(find.place -1)].node =find.node;
            savefile(filename,bTree, m);
        }
    // implement case 3 , 4
    }else{// case 3 , 4
//        if(){ // case 3
//
//        }else{// case 4
//
//        }
    }



}

void BTreeIndex::DisplayIndexFileContent(const char *filename) {
    ifstream inputFile(filename, ios::in | ios::binary);
    vector<BTreeNode> Tree = readFile(filename);
    cout << "<---------------------Head--------------------->\n ";
    cout << "Empty Place: " <<  head << endl;
    for (int i = 0; i < Tree.size(); ++i) {
        cout << " Place: " << Tree[i].place << " | IsLeaf: " << Tree[i].isLeaf <<  " | Node: " ;
        for (const auto& pair : Tree[i].node) {
            cout << "(" << pair.first << ", " << pair.second << ") ";
        }

        cout << endl;
    }


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

        for (int i = 1; i <= numberOfRecords; i += 2) {
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
    int choice, recordID, reference;

    do {
        cout << "\nB-Tree Index Menu:" << endl;
        cout << "1. Insert New Record" << endl;
        cout << "2. Delete Record" << endl;
        cout << "3. Display Index File Content" << endl;
        cout << "4. Search for a Record" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter RecordID and Reference: ";
                cin >> recordID >> reference;
                int result = InsertNewRecordAtIndex("BTreeIndex.txt", recordID, reference);
                if (result == -1) {
                    cout << "Error: No place to insert the record." << endl;
                } else {
                    cout << "Record inserted at node index: " << result << endl;
                }
                break;
            }
            case 2: {
                cout << "Enter RecordID to delete: ";
                cin >> recordID;
                int referenceValue = SearchARecord("BTreeIndex.txt", recordID);
                if (referenceValue == -1) {
                    cout << "Record not found in the index." << endl;
                } else {
                    DeleteRecordFromIndex("BTreeIndex.txt", recordID, m);  // Assuming m is 5, adjust as needed
                    cout << "Record deleted successfully." << endl;
                }
                break;
            }

            case 3: {
                DisplayIndexFileContent("BTreeIndex.txt");
                break;
            }

            case 4: {
                cout << "Enter RecordID to search: ";
                cin >> recordID;
                int referenceValue = SearchARecord("BTreeIndex.txt", recordID);
                if (referenceValue == -1) {
                    cout << "Record not found in the index." << endl;
                } else {
                    cout << "Record found at reference: " << referenceValue << endl;
                }
                break;
            }

            case 5: {
                cout << "Exiting program." << endl;
                break;
            }

            default: {
                cout << "Invalid choice. Please enter a valid option." << endl;
            }
        }
    } while (choice != 5);
}

vector<BTreeNode> BTreeIndex::readFile(const char *filename) {
    ifstream File(filename);
    vector<BTreeNode> Btree;
    string line;
    int i = 1;
    File >> head >> head;
    getline(File,line);
    while (getline(File, line)){
        istringstream iss(line);
        BTreeNode Node;
        iss >> Node.isLeaf;
        Node.place = i;
        int key, value;
        while ((iss>> key >> value)){
            Node.node.emplace_back(key, value);
        }
        Btree.push_back(Node);
        i++;
    }
    for (int j = (Btree.size() -1); j >= 0; --j) {
        if (Btree[j].isLeaf == 1){
            for (int k = 0; k < Btree[j].node.size(); ++k) {
                if (Btree[j].node[k].second != -1){
                    Btree[j].children.push_back(Btree[Btree[j].node[k].second -1]);
                }
            }
        }
    }
    File.close();
    return Btree ;
}

void BTreeIndex::savefile(const char *filename, vector<BTreeNode> bTree, int m) {
    ofstream outFile(filename, ios::binary);
    outFile << -1  << "  "<< head << "  ";
    for (int i = 0; i < (m * 2) -1; ++i) {
        outFile << -1 << "  ";
    }
    outFile << "\n";
    for (const auto& node : bTree) {
        outFile << node.isLeaf << "  ";

        for (const auto& pair : node.node) {
            outFile << pair.first << "  ";
            outFile << pair.second << "  ";
        }
        outFile << "\n";
    }

    outFile.close();
}





