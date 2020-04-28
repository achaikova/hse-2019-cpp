#include "doctest.h"
#include <unordered_map>

#include "Huffman.h"

bool prefix_free(int max_code_length, HuffmanTree *tree, const std::string& code) {
    if (code.size() == max_code_length) {
        return true;
    }

    if (tree->get_symbol(code + "1") || tree->get_symbol(code + "0")) {
        return false;
    } else {
        return prefix_free(max_code_length, tree, code + "1") && prefix_free(max_code_length, tree, code + "0");
    }
}

TEST_CASE ("prefix-free huffman codes") {
    uint32_t freq_table[256] = {0};
            SUBCASE("test_text_01") {
        std::string test_text = "abraKqwert  yuiopasdfghjkl;zxcvbnm,\n\n\t"
                                ".+_)(!12345KKKbdviuf  bv1234567890cadabra";
        for (const char &sym : test_text) {
            freq_table[sym]++;
        }

        HuffmanTree tree;
        tree.build_tree(freq_table);
        tree.make_decode_table();

        int max_code_length = 0;
        std::unordered_map<char, std::string> *encode_table = tree.get_encode_table();
        for (const char &i : test_text) {
            max_code_length = std::max(max_code_length, (int) tree.get_code(i).size());
        }

        for (const auto &i : *encode_table) {
                    CHECK(prefix_free(max_code_length, &tree, i.second));
        }
    }

            SUBCASE("test_text_02") {
        std::string test_text = "-9.498353e+02  1.934651e+01 -1.933336e+03  2.987594e+03\n"
                                "-1.707563e+02  9.458976e+00 -1.442193e+03 -9.248797e+02\n"
                                "42Cr (ap) 45Mn";
        for (const char &sym : test_text) {
            freq_table[sym]++;
        }

        HuffmanTree tree;
        tree.build_tree(freq_table);
        tree.make_decode_table();

        int max_code_length = 0;
        std::unordered_map<char, std::string> *encode_table = tree.get_encode_table();
        for (const char &i : test_text) {
            max_code_length = std::max(max_code_length, (int) tree.get_code(i).size());
        }

        for (const auto &i : *encode_table) {
                    CHECK(prefix_free(max_code_length, &tree, i.second));
        }
    }
}