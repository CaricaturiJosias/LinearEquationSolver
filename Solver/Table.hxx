/**
 * @file Table.hxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File implemented to define the representation of a table on the symplex solver
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "../Representation/LinearSystems/System.hxx"

namespace Solver {

    class Table {

        public:

            Table(LinearSystems::System linearSystem);

            ~Table();

        private:

            

    };

};
