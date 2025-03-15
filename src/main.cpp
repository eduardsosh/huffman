#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<queue>
#include<stdio.h>

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

typedef std::pair<char,int> charLenPair;

void print_map(std::map<char,int> *mmap){
    printf("Printing map: \n");
    for(auto it = mmap->begin();it != mmap->end(); it++){
        std::cout << " '" << it->first << "' " << it->second << std::endl;
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
void traverseAndCount(Node* cNode, int depth, std::vector<charLenPair>& vecpoint){
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

std::vector<charLenPair>& getCodeVector(Node* treeRoot){
    static std::vector<charLenPair> clv;
    traverseAndCount(treeRoot,0,clv);
    return clv; 
}

void print_clp_vec(std::vector<charLenPair> &clv){
    for(auto i : clv){
        std::cout<< i.first << " :" << i.second << "\n";
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

    std::vector<charLenPair> cvl;
    cvl = getCodeVector(tree);
    print_clp_vec(cvl);
    return 0;
}
