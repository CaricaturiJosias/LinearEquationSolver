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
     restrictionNumber(restrictionNumber), objectiveType(type) {
        /**
         * Create a restriction entirely from user input
         * For example:
         * (input sequence)
         *  1   2   12  <=  4
         * should result in 
         * 1*x1 + 2*x2 + 12*x3 <= 4
         */
        std::string input;
        bool hasSymbol = false;
        Value::Number valueToStore;
        bool done = false;
        std::string message;
        // Run variables + 1, the +1 is for the symbol and the value on the right
        for (int i = 0; i <= variables+1; ++i) {
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
                zeroOut(i, variables);
            } else {
                // Should it be a pure value?
                if (i == variables) { // No
                    message = "Please insert a symbol (<, >, <=, >=, =)";
                    --i;
                    continue;
                }

                // Value itself
                valueToStore = input;
            }

            restrictionInstance.push_back(
                restrictionItem{
                    isInputSymbol,
                    valueToStore}
            );
        };
        displayRestriction();
    }

    Restriction::~Restriction() {
        // Nothing lol
    }

    // Make sure remaining variables are 0*xn
    void Restriction::zeroOut(int &symbolIndex, int variableNumber) {
        if (symbolIndex >= variableNumber) {
            return;
        }

        for (int j = symbolIndex; j < variableNumber; j++) {
            restrictionInstance.push_back(
                restrictionItem{
                    false, // Normal value
                    0}
            );
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
        restriction::iterator it = restrictionInstance.begin();
        std::string output;
        if (objectiveType == MAX) {
            output = "Z: ";
        } else if (objectiveType == MIN) {
            output = "C: ";
        } else {
            output = "R" + std::to_string(line) + ": ";
        }

        int i = 1;
        bool afterSymbol = false;

        while (it != restrictionInstance.end()) {
            if (it->first == true) { // Its a symbol
                output += " " + symbolMap[it->second.getValue()];
                afterSymbol = true;
                it++;
                continue;
            }
            output += " " + it->second.to_string() + 
                        ((afterSymbol) ? // Is this after the < > = <= >=?
                        "" : // Yes, numbers don't have a x after it
                        ("*x" + std::to_string(i++))); // No, numbers have x1 x2 ... xn after
            it++;
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

};