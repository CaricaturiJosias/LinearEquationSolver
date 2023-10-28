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
#include "Table.hxx"
#include <vector>

namespace Solver {

    enum resolutionOption {
        RESULT_ONLY = 1,
        FAST_ITERATIONS = 2,
        PAUSED_ITERATIONS = 3
    };

    class Simplex {

        public:
    
            Simplex();

            ~Simplex();

        private:

            resolutionOption chosenOption;

            void solverMain();

            Table * tableInstance;

            int iterations;

            Table * resolutionOrder;
            
            int selectedOption;
    };

};