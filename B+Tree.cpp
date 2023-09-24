#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

enum NodeType {
    INTERNAL,
    LEAF
};
enum SearchMode {
    EQUAL,
    LT,
    LE,
    GT,
    GE
};

class Node{
public:
    vector<int> key;
    vector<Node*> child;
    Node* left;
    Node* right;
    int type;

    Node(int type){
        this->type=type;
        if(type == LEAF){
            left=right=NULL;
        }
    }
};

class Tree{
public:
    Node* root;
    int order;
    
    Tree(int order){
        this->order=order;
        root = new Node(LEAF);
    }
    
    bool search(int data, SearchMode mode);
    bool searchRecur(Node* node, int data, SearchMode mode);
    void insert(int data, bool redis);
    bool insertRecur(Node* node, int data, bool redis);
    void splitChild(Node* node, int &key, Node* &rightChild);
    void bulkLoading(vector<int> data);
    bool bulkLoadingRecur(Node* node, vector<int> data);
    void display();
    void displayRecur(Node* r,int level);
    void dele(int data);
    bool deleteRecur(Node* node, int data);
    bool empty();
};

bool isNumber(string s){
    int len=s.size();
    if(len == 1 && !isdigit(s[0])){
        return false;
    }    

    if(s[0] != '-' && !isdigit(s[0])){
        return false;
    }
    for(int i=1; i<len; i++) {
        if(!isdigit(s[i])) 
            return false;
    }
    return true;
}

void usage(){
    cout<<"\n------------------------------------Usage------------------------------------\n";
    cout<<"You can using the following instruction.\n\n";
    cout<<"> insert data1 data2 ...    : insert one or more data\n";
    cout<<"> insert -r data1 data2 ... : insert and redistribute one or more data\n";
    cout<<"> insert -b data1 data2 ... : insert data by bulk loading\n\n";

    cout<<"> delete data1 data2 ...    : delete one or more data\n\n";
    
    cout<<"> search data               : search the data is exist or not\n";
    cout<<"> search operator data      : range search data (operator can be <, <=, >, >=)\n\n";
    
    cout<<"> display                   : display the B+ tree\n";
    cout<<"> quit                      : end the program\n";
    cout<<"> help                      : get some help, just like you are here! \n";
    cout<<"-------------------------------------------------------------------------------\n\n";
}

void error(){
    cout<<"Please check your entry is correct and try again.\n";
}

