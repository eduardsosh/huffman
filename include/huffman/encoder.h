#ifndef PROJECT_HUFFMAN_H
#define PROJECT_HUFFMAN_H

#include <map>
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>

namespace huffman {

using CharIntPair = std::pair<char, int>;
using IntPair = std::pair<int, int>;

struct Node{
    int weight;
    char ch;
    Node* left;
    Node* right;

    Node(int weight, char ch){
        this->ch = ch;
        this-> weight = weight;
        left = nullptr;
        right = nullptr;
    }
    Node(int weight){
        this->weight = weight;
        this->ch = 0;
        left = nullptr;
        right = nullptr;
    }
};

struct CompareNode{
    bool operator()(Node* const& n1,Node* const& n2){
        return n1->weight > n2->weight;
    }
};

struct VecComparator {
    bool operator()(CharIntPair a, CharIntPair b) const {
        if (a.second != b.second) {
            return a.second < b.second;
        }
        return a.first < b.first;
    }
};

inline VecComparator vec_comp;

class Encoder {
private:
    Node* tree;
    std::map<char,int> frequency_table;
    std::vector<CharIntPair> cvl;
    std::unordered_map<char,IntPair> encoding_table;

    void print_map(const std::map<char, int>&);
    void print_clp_vec(const std::vector<CharIntPair>&);
    void print_uo_map(const std::unordered_map<char, IntPair>&);
    void print_tree(Node*, int, int);

    Node* get_tree(const std::map<char, int>&);
    std::vector<CharIntPair>& get_code_vector(Node*);
    std::vector<CharIntPair>& getCodeVector(huffman::Node* treeRoot);
    void fill_codes_table(std::unordered_map<char, IntPair>&, const std::vector<CharIntPair>&);
    void traverse_and_count(huffman::Node*, int, std::vector<CharIntPair>&);
    
public:
    int encode(const std::string&);
};

} // namespace huffman

#endif
