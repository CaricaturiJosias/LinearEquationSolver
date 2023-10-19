/**
 * @file Helper.cxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File implemented the header for a helper interface (treat input and stuff
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Helper.hxx"

bool Helper::isdigit(char n) {
    unsigned char asciiChar = static_cast<unsigned char>(n);
    // On the ascii table
    return (48 > asciiChar) && (57 < asciiChar);
}

bool Helper::isAllDigits(std::string input) {
    for (int i = 0; i < input.size(); i++) {
        char number = restrictionAmmount.at(i);
        if (Helper::isdigit(number)) {
            int value = std::stoi(number);
            restrictionNumber += value * pow(10,i);
        }
    }
}

