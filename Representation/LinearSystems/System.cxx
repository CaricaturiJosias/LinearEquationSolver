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

    System::System() {
        // We should ask for values about:
        //  Ammount of restrictions
        //  Ammount of variables
        // Ask variable size
        bool validRestrictionAmmount = false;
        std::string restrictionAmmount;
        while (!validRestrictionAmmount) {
            std::cout << "Insert the number of restrictions: ";
            std::cin >> restrictionAmmount;
            int scale = restrictionAmmount.size();
            restrictionNumber = std::stoi(restrictionAmmount);
        }
    }

    System::~System() {
    }

};