int main(){
    int order=0;
    int data;
    int inputLen;
    vector<string> inputVec;
    string input;
    string subInput;
    stringstream ssInput;
    SearchMode searchMode;
    Tree* tree;
    
    // init setting
    cout<<"Welcome to use the B+ Tree.\n";
    cout<<"Please enter the order of the B+ Tree: ";
    getline(cin, input);
    while(1){
        if(isNumber(input)){
            order=stoi(input);
        }
        if(order >= 1){
            break;
        }
        cout<<"The order must large than 0, please enter again: ";
        getline(cin, input);
    }
    
    tree = new Tree(order);

    cout<<"\n";
    cout<<"If you need help or usage, please enter help.\n";
    cout<<"Now, you can use your B+ tree.\n";

    while(1){

        // get the input
        cout<<"> ";
        getline(cin, input);
        if(cin.eof()){
            break;
        }
        ssInput.clear();
        ssInput.str(input);
        inputVec.clear();
        while(ssInput>>subInput) {
            inputVec.push_back(subInput);
        }

        // check instruction
        inputLen=inputVec.size();
        if(inputLen == 0){
            continue;
        }
        else if(inputLen == 1 && inputVec[0] == "display"){
            tree->display();
            cout<<"\n";
        }
        else if(inputLen == 1 && inputVec[0] == "quit"){
            break;
        }
        else if(inputLen == 1 && inputVec[0] == "help"){
            usage();
        }
        else if(inputVec[0] == "search"){
            if(inputLen == 2 && isNumber(inputVec[1])){
                data=stoi(inputVec[1]);
                if(tree->search(data, EQUAL)){
                    cout<<"The tree contain the value: "<<data<<"\n\n";
                }
                else{
                    cout<<"The tree does not contain the value: "<<data<<"\n\n";
                }
            }
            else if(inputLen == 3){
                if(inputVec[1] == "<="){
                    searchMode=SearchMode::LE;
                }
                else if(inputVec[1] == "<"){
                    searchMode=SearchMode::LT;
                }
                else if(inputVec[1] == ">="){
                    searchMode=SearchMode::GE;
                }
                else if(inputVec[1] == ">"){
                    searchMode=SearchMode::GT;
                }
                else{
                    error();
                    continue;
                }

                if(isNumber(inputVec[2])){
                    data=stoi(inputVec[2]);
                    tree->search(data, searchMode);
                    cout<<"\n";
                }
                else{
                    error();
                }
            }
            else{
                error();
            }
        }
        else if(inputVec[0] == "insert"){
            int start=1;
            bool redis=false;
            bool bulkLoading=false;
            bool check=true;

            // check input
            if(inputLen > 2 && inputVec[1] == "-r"){
                start=2;
                redis=true;
            }
            else if(inputLen > 2 && inputVec[1] == "-b"){
                start=2;
                bulkLoading=true;
            }

            if(start == inputLen){
                error();
                continue;
            }

            for(int i=start; i<inputLen; i++){
                if(!isNumber(inputVec[i])){
                    check=false;
                    break;
                }
            }

            if(check == false){
                error();
                continue;
            }
            
            if(bulkLoading == true && tree->empty()){
                vector<int> dataVec;
                for(int i=start; i<inputLen; i++){
                    data=stoi(inputVec[i]);
                    dataVec.push_back(data);
                }
                tree->bulkLoading(dataVec);
                // tree->display();
                // cout<<"\n";
            }
            else if(bulkLoading == true && tree->empty() == false){
                cout<<"You can only use bulk loading when the tree is empty.\n";
            }
            else{
                for(int i=start; i<inputLen; i++){
                    data=stoi(inputVec[i]);
                    cout<<"Insert data "<<data<<" :\n";
                    tree->insert(data,redis);
                    tree->display();
                    cout<<"\n";
                }
            }
        } 
        else if(inputVec[0] == "delete"){
            bool check=true;
            // check input
            if(inputLen < 2){
                error();
                continue;
            }

            for(int i=1; i<inputLen; i++){
                if(!isNumber(inputVec[i])){
                    check=false;
                    break;
                }
            }
            if(check == false){
                error();
                continue;
            }

            for(int i=1; i<inputLen; i++){
                data=stoi(inputVec[i]);
                cout<<"Delete data "<<data<<" :\n";
                tree->dele(data);
                tree->display();
                cout<<"\n";
            }
        }
        else{
            error();
        }
    }

    // quit
    cout<<"\n";
    cout<<"Thank you for using.\n";
    cout<<"Bye~\n";
    return 0;
}

bool Tree::search(int data, SearchMode mode){
    return searchRecur(root,data,mode);
}
bool Tree::searchRecur(Node* node, int data, SearchMode mode){
    if(node->type == LEAF){
        // find the position
        int len=node->key.size();
        int i=0;
        int pos=-1; // the position of key which <= data
        bool find=false;
        for(; i<len; i++){
            if(data == node->key[i]){
                find=true;
            }
            if(data >= node->key[i]){
                pos=i;
            }
        }
    
        // left
        if(mode == LT || mode == LE){
            if(find == true and mode == LT){
                pos--;
            }
            for(i=pos; i>=0; i--){
                cout<<node->key[i]<<" ";
            }
            node=node->left;
            while(node != NULL){
                len=node->key.size();
                for(i=len-1; i>=0; i--){
                    cout<<node->key[i]<<" ";
                }
                node=node->left;
            }
            cout<<"\n";
        }
        // right
        if(mode == GT || mode == GE){
            if(!(find == true and mode == GE)){
                pos++;
            }
            for(i=pos; i<len; i++){
                cout<<node->key[i]<<" ";
            }
            node=node->right;
            while(node != NULL){
                len=node->key.size();
                for(i=0; i<len; i++){
                    cout<<node->key[i]<<" ";
                }
                node=node->right;
            }
            cout<<"\n";
        }
        return find;        
    }
    else{
        int len=node->key.size();
        int i=0;
        for(; i<len; i++){
            if(data < node->key[i]){
                break;
            }
        }
        return searchRecur(node->child[i],data,mode);
    }
}

