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
#include "huffman/encoder.h"

using namespace huffman;

void Encoder::print_map(const std::map<char,int> &mmap){
    if(mmap.begin()++ == mmap.end()) return;

    printf("Printing map: \n");
    for(auto it = mmap.begin();it != mmap.end(); it++){
        std::cout << " '" << it->first << "' " << it->second << std::endl;
    }
}

void Encoder::print_uo_map(const std::unordered_map<char, IntPair> &map) {
    for (const auto &it : map) {
        IntPair code_len_pair = it.second;
        std::bitset<16> bit_repr(code_len_pair.first);
        std::string str_repr = bit_repr.to_string().substr(16 - code_len_pair.second);
        printf("[%c : %s]\n", it.first, str_repr.c_str());
    }
}

Node* Encoder::get_tree(const std::map<char,int> &mmap){
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;

    // Create and enque all nodes
    for(auto it = mmap.begin();it != mmap.end(); it++){
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
        int new_weigth = node1->weight + node2->weight;
        Node* new_node = new Node(new_weigth);
        new_node->left = node1;
        new_node->right = node2;
        pq.push(new_node);
    }

    Node* root_node = pq.top();
    return root_node;
}

void Encoder::print_tree(huffman::Node* root, int space = 0, int height = 1) {
    if (root == nullptr) return;
    space += height;

    print_tree(root->right, space);

    for (int i = height; i < space; i++) std::cout << "      ";
    std::cout << root->weight << ":" <<root->ch << "<\n";

    // Process left child
    print_tree(root->left, space);
}

// DFS to get symbols and their length
void Encoder::traverse_and_count(huffman::Node* cNode, int depth, std::vector<CharIntPair>& vecpoint){
    // Base case
    if(cNode->right == nullptr && cNode->left == nullptr){
        vecpoint.push_back({cNode->ch,depth});
        return;
    }

    if(cNode->right != nullptr){
        traverse_and_count(cNode->right,depth+1,vecpoint);
    }
    if(cNode->left != nullptr){
        traverse_and_count(cNode->left,depth+1,vecpoint);
    }
    return;
}

std::vector<CharIntPair>& Encoder::getCodeVector(huffman::Node* treeRoot){
    static std::vector<CharIntPair> clv;
    clv.clear();
    traverse_and_count(treeRoot,0,clv);
    return clv; 
}

void Encoder::print_clp_vec(const std::vector<CharIntPair> &clv){
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

void Encoder::fill_codes_table(
    std::unordered_map<char, IntPair> &table, 
    const std::vector<CharIntPair> &cvl
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

int Encoder::encode(const std::string& filename){
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file.\n";
        return -1;
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
    file.close();

    print_map(frequency_table);
    tree = get_tree(frequency_table);
    printf("Printing tree:\n");
    print_tree(tree);

    cvl = getCodeVector(tree);
    printf("Code vector: \n");
    std::sort(cvl.begin(),cvl.end(),VecComp);
    print_clp_vec(cvl);
    fill_codes_table(encoding_table,cvl);
    print_uo_map(encoding_table);
    return 0;
}