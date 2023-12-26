#include "BTreeIndex.cpp"
using namespace std;

int main() {
    BTreeIndex bTreeIndex(10);
    cout << bTreeIndex.SearchARecord("BTreeIndex.txt", 24) << endl;

    return 0;
}