void Tree::splitChild(Node* node,int &key, Node* &rightChild){
    if(node->type == LEAF){
        Node* newLeaf=new Node(LEAF);
    
        // 0 ~ order-1          -> left
        // order ~ end(2*order) -> right
        newLeaf->key.assign(node->key.begin()+order, node->key.end()); // right
        node->key.assign(node->key.begin(), node->key.begin()+order);  // left
        
        // handle the link between leaf 
        newLeaf->right=node->right;
        newLeaf->left=node;
        if(node->right != NULL){
            node->right->left=newLeaf;
        }
        node->right=newLeaf;
        
        key=newLeaf->key[0];
        rightChild=newLeaf;
    }
    else{
        Node* newNode=new Node(INTERNAL);
        
        // key
        // 0 ~ order-1            -> left
        // order                  -> push key
        // order+1 ~ end(2*order) -> right
        int pushKey=node->key[order];
        newNode->key.assign(node->key.begin()+order+1, node->key.end()); // right 
        node->key.assign(node->key.begin(), node->key.begin()+order);    // left

        // child
        // 0 ~ order              -> left parent
        // order+1 ~ end(2*order) -> right parent
        newNode->child.assign(node->child.begin()+order+1, node->child.end()); // right
        node->child.assign(node->child.begin(), node->child.begin()+order+1);  // left

        key=pushKey;
        rightChild=newNode;
    }
}

void Tree::insert(int data,bool redis){
    bool result=insertRecur(root, data,redis);
    int key;
    Node* rightChild;
    
    if(result == false){ // root is split, increase height
        splitChild(root,key,rightChild);
        
        Node* newRoot=new Node(INTERNAL);
        newRoot->key.push_back(key);
        newRoot->child.push_back(root);
        newRoot->child.push_back(rightChild);
        root=newRoot;
    }
}

// return true: don't need no change
// return false: need to change
bool Tree::insertRecur(Node* node, int data, bool redis){
    
    if(node->type == LEAF){
        // find the insert position
        int len=node->key.size();
        int i=0;
        for(; i<len; i++){
            if(node->key[i] == data){
                cout<<"already contain value: "<<data<<"\n";
                return true;
            }
            if(node->key[i] > data){
                break;
            }
        }

        // insert new value
        node->key.insert(node->key.begin()+i,data);
        
        // check for overflow
        if(node->key.size() <= order*2){
            return true;
        }
        else{
            return false; // overflow, need to split
        }
    }
    else{ // internal node
        int len=node->key.size();
        int i=0;
        bool result;
        Node* rightChild;
        int key;

        // find the position
        for(; i<len; i++){
            if(data < node->key[i]){
                break;
            }
        }
        
        result=insertRecur(node->child[i],data,redis);

        if(result == true){ // no change
            return true;
        }
        else{ // child need to split
            // redistribution
            if(redis == true && i-1 >= 0 && node->child[i-1]->key.size() < 2*order){
                Node* child1=node->child[i-1];
                Node* child2=node->child[i];

                if(child1->type == LEAF){
                    child1->key.push_back(child2->key.front());
                    child2->key.erase(child2->key.begin());
                    
                    node->key[i-1]=child2->key.front();
                }

                else if(child1->type == INTERNAL){
                    child1->key.push_back(node->key[i-1]);
                    node->key[i-1]=child2->key.front();
                    child2->key.erase(child2->key.begin());

                    child1->child.push_back(child2->child.front());
                    child2->child.erase(child2->child.begin());
                }
                return true;
            }
            else if(redis == true && i+1 <= len && node->child[i+1]->key.size() < 2*order){
                Node* child1=node->child[i];
                Node* child2=node->child[i+1];

                if(child1->type == LEAF){
                    node->key[i]=child1->key.back();
                    
                    child2->key.insert(child2->key.begin(), child1->key.back());
                    child1->key.pop_back();
                }
                else if(child1->type == INTERNAL){
                    child2->key.insert(child2->key.begin(), node->key[i]);
                    node->key[i]=child1->key.back();
                    child1->key.pop_back();

                    child2->child.insert(child2->child.begin(), child1->child.back());
                    child1->child.pop_back();
                }
                return true;
            }
            // default
            else{
                splitChild(node->child[i],key,rightChild);

                // insert
                node->key.insert(node->key.begin()+i,key);
                node->child.insert(node->child.begin()+i+1,rightChild);

                // check for overflow
                if(node->key.size() <= order*2){
                    return true;
                }
                else{
                    return false;  // overflow, need to split
                }
            }
        }
    }
}

