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
#include <set>

namespace Solver {

    Table::Table(LinearSystems::System * toSolveSystem) : systemToSolve(toSolveSystem),
                                                            addedM(0), addedX(0) {
        
        action  = systemToSolve->getAction();
        objective  = systemToSolve->getAction();

        // Same number as number of restrictions
        baseVariables = static_cast<baseVariableItem *>(
                            malloc(sizeof(baseVariableItem)
                                    *systemToSolve->getNumberOfRestrictions()));
        
        // Checks for artificial variables,
        // insert them and adjust the restrictions
        reviewSystem();

        // TODO - REMOVE
        std::cout << systemToSolve->to_string() << std::endl;

        // Checks for artificial variables,
        // insert them and adjust the restrictions
        decideBaseVariables();

    }

    Table::~Table() {
        free(baseVariables);
    }



    void Table::reviewSystem() {
        LinearSystems::Restriction * restrictions = systemToSolve->getRestrictions();
        LinearSystems::Restriction * objective = systemToSolve->getObjective();

        int restrictionNbr = systemToSolve->getNumberOfRestrictions();
        int variableNbr = systemToSolve->getNumberOfVariables();

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
        restrictions = systemToSolve->getRestrictions();
        systemToSolve->setVariableNumber(artificialVariables.size() + systemToSolve->getNumberOfVariables());

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

    void Table::decideBaseVariables() {
        LinearSystems::restrictionItem *  objectiveItem = systemToSolve->getObjective()->getRestriction();

        int numVariables = systemToSolve->getNumberOfVariables();
        std::cout << "Number of variables " << numVariables << std::endl;
        int numRestrictions = systemToSolve->getNumberOfRestrictions();

        bool lookForM = true;

        std::set<int> insertedBase;
        /**
         * All base variables must be set, the order to prioritize is:
         * 1 - M values
         * 2 - values without M
         */
        bool alreadyIn, hasMvalue;

        for (int i = 0; i < numRestrictions; ++i) {
            /**
             * Look for each M value or values without M on the next
             */
            for (int j = numVariables-1; j >= 0; --j) {
                // If we are looking for M values (first loop)
                alreadyIn  = insertedBase.find(j) != insertedBase.end();
                hasMvalue = objectiveItem[j].second.getMvalue();
                if (alreadyIn) {
                     std::cout << "Already in" << std::endl;
                    continue;
                }

                if (lookForM && hasMvalue) {
                    std::cout << "lookForM && hasMvalue " << std::endl;
                    insertedBase.insert(j);
                    baseVariables[i].value.second = objectiveItem[j].second;
                    baseVariables[i].index = j+1;
                    break;
                } // if (lookForM && hasMvalue)

                if (!lookForM) {
                    std::cout << "!lookForM " << std::endl;
                    // Last item? we cannot reach this if its a M value in the end
                    if (j == (numVariables-1)) {
                        insertedBase.insert(j);
                        baseVariables[i].value.second = objectiveItem[j].second;
                        baseVariables[i].index = j+1;
                        break;
                    }

                    // Check if this doesn't belong to an M value
                    bool hasNextM = objectiveItem[j+1].second.getMvalue();
                    if (hasNextM) {
                        std::cout << "Has next M " << std::endl;
                        continue;
                    }

                    std::cout << "Normal value " << std::endl;
                    insertedBase.insert(j);
                    baseVariables[i].value.second = objectiveItem[j].second;
                    baseVariables[i].index = j+1;
                }

            } // for (int j = numVariables-1;

            std::cout << "insertedBase.size() " << insertedBase.size() << std::endl
                      << "(i+1) " << (i+1) << std::endl;
            if (insertedBase.size() != (i+1)) {
                --i;
                std::cout << "Not done" << std::endl;
                lookForM = false;
            }
        }

        std::cout << "Base variables" << std::endl;
        for (int i = 0; i <  numRestrictions; ++i) {
            std::cout << "Item: " << baseVariables[i].value.second.to_string() << std::endl;
            std::cout << "Index: " << baseVariables[i].index << std::endl;
        }
    }

};