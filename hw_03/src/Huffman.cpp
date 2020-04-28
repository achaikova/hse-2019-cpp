#include "Huffman.h"

//------------------------HuffmanTree------------------------------------//

HuffmanTree::HuffmanTree() {
    buff.resize(1000);
}

void HuffmanTree::build_tree(uint32_t (&freq_table)[256]) {
    int tmp_freq;
    for (int i = 0; i < 256; i++) {
        if (freq_table[i]) {
            tree.push(HuffmanNode(static_cast<char>(i), freq_table[i]));
        }
    }

    while (tree.size() > 1) {
        HuffmanNode tmp_l = tree.top();
        tree.pop();
        buff.push_back(tmp_l);

        HuffmanNode tmp_r = tree.top();
        tree.pop();
        buff.push_back(tmp_r);

        tmp_freq = tmp_l.get_freq() + tmp_r.get_freq();
        HuffmanNode new_node(tmp_freq);
        new_node.set_children_nodes(&buff[buff.size() - 2], &buff[buff.size() - 1]);
        tree.push(new_node);
    }
    if (!tree.empty()) {
        head = tree.top();
        traverse(head, "");
    }
}

void HuffmanTree::traverse(HuffmanNode &node, const std::string &code) {
    if (node.get_right_child() != nullptr) {
        std::string code1 = code;
        code1 += "0";
        traverse(*node.get_right_child(), code1);
    }
    if (node.get_left_child() != nullptr) {
        std::string code1 = code;
        code1 += "1";
        traverse(*node.get_left_child(), code1);
    }
    if (node.get_left_child() == nullptr && node.get_right_child() == nullptr) {
        encode_table[node.get_symbol()] = code;
    }
}

void HuffmanTree::make_decode_table() {
    for (const auto &i : encode_table) {
        decode_table[i.second] = i.first;
    }
}

//------------------------HuffmanTree::HuffmanNode------------------------------------//

HuffmanTree::HuffmanNode::HuffmanNode(const char &data, int freq)
        : data(data), freq(freq),
          left(nullptr), right(nullptr) {}

void HuffmanTree::HuffmanNode::set_children_nodes(HuffmanTree::HuffmanNode *new_left,
                                                  HuffmanTree::HuffmanNode *new_right) {
    left = new_left;
    right = new_right;
}