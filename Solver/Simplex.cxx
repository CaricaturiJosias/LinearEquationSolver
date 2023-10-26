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
        }
        // Populate system
        LinearSystems::System * toSolveSystem = new LinearSystems::System();
        tableInstance = new Table(toSolveSystem);
        solverMain();
    }

    Simplex::~Simplex() {

    }

    void Simplex::solverMain() {
        /**
         * Steps
         * 
         * 1 - Calculate (Cj - Zj)
         * 
         * 2 - Find higher value
         * 
         * 3 - Calculate Theta
         * 
         * 4 - Assign pivot variable
         * 
         * 5 - Change base variables
         * 
         * 6 - Calculate the next matrix
        */

        status solutionStatus = status::WORK;
        // while (solutionStatus == WORK) {
            std::cout << tableInstance->to_string() << std::endl;
            std::cout << "Calculate CjZj" << std::endl;
            tableInstance->calculateCjZj();
            std::cout << "Calculated CjZj" << std::endl;
            solutionStatus = tableInstance->evaluateCjZj();
            std::cout << tableInstance->to_string() << std::endl;
            // if (solutionStatus != WORK) {
            //     continue;
            // }
            tableInstance->calculateTheta();
            std::cout << tableInstance->to_string() << std::endl;

        // }
    }

};