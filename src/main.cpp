#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<queue>
#include<stdio.h>
#include<algorithm>
#include<unordered_map>
#include<string>
#include<bitset>

struct Node{
    int weigth;
    char ch;
    Node* left;
    Node* right;

    Node(int weigth, char ch){
        this->ch = ch;
        this-> weigth = weigth;
        left = nullptr;
        right = nullptr;
    }
    Node(int weigth){
        this->weigth = weigth;
        this->ch = 0;
        left = nullptr;
        right = nullptr;
    }
};

typedef std::pair<char,int> CharIntPair;
typedef std::pair<int,int> IntPair;


void print_map(std::map<char,int> *mmap){
    if(mmap->begin()++ == mmap->end()) return;

    printf("Printing map: \n");
    for(auto it = mmap->begin();it != mmap->end(); it++){
        std::cout << " '" << it->first << "' " << it->second << std::endl;
    }
}

void print_uo_map(const std::unordered_map<char, IntPair> &map) {
    for (const auto &it : map) {
        IntPair code_len_pair = it.second;
        std::bitset<16> bit_repr(code_len_pair.first);
        std::string str_repr = bit_repr.to_string().substr(16 - code_len_pair.second);
        printf("[%c : %s]\n", it.first, str_repr.c_str());
    }
}

struct CompareNode{
    bool operator()(Node* const& n1,Node* const& n2){
        return n1->weigth > n2->weigth;
    }
};

Node* get_tree(std::map<char,int> *mmap){
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;

    // Create and enque all nodes
    for(auto it = mmap->begin();it != mmap->end(); it++){
        Node* new_node = new Node(it->second,it->first);
        pq.push(new_node);
    }
    if(pq.size() == 1){
        printf("111\n");
        return pq.top();
    }

    while(pq.size() > 1){
        Node* node1 = pq.top();
        pq.pop();
        Node* node2 = pq.top();
        pq.pop();

        // New node with these two as children
        int new_weigth = node1->weigth + node2->weigth;
        Node* new_node = new Node(new_weigth);
        new_node->left = node1;
        new_node->right = node2;
        pq.push(new_node);
    }

    Node* root_node = pq.top();
    return root_node;
}

void printTree(Node* root, int space = 0, int height = 1) {
    if (root == nullptr) return;
    space += height;

    printTree(root->right, space);

    for (int i = height; i < space; i++) std::cout << "      ";
    std::cout << root->weigth << ":" <<root->ch << "<\n";

    // Process left child
    printTree(root->left, space);
}

// DFS to get symbols and their length
void traverseAndCount(Node* cNode, int depth, std::vector<CharIntPair>& vecpoint){
    // Base case
    if(cNode->ch){
        vecpoint.push_back({cNode->ch,depth});
        return;
    }

    if(cNode->right != nullptr){
        traverseAndCount(cNode->right,depth+1,vecpoint);
    }
    if(cNode->left != nullptr){
        traverseAndCount(cNode->left,depth+1,vecpoint);
    }
    return;
}

std::vector<CharIntPair>& getCodeVector(Node* treeRoot){
    static std::vector<CharIntPair> clv;
    traverseAndCount(treeRoot,0,clv);
    return clv; 
}

void print_clp_vec(std::vector<CharIntPair> &clv){
    for(auto i : clv){
        std::cout<< i.first << " :" << i.second << "\n";
    }
}

struct{
    bool operator()(CharIntPair a, CharIntPair b) const {
        if (a.second != b.second) {
            return a.second < b.second; // Sort by second value first
        }
        return a.first < b.first; // If second values are equal, sort by first
    }
}VecComp;

void fill_codes_table(
    std::unordered_map<char, IntPair> &table, 
    std::vector<CharIntPair> &cvl
) {
    if (cvl.empty()) return;

    int prev_length = 0;
    int code = 0;

    for (size_t idx = 0; idx < cvl.size(); ++idx) {
        auto& i = cvl[idx];

        // idx=0 handles first case
        if (idx > 0 && i.second > prev_length) {
            code <<= (i.second - prev_length);
        }

        table[i.first] = {code, i.second};
        code++;

        prev_length = i.second;
    }
}

int main() {
    std::ifstream file("input.txt");

    std::map<char,int> frequency_table;

    if (!file.is_open()) {
        std::cerr << "Failed to open the file.\n";
        return 1;
    }

    char ch;
    while (file.get(ch)) {
        //std::cout << ch << " " << (int)ch << std::endl;
        if(auto it = frequency_table.find(ch); it != frequency_table.end()){
            it->second++;
        }else{
            frequency_table.insert({ch,1});
        }
    }

    print_map(&frequency_table);
    file.close();

    Node* tree = get_tree(&frequency_table);
    printf("Printing tree:\n");
    printTree(tree);

    std::vector<CharIntPair> cvl;
    cvl = getCodeVector(tree);
    printf("Code vector: \n");
    //print_clp_vec(cvl);
    std::sort(cvl.begin(),cvl.end(),VecComp);
    print_clp_vec(cvl);
    std::unordered_map<char,IntPair> encoding_table;

    fill_codes_table(encoding_table,cvl);
    print_uo_map(encoding_table);
    
    return 0;
}
