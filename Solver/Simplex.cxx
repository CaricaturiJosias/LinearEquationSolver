/**
 * @file Simplex.cxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File implemented to implement the representation of a symplex solver
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>

#include "Simplex.hxx"
#include "../Helpers/Helper.hxx"

namespace Solver {

    Simplex::Simplex() {
        std::string input;
        bool inputNotValid = true;
        int selectedOption = 0;

        while (inputNotValid) {
            std::cout   << "Welcome, please select an option for the resolution" << std::endl
                        << "1) Show me just the results" << std::endl
                        << "2) Show me step by step quickly" << std::endl
                        << "3) Show me each step and wait for input before continuing" << std::endl;
            std::cin >> input;
            Helper::isAllDigits(input, selectedOption);
            if (selectedOption >= 1 && selectedOption <= 3) {
                inputNotValid = false;
                chosenOption = static_cast<resolutionOption>(selectedOption);
            }
            
            // Populate system
            resolutionOrder = new LinearSystems::System();

        }
    
    }

    Simplex::~Simplex() {

    }


};