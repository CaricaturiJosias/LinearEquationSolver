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
#include "../Representation/LinearSystems/Restriction.hxx"
#include <iostream>
#include <string>
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

        // Checks for artificial variables,
        // insert them and adjust the restrictions
        decideBaseVariables();

        defineTable();
        std::cout << "CABO" << std::endl;
    }

    Table::~Table() {
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
                    continue;
                }

                if (lookForM && hasMvalue) {
                    insertedBase.insert(j);
                    baseVariables[i].value.second = objectiveItem[j].second;
                    baseVariables[i].index = j+1;
                    break;
                } // if (lookForM && hasMvalue)

                if (!lookForM) {
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
                        continue;
                    }

                    insertedBase.insert(j);
                    baseVariables[i].value.second = objectiveItem[j].second;
                    baseVariables[i].index = j+1;
                    break;
                }

            } // for (int j = numVariables-1;
            // The number of base variables must match the number of the current restriction + 1
            if (insertedBase.size() != (i+1)) {
                --i;
                lookForM = false;
            }
        }
    }

    void Table::defineTable() {
        /**
         * The table should be 
         * columns:
         * 1 + number of variables + 2
         * 
         * lines:
         * 1 + number of restrictions + 1
        */
        numVar = systemToSolve->getNumberOfVariables();
        numRes = systemToSolve->getNumberOfRestrictions();

        LinearSystems::Restriction * restriction = systemToSolve->getRestrictions();
        
        std::vector<Value::Number> toInsert;

        // Build the restriction lines
        for (int i = 0;  i <= numRes; ++i) {
            toInsert.clear();
            LinearSystems::restrictionItem * restrictionIt =  restriction[i].getRestriction();
            for (int j = 0;  j <= numVar; ++j) {
                if (j == numVar) {
                    toInsert.push_back(restrictionIt[j].second);
                    // Add default theta value (0)
                    toInsert.push_back(Value::Number(0,0));
                    continue;
                } else if (restrictionIt[j].second.getMvalue()) { // Turn M value into normal value
                    toInsert.push_back(Value::Number(restrictionIt[j].second.getMvalue()));
                    continue;
                } else {
                    toInsert.push_back(restrictionIt[j].second);    
                }

            } // for (int j = 0
            tableArray.push_back(toInsert);
        } // for (int i = 0

        toInsert.clear();
        // Empty (Cj - Zj)
        for (int j = 0;  j < numVar; ++j) {
            toInsert.push_back(Value::Number(0,0));
        }
        tableArray.push_back(toInsert);
    }
    
    std::string Table::to_string() {

        std::string output;

        LinearSystems::restrictionItem * objective = systemToSolve->getObjective()->getRestriction();
        std::cout << "NumVar: " << numVar << std::endl 
                  << "NumRes: " << numRes << std::endl;
        output = printSizing("| Base ");

        for (int i = 0;  i < numVar; ++i) {
            output += printSizing( " | " + objective[i].second.to_string() + "*x"+std::to_string(i+1));
        } // for (int i = 0

        output += printSizing(" | b ");
        output += printSizing(" | Theta");
        output += "\n";

        for (int i = 0; i < numVar+3; ++i) {
            output += std::string("-------------");
        }
        output += "\n";

        for (int i = 0;  i < numRes; ++i) {
            for (int j = 0;  j <= numVar+2; ++j) {
                // Base variables that are not the first
                if (j == 0) {
                    // Base variable: value itself (ie. 2 - 3*M) and them
                    // index of the variable (ie. x8)
                    // We would have for example: (2-3*M)*x8
                    output += printSizing("| " + baseVariables[i].value.second.to_string() +
                            "*x"  + std::to_string(baseVariables[i].index));
                    continue;
                } // if (j == 0)
                // Include a Number into the output, we don't want M as it isn't supposed to appear here
                output += printSizing(" | "+tableArray[i][j-1].to_string());
            } // for (int j = 0;  j <= numVar+1; ++j)
            output += "\n";
        } // for (int i = 0;  i <= numRes; ++i)
        
        // Print (Cj - Zj)
        output += printSizing("| Cj - Zj");
        for (int j = 0;  j < numVar; ++j) {
            output += printSizing(" | "+tableArray[numRes+1][j].to_string());
        }
        return output;
    }

    std::string Table::printSizing(std::string toSizeInput) {
        std::string base = "             ";
        std::string output;
        // Fits the bill
        if (base.size() == toSizeInput.size()) {
            output = toSizeInput;
        } else if (base.size() > toSizeInput.size()) { // Add n blank spaces
            int diff = base.size() - toSizeInput.size();
            output = toSizeInput;
            for (int i = 0; i < diff; ++i) {
                output += " ";
            }
        } else { // Input is higher that max value
            toSizeInput.resize(base.size());
            output = toSizeInput;
        }
        return output;
    }

    void Table::calculateCjZj() {
        /**
         * Get each column, 
         * multiply the values with the base on each line
         * 
         * 
         * 
         */

        LinearSystems::restrictionItem * objectives = systemToSolve->getObjective()->getRestriction();

        // Each column
        for (int j = 0; j < numVar; ++j) {
            // Each value
            Value::Number current;
            for (int i = 0; i < numRes; ++i) {
                current = current + tableArray[i][j] * baseVariables[i].value.second;
            }
            tableArray[numRes+1][j] = current - objectives[j].second;
        }        

    }

    status Table::evaluateCjZj() {
        pivotColumn = 0;
        Value::Number current = tableArray[numRes+1][0];
        // Each column
        for (int j = 0; j < numVar; ++j) {
            // Keep track of highest column
            if (current < tableArray[numRes+1][j]) {
                
                current = tableArray[numRes+1][j];
                // Saves the pivot column for further calculations
                pivotColumn = j;
            }   
        }

        Value::Number zero = Value::Number(0,0);
        if (current < zero) {
            return NON_VIABLE;
        } else if (current == zero) {
            return DONE;
        }
        return WORK;
    }

    void Table::calculateTheta() {
        std::cout << "pivot column is " << pivotColumn+1 << std::endl;
        // For each line calculate theta
        for (int i = 0; i < numRes; ++i) {
            tableArray[i][numVar+1] = tableArray[i][numVar] / tableArray[i][pivotColumn];
        }

        pivotLine = 0;
        Value::Number current = tableArray[numRes+1][0];
        // Each column
        for (int i = 0; i < numRes; ++i) {
            // Keep track of highest column
            if (current < tableArray[i][numVar+1]) {
                
                current = tableArray[i][numVar+1];
                // Saves the pivot column for further calculations
                pivotLine = i;
            }   
        }
        std::cout << "Pivot line: " << pivotLine + 1 << std::endl;
    }

    void executeIteration() {
        
    }

};