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
#include <cmath>
#include <cctype>

void Helper::isAllDigits(std::string input, int &outputValue) {
    bool isNumber = true;
    for (int i = 0; i < input.size(); i++) {
        char number = input.at(i);
        if (!std::isdigit(number)) {
            isNumber = false;
        }
    }

    if (isNumber) {
        outputValue = std::stoi(input);  
    }
    return;
}

