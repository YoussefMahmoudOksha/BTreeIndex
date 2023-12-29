#include "BTreeIndex.h"
using namespace std;

int main()
{
    BTreeIndex bTreeIndex;
    cout << "1. Create Index File" << endl;
    cout << "Enter the m records: ";
    int M;
    cin >> M;
    bTreeIndex.CreateIndexFile("BTreeIndex.txt", M * 2, M);
    cout << "\nIndex file created successfully." << endl;
    bTreeIndex.run();
    /////////////////for test insert code/////////////////////////
//    bTreeIndex.CreateIndexFile("BTreeIndex.txt", 10, 5);
//    bTreeIndex.InsertNewRecordAtIndex(3, 12);
//    bTreeIndex.InsertNewRecordAtIndex(7, 24);
//    bTreeIndex.InsertNewRecordAtIndex(10, 48);
//    bTreeIndex.InsertNewRecordAtIndex(24, 60);
//    bTreeIndex.InsertNewRecordAtIndex(14, 72);
//    bTreeIndex.InsertNewRecordAtIndex(19, 84);
//    bTreeIndex.InsertNewRecordAtIndex(30, 96);
//    bTreeIndex.InsertNewRecordAtIndex(15, 108);
//    bTreeIndex.InsertNewRecordAtIndex(1, 120);
//    bTreeIndex.InsertNewRecordAtIndex(5, 132);
//    bTreeIndex.InsertNewRecordAtIndex(2, 144);
//    bTreeIndex.InsertNewRecordAtIndex(8, 156);
//    bTreeIndex.InsertNewRecordAtIndex(9, 168);
//    bTreeIndex.InsertNewRecordAtIndex(6, 180);
//    bTreeIndex.InsertNewRecordAtIndex(11, 192);
//    bTreeIndex.InsertNewRecordAtIndex(12, 204);
//    bTreeIndex.InsertNewRecordAtIndex(17, 216);
//    bTreeIndex.InsertNewRecordAtIndex(18, 228);
//    bTreeIndex.InsertNewRecordAtIndex(32, 240);
    return 0;
}
