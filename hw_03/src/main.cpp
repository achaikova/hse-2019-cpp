#include <iostream>
#include <fstream>
#include "HuffmanArchiver.h"
#include "CLI.h"


int main(int argc, char *argv[]) {
    std::string s_input, s_output;
    CLI::Mode mode = CLI::Mode::INITIALIZING;

    if (parse_args(argc, argv, s_input, s_output, mode) == CLI::State::INVALID_ARGS) {
        exit(1);
    }

    std::ifstream in(s_input);
    std::ofstream out(s_output);

    switch (mode) {
        case CLI::Mode::ENCODE:
            try {
                HuffmanArchiverEncode h(in, out);
                h.encode_input();
                h.write_statistics();
            } catch (HuffmanArchiverException &e) {
                in.close(), out.close();
                std::cerr << e.what() << std::endl;
                exit(1);
            }
            break;
        case CLI::Mode::DECODE:
            try {
                HuffmanArchiverDecode h(in, out);
                h.decode_input();
                h.write_statistics();
            } catch (HuffmanArchiverException &e) {
                in.close(), out.close();
                std::cerr << e.what() << std::endl;
                exit(1);
            }
            break;
    }
    in.close(), out.close();
}

