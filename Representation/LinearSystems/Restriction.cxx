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
        std::string input;
        bool hasSymbol = false;
        Value::Number valueToStore;
        do {
            input = askForInput(hasSymbol);
            if (input == "DONE") {
                break;
            }

            if (isSymbol(input)) {
                hasSymbol = true;
                valueToStore = getSymbolVal(input);
            } else {
                valueToStore = input;
            }

            restrictionInstance.push_back(
                restrictionItem{
                    isSymbol(input),
                    valueToStore}
            );
        } while (input != "DONE");
        std::cout << to_string(restrictionInstance, 0) << std::endl;
    }

    Restriction::~Restriction() {
        // Nothing lol
    }

    std::string Restriction::askForInput(bool hasSymbol) {
        std::string input;
        std::cout << "Enter a number or anything else to exit: ";
        std::cin >> input;
        bool allowSymbol = isSymbol(input) && !hasSymbol;
        if (isDouble(input)) {
            return input;
        } else if (allowSymbol) {
            return input;
        }
        // Invalid
        return "DONE";
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
        auto it = std::find(symbolVec.begin(), symbolVec.end(), input);
        if (it != symbolVec.end()) {
            return true;
        }
        return false;
    }

    std::string Restriction::to_string(restriction instance, int line) {
        restriction::iterator it = instance.begin();
        std::string output = "R" + std::to_string(line) + ": ";
        int i = 1;
        bool afterSymbol = false;

        while (it != instance.end()) {
            std::cout << output << std::endl;
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
};