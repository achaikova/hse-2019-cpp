#pragma once

#include <string>
#include <iostream>
#include <cstring>

namespace CLI {
    enum class Mode {
        INITIALIZING,
        ENCODE,
        DECODE
    };

    enum class ParseError {
        INVALID_NUMBER,
        MISSING_INPUT_FILE,
        MISSING_OUPUT_FILE,
        INVALID_ARGS
    };

    enum class State{
        INVALID_ARGS,
        ARGS_PROCESSED
    };

    State parse_args(int argc, char *argv[], std::string &s_input, std::string &s_output, Mode &mode);

    State error_handler(ParseError error);
}
