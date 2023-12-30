#include "BTreeIndex.h"

using namespace std;

void BTreeIndex::CreateIndexFile(const char *filename, int numberOfRecords, int m)
{
    BTreeFile.open(BTreeFileName, ios::in | ios::out | ios::binary);
    this->numberOfRecords = numberOfRecords;
    this->m = m;
    /////////////////////////////////////////////////////////
    /*ofstream outfile(filename, ios::binary);
    outfile.clear();
    int i = 1;
    for (int j = 0; j < numberOfRecords; ++j) {
        if (i == numberOfRecords) {
            i = -1;
        }
        outfile << -1 << "  " << i << "  ";
        for (int k = 0; k < numberOfRecords - 1; ++k) {
            outfile << "-1" << "  ";
        }
        i++;
        outfile << "\n";
    }
    readFile(filename);
    outfile.close();*/
    ///////////////////////////////////////////////////////////////
}

int BTreeIndex::InsertNewRecordAtIndex(int RecordID, int Reference)
{
    vector<BTreeNode> bTree = readFile(BTreeFileName);
    if (bTree[0].count == 0){
        head = bTree[0].node[0].first;
        bTree[0].node[0].first = RecordID;
        bTree[0].node[0].second = Reference;
        bTree[0].isLeaf = 0;
        bTree[0].count++;
        savefile(BTreeFileName, bTree,m);
        return 1;
    }
    stack<int> visited;
    int i = 0;
    bool found = false;
    while (bTree[i].isLeaf){
        visited.push(i);
        found = false;
        for (int j = 0; j < bTree[i].node.size(); ++j) {
            if (bTree[i].node[j].first >= RecordID){
                i = bTree[i].node[j].second -1;
                found = true;
                break;
            }
        }

        if (!found){
            i = bTree[i].node[bTree[i].count -1].second -1;
        }
    }
    bTree[i].node.push_back(make_pair(RecordID,Reference));
    sort(bTree[i].node.begin(), bTree[i].node.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        if (a.first != -1 && b.first != -1) {
            return a.first < b.first;
        }
        return a.first != -1;
    });

    bTree[i].count++;
    int newFromSplitIndex = -1;
    if (bTree[i].count > m){
        newFromSplitIndex = split(i, bTree);
    }else{
        bTree[i].node.pop_back();
        savefile(BTreeFileName, bTree, m);
    }

    if(i == 0){
        return 1;
    }

    while (!visited.empty()) {
        int lastVisitedIndex = visited.top();
        visited.pop();
        newFromSplitIndex = updateAfterInsert(lastVisitedIndex, newFromSplitIndex);
    }

    return -1;
}
void BTreeIndex::DeleteCase2(const char *filename, vector<BTreeNode> &bTree, BTreeNode &find, int RecordID, int &count, int &temp)
{
    for (int i = 0; i < find.node.size(); ++i)
    { // case 2
        if (find.node[i].first == RecordID)
        {
            find.node[i].first = -1;
            find.node[i].second = -1;
        }
        sort(find.node.begin(), find.node.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
             {
			if (a.first != -1 && b.first != -1) {
				return a.first < b.first;
			}
			return a.first != -1; });
    }
    temp = find.node[count - 2].first;
    bTree[(find.place - 1)].node = find.node;
    for (auto &i : bTree)
    {
        for (auto &k : i.node)
        {
            if (k.first == RecordID)
            {
                k.first = temp;
            }
        }
    }
    find.count--;
    savefile(filename, bTree, m);
}
void BTreeIndex::DeleteRecordFromIndex(const char *filename, int RecordID, int m)
{
    vector<BTreeNode> bTree;
    bTree = readFile(filename);
    int balance = m / 2;
    int count = 0;
    int temp;
    BTreeNode find; // to find the node
    for (auto &i : bTree)
    {
        if (i.isLeaf == 0)
        {
            for (int k = 0; k < i.node.size(); ++k)
            {
                if (i.node[k].first == RecordID)
                {
                    find = i;
                    break;
                }
            }
        }
    }
    if (find.place == 1)
    { // if the node is the root
        for (int i = 0; i < find.node.size(); ++i)
        {
            if (find.node[i].first == RecordID)
            {
                find.node[i].first = -1;
                find.node[i].second = -1;
            }
            sort(find.node.begin(), find.node.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
                 {
				if (a.first != -1 && b.first != -1) {
					return a.first < b.first;
				}
				return a.first != -1; });
        }
        if (find.node[0].first == -1)
        {
            find.node[0].first = head;
            head = find.place;
            find.isLeaf = -1;
        }
        bTree[(find.place - 1)].node = find.node;
        bTree[(find.place - 1)].isLeaf = find.isLeaf;
        savefile(filename, bTree, m);
    }
    for (auto &i : find.node)
    { // get the node balance
        if (i.first != -1)
        {
            count++;
        }
    }
    if ((count - 1) >= balance)
    {
        if (find.node[count - 1].first == RecordID)
        { // check the case 1 or 2
            DeleteCase2(filename, bTree, find, RecordID, count, temp);
            // for (int i = 0; i < find.node.size(); ++i) { // case 2
            //	if (find.node[i].first == RecordID) {
            //		find.node[i].first = -1;
            //		find.node[i].second = -1;
            //	}
            //	sort(find.node.begin(), find.node.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            //		if (a.first != -1 && b.first != -1) {
            //			return a.first < b.first;
            //		}
            //		return a.first != -1;
            //		});
            // }
            // temp = find.node[count - 2].first;
            // bTree[(find.place - 1)].node = find.node;
            // for (auto& i : bTree) {
            //	for (auto& k : i.node) {
            //		if (k.first == RecordID) {
            //			k.first = temp;
            //		}
            //	}
            // }
            // savefile(filename, bTree, m);
        }
        else
        {
            for (int i = 0; i < find.node.size(); ++i)
            { // case 1
                if (find.node[i].first == RecordID)
                {
                    find.node[i].first = -1;
                    find.node[i].second = -1;
                }
                sort(find.node.begin(), find.node.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
                     {
					if (a.first != -1 && b.first != -1) {
						return a.first < b.first;
					}
					return a.first != -1; });
            }
            find.count--;
            bTree[(find.place - 1)].node = find.node;
            savefile(filename, bTree, m);
        }
    }
    else
    {
        // case 3 or 4
        BTreeNode parent;
        BTreeNode siblings;
        bool flag = 0;
        for (size_t i = 0; i < bTree.size(); i++)
        {
            if (bTree[i].isLeaf == 1)
            {
                for (size_t j = 0; j < bTree[i].node.size(); j++)
                {
                    if (bTree[i].node[j].second == find.place)
                    {
                        parent = bTree[i];
                        break;
                    }
                }
            }
        }

        for (size_t i = 0; i < parent.children.size(); i++)
        {
            int ct = 0;
            for (size_t j = 0; j < parent.children[i].node.size(); j++)
            {
                if (parent.children[i].node[j].first != -1)
                {
                    ct++;
                }
            }
            parent.children[i].count = ct;
        }

        for (size_t i = 0; i < parent.children.size(); i++)
        {
            if ((parent.children[i].count > balance) && (parent.children[i +1].place == find.place))
            {
                siblings = parent.children[i];
                flag = 1;
                break;
            }
            if ((parent.children[i].count > balance) && (parent.children[i -1].place == find.place  )){
                siblings = parent.children[i];
                flag = 1;
                break;
            }
        }
        if (flag)
        {
            // case 3
            auto tmp = siblings.node[siblings.count - 1];
            DeleteCase2(filename, bTree, siblings, siblings.node[siblings.count - 1].first, siblings.count, temp);
            find.node[find.count] = tmp;
            find.count++;
            sort(find.node.begin(), find.node.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
                 {
						if (a.first != -1 && b.first != -1) {
							return a.first < b.first;
						}
						return a.first != -1; });
            DeleteCase2(filename, bTree, find, RecordID, find.count, temp);
        }
        else
        {
            // case 4
            cout << "case4" << endl;
        }
    }
}

