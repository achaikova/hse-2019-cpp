#include "HuffmanArchiver.h"

template<typename T>
void write_bin(std::ostream &out, T &t) {
    out.write(reinterpret_cast<const char *>(&t), sizeof(t));
}

template<typename T>
void write_bin(std::ostream &out, T &&t) {
    out.write(reinterpret_cast<const char *>(&t), sizeof(t));
}

template<typename T>
void read_bin(std::istream &in, T &t) {
    in.read(reinterpret_cast<char *>(&t), sizeof(t));
}

//------------------------HuffmanArchiverEncode------------------------------------//

HuffmanArchiverEncode::HuffmanArchiverEncode(std::istream &in, std::ostream &out)
        : input_handler(&in),
          output_handler(&out),
          huff_tree(new HuffmanTree()),
          input_file_size(0), output_file_size(0), additional_data(0), table_size(0) {

    if (input_handler->fail()) {
        throw HuffmanArchiverException("Cannot open input file");
    }
    if (output_handler->fail()) {
        throw HuffmanArchiverException("Cannot open output file");
    }
}

void HuffmanArchiverEncode::encode_input() {
    load_file();
    save_file();
}

HuffmanArchiverEncode::~HuffmanArchiverEncode() {
    delete (huff_tree);
}

void HuffmanArchiverEncode::load_file() {
    set_input_size();

    char sym;
    while (input_handler->get(sym)) {
        if (!freq_table[sym])table_size++;
        freq_table[sym]++;
    }

    additional_data = table_size * (sizeof(char) + sizeof(uint32_t));

    huff_tree->build_tree(freq_table);
}

void HuffmanArchiverEncode::set_input_size() {
    input_handler->ignore(std::numeric_limits<std::streamsize>::max());
    input_file_size = input_handler->gcount();
    input_handler->clear();
    input_handler->seekg(0, std::ios_base::beg);
}

void HuffmanArchiverEncode::save_file() {
    write_table();
    write_text();
}

void HuffmanArchiverEncode::write_table() {
    if (table_size != 0) {
        additional_data += sizeof(uint32_t);
        write_bin(*output_handler, table_size);
    }

    for (int i = 0; i < 256; i++) {
        if (freq_table[i]) {
            write_bin(*output_handler, static_cast<char>(i));
            write_bin(*output_handler, freq_table[i]);
        }
    }
}

void HuffmanArchiverEncode::write_text() {
    char sym;
    std::string code;

    input_handler->clear();
    input_handler->seekg(0, std::ios_base::beg);

    while (input_handler->get(sym)) {
        code += huff_tree->get_code(sym);
    }

    while (code.size() % 8 != 0) {
        code += "0";
    }

    write_code(code);
}

void HuffmanArchiverEncode::write_code(std::string &code) {
    uint8_t byte = 0, current_bit = 0;
    for (const char &bit : code) {
        if (bit == '0') {
            byte &= ~(1UL << current_bit++);
        } else if (bit == '1') {
            byte |= 1UL << current_bit++;
        }

        if (current_bit == 8) {
            write_bin(*output_handler, byte);
            byte = 0, current_bit = 0;
            output_file_size++;
        }
    }
}

void HuffmanArchiverEncode::write_statistics() {
    std::cout << input_file_size << '\n'
              << output_file_size << '\n'
              << additional_data << '\n';

}

//------------------------HuffmanArchiverDecode------------------------------------//

HuffmanArchiverDecode::HuffmanArchiverDecode(std::istream &in, std::ostream &out)
        : input_handler(&in),
          output_handler(&out),
          huff_tree(new HuffmanTree()),
          input_file_size(0), output_file_size(0), num_of_symbols(0), additional_data(0) {

    if (input_handler->fail()) {
        throw HuffmanArchiverException("Cannot open input file");
    }
    if (output_handler->fail()) {
        throw HuffmanArchiverException("Cannot open output file");
    }
}

void HuffmanArchiverDecode::decode_input() {
    load_file();
    save_file();
}

void HuffmanArchiverDecode::set_input_size() {
    input_handler->ignore(std::numeric_limits<std::streamsize>::max());
    input_file_size = input_handler->gcount();
    input_handler->clear();
    input_handler->seekg(0, std::ios_base::beg);
}

void HuffmanArchiverDecode::load_file() {
    set_input_size();
    if (input_file_size == 0) return;

    uint32_t table_size, freq;
    char sym;

    read_bin(*input_handler, table_size);
    additional_data = table_size * (sizeof(sym) + sizeof(freq)) + sizeof(table_size);
    input_file_size -= additional_data;

    for (uint32_t i = 0; i < table_size; i++) {
        read_bin(*input_handler, sym);
        read_bin(*input_handler, freq);
        freq_table[sym] = freq;
        num_of_symbols += freq;
    }

    huff_tree->build_tree(freq_table);
    huff_tree->make_decode_table();
}

void HuffmanArchiverDecode::save_file() {
    write_text();
}

void HuffmanArchiverDecode::write_text() {
    uint8_t byte;
    char sym;
    std::string code;
    for (size_t i = 0; i < input_file_size; i++) {
        read_bin(*input_handler, byte);
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t bit = (byte >> j) & 1;
            if (bit == 1) {
                code += "1";
            } else {
                code += "0";
            }
            if (huff_tree->check_code(code) && num_of_symbols > 0) {
                sym = huff_tree->get_symbol(code);
                num_of_symbols--;
                output_file_size++;
                write_bin(*output_handler, sym);
                code = "";
            }
        }
    }
    output_file_size *= sizeof(sym);
}

HuffmanArchiverDecode::~HuffmanArchiverDecode() {
    delete (huff_tree);
}

void HuffmanArchiverDecode::write_statistics() {
    std::cout << input_file_size << '\n'
              << output_file_size << '\n'
              << additional_data << '\n';
}

//------------------------HuffmanArchiverException------------------------------------//

HuffmanArchiverException::HuffmanArchiverException(std::string _error) : _error(std::move(_error)) {}

std::string HuffmanArchiverException::get() {
    return _error;
}

const char *HuffmanArchiverException::what() const throw() {
    return _error.c_str();
}
