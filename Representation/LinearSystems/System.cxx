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

        buildObjective();

        restrictions = static_cast<Restriction *>(malloc(sizeof(Restriction) * restrictionNumber));
        if (restrictions == nullptr) {
            std::cout << "Memory allocation error" << std::endl;
            return;
        }

        for (int i = 1; i <= restrictionNumber; ++i) {
            restrictions[i-1] = Restriction(variables, i, objType::NONE);
        }

        std::cout << "Printing all items" << std::endl << to_string() << std::endl;

    }

    System::~System() {
    }

    void System::buildObjective() {
        bool inputNotValid = true;
        std::string input;
        while (inputNotValid) {
            std::cout << "Is the objective to minimize or maximize? [M,m]: ";
            std::cin >> input;
            if (input == std::string("M")) {
                objectiveAction = objType::MAX;
                inputNotValid = false;
            } else if (input == std::string("m")) {
                objectiveAction = objType::MIN;
                inputNotValid = false;
            }
        }

        objective = new Restriction(variables, 0, objectiveAction);
    }

    std::string System::to_string() {
        std::string output = objective->to_string() + "\n";
        int line = 1;
        for (int i = 0; i < restrictionNumber; ++i) {
            output += restrictions[i].to_string(line++) + "\n";
        }
        return output;
    }
};
