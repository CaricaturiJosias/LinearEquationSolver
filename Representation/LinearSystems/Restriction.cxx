/**
 * @file Restriction.cxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File to define the behaviour of Restrictions
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Restriction.hxx"
#include <iostream>
#include <algorithm>
#include <utility>

namespace LinearSystems {

    std::vector <std::string> symbolVec {
        "<",
        ">",
        "<=",
        ">=",
        "="
    };

    std::map<int, std::string> symbolMap {
        {LOWER, "<"},
        {HIGHER, ">"},
        {LOWER_EQUAL, "<="},
        {HIGHER_EQUAL, ">="},
        {EQUAL, "="}
    };

    Restriction::Restriction() {

    }

    Restriction::Restriction(int variables, int restrictionNumber, objType type) :
        restrictionNumber(restrictionNumber), objectiveType(type), variableNumber(variables) {
        /**
         * Create a restriction entirely from user input
         * For example:
         * (input sequence)
         *  1   2   12  <=  4
         * should result in 
         * 1*x1 + 2*x2 + 12*x3 <= 4
         */

        restrictionInstance = static_cast<restrictionItem *>(malloc(sizeof(restrictionItem) * (variableNumber+2)));
        if (restrictionInstance == nullptr) {
            std::cout << "Failed to create the restriction of " << variableNumber << " variables" << std::endl;
        }

        std::string input;
        bool hasSymbol = false;
        Value::Number valueToStore;
        bool done = false;
        std::string message;
        // Run variables + 1, the +1 is for the symbol and the value on the right
        for (int i = 0; i < (variableNumber+2); ++i) {
            if (type != NONE && (i >= variableNumber)) {
                // Objetive asks nothing about symbol and right side value
                if (i == variableNumber) {
                    restrictionInstance[i] = 
                    restrictionItem{
                        static_cast<variableType>(SYMBOL),
                        Value::Number(EQUAL)};
                } else if (i > variableNumber) {
                    restrictionInstance[i] = 
                    restrictionItem{
                        static_cast<variableType>(VALUE),
                        Value::Number(0)};
                }
                continue;
            }
            input = askForInput(hasSymbol, message);
            bool isInputSymbol = isSymbol(input);

            if (hasSymbol && isInputSymbol) {
                // Prohibited
                --i;
                message = "Invalid input, more than 1 symbol (<, >, <=, >=, =) is prohibited";
                continue;
            }

            if (isInputSymbol) {
                hasSymbol = true;
                valueToStore = getSymbolVal(input);
                // Make remaining variables until symbol 0 if possible
                zeroOut(i);
            } else {
                // Should it be a pure value?
                if (i == variableNumber) { // No
                    message = "Please insert a symbol (<, >, <=, >=, =)";
                    --i;
                    continue;
                }

                // Value itself
                valueToStore = input;
            }
            if (objectiveType == MIN) {
                valueToStore = valueToStore*-1;
            }

            restrictionInstance[i] = 
                restrictionItem{
                    static_cast<variableType>(isInputSymbol),
                    Value::Number(valueToStore)};
        };
        displayRestriction();
    }

    Restriction::~Restriction() {
        // Nothing lol
    }

    // Make sure remaining variables are 0*xn
    void Restriction::zeroOut(int &symbolIndex) {
        if (symbolIndex >= variableNumber) {
            return;
        }

        for (int j = symbolIndex; j < variableNumber; j++) {
            restrictionInstance[j] = 
                restrictionItem{
                    variableType::VALUE, // Normal value
                    0};
            symbolIndex = variableNumber;
        }
    }

    std::string Restriction::askForInput(bool hasSymbol, std::string message) {
        displayRestriction();
        std::string input;
        std::cout << message << std::endl;
        std::cout << "Enter a number or equation/inequation symbol: ";
        std::cin >> input;
        bool allowSymbol = isSymbol(input) && !hasSymbol;
        if (isDouble(input)) {
            return input;
        } else if (allowSymbol) {
            return input;
        }
        // Invalid
        return "0";
    }

    bool Restriction::isDouble(std::string input) {
        try {
            size_t pos;

            // Try the conversion
            double result = std::stod(input, &pos);
            
            // Is there more text?
            if (pos != input.size()) {
                return false;
            }
            
            // Number ok
            return true;
        } catch (const std::invalid_argument& e) {
            // No number
            return false;
        } catch (const std::out_of_range& e) {
            // Number too large :c wont fit in double variable
            return false;
        }
    }

    bool Restriction::isSymbol(std::string input) {
        for (std::string symbol : symbolVec) {
            if (symbol == input) {
                return true;
            } 
        }
        return false;
    }

    std::string Restriction::to_string(int line) {
        std::string output;
        if (objectiveType == MAX) {
            output = "Z: ";
        } else if (objectiveType == MIN) {
            output = "C: ";
        } else {
            output = "R" + std::to_string(line) + ": ";
        }

        int j = 1;

        if (restrictionInstance == nullptr) {
            return output;
        }

        std::string symbol;
        for (int i = 0; i < (variableNumber+2); ++i) {
            if (objectiveType != NONE && i >= variableNumber) {
                    continue;
            }
            if (i == variableNumber) { // Its a symbol
                symbol = symbolMap[static_cast<int>(restrictionInstance[i].second.getValue())];
                if (symbol.empty()) {
                    output += " symbol";
                } else {
                    output += " " + symbol;
                }
                continue;
            } else if (i < (variableNumber)) {
                output += " " + restrictionInstance[i].second.to_string() + 
                            ("*x" + std::to_string(j++)); // No, numbers have x1 x2 ... xn after
                continue;
            }
            output += " " + restrictionInstance[i].second.to_string();
        }
        return output;
    }

    Value::Number Restriction::getSymbolVal(std::string input) {
        auto it = symbolMap.begin();
        while (it != symbolMap.end()) {
            if (input == it->second) {
                Value::Number value(it->first);
                return value;
            }
            ++it;
        }
        // Should never come to this, its a placeholder for compilation errors
        return -1;
    }

    void Restriction::displayRestriction() {
        system(CLEAR_COMMAND);
        std::cout << to_string(restrictionNumber) << std::endl;
    }

    Value::Number Restriction::getRestrictionSymbol() {
        return restrictionInstance[variableNumber].second;
    }

    void Restriction::addSlackVariable(std::vector<restrictionItem> &symbolVec, bool isFirst) {
        /**
         * I have a given restriction
         * Rn = 1*x1 -4*x2 + 7*x3 <= 10
         * 
         * And I have new slack variables x4, x5, x6, i want to insert them
         */

        // variableNumber + 1 is for the system without the slack variables

        int oldVariableNumber = variableNumber;
        variableNumber += symbolVec.size();

        restrictionItem * newRestrictionInstance = 
            static_cast<restrictionItem *>(
                malloc(sizeof(restrictionItem) * (variableNumber+2)));

        bool isNotEqualSign = restrictionInstance[oldVariableNumber].second.getValue() != symbolEnum::EQUAL;
        /**
         * If the variable is below the oldVariable number, just copy
         * 
         * If it is above the oldVariable number and is not == to the new variableNumber (symbol index)
         * 
         * then insert symbolVec[i-oldVariableNumber] which is the 
         * slack variable until the new symbol index
         */
        
        // Each restrictions can have in maximum 2 slack (variables)
        bool changeDone = false;

        for (int i = 0; i < (variableNumber+2); ++i) {

            bool isLastSlack = i == (variableNumber-1);
            bool isNextM = symbolVec[i-oldVariableNumber + 1].second.getMvalue();

            if (i < oldVariableNumber) {
                newRestrictionInstance[i] = restrictionInstance[i];

            } else if (i < variableNumber) {

                // Is the first restriction of the system and iterator is on the first slack
                if (((i-oldVariableNumber) == 0) && isFirst) {
                    symbolVec[i-oldVariableNumber].second.setValue(1);
                }

                // Current value is the one with 1, ignore the ones with 0
                // save it, set the next one to 1, zero out the current
                double currentItem = symbolVec[i-oldVariableNumber].second.getValue();
                double isMItem = symbolVec[i-oldVariableNumber].second.getMvalue();

                if (currentItem == 1 && !changeDone) {
                    changeDone = true;

                    // values with M are isolated, so (0,1) became (1,1)
                    if (isMItem) {
                        // We want to save as (0,1) so it doesnt comes out as (1 -1M)*xn but -1M*xn
                        symbolVec[i-oldVariableNumber].second.setValue(0);
                        symbolVec[i-oldVariableNumber].second.setMValue(1);

                    } else if (isNextM) {
                        // A value right before the M is always negative, as the M value
                        // exists to allow it to be < 0
                        symbolVec[i-oldVariableNumber].second.setValue(-1);
                    }

                    // If it contains an = already, we don't save any with the value 1
                    if (!isNotEqualSign) {
                        symbolVec[i-oldVariableNumber].second.setValue(0);
                        symbolVec[i-oldVariableNumber].second.setMValue(0);
                    }

                    newRestrictionInstance[i] = symbolVec[i-oldVariableNumber];

                    // Set up next variable
                    symbolVec[i-oldVariableNumber].second.setValue(0);

                    // If its the last item skips this, avoid core dump and resets all items
                    if (!isLastSlack) {
                        symbolVec[i-oldVariableNumber + 1].second.setValue(1);
                        // Some restrictions might have more than one artificial variable allowed
                        // And the second is always an M item, there is NEVER an restriction with
                        // Just a M item

                        if (isNextM) {
                            changeDone = false;
                        }
                    }

                    // Revert M item for next restriction for any restriction with =
                    if (!isNotEqualSign && isMItem) {
                        symbolVec[i-oldVariableNumber].second.setMValue(1);
                    }
                }
            } else {
                // For each step after the end of symbolVec, we should target symbol and result from
                // restrictionInstance (hence taking the size of artificial variables from i)
                newRestrictionInstance[i] = restrictionInstance[i-symbolVec.size()];

                // Replace the symbol <= or >= with =
                if (i == variableNumber && isNotEqualSign) {
                    newRestrictionInstance[i].second.setValue(symbolEnum::EQUAL);
                }
            }
        }

        restrictionInstance = newRestrictionInstance;
    }
    
    void Restriction::addArtificialVariableToObjective(std::vector<restrictionItem> &symbolVec) {
         /**
         * I have a given objective
         * Rn = 1*x1 -4*x2 + 7*x3 <= 10
         * 
         * And i have new artificial variables x4, x5, x6, i want to insert them AS 0
         */

        int oldVariableNumber = variableNumber;
        variableNumber += symbolVec.size();
        restrictionItem * newObjetiveInstance = 
            static_cast<restrictionItem *>(
                malloc(sizeof(restrictionItem) * (variableNumber+2)));
        bool isNotEqualSign = restrictionInstance[oldVariableNumber].second.getValue() != symbolEnum::EQUAL;

        /**
         * If the variable is below the oldVariable number, just copy
         * 
         * If it is above the oldVariable number and is not == to the new variableNumber (symbol index)
         * 
         * then insert symbolVec[i-oldVariableNumber] which is the 
         * artificial variable until the new symbol inedx
         */

        // Each restrictions can have in maximum 2 artificial
        for (int i = 0; i < (variableNumber+2); ++i) {
            if (i < oldVariableNumber) {
                newObjetiveInstance[i] = restrictionInstance[i];

            } else if (i < variableNumber) {

                if (symbolVec[i-oldVariableNumber].second.getMvalue()) {
                    symbolVec[i-oldVariableNumber].second.setMValue(-1);
                    newObjetiveInstance[i] = symbolVec[i-oldVariableNumber];
                    newObjetiveInstance[i].first = LinearSystems::SLACK_VARIABLE;
                }
                newObjetiveInstance[i] = symbolVec[i-oldVariableNumber];

            } else {
                // For each step after the end of symbolVec, we should target symbol and result from
                // restrictionInstance (hence taking the size of artificial variables from i)
                newObjetiveInstance[i] = restrictionInstance[i-symbolVec.size()];

                // Replace the symbol <= or >= with =
                if (i == variableNumber && isNotEqualSign) {
                    newObjetiveInstance[i].second.setValue(symbolEnum::EQUAL);
                    newObjetiveInstance[i].first = LinearSystems::SYMBOL;
                }
            }
        }

        restrictionInstance = newObjetiveInstance;
    }

};