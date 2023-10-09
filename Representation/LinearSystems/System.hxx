/**
 * @file System.hxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief Header file to define the representation of a linear system
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <map>
#include <vector>
#include "Restriction.hxx"

namespace LinearSystems {

    class System {

        private:

            static std::vector<LinearSystems::restriction> restrictions;

        public:

            System();
            ~System();
            
            int getNumberOfRestrictions() { return restrictions.size(); }



    };
};