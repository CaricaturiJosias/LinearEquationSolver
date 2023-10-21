/**
 * @file Restriction.hxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief Header file to describe the behaviour of Restrictions
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <map>
#include <vector>
#include "../Values/Number.hxx"

namespace LinearSystems {

    /**
     * First item: isSymbol?
     * Second item: number itself or symbol enum value
     * 
     * Example:
     *      2*A + 5*B - 12*C <= 20
     * equivalent:
     *      {{0, 2}, {0, 5}, {0, -12}, {1, LOWER_EQUAL}, {0, 20}}
     */
    typedef bool isSymbol;
    typedef std::pair<isSymbol, Value::Number> restrictionItem;
    typedef std::vector<restrictionItem> restriction;

    enum symbolEnum {
        NUMBER,
        LOWER,
        HIGHER,
        LOWER_EQUAL,
        HIGHER_EQUAL,
        EQUAL
    };

    extern std::vector <std::string> symbolVec;

    extern std::map<int, std::string> symbolMap;

    class Restriction {

        private:

            std::string askForInput(bool hasSymbol);

            restriction restrictionInstance;

            static bool isDouble(std::string input);

            static bool isSymbol(std::string input);

            static Value::Number getSymbolVal(std::string input);

            void displayRestriction();

        public:

            Restriction(int variables);

            ~Restriction();

            restriction getRestriction() { return restrictionInstance; }
            
            std::string to_string(int line = 0);

    };

};