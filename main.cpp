#include "BTreeIndex.cpp"
using namespace std;

int main() {
    BTreeIndex bTreeIndex;
    bTreeIndex.CreateIndexFile("BTreeIndex.txt",10,5);
    cout << bTreeIndex.SearchARecord("BTreeIndex.txt", 12) << endl;


    return 0;
}