void BTreeIndex::DisplayIndexFileContent(const char *filename)
{
    ifstream inputFile(filename, ios::in | ios::binary);
    vector<BTreeNode> Tree = readFile(filename);
    cout << "<---------------------Head--------------------->\n ";
    cout << "Empty Place: " << head << endl;
    for (int i = 0; i < Tree.size(); ++i)
    {
        cout << " Place: " << Tree[i].place << " | HasLeaf: " << Tree[i].isLeaf << " | Node: ";
        for (const auto &pair : Tree[i].node)
        {
            cout << "(" << pair.first << ", " << pair.second << ") ";
        }

        cout << endl;
    }
}

//////////////////////////////////////Functions for searching//////////////////////////////////////

bool BTreeIndex::isEmpty(int recordNumber)
{
    return read_val(recordNumber, 0) == -1;
}

bool BTreeIndex::isLeaf(int recordNumber)
{
    return read_val(recordNumber, 0) == 0;
}

bool BTreeIndex::record_valid(int recordNumber) const
{
    if (recordNumber <= 0 || recordNumber > numberOfRecords)
        return false;

    return true;
}

int BTreeIndex::read_val(int rowIndex, int columnIndex)
{
    BTreeFile.seekg(0, std::ios::beg);
    int x;
    for (int i = 0; i < rowIndex * (2 * m + 1) + columnIndex + 1; i++)
    {
        BTreeFile >> x;
    }

    return x;
}

