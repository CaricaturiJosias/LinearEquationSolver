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
        std::cout << systemToSolve.to_string() << std::endl;
    }

    Table::~Table() {

    }



    void Table::reviewSystem() {
        LinearSystems::Restriction * restrictions = systemToSolve.getRestrictions();
        LinearSystems::Restriction * objective = systemToSolve.getObjective();

        int restrictionNbr = systemToSolve.getNumberOfRestrictions();
        int variableNbr = systemToSolve.getNumberOfVariables();

        std::vector<LinearSystems::restrictionItem> artificialVariables;
        std::vector<LinearSystems::restrictionItem> results;
        for (int i = 0; i < restrictionNbr && restrictions != nullptr; i++, restrictions++) {
            // look for all <= or >= symbols to gather all needed artificial variables
            results = probeRestriction(restrictions, variableNbr);

            // For all added variables
            for (LinearSystems::restrictionItem artificialVar : results) {
                artificialVariables.push_back(artificialVar);
            }
        }

        // inserts all the artificial variables into the objective
        objective->addArtificialVariableToObjective(artificialVariables);

        // start the loop again because we were 
        // collecting all the artificial variables needed
        restrictions = systemToSolve.getRestrictions();
        bool isFirst = false;
        for (int i = 0; i < restrictionNbr && restrictions != nullptr; i++, restrictions++) {
            isFirst = i == 0;
            restrictions->addArtificialVariable(artificialVariables, isFirst);
        }


    }

    std::vector<LinearSystems::restrictionItem> Table::probeRestriction(LinearSystems::Restriction * restriction, int variableNbr) {
        /**
         * Locate the symbol, check it and if needed revamp the variables
         */
        std::vector<LinearSystems::restrictionItem> result;
        int varNbr = restriction->getVariableNumber();

        std::string restrictionSymbol = 
            LinearSystems::symbolMap[restriction->getRestrictionSymbol().getValue()];

        // What do we do with < and >?
        bool needsToBeAdjusted =   ((restrictionSymbol == LinearSystems::symbolMap[LinearSystems::symbolEnum::LOWER_EQUAL])||
                                    (restrictionSymbol == LinearSystems::symbolMap[LinearSystems::symbolEnum::HIGHER_EQUAL]));
        if (!needsToBeAdjusted) {
            return result;
        }

        // Adjusting
        LinearSystems::restrictionItem * restrictionData = restriction->getRestriction();

        // We want to have only
        ++addedX; // We always need to add an X variable, not always an M variable
        Value::Number valueX(0, 0);
        result.push_back(LinearSystems::restrictionItem(
                LinearSystems::variableType::ARTIFICIAL_VARIABLE, valueX));

        bool addM = restrictionSymbol == LinearSystems::symbolMap[LinearSystems::symbolEnum::HIGHER_EQUAL];
        if (addM) {
            ++addedM;
            Value::Number valueM(0, 1);
            result.push_back(LinearSystems::restrictionItem(
                    LinearSystems::variableType::ARTIFICIAL_VARIABLE, valueM));
        }

        return result;
    }

};