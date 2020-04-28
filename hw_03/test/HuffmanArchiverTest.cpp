#include "doctest.h"
#include <vector>
#include <sstream>
#include "HuffmanArchiver.h"

TEST_SUITE ("huffman encoder") {

    TEST_CASE ("load symbols and frequencies") {

                SUBCASE("load nonempty file") {
            std::stringstream in;
            std::stringstream out;
            in << "ABCDEFGHabcc12345   \n /?|-__-??oh hiMARk2.815528e+03";

            HuffmanArchiverEncode h_encode(in, out);
            h_encode.load_file();

                    SUBCASE("check frequency table") {
                size_t freq = 0;
                const uint32_t *freq_table = h_encode.get_freq_table();
                for (auto sym : in.str()) {
                    size_t pos = in.str().find(sym);
                    while (pos != std::string::npos) {
                        freq++;
                        pos = in.str().find(sym, pos + 1);
                    }
                            CHECK(*(freq_table + static_cast<int >(sym)) == freq);
                    freq = 0;
                }
            }

                    SUBCASE("check size") {
                auto[input_size, output_size, table_size] = h_encode.get_statistics();
                        CHECK(table_size == 34);
                        CHECK(input_size == in.str().size());
            }
        }

                SUBCASE("load empty file") {
            std::stringstream in;
            std::stringstream out;

            HuffmanArchiverEncode h_encode(in, out);
            h_encode.load_file();

                    SUBCASE("check size") {
                auto[input_size, output_size, table_size] = h_encode.get_statistics();
                        CHECK(table_size == 0);
                        CHECK(input_size == 0);
            }
        }
    }

    TEST_CASE ("write encoded text") {
        std::stringstream in;
        std::stringstream out;
                SUBCASE("write code_01") {
            std::string code("001011010011111111111100");

            HuffmanArchiverEncode h(in, out);
            h.write_code(code);

            uint8_t check_code[3] = {0, 0, 0};
            uint8_t current_bit = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 8; j++) {
                    if (code[i * 8 + j] == '0') {
                        check_code[i] &= ~(1UL << (current_bit++));
                    } else if (code[i * 8 + j] == '1') {
                        check_code[i] |= 1UL << (current_bit++);
                    }
                }
                current_bit = 0;
            }
            size_t pos = 0;
            char sym[3];
            while (out.get(sym[pos++])) {
                        CHECK(static_cast<uint8_t>(sym[pos - 1]) == check_code[pos - 1]);
            }
        }
    }
}

TEST_SUITE ("huffman decoder") {

    TEST_CASE ("load frequency table from binary") {

                SUBCASE("load nonempty file") {
            std::stringstream in;
            std::stringstream out1, out2;
            in << "1234567a2lmlm!@~~''""#$%^&*(-=-=-bcdefAbCD112";

            HuffmanArchiverEncode h_encode(in, out1);
            h_encode.encode_input();

            HuffmanArchiverDecode h_decode(out1, out2);
            h_decode.load_file();

                    SUBCASE("check frequency table") {
                size_t freq = 0;
                const uint32_t *freq_table = h_decode.get_freq_table();

                for (auto sym : in.str()) {
                    size_t pos = in.str().find(sym);
                    while (pos != std::string::npos) {
                        freq++;
                        pos = in.str().find(sym, pos + 1);
                    }
                            CHECK(*(freq_table + static_cast<int >(sym)) == freq);
                    freq = 0;
                }
            }

                    SUBCASE("check size") {
                auto[en_input_size, en_output_size, en_table_size] = h_encode.get_statistics();
                auto[dec_input_size, dec_output_size, dec_table_size] = h_encode.get_statistics();
                        CHECK(en_table_size == dec_table_size);
                        CHECK(dec_input_size == in.str().size());
            }

        }

                SUBCASE("load empty file") {
            std::stringstream in;
            std::stringstream out;

            HuffmanArchiverDecode h_decode(in, out);
            h_decode.load_file();

                    SUBCASE("check size") {
                auto[input_size, output_size, table_size] = h_decode.get_statistics();
                        CHECK(table_size == 0);
                        CHECK(input_size == 0);
            }
        }
    }

    TEST_CASE ("write decoded text") {
        std::stringstream in, out1, out2;
        in << "1234567a2lmlm!@~~''""#$%^&*(-=-=-bcdefAbCD112";

        HuffmanArchiverEncode h_encode(in, out1);
        h_encode.encode_input();

        HuffmanArchiverDecode h_decode(out1, out2);
        h_decode.load_file();
        h_decode.write_text();
                CHECK(in.str() == out2.str());
    }
}

TEST_CASE ("end-to-end check") {
    std::stringstream in_encode, out_encode, out_decode;
            SUBCASE("all letters") {
        in_encode << "all letters? try, the quick brown fox jumps over the lazy dog. \n";
        HuffmanArchiverEncode h_encode(in_encode, out_encode);
        h_encode.encode_input();
        HuffmanArchiverDecode h_decode(out_encode, out_decode);
        h_decode.decode_input();
                CHECK(in_encode.str() == out_decode.str());
    }

            SUBCASE("some ascii symbols") {
        in_encode << "\t\t\n\n\n\t+_)))()9012345678ksjdvJ\n      "
                     "\n-___-;););)'ttoslowto ??\nJmlsmv__???++\t"
                     "tttt\n\n|ppp__-_!!!{}()[]  [;[plplokok;;::;"
                     "aajsdYYh!##YyyyHYkjbkbsd2213r7ohhimark  /|c";
        HuffmanArchiverEncode h_encode(in_encode, out_encode);
        h_encode.encode_input();
        HuffmanArchiverDecode h_decode(out_encode, out_decode);
        h_decode.decode_input();
                CHECK(in_encode.str() == out_decode.str());
    }

            SUBCASE("some numbers") {
        in_encode << "-9.498353e+02  1.934651e+01 -1.933336e+03  2.987594e+03\n"
                     "-1.707563e+02  9.458976e+00 -1.442193e+03 -9.248797e+02\n"
                     "42Cr (ap) 45Mn";
        HuffmanArchiverEncode h_encode(in_encode, out_encode);
        h_encode.encode_input();
        HuffmanArchiverDecode h_decode(out_encode, out_decode);
        h_decode.decode_input();
                CHECK(in_encode.str() == out_decode.str());
    }

            SUBCASE("no letters") {
        in_encode << "\n\n\n\t\t\t   \t\t\t";
        HuffmanArchiverEncode h_encode(in_encode, out_encode);
        h_encode.encode_input();
        HuffmanArchiverDecode h_decode(out_encode, out_decode);
        h_decode.decode_input();
                CHECK(in_encode.str() == out_decode.str());
    }

            SUBCASE("empty file") {
        in_encode << "";
        HuffmanArchiverEncode h_encode(in_encode, out_encode);
        h_encode.encode_input();
        HuffmanArchiverDecode h_decode(out_encode, out_decode);
        h_decode.decode_input();
                CHECK(in_encode.str() == out_decode.str());
    }
}