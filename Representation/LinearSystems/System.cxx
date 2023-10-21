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
#include "Restriction.hxx"
#include <cctype>
#include <iostream>
#include <cmath>
#include <string>

namespace LinearSystems {

    std::vector<Restriction> System::restrictions{};

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
            std::cout << "Insert the number of variables: ";
            std::cin >> variableAmmountStr;
            Helper::isAllDigits(variableAmmountStr, variables);
            if (variables > 0) {
                validVariableAmmount = true;
            }
        }
    }

    System::System() {
        // Vai pedir quantidade de restrições e variáveis
        getInputs();
        // Vai pedir para fazer as restrições necessárias
        for (int i = 0; i < restrictionNumber; ++i) {
            std::cout << "Creating variable number: " << i+1 << std::endl;
            restrictions.push_back(Restriction(variables));
        }
    }

    System::~System() {
    }

    std::string System::to_string() {
        std::string output;
        int i = 1;
        for (Restriction restriction : restrictions) {
            output = restriction.to_string(i++) + "\n";
        }
        return output;
    }
};
