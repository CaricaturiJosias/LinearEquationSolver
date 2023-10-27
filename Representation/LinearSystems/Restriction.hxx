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

#ifdef _WIN32
#include <windows.h>
#define CLEAR_COMMAND "cls"
#else
#include <cstdlib>
#define CLEAR_COMMAND "clear"
#endif

namespace LinearSystems {

    /**
     * First item: isValue, symbol, or artificial?
     * Second item: number itself or symbol enum value
     * 
     * Example:
     *      2*A + 5*B - 12*C <= 20
     * equivalent:
     *      {{0, 2}, {0, 5}, {0, -12}, {1, LOWER_EQUAL}, {0, 20}}
     */
    enum variableType {
        VALUE,
        SYMBOL,
        ARTIFICIAL_VARIABLE
    };

    typedef variableType isSymbol;
    typedef std::pair<isSymbol, Value::Number> restrictionItem;

    enum symbolEnum {
        NUMBER,
        LOWER,
        HIGHER,
        LOWER_EQUAL,
        HIGHER_EQUAL,
        EQUAL
    };

    typedef enum objectiveType {
        NONE, // Its a restriction
        MIN,
        MAX
    } objType;

    extern std::vector <std::string> symbolVec;

    extern std::map<int, std::string> symbolMap;

    class Restriction {

        private:

            int restrictionNumber;

            int variableNumber;

            objType objectiveType;

            std::string askForInput(bool hasSymbol, std::string message);

            restrictionItem * restrictionInstance;

            static bool isDouble(std::string input);

            static bool isSymbol(std::string input);

            static Value::Number getSymbolVal(std::string input);

            void displayRestriction();

            void zeroOut(int &symbolIndex);

        public:

            Restriction();

            // TODO - REMOVE - THIS IS FOR QUICK TESTING
            Restriction(int i);

            Restriction(int variables, int restrictionNumber, objType type = NONE);

            ~Restriction();

            restrictionItem * getRestriction() { return restrictionInstance; }
            
            std::string to_string(int line = 0);

            int getRestrictionNumber() { return restrictionNumber; }
            
            int getVariableNumber() { return variableNumber; }

            Value::Number getRestrictionSymbol();

            void addArtificialVariable(std::vector<restrictionItem> &symbolVec, bool isFirst);

            void addArtificialVariableToObjective(std::vector<restrictionItem> &symbolVec);

    };

};