void Tree::bulkLoading(vector<int> data){
    int len;
    int i=0,j=order*2;
    bool result;
    int key;
    vector<int> nodeData;
    Node* rightChild;

    // sort and remove duplicate 
    len=data.size();
    sort(data.begin(),data.end());
    auto last = unique(data.begin(), data.end());
    data.erase(last, data.end());
    if(len !=data.size()){
        cout<<"The duplicate data is removed.\n";
        len=data.size();
    }

    while(j<=len){
        nodeData.assign(data.begin()+i, data.begin()+j);
        result=bulkLoadingRecur(root,nodeData);
        
        if(result == false){ // root is split, increase height
            if(root->type == LEAF && empty()){
                root->key=nodeData;
            }
            else{
                if(root->type == LEAF){
                    Node *newLeaf=new Node(LEAF);
                    newLeaf->key=nodeData;
                    newLeaf->right=NULL;
                    newLeaf->left=root;
                    root->right=newLeaf;
                    
                    key=nodeData[0];
                    rightChild=newLeaf;
                }
                else{
                    splitChild(root,key,rightChild);
                }
                Node* newRoot=new Node(INTERNAL);
                newRoot->key.push_back(key);
                newRoot->child.push_back(root);
                newRoot->child.push_back(rightChild);
                root=newRoot;
            }

        }

        // display
        cout<<"Insert data ";
        for(int d:nodeData){
            cout<<d<<" ";
        }
        cout<<":\n";
        display();
        cout<<"\n";

        // next range
        i=j;
        j+=order*2;
        if(j>len && len-i>=order){
            j=len;
        }
    }
    // just like insert
    for(; i<len; i++){
        insert(data[i],false);
        cout<<"Insert data "<<data[i]<<" :\n";
        display();
        cout<<"\n";

    }
}

bool Tree::bulkLoadingRecur(Node* node, vector<int> data){
    if(node->type == LEAF){
        return false; // need to create a new leaf 
    }
    else{ // internal node, same as insert
        bool result;
        Node* rightChild;
        int key;
        
        result=bulkLoadingRecur(node->child.back(),data);

        if(result == true){ // no change
            return true;
        }
        else{ // child need to split
            if(node->child.back()->type == LEAF){
                Node *newLeaf=new Node(LEAF);
                newLeaf->key=data;
                
                newLeaf->right=NULL;
                newLeaf->left=node->child.back();
                node->child.back()->right=newLeaf;

                key=data[0];
                rightChild=newLeaf;
            }
            else{
                splitChild(node->child.back(),key,rightChild);
            }

            // insert
            node->key.push_back(key);
            node->child.push_back(rightChild);

            // check for overflow
            if(node->key.size() <= order*2){
                return true;
            }
            else{
                return false;  // overflow, need to split
            }
        }
    }
}

void Tree::display(){
    displayRecur(root,0);
}

void Tree::displayRecur(Node* node,int level){
    for(int i=0; i<level; i++){
        cout<<"     ";
    }
    if(node->type == LEAF){
        cout<<"[";
        for(int i=0; i < order*2; i++){
            if(i < node->key.size()){
                cout << node->key[i];
            }
            else{
                cout << "_";
            }

            if(i != order*2-1){
                cout<<",";
            }
        }
        cout<<"]\n";
        return;
    }
    else{
        cout<<"(";
        for(int i=0; i < order*2; i++){
            if(i < node->key.size()){
                cout << node->key[i];
            }
            else{
                cout << "_";
            }

            if(i != order*2-1){
                cout<<":";
            }
        }    
        cout<<")\n";

        for(auto child: node->child){
            displayRecur(child,level+1);
        }
    }
}

void Tree::dele(int data){
    bool result=deleteRecur(root,data);
    if(result == false){
        if(root->type == INTERNAL && root->key.size() == 0){ // root is empty, decrease the height
            Node* tmp=root;
            root=root->child[0];
            delete tmp;
        }
    }

}

