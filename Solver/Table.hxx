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

/**
 * A table resembles this:
 * objective function          | b  | theta
 * -------------------------------------
 * base    | Matrix containing |
 * vars    | all restrictions  |
 * -------------------------------------
 * (Cj-Zj) |                   | total
 * 
 * We need to:
 * 
 *  1 - Redefine restrictions to have =
 *  2 - include new artificial variables
 *  3 - choose base variables
 *  4 - Execute the rounds of the method:
 *      1 - Calculate (Cj-Zj)
 *      2 - Find the lowest >= 0 value (identify degenerated system)
 *          1 - this may end here
 *      3 - calculate b and theta
 *      4 - define Pivot element
 *      5 - recalculate the matrix
 *      6 - repeat
*/
namespace Solver {

    class Table {

        public:

            Table(LinearSystems::System linearSystem);

            ~Table();

        private:

            void reviewSystem();

            void redefineRestriction(LinearSystems::Restriction * restriction, int variableNbr);

            LinearSystems::System systemToSolve;
            
            int addedX;
            int addedM;

            LinearSystems::objectiveType objective;
    };

};
