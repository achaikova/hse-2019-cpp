#pragma once

#include <iostream>
#include <exception>
#include <unordered_map>
#include <fstream>
#include "Huffman.h"

class HuffmanArchiverEncode {
public:
    std::istream *input_handler;
    std::ostream *output_handler;
private:
    HuffmanTree *huff_tree;
    uint32_t freq_table[256] = {0};
    uint32_t input_file_size, output_file_size, additional_data, table_size;
public:
    HuffmanArchiverEncode(std::istream &in, std::ostream &out);

    void encode_input();

    ~HuffmanArchiverEncode();

    void load_file();

    void save_file();

    void write_table();

    void write_text();

    void write_code(std::string &code);

    void write_statistics();

    // for testing

    std::tuple<uint32_t, uint32_t, uint32_t> get_statistics() noexcept {
        return std::make_tuple(input_file_size, output_file_size, table_size);
    }

    const uint32_t *get_freq_table() const { return freq_table; }

private:
    void set_input_size();
};

class HuffmanArchiverDecode {
public:
    std::istream *input_handler;
    std::ostream *output_handler;
private:
    HuffmanTree *huff_tree;
    uint32_t freq_table[256] = {0};
    size_t input_file_size, output_file_size, additional_data, num_of_symbols;
public:
    HuffmanArchiverDecode(std::istream &in, std::ostream &out);

    void decode_input();

    ~HuffmanArchiverDecode();

    void load_file();

    void save_file();

    void write_text();

    void write_statistics();

    // for testing

    std::tuple<uint32_t, uint32_t, uint32_t> get_statistics() noexcept {
        return std::make_tuple(input_file_size, output_file_size, additional_data);
    }

    const uint32_t *get_freq_table() const { return freq_table; }

private:
    void set_input_size();

};

class HuffmanArchiverException : public std::exception {
private:
    std::string _error;
public:
    HuffmanArchiverException(std::string _error);

    std::string get();

    virtual const char *what() const throw();
};