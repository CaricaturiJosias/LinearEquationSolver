/**
 * @file Simplex.hxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File implemented to define the representation of a symplex solver
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "../Representation/LinearSystems/System.hxx"
#include <vector>

namespace Solver {

    class Simplex {

        public:
    
            Symplex();

            ~Symplex();

        private:

            static std::vector<LinearSystems::System> resolutionOrder;

            

    }

}