vector<pair<int, int>> BTreeIndex::read_node_values(int recordNumber)
{
    if (record_valid(recordNumber))
    {

        vector<pair<int, int>> theNode;

        for (int i = 1; i <= numberOfRecords; i += 2)
        {
            int key = read_val(recordNumber, i);
            int value = read_val(recordNumber, i + 1);
            theNode.emplace_back(key, value);
        }
        return theNode;
    }
    else
    {
        return {};
    }
}

int BTreeIndex::SearchARecord(const char *filename, int RecordID)
{
    if (isEmpty(1))
        return -1;
    std::vector<std::pair<int, int>> current;

    int i = 1;
    bool found;
    while (!isLeaf(i))
    {
        current = read_node_values(i);
        found = false;
        for (auto p : current)
        {
            if (p.first >= RecordID)
            {
                i = p.second;
                found = true;
                break;
            }
        }

        if (!found)
            return -1;
    }

    current = read_node_values(i);

    for (auto pair : current)
        if (pair.first == RecordID)
            return pair.second;

    return -1;
}

void BTreeIndex::run()
{
    int choice, recordID, reference;

    do
    {
        cout << "\nB-Tree Index Menu:" << endl;
        cout << "1. Insert New Record" << endl;
        cout << "2. Delete Record" << endl;
        cout << "3. Display Index File Content" << endl;
        cout << "4. Search for a Record" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cout << "Enter RecordID and Reference: ";
            cin >> recordID >> reference;
            int referenceValue = SearchARecord("BTreeIndex.txt", recordID);
            if(referenceValue == -1){
                int result = InsertNewRecordAtIndex( recordID, reference);
                if (result == -1)
                {
                    cout << "Error: No place to insert the record." << endl;
                }
                else
                {
                    cout << "Record inserted at node index: " << result << endl;
                }
            }else{
                cout << "Error: Can't insert RecordID two time." << endl;
            }

            break;
        }
        case 2:
        {
            cout << "Enter RecordID to delete: ";
            cin >> recordID;
            int referenceValue = SearchARecord("BTreeIndex.txt", recordID);
            if (referenceValue == -1)
            {
                cout << "Record not found in the index." << endl;
            }
            else
            {
                DeleteRecordFromIndex("BTreeIndex.txt", recordID, m); // Assuming m is 5, adjust as needed
                cout << "Record deleted successfully." << endl;
            }
            break;
        }

        case 3:
        {
            DisplayIndexFileContent("BTreeIndex.txt");
            break;
        }

        case 4:
        {
            cout << "Enter RecordID to search: ";
            cin >> recordID;
            int referenceValue = SearchARecord("BTreeIndex.txt", recordID);
            if (referenceValue == -1)
            {
                cout << "Record not found in the index." << endl;
            }
            else
            {
                cout << "Record found at reference: " << referenceValue << endl;
            }
            break;
        }

        case 5:
        {
            cout << "Exiting program." << endl;
            break;
        }

        default:
        {
            cout << "Invalid choice. Please enter a valid option." << endl;
        }
        }
    } while (choice != 5);
}

vector<BTreeNode> BTreeIndex::readFile(const char *filename)
{
    ifstream File(filename);
    vector<BTreeNode> Btree;
    string line;
    int i = 1;
    File >> head >> head;
    getline(File, line);
    while (getline(File, line))
    {
        istringstream iss(line);
        BTreeNode Node;
        iss >> Node.isLeaf;
        Node.place = i;
        int key, value;
        while ((iss >> key >> value))
        {
            Node.node.emplace_back(key, value);
        }
        Btree.push_back(Node);
        i++;
    }
    int count = 0;
    for (size_t i = 0; i < Btree.size(); i++)
    {
        for (size_t j = 0; j < Btree[i].node.size(); j++)
        {
            if ((Btree[i].node[j].first != -1) && (Btree[i].node[j].second != -1) )
            {
                count++;
            }
        }
        Btree[i].count = count;
        count = 0;
    }
    for (int j = (Btree.size() - 1); j >= 0; --j)
    {
        if (Btree[j].isLeaf == 1)
        {
            for (int k = 0; k < Btree[j].node.size(); ++k)
            {
                if (Btree[j].node[k].second != -1)
                {
                    Btree[j].children.push_back(Btree[Btree[j].node[k].second - 1]);
                }
            }
        }
    }
    File.close();



    return Btree;
}

void BTreeIndex::savefile(const char *filename, vector<BTreeNode> bTree, int m)
{
    ofstream outFile(filename, ios::binary);
    outFile << -1 << "  " << head << "  ";
    for (int i = 0; i < (m * 2) - 1; ++i)
    {
        outFile << -1 << "  ";
    }
    outFile << "\n";
    for (const auto &node : bTree)
    {
        outFile << node.isLeaf << "  ";

        for (const auto &pair : node.node)
        {
            outFile << pair.first << "  ";
            outFile << pair.second << "  ";
        }
        outFile << "\n";
    }

    outFile.close();
}

int BTreeIndex::split(int i, vector<BTreeNode> bTree) {
    int newRecordNumber = head - 1;
    if (i == 0){
        return split_root(bTree);
    }
    if (head == -1){
        return -1;
    }
    head = bTree[head -1].node[0].first;
    vector<pair<int, int>> firstNode, secondNode;
    tie(firstNode, secondNode) = splitOriginalNode(bTree[i].node);
    int size = firstNode.size();
    int size2 = secondNode.size();

    bTree[i].isLeaf = 0;
    bTree[i].count = size;
    for (int j = size; j < m; ++j) {
        firstNode.push_back(make_pair(-1,-1));
    }
    for (int j = 0; j < m; ++j) {
        bTree[i].node[j] = firstNode[j];
    }
    bTree[i].node.pop_back();
    bTree[newRecordNumber].isLeaf = 0;
    bTree[newRecordNumber].count = size2;
    for (int j = size; j < m; ++j) {
        secondNode.push_back(make_pair(-1,-1));
    }
    for (int j = 0; j < m; ++j) {
        bTree[newRecordNumber].node[j] = secondNode[j];
    }
    savefile(BTreeFileName, bTree,m);
    return newRecordNumber;
}

