#include "doctest.h"
#include <iostream>
#include <sstream>
#include "CLI.h"

TEST_CASE ("incorrect number of arguments") {
    std::stringstream buffer;
    std::streambuf * old = std::cerr.rdbuf(buffer.rdbuf());

    int argc = 0;
    char *argv[3];
    std::string input, output;
    CLI::Mode mode;
            SUBCASE("not enough arguments") {
        argc = 3;
                CHECK(CLI::parse_args(argc, argv, input, output, mode) == CLI::State::INVALID_ARGS);
                CHECK(buffer.str() == "invalid number of arguments.\n");
    }
    buffer.str(std::string());
            SUBCASE("too many arguments") {
        argc = 7;
                CHECK(CLI::parse_args(argc, argv, input, output, mode) == CLI::State::INVALID_ARGS);
                CHECK(buffer.str() == "invalid number of arguments.\n");
    }
    std::cerr.rdbuf( old );
}

TEST_CASE ("unknown command") {
    std::stringstream buffer;
    std::streambuf * old = std::cerr.rdbuf(buffer.rdbuf());

    int argc = 6;
    char *argv_1[6] = {
            (char *) ("./hw_03"),
            (char *) ("-c"),
            (char *) ("--file"),
            (char *) ("new_file.txt"),
            (char *) ("-k"),
            (char *) ("binary.bin")
    };
    std::string input, output;
    CLI::Mode mode = CLI::Mode::INITIALIZING;

            SUBCASE("unknown command") {
                CHECK(CLI::parse_args(argc, argv_1, input, output, mode) == CLI::State::INVALID_ARGS);
                CHECK(buffer.str() == "unknown command.\n");
    }

    mode = CLI::Mode::INITIALIZING;
    char *argv_2[6] = {
            (char *) ("./hw_03"),
            (char *) ("-c"),
            (char *) ("-file"),
            (char *) ("new_file.txt"),
            (char *) ("-o"),
            (char *) ("binary.bin")
    };
    buffer.str(std::string());
            SUBCASE("error in command name") {
                CHECK(CLI::parse_args(argc, argv_2, input, output, mode) == CLI::State::INVALID_ARGS);
                CHECK(buffer.str() == "unknown command.\n");
    }

    mode = CLI::Mode::INITIALIZING;
    char *argv_3[6] = {
            (char *) ("./hw_03"),
            (char *) ("-c"),
            (char *) ("-file"),
            (char *) ("new_file.txt"),
            (char *) ("-u"),
            (char *) ("binary.bin")
    };
    buffer.str(std::string());

            SUBCASE("set two different mods") {
                CHECK(CLI::parse_args(argc, argv_3, input, output, mode) == CLI::State::INVALID_ARGS);
                CHECK(buffer.str() == "unknown command.\n");
    }
    std::cerr.rdbuf( old );
}

TEST_CASE ("missing file") {
    std::stringstream buffer;
    std::streambuf * old = std::cerr.rdbuf(buffer.rdbuf());

    int argc = 6;
    char *argv_1[6] = {
            (char *) ("./hw_03"),
            (char *) ("-c"),
            (char *) ("--file"),
            (char *) ("new_file.txt"),
            (char *) ("-o"),
            (char *) ("-d")
    };
    std::string input, output;
    CLI::Mode mode = CLI::Mode ::INITIALIZING;

            SUBCASE("no output file name") {
                CHECK(CLI::parse_args(argc, argv_1, input, output, mode) == CLI::State::INVALID_ARGS);
                CHECK(buffer.str() == "-o/--output option requires file name.\n");
    }

    mode = CLI::Mode::INITIALIZING;
    input = "";
    char *argv_2[6] = {
            (char *) ("./hw_03"),
            (char *) ("-c"),
            (char *) ("--file"),
            (char *) ("--output"),
            (char *) ("new_file.txt"),
            (char *) ("-l")
    };
    buffer.str(std::string());

            SUBCASE("no input file name") {
                CHECK(CLI::parse_args(argc, argv_2, input, output, mode) == CLI::State::INVALID_ARGS);
                CHECK(buffer.str() == "-f/--file option requires file name.\n");
    }
    std::cerr.rdbuf( old );
}

TEST_CASE ("parse input") {
    std::stringstream buffer;
    std::streambuf * old = std::cerr.rdbuf(buffer.rdbuf());

    int argc = 6;
    char *argv_1[6] = {
            (char *) ("./hw_03"),
            (char *) ("-c"),
            (char *) ("--file"),
            (char *) ("new_file.txt"),
            (char *) ("-o"),
            (char *) ("binary.bin")
    };
    std::string input, output;
    CLI::Mode mode = CLI::Mode::INITIALIZING;

            SUBCASE("set encode mode") {
                CHECK(CLI::parse_args(argc, argv_1, input, output, mode) == CLI::State::ARGS_PROCESSED);
                CHECK(mode == CLI::Mode::ENCODE);
    }

    mode = CLI::Mode::INITIALIZING;
    char *argv_2[6] = {
            (char *) ("./hw_03"),
            (char *) ("-u"),
            (char *) ("--output"),
            (char *) ("binary.bin"),
            (char *) ("-f"),
            (char *) ("output.txt")
    };
    buffer.str(std::string());

            SUBCASE("set decode mode") {
                CHECK(CLI::parse_args(argc, argv_2, input, output, mode) == CLI::State::ARGS_PROCESSED);
                CHECK(mode == CLI::Mode::DECODE);
    }
    std::cerr.rdbuf( old );
}
