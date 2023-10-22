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
#include "../Representation/Values/Number.hxx"
#include <iostream>

namespace Solver {

    Table::Table(LinearSystems::System linearSystem) : 
        systemToSolve(linearSystem),
        addedM(0), addedX(0),
        action(linearSystem.getAction()),
        objective(linearSystem.getAction()) {
        // nothing so far huh
        reviewSystem();
    }

    Table::~Table() {

    }



    void Table::reviewSystem() {
        LinearSystems::Restriction * restrictions = systemToSolve.getRestrictions();
        int restrictionNbr = systemToSolve.getNumberOfRestrictions();
        int variableNbr = systemToSolve.getNumberOfVariables();

        std::vector<LinearSystems::restrictionItem> artificialVariables;
        std::vector<LinearSystems::restrictionItem> results;
        for (int i = 0; i < restrictionNbr && restrictions != nullptr; i++, restrictions++) {
            results = probeRestriction(restrictions, variableNbr);
            // For all added variables
            for (LinearSystems::restrictionItem artificialVar : results) {
                artificialVariables.push_back(artificialVar);
            }
        }
        for (auto item: artificialVariables){
            std::cout << item.second.to_string() << std::endl;
        }

    }

    std::vector<LinearSystems::restrictionItem> Table::probeRestriction(LinearSystems::Restriction * restriction, int variableNbr) {
        /**
         * Locate the symbol, check it and if needed revamp the variables
         */
        std::vector<LinearSystems::restrictionItem> result;
        int varNbr = restriction->getVariableNumber();
        
        LinearSystems::symbolEnum restrictionSymbol = 
            static_cast<LinearSystems::symbolEnum>(restriction->getRestrictionSymbol());

        // What do we do with < and >?
        bool needsToBeAdjusted =   ((restrictionSymbol == LinearSystems::symbolEnum::LOWER_EQUAL)||
                                    (restrictionSymbol == LinearSystems::symbolEnum::HIGHER_EQUAL));
        if (!needsToBeAdjusted) {
            return result;
        }

        // Adjusting
        LinearSystems::restrictionItem * restrictionData = restriction->getRestriction();
        // int variableNbr = restriction->getVariableNumber();

        // We want to have only =
        ++addedX; // We always need to add an X variable, not always an M variable
        Value::Number valueX(0, 0);
        result.push_back(LinearSystems::restrictionItem(
                LinearSystems::variableType::ARTIFICIAL_VARIABLE, valueX));

        bool addM = restrictionSymbol == LinearSystems::symbolEnum::HIGHER_EQUAL;
        if (addM) {
            ++addedM;
            Value::Number valueM(0, -1);
            result.push_back(LinearSystems::restrictionItem(
                    LinearSystems::variableType::ARTIFICIAL_VARIABLE, valueM));
        }

        return result;
    }

};