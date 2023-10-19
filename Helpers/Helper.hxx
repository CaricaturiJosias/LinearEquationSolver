/**
 * @file Helper.cxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File declares the header for a helper interface (treat input and stuff)
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <string>

#pragma once

class Helper {

    public:
        
        static bool isdigit(char n);

        static bool isAllDigits(std::string input);

};