// return true: don't need no change
// return false: need to change
bool Tree::deleteRecur(Node* node, int data){
    if(node->type == LEAF){
        // find the position of data 
        int len=node->key.size();
        int pos=-1;
        for(int i=0; i<len; i++){
            if(node->key[i] == data){
                pos=i;
                break;
            }
        }

        if(pos !=  -1){ // found
            node->key.erase(node->key.begin() + pos); // delete

            // check for underflow
            if(node->key.size() < order){
                return false;
            }
            else{
                return true; // have enough value
            }
        }
        else{ // not found
            cout<<"does not contain value: "<<data<<"\n";
            return true; // no change
        }
    }
    else{ // internal node
        int len=node->key.size();
        int i=0;
        bool result;
        int val,key; // val: the number need to move to another child node, key: the key of parent
        Node* child1, * child2, * child;

        // find the position
        for(; i<len; i++){
            if(data < node->key[i]){
                break;
            }
        }
        
        result=deleteRecur(node->child[i],data);
        
        if(result == true){ // no change
            return true;
        }
        else{
            
            if(i-1>=0 && node->child[i-1]->key.size() > order){ // borrow from left sibling
                child1=node->child[i-1];
                child2=node->child[i];
                
                if(child1->type == LEAF){
                    // handel key
                    val=child1->key.back();
                    child1->key.pop_back();

                    node->key[i-1]=val;
                    child2->key.insert(child2->key.begin(), val);
                }
                
                else{
                    // handel key
                    val=node->key[i-1];
                    key=child1->key.back(); // index key
                    child1->key.pop_back();
                    
                    node->key[i-1]=key;
                    child2->key.insert(child2->key.begin(), val);
                    
                    // handel child
                    child=child1->child.back();
                    child1->child.pop_back();
                    child2->child.insert(child2->child.begin(), child);
                }

                return true;
            }
            else if(i+1<=len && node->child[i+1]->key.size() > order){ // borrow from right sibling
                child1=node->child[i];
                child2=node->child[i+1];
                
                if(child1->type == LEAF){
                    // handel key
                    val=child2->key.front();
                    child2->key.erase(child2->key.begin());
                    key=child2->key.front(); // index key
                    
                    node->key[i]=key;
                    child1->key.push_back(val);
                }
                

                else{
                    // handel key
                    val=node->key[i];
                    key=child2->key.front(); // index key
                    child2->key.erase(child2->key.begin());
                    
                    node->key[i]=key;
                    child1->key.push_back(val);
                    
                    // handel child
                    child=child2->child.front();
                    child2->child.erase(child2->child.begin());
                    child1->child.push_back(child);
                }

                return true;
            }
            else if(i>0){ // merge with left sibling
                child1 = node->child[i-1];
                child2 = node->child[i];

                if(child1->type == LEAF){
                    child1->key.insert(child1->key.end(), child2->key.begin(), child2->key.end());
                    
                    child1->right=child2->right;
                    if(child2->right != NULL){
                        child2->right->left=child1;
                    }
                }
                else{
                    child1->key.push_back(node->key[i-1]); // merge with parent and sibling
                    child1->key.insert(child1->key.end(), child2->key.begin(), child2->key.end());
                    child1->child.insert(child1->child.end(), child2->child.begin(), child2->child.end());
                }

                node->key.erase(node->key.begin() + i-1); // delete key
                node->child.erase(node->child.begin() + i); // delete child2
                delete child2;
            }
            else if(i < len){ // merge with right sibling
                child1 = node->child[i];
                child2 = node->child[i+1];

                if(child1->type == LEAF){
                    child1->key.insert(child1->key.end(), child2->key.begin(), child2->key.end());
                    
                    child1->right=child2->right;
                    if(child2->right != NULL){
                        child2->right->left=child1;
                    }
                }
                else{
                    child1->key.push_back(node->key[i]);  // merge with parent and sibling
                    child1->key.insert(child1->key.end(), child2->key.begin(), child2->key.end());
                    child1->child.insert(child1->child.end(), child2->child.begin(), child2->child.end());
                }

                node->key.erase(node->key.begin() + i); // delete key
                node->child.erase(node->child.begin() + i + 1); // delete child2
                delete child2;
            }
            else{
                cout<<"error!!!\n";
                return true;
            }
            
            // check for underflow
            if(node->key.size()>=order){
                return true;
            }
            else{
                return false;
            }
        }
    }
}

bool Tree::empty(){
    if(root->key.size() == 0){
        return true;
    }
    else{
        return false;
    }
}

