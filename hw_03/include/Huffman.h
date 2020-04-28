#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <utility>
#include <unordered_map>
#include <map>

struct HuffmanTree {
private:
    class HuffmanNode {
    public:
        HuffmanNode() : data(), freq(0), left(nullptr), right(nullptr) {}

        HuffmanNode(int freq) : data(127), freq(freq) {}

        HuffmanNode(const char &data, int freq);

        int get_freq() const noexcept { return freq; }

        void set_children_nodes(HuffmanNode *left, HuffmanNode *right);

        HuffmanNode *get_right_child() const noexcept { return right; }

        HuffmanNode *get_left_child() const noexcept { return left; }

        char get_symbol() const noexcept { return data; }

    private:
        char data;
        int freq;
        HuffmanNode *left;
        HuffmanNode *right;
    };

public:
    HuffmanTree();

    void build_tree(uint32_t (&freq_table)[256]);

    void traverse(HuffmanNode &node, const std::string &code);

    void make_decode_table();

    bool check_code(const std::string &code) { return decode_table.find(code) != decode_table.end(); }

    char get_symbol(const std::string &code) { return decode_table[code]; }

    std::string get_code(char symbol) { return encode_table[symbol]; }

    std::unordered_map<char, std::string> *get_encode_table() { return &encode_table; }

    struct Comp {

        bool operator()(const HuffmanNode &lhs, const HuffmanNode &rhs) const {
            return lhs.get_freq() > rhs.get_freq();
        }
    };


private:
    HuffmanNode head;
    std::priority_queue<HuffmanNode, std::vector<HuffmanNode>, Comp> tree;
    std::unordered_map<std::string, char> decode_table;
    std::unordered_map<char, std::string> encode_table;
    std::vector<HuffmanNode> buff;
};