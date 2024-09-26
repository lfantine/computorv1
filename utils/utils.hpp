#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

// ANSI escape codes for text colors
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";

// PREFIXE for message
const std::string P_ERR = "Error : ";
const std::string P_Inf = "Info : ";


// NAMES and CODE used in the program
const char C_VAR = 'X';

struct parcel {
    char op = '?';
    float value = 0;
};

// data class
struct  chunk {
    int    sign;
    int     power;
    bool    X;
    std::vector<parcel>   values = {};
};

struct  process_store {
    char    compute_op = ' ';
    float   store_nb = 0;
    bool    is_store_nb = false;
};

chunk    initChunk() {
    chunk ck;
    ck.sign = 1;
    ck.power = 1;
    ck.X = false;
    return ck;
};

void printChunk(const chunk& ck, size_t idx) {

    // std::cout << "a chunk is printed : " << ck.power << std::endl;


    if (ck.sign >= 1 && idx > 0) {
        std::cout << "+" << " ";
    } else if (ck.sign <= 0) {
        std::cout << "-";
        if (idx > 0) {std::cout << " ";};
    }
    for (size_t i = 0; i < ck.values.size(); i++) {
        if (ck.values[i].op == C_VAR) {
            if (i > 0) {
                std::cout << "* ";
            }
            std::cout << C_VAR << "^" << ck.power;
        } else {
            if (ck.values[i].op == '/' && i == 0) {
                std::cout << "(1/" << ck.values[i].value << ")";
            } else {
                if (ck.values[i].op != ' ' && i > 0) {
                    std::cout << ck.values[i].op << " ";
                }
                std::cout << ck.values[i].value;
            }
        }
        std::cout << " ";
    }
}

void    printEqua(const std::vector<chunk>& equat, std::vector<chunk>& equal) {
    std::cout << " : " << GREEN;
    if (equat.size() == 0) {
        std::cout << "0 ";
    } else {
        for (size_t i = 0; i < equat.size(); i++) {
            if (equat[i].sign <= 0) {
                std::cout << "- ";
            } else if (i > 0) {
                std::cout << "+ ";
            }
            std::cout << equat[i].values[0].value << " ";
            if (equat[i].power > 0) {
                std::cout << "X ^ " << equat[i].power << " ";
            }
        }
    }
    std::cout << "= ";
    if (equal.size() == 0) {
        std::cout << "0";
    } else {
        for (size_t i = 0; i < equal.size(); i++) {
            if (equal[i].sign <= 0) {
                std::cout << "- ";
            } else if (i > 0) {
                std::cout << "+ ";
            }
            std::cout << equal[i].values[0].value << " ";
            if (equal[i].power > 0) {
                std::cout << "X ^ " << equal[i].power << " ";
            }
        }
    }
    std::cout << RESET << std::endl;
}

bool    logError(const std::string mess, bool retValue = false) {
    std::cout << RED << mess  << RESET <<std::endl;
    return retValue;
}

bool    logErrorWithPrefix(const std::string mess, bool retValue = false) {
    std::cout << RED << P_ERR << mess << RESET <<std::endl;
    return retValue;
}

// Trim du début (left trim)
std::string ltrim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r\f\v"); // Liste des espaces possibles
    return (start == std::string::npos) ? "" : s.substr(start);
}

// Trim de la fin (right trim)
std::string rtrim(const std::string& s) {
    size_t end = s.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

// Trim du début et de la fin
std::string trim(const std::string& s) {
    return rtrim(ltrim(s));
}

// Fonction pour séparer les éléments d'une ligne en utilisant un délimiteur
std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

// Fonction pour séparer les éléments d'une ligne en utilisant plusieurs délimiteurs
std::vector<std::string> split_chunks(const std::string& line, std::vector<char>& ops) {
    const std::string delimiters = "+-=";
    std::vector<std::string> result;
    std::string item;
    
    bool last_neg = false;
    // Parcourir la chaîne de caractères
    for (char ch : line) {
        // Si le caractère fait partie des délimiteurs, on ajoute l'élément courant
        if (delimiters.find(ch) != std::string::npos) {
            item = trim(item);
            if (!item.empty()) {
                result.push_back(last_neg ? std::string("-" + item) : std::string("+" + item));
                last_neg = false;
                item.clear();
            }
            if (ch == '-') last_neg = true;
            ops.push_back(ch);
        } else {
            item += ch;
        }
    }

    item = trim(item);
    // Ajouter le dernier élément s'il en reste
    if (!item.empty()) {
        result.push_back(last_neg ? std::string("-" + item) : std::string("+" + item));
    }

    return result;
}
