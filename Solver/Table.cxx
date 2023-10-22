/**
 * @file Table.cxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File implemented to implement the representation of a table on the symplex solver
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Table.hxx"

namespace Solver {

    Table::Table(LinearSystems::System linearSystem) : 
        systemToSolve(linearSystem),
        addedM(0), addedX(0),
        objective(linearSystem.getAction()) {
        // nothing so far huh
    }

    Table::~Table() {

    }



    void Table::reviewSystem() {
        LinearSystems::Restriction * restrictions = systemToSolve.getRestrictions();
        int restrictionNbr = systemToSolve.getNumberOfRestrictions();
        int variableNbr = systemToSolve.getNumberOfVariables();

        for (int i = 0; i < restrictionNbr && restrictions != nullptr; i++, restrictions++) {
            redefineRestriction(restrictions, variableNbr);
        }

    }

    void Table::redefineRestriction(LinearSystems::Restriction * restriction, int variableNbr) {
        /**
         * Locate the symbol, check it and if needed revamp the variables
         */
        int varNbr = restriction->getVariableNumber();
        
        LinearSystems::symbolEnum restrictionSymbol = 
            static_cast<LinearSystems::symbolEnum>(restriction->getRestrictionSymbol());

        // What do we do with < and >?
        bool needsToBeAdjusted =(restrictionSymbol == LinearSystems::symbolEnum::LOWER_EQUAL) ||
                                (restrictionSymbol == LinearSystems::symbolEnum::HIGHER_EQUAL);
        if (!needsToBeAdjusted) {
            return;
        }

        // Adjusting

    }

};