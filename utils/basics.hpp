#pragma once
#include "./utils.hpp"
#include <algorithm>
#include <string>

inline bool is_equal_char(char c) {
    return c == '=';
}

inline bool is_var_char(char c) {
    return c == C_VAR;
}

inline bool is_valid_special(char c) {
    return c == '+' || c == '-' || c == '^' || c == '*' || c == '/' || c == '.';
}

static inline bool is_not_valid(char c) {
    return !std::isdigit(c) && !std::isspace(c) && !is_valid_special(c) && !is_equal_char(c) && !is_var_char(c);
}

/*
    Accepted char : 
    letter :   X, 
    operator: +, -, *, /, ^, ., 
    number: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    special : 'white_space', 
*/

// std::string NOT_VALID_CHAR_MESS(char c) {
//     std::string str = "\'";
//     str += c + "\' is not a valid character.";
//     return str;
// }

// std::string TEST() {
//     std::string str = "Multiple operators are not allowed_ inline.";
//     return str;
// }


std::string TOO_MANY_EQUALS() {
    std::string str = "Multiple \'=\' is prohibed!";
    return str;
}

std::string NO_EQUAL() {
    std::string str = "You need at least 1 \'=\' !";
    return str;
}

static bool    parseOneLine(const char** argv) {
    std::string arg = argv[1];

    if (arg.size() <= 0) return logErrorWithPrefix("Please enter an equation as input");

    char last_char = arg[0];
    int equal_count = 0;
    
    for (size_t i = 0; i < arg.size(); i++) {
        if (std::isspace(static_cast<unsigned char>(arg[i]))) continue;
        if (is_not_valid(arg[i])) return logErrorWithPrefix("Some invalid char are present in input");
        if (is_valid_special(arg[i]) || is_equal_char(arg[i])) {
            if (is_valid_special(last_char)) {
                return logErrorWithPrefix("Can't put 2 op in line");
            }
        }
        if (is_equal_char(arg[i])) {
            equal_count++;
            if (equal_count > 1) return logErrorWithPrefix(TOO_MANY_EQUALS());
        }
        last_char = arg[i];
    }
    if (is_valid_special(last_char) || is_equal_char(last_char)) return logErrorWithPrefix("You absolutly cannot ends with an operator or equal");
    if (equal_count < 1) return logErrorWithPrefix(NO_EQUAL());

    // more basis parse here

    return true;
}

static bool  parseMultipleLine(const char** argv) {
    return (argv);
}

bool  basisArgsParse(const int& argc, const char** argv) {
    if (argc <= 1) return logError(P_ERR + "You need to input some data.");
    else if (argc == 2) return parseOneLine(argv);
    else return parseMultipleLine(argv);
}