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

#include "../Representation/LinearSystems/Restriction.hxx"
#include "../Representation/Values/Number.hxx"
#include "Table.hxx"
#include <limits.h>
#include <iostream>
#include <string>
#include <set>

namespace Solver {

    Table::Table(LinearSystems::System * toSolveSystem) : systemToSolve(toSolveSystem) {
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
            restrictions->addSlackVariable(artificialVariables, isFirst);
        }

    }

    std::vector<LinearSystems::restrictionItem> Table::probeRestriction(LinearSystems::Restriction * restriction, int variableNbr) {
        /**
         * Locate the symbol, check it and if needed change the variables
         * 
         * Example: minimize and  >= : creates xn and -Mx(n+1) and sets the symbol as =
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
        Value::Number valueX(0, 0);
        result.push_back(LinearSystems::restrictionItem(
                LinearSystems::variableType::ARTIFICIAL_VARIABLE, valueX));

        bool addM = restrictionSymbol == LinearSystems::symbolMap[LinearSystems::symbolEnum::HIGHER_EQUAL];
        if (addM) {
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
        int repetition = 0;

        bool lookForM = true;

        std::set<int> insertedBase;
        /**
         * All base variables must be set, the order to prioritize is:
         * 1 - M values
         * 2 - values without M
         */
        bool alreadyIn, hasMvalue, isNaturalVariable, lookForSlack, lookForNonSlack;

        for (int i = 0; i < numRestrictions; ++i) {
            /**
             * Look for each M value or values without M on the next
             */
            for (int j = 0; j < numVariables; ++j) {
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

                isNaturalVariable = objectiveItem[j].first == LinearSystems::VALUE;
                lookForSlack = (!lookForM) && !isNaturalVariable;
                lookForNonSlack = ((!lookForM) && isNaturalVariable) && repetition > 1;

                // If we already looked for M (M) AND its (not) a non-slack (N)
                // OR
                // we already looked for M (M) AND its a non-slack AND (N) we look for a non-slack (A)
                // (M && !N) || ((M && N) && S)
                // 
                if (lookForSlack || lookForNonSlack) {

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
                ++repetition;
                continue;
            }
            repetition = 0;
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
        for (int i = 0;  i < numRes; ++i) {
            toInsert.clear();
            LinearSystems::restrictionItem * restrictionIt =  restriction[i].getRestriction();
            for (int j = 0;  j <= numVar; ++j) {
                if (restrictionIt[j].first == LinearSystems::SYMBOL) {
                    toInsert.push_back(restrictionIt[j+1].second);
                    continue; // Avoid symbol
                }
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
        for (int j = 0;  j <= numVar; ++j) {
            toInsert.push_back(Value::Number(0,0));
        }
        tableArray.push_back(toInsert);
    }
    
    std::string Table::to_string() {

        std::string output;

        LinearSystems::restrictionItem * objective = systemToSolve->getObjective()->getRestriction();
        output = printSizing("| Base");

        for (int i = 0;  i < numVar; ++i) {
            output += printSizing( "|" + objective[i].second.to_string() + "*x"+std::to_string(i+1));
        } // for (int i = 0

        output += printSizing("|b");
        output += printSizing("|Theta");
        output += "\n";

        for (int i = 0; i < numVar+3; ++i) {
            output += std::string("-------------");
        }
        output += "\n";

        for (int i = 0;  i < numRes; ++i) {
            for (int j = 0;  j <= numVar+1; ++j) {
                // Base variables that are not the first
                if (j == 0) {
                    // Base variable: value itself (ie. 2 - 3*M) and them
                    // index of the variable (ie. x8)
                    // We would have for example: (2-3*M)*x8
                    output += printSizing("|" + baseVariables[i].value.second.to_string() +
                            "*x"  + std::to_string(baseVariables[i].index));
                } // if (j == 0)
                // Include a Number into the output, we don't want M as it isn't supposed to appear here
                output += printSizing("|"+tableArray[i][j].to_string());
            } // for (int j = 0;  j <= numVar+1; ++j)
            output += "\n";
        } // for (int i = 0;  i <= numRes; ++i)
        
        // Print (Cj - Zj)
        output += printSizing("|Cj - Zj");
        for (int j = 0;  j <= numVar; ++j) {
            output += printSizing("|"+ tableArray[numRes][j].to_string());
        }
        output += printSizing("|");
        output += "\n";
        return output;
    }

    std::string Table::printSizing(std::string toSizeInput) {
        std::string base = "              ";
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

    bool Table::isBaseVariable(int index) {
        for (int i = 0; i < numRes; ++i) {
            if (baseVariables[i].index == (index+1)) {
                return true;
            }
        }
        return false;
    }

    Value::Number Table::getFirstNonBase() {
        for (int i = 0; i < numVar; ++i) {
            if (!isBaseVariable(i)) {
                return tableArray[numRes][i];
            }  
        }
        return Value::Number(0);
    }

    int Table::getFirstNonColumn() {
        for (int i = 0; i < numVar; ++i) {
            if (!isBaseVariable(i)) {
                return i;
            }  
        }
        return 0;
    }

    void Table::calculateCjZj() {
        /**
         * Get each column, 
         * multiply the values with the base on each line
         */
        LinearSystems::restrictionItem * objectives = systemToSolve->getObjective()->getRestriction();

        Value::Number current;
        // Each column
        for (int j = 0; j < numVar; ++j) {
            // Each value
            current = Value::Number(0,0);
            for (int i = 0; i < numRes; ++i) {
                current += tableArray[i][j] * baseVariables[i].value.second;
            }
            std::cout << "Current: " << current.to_string() << std::endl;
            tableArray[numRes][j] = objectives[j].second - current;
        }     
        current = Value::Number(0,0);   
        for (int i = 0; i < numRes; ++i) {
            current += tableArray[i][numVar] * baseVariables[i].value.second;
        }
        tableArray[numRes][numVar] = current;
    }

    status Table::evaluateCjZj() {
        pivotColumn = 0;
        Value::Number current = getFirstNonBase();
        pivotColumn = getFirstNonColumn();

        // Each column
        for (int j = 0; j < numVar; ++j) {
            // Keep track of highest column
            // We don't need to check the zeroes that the base variables generate
            if (isBaseVariable(j)) {
                // std::cout   << "Skiping j: " << j << std::endl 
                //             << "With value: " << tableArray[numRes][j].to_string() << std::endl;
                continue;
            }
            // std::cout   << "Did not skip j: " << j << std::endl 
            //             << "With value: " << tableArray[numRes][j].to_string() << std::endl;
            if (tableArray[numRes][j] > current) {
                
                current = tableArray[numRes][j];
                // Saves the pivot column for further calculations
                // std::cout << "Changing pivot" << std::endl;
                pivotColumn = j;
            }   
            // std::cout << "Printing current " << current.to_string() << std::endl;
        }
        // std::cout << "Pivot column: " << pivotColumn+1 << std::endl;
        Value::Number zero = Value::Number(0,0);
        // std::cout << "Current: " << current.to_string() << std::endl;
        // std::cout << "zero: " << zero.to_string() << std::endl;
        if (current < zero) {
            std::cout << "done" << std::endl;
            return DONE;
        } else if (current == zero) {
            std::cout << "alternado" << std::endl;
            return ALTERNATED_OPTIMAL;
        }
        return WORK;
    }

    status Table::calculateTheta() {
        // std::cout << "pivot column is " << pivotColumn+1 << std::endl;
        // For each line calculate theta
        for (int i = 0; i < numRes; ++i) {
            tableArray[i][numVar+1] = tableArray[i][numVar] / tableArray[i][pivotColumn];
        }

        pivotLine = 0;
        Value::Number current = tableArray[0][numVar+1];
        // Each column

        // Checks which is lower
        for (int i = 0; i < numRes; ++i) {
            // Keep track of lower column
            if (current > tableArray[i][numVar+1]) {
                
                current = tableArray[i][numVar+1];
                // Saves the pivot column for further calculations
                pivotLine = i;
            } 
        }
        int same = 0;
        for (int i = 0; i < numRes; ++i) {
            if (current == tableArray[i][numVar+1] && i != 0) {
                ++same;
            }
        }

        // std::cout << "Pivot Line: " << pivotLine+1 << std::endl;
        // std::cout << "Pivot Element: " << current.to_string() << std::endl;
        // std::cout << "Pivot (Cj - Zj): " << tableArray[numRes][pivotColumn].to_string() << std::endl;
        if (same>1) {
            return DEGENERATED;
        } else if (current < Value::Number(0,0)) {
            return NON_VIABLE;
        }

        return WORK;
    }

    void Table::updateBaseVariables() {
        /**
         * Now we have the pivot column and line
         * The line indicates the one gone
         * The column indicates the one in
         * 
         * Now we do the exchange
        */
        LinearSystems::restrictionItem * objectives = systemToSolve->getObjective()->getRestriction();
        // std::cout << "Old base variable that will be gone: " << baseVariables[pivotLine].value.second.to_string() << std::endl;
    
        baseVariables[pivotLine] = baseVariableItem{objectives[pivotColumn], pivotColumn+1};
        // std::cout << "New base variable that is here now: " << baseVariables[pivotLine].value.second.to_string() << std::endl;
        // Zero out the Theta column
        for (int i = 0; i < numRes; ++i) {
            tableArray[i][numVar+1] = Value::Number(0);
        }
        // Zero out the Cj - Zj column
        for (int i = 0; i <= numVar; ++i) {
            tableArray[numRes][i] = Value::Number(0);
        }
    }

    void Table::executeIterationChange() {
        /**
         * WE DON'T THE ENTIRE GAUSS JORDAN, YAY
         * 
         * We need however, to 0 out the column of the new base variable on all the other ones
        */

        Value::Number pivotElement = tableArray[pivotLine][pivotColumn];

        // Pivot line is easy, yay
        for (int i = 0; i <= numVar; ++i) {
            tableArray[pivotLine][i] = tableArray[pivotLine][i]/pivotElement;
        }

        // We will use the created line as reference for the next lines
        // We will find the value 'A' that makes 
        // tableArray[nonPivotLine][pivotColumn] -A*tableArray[pivotLine][pivotColumn]
        // Become zero

        // Non pivot lines is a bit harder
        // Pivot line is easy, yay
        for (int i = 0; i < numRes; ++i) {
            if (i == pivotLine) continue;
            // Value of the non pivot line on the pivot column, so we can always remember it ahead
            Value::Number pivotColumnEqualizer = tableArray[i][pivotColumn];
            for (int j = 0; j <= numVar; ++j) {
                tableArray[i][j] = tableArray[i][j] - tableArray[pivotLine][j]*pivotColumnEqualizer;
            }
        }

        // Discover which is the value to divide all by


    }

};