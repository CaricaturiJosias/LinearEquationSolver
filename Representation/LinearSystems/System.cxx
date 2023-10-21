/**
 * @file System.cxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File implemented to define the representation of a linear system
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "System.hxx"
#include "../../Helpers/Helper.hxx"
#include <cctype>
#include <iostream>
#include <cmath>
#include <string>

namespace LinearSystems {

    std::vector<restriction> System::restrictions{};

    void System::getInputs() {
        // We should ask for values about:
        //  Ammount of restrictions
        //  Ammount of variables
        // Ask variable size

        std::string restrictionAmmountStr;
        std::string variableAmmountStr;
        double restrictionAmmountDouble;

        bool validRestrictionAmmount = false;
        while (!validRestrictionAmmount) {
            std::cout << "Insert the number of restrictions: ";
            std::cin >> restrictionAmmountStr;
            Helper::isAllDigits(restrictionAmmountStr, restrictionNumber);
            if (restrictionNumber > 0) {
                validRestrictionAmmount = true;
            } 
        }

        bool validVariableAmmount = false;
        while (!validVariableAmmount) {
            std::cout << "Insert the number of restrictions: ";
            std::cin >> variableAmmountStr;
            Helper::isAllDigits(variableAmmountStr, variables)
            if (variables > 0) {
                validVariableAmmount = true;
            }
        }
    }

    System::System() {
        // Vai pedir quantidade de restrições e variáveis
        getInputs();
    }

    System::~System() {
    }

};
