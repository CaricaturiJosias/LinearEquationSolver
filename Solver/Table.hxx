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
#include "../Representation/LinearSystems/Restriction.hxx"

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
    
    struct baseVariableItem {
        LinearSystems::restrictionItem value;
        int index;
    };

    enum status {
        WORK,
        DONE,
        NO_FRONTIER,
        NON_VIABLE,
        DEGENERATED,
        ALTERNATED_OPTIMAL,
        CYCLIC,
    };

    class Table {

        public:

            Table(LinearSystems::System * toSolveSystem);

            ~Table();

            std::string to_string();

            int returnTable();

            std::vector< std::vector<Value::Number> > getTable() { return tableArray; }

            void calculateCjZj();

            status evaluateCjZj();

            status calculateTheta();

            void updateBaseVariables();

            void executeIterationChange();

            std::string getResults(bool isAlternated = false);

            LinearSystems::System * getSystemToSolve() { return systemToSolve; }

        private:

            void reviewSystem();

            std::vector<LinearSystems::restrictionItem> probeRestriction(LinearSystems::Restriction * restriction, int variableNbr);

            void decideBaseVariables();

            void defineTable();

            static std::string printSizing(std::string toSizeInput);

            bool isBaseVariable(int index);

            Value::Number getFirstNonBase();

            int getFirstNonColumn();

            bool isAlreadyInList(int index, baseVariableItem * givenList);

            LinearSystems::System * systemToSolve;

            int numVar;
            int numRes;
            int pivotColumn;
            int pivotLine;
            int results;

            LinearSystems::objectiveType objective;

            baseVariableItem * baseVariables;
            
            std::vector< std::vector<Value::Number> > tableArray;


    };

};
