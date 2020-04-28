#include "CLI.h"


CLI::State CLI::parse_args(int argc, char *argv[], std::string &s_input, std::string &s_output, Mode &mode) {
    if (argc != 6) {
        return error_handler(ParseError::INVALID_NUMBER);
    }

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-f" || std::string(argv[i]) == "--file") {
            if (i < argc - 1 && std::string(argv[i + 1])[0] != '-') {
                s_input = argv[++i];
            } else if (s_input.empty()) {
                return error_handler(ParseError::MISSING_INPUT_FILE);
            } else {
                return error_handler(ParseError::INVALID_ARGS);
            }
        } else if (std::string(argv[i]) == "-o" || std::string(argv[i]) == "--output") {
            if (i < argc - 1 && std::string(argv[i + 1])[0] != '-') {
                s_output = argv[++i];
            } else if (s_output.empty()) {
                return error_handler(ParseError::MISSING_OUPUT_FILE);
            } else {
                return error_handler(ParseError::INVALID_ARGS);
            }
        } else if (std::string(argv[i]) == "-u") {
            if (mode != Mode::INITIALIZING) {
                return error_handler(ParseError::INVALID_ARGS);
            }
            mode = Mode::DECODE;
        } else if (std::string(argv[i]) == "-c") {
            if (mode != Mode::INITIALIZING) {
                return error_handler(ParseError::INVALID_ARGS);
            }
            mode = Mode::ENCODE;
        } else {
            return error_handler(ParseError::INVALID_ARGS);
        }
    }
    return State::ARGS_PROCESSED;
}

CLI::State CLI::error_handler(const ParseError error) {
    switch (error) {
        case ParseError::INVALID_NUMBER:
            std::cerr << "invalid number of arguments." << std::endl;
            break;
        case ParseError::MISSING_INPUT_FILE:
            std::cerr << "-f/--file option requires file name." << std::endl;
            break;
        case ParseError::MISSING_OUPUT_FILE:
            std::cerr << "-o/--output option requires file name." << std::endl;
            break;
        case ParseError::INVALID_ARGS:
            std::cerr << "unknown command." << std::endl;
            break;
    }
    return State::INVALID_ARGS;
}