bool BTreeIndex::split_root(vector<BTreeNode> bTree){
    int firstNodeIndex = head;
    int secondNodeIndex = bTree[head -1].node[0].first;
    if(firstNodeIndex == -1){
        return false;
    }
    if (secondNodeIndex == -1){
        return false;
    }
    head = bTree[head].node[0].first;
    vector<pair<int, int>> firstNode, secondNode , root;
    tie(firstNode, secondNode) = splitOriginalNode(bTree[0].node);
    int size = firstNode.size();
    int size2 = secondNode.size();
    bTree[firstNodeIndex -1].isLeaf = 0;
    bTree[firstNodeIndex -1].count = size;
    for (int i = 0; i < size; ++i) {
        bTree[firstNodeIndex -1].node[i] = firstNode[i];
    }
    bTree[secondNodeIndex -1].isLeaf = 0;
    bTree[secondNodeIndex -1].count = size2;
    for (int i = 0; i < size2; ++i) {
        bTree[secondNodeIndex -1].node[i] = secondNode[i];
    }
    root.push_back(firstNode[size-1]);
    root[0].second = firstNodeIndex;
    root.push_back(secondNode[size2-1]);
    root[1].second = secondNodeIndex;
    for (int i = root.size(); i < m; ++i) {
        root.push_back(make_pair(-1,-1));
    }
    if (firstNodeIndex > 2 && secondNodeIndex > 3){
        bTree[firstNodeIndex-1].isLeaf = 1;
        bTree[secondNodeIndex -1].isLeaf = 1;
    }
    bTree[0].node = root;
    bTree[0].isLeaf = 1;
    bTree[0].count = 2;
    savefile(BTreeFileName, bTree, m);


    return true;
}

pair<vector<pair<int, int>>, vector<pair<int, int>>> BTreeIndex::splitOriginalNode(const vector<pair<int, int>>& originalNode) {
    vector<pair<int, int>> firstNode, secondNode;

    auto middle = originalNode.begin() + originalNode.size() / 2;

    for (auto it = originalNode.begin(); it != originalNode.end(); ++it) {
        if (distance(it, middle) > 0) {
            firstNode.push_back(*it);
        } else {
            secondNode.push_back(*it);
        }
    }

    return make_pair(firstNode, secondNode);
}
int BTreeIndex::updateAfterInsert(int parentRecordNumber, int newChildRecordNumber){
    vector<pair<int,int>> newParent;
    vector<BTreeNode> bTree = readFile(BTreeFileName);
    for (int i = 0; i < bTree[parentRecordNumber].children.size(); ++i) {
        newParent.push_back(make_pair(bTree[parentRecordNumber].children[i].node[bTree[parentRecordNumber].children[i].count -1].first, bTree[parentRecordNumber].children[i].place));
    }

    if (newChildRecordNumber != -1){
        newParent.push_back(make_pair(bTree[newChildRecordNumber].node[bTree[newChildRecordNumber].count -1].first,newChildRecordNumber +1));
    }
    sort(newParent.begin(), newParent.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        if (a.first != -1 && b.first != -1) {
            return a.first < b.first;
        }
        return a.first != -1;
    });
    int size = newParent.size();
    bTree[parentRecordNumber].count = size;

    for (int i = newParent.size(); i < m; ++i) {
        newParent.push_back(make_pair(-1,-1));
    }
    bTree[parentRecordNumber].node = newParent;


    int newFromSplitIndex = -1;

    if (size > m) {
        newFromSplitIndex = split(parentRecordNumber, bTree);
    } else {
        savefile(BTreeFileName, bTree, m);
    }

    return newFromSplitIndex;
}