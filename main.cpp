#include "BTreeIndex.h"
using namespace std;

int main() {
    BTreeIndex bTreeIndex;
    cout << "1. Create Index File" << endl;
    cout << "Enter the m records: ";
    int M;
    cin >> M;
    bTreeIndex.CreateIndexFile("BTreeIndex.txt", M*2, M);
    cout << "Index file created successfully." << endl;
    bTreeIndex.run();
    return 0;
}
