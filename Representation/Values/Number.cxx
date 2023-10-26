/**
 * @file Number.cxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File implemented to define the representation of a linear Number
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Number.hxx"
#include <limits.h>
#include <iomanip>
#include <string>
#include <cmath>

namespace Value {

    Number::Number() {
        value = 0;
        Mvalue = 0;
    }

    // We expect it to be checked before coming here
    Number::Number(std::string input) {
        value = std::stod(input);
        Mvalue = 0;
    }

    Number::Number(double inputValue, double inputMvalue) {
        value = inputValue;
        Mvalue = inputMvalue;
    }

    Number::Number(double inputValue) {
        value = inputValue;
        Mvalue = 0;
    }

    Number::~Number() {
        // Nothing lmao
    }

    Number& Number::operator=(Number input) {
        value = input.getValue();
        Mvalue = input.getMvalue();
        return *this;
    }

    Number& Number::operator=(double input){
        value = input;
        Mvalue = 0;
        return *this;
    }

    // This is to remove redundancy on creating restrictions
    Number& Number::operator=(std::string input) {
        value = std::stod(input);
        Mvalue = 0;
        return *this;
    }

    Number& Number::operator+(Number input) {
        value += input.getValue();
        Mvalue += input.getMvalue();
        return *this;
    }

    Number& Number::operator+(double input) {
        value += input;
        Mvalue += 0;
        return *this;
    }

    Number& Number::operator-(Number input) {
        value -= input.getValue();
        Mvalue -= input.getMvalue();
        return *this;
    }

    Number& Number::operator-(double input) {
        value -= input;
        Mvalue -= 0;
        return *this;
    }

    Number& Number::operator*(Number input) {
        Mvalue = Mvalue*input.getValue() + input.getMvalue()*value; // No scenarios where it multiplies with M and M
        value = value*input.getValue();
        return *this;
    }

    Number& Number::operator*(double input) {
        value *= input;
        Mvalue *= input;
        return *this;
    }

    Number& Number::operator/(Number input) {
        // No scenarios where it divides by M too
        // But some Number instances might just be normal ints
        if (input.getValue() != 0) {
            value /= input.getValue();
            Mvalue /= input.getValue();
        } else {
            value = INT_MAX;
            Mvalue = INT_MAX;
        }
        return *this;
    }

    Number& Number::operator/(double input) {
        if (input != 0) {
            value /= input;
            Mvalue /= input;
        } else {
            value = INT_MAX;
            Mvalue = INT_MAX;
        }
        return *this;
    }

    bool Number::operator==(Number input) {
        if (value == 0 && input.getValue() == 0) {
            return (Mvalue == input.getMvalue());
        }
        return (value == input.getValue());
    }

    bool Number::operator==(double input) {
        return (value == input);
    }

    bool Number::operator>(Number input) {
        /**
         * cases:
         * 1 + 0M
         * 1 + 1M
         * 0 + xM
         * 0 + 0M
         * 
        */
        
        if (value > 0) {
            return value > input.getValue();
        } else if (input.getValue() > 0) {
            return false;
        } else if (Mvalue > 0) { // Both value and input's value are 0 or negative from here
            return Mvalue > input.getMvalue();
        } else if (!(value == 0 && input.getValue() == 0)) { // Both not 0, so 0 > -1 or -1 > -1 or -1 > -4
            return value > input.getValue();
        }
        // normal value is hopeless (0 and 0)
        if (Mvalue > 0) {
            return Mvalue > input.getMvalue();
        } else if (input.getMvalue() > 0) {
            return false;
        } else if (Mvalue > 0) { // Both Mvalue and input's Mvalue are 0 or negative from here
            return Mvalue > input.getMvalue();
        } else if (!(Mvalue == 0 && input.getMvalue() == 0)) { // Both not 0, so 0 > -1 or -1 > -1 or -1 > -4
            return Mvalue > input.getMvalue();
        }
        return false;
    }

    bool Number::operator>(double input) {
        return (value > input);
    }

    bool Number::operator<(Number input) {
         if (value > 0) {
            return value < input.getValue();
        } else if (input.getValue() > 0) {
            return false;
        } else if (Mvalue > 0) { // Both value and input's value are 0 or negative from here
            return Mvalue < input.getMvalue();
        } else if (!(value == 0 && input.getValue() == 0)) { // Both not 0, so 0 > -1 or -1 > -1 or -1 > -4
            return value < input.getValue();
        }
        // normal value is hopeless (0 and 0)
        if (Mvalue > 0) {
            return Mvalue < input.getMvalue();
        } else if (input.getMvalue() > 0) {
            return false;
        } else if (Mvalue > 0) { // Both Mvalue and input's Mvalue are 0 or negative from here
            return Mvalue < input.getMvalue();
        } else if (!(Mvalue == 0 && input.getMvalue() == 0)) { // Both not 0, so 0 > -1 or -1 > -1 or -1 > -4
            return Mvalue < input.getMvalue();
        }
        return (value < input.getValue());
    }

    bool Number::operator<(double input) {
        return (value < input);
    }

    bool Number::operator>=(Number input) {
        if (value == 0 && input.getValue() == 0) {
            return (Mvalue >= input.getMvalue());
        }
        return (value >= input.getValue());
    }

    bool Number::operator>=(double input) {
        return (value >= input);
    }

    bool Number::operator<=(Number input) {
        if (value == 0 && input.getValue() == 0) {
            return (Mvalue <= input.getMvalue());
        }
        return (value <= input.getValue());
    }

    bool Number::operator<=(double input) {
        return (value <= input);
    }

    std::string Number::to_string() {
        if (Mvalue == 0 && value == 0) {
            return "0";
        } else if (Mvalue == 0) {
            return roundValue(value);
        } else if (value == 0) {
            return roundValue(-Mvalue) + "*M";
        }

        std::string symbol = (Mvalue > 0) ? " +" : " ";
        return "(" + roundValue(value) + symbol + roundValue(Mvalue) + "*M)";
    }

    std::string Number::to_string_no_m() {
        if (Mvalue == 0 && value == 0) {
            return "0";
        } else if (Mvalue == 0) {
            return roundValue(value);
        } else if (value == 0) {
            return roundValue(Mvalue);
        }
        return "";
    }

    bool Number::hasBothValues() {
        return (
            value != 0 &&
            Mvalue != 0
        );
    }

    std::string Number::roundValue(double input) {
        int precision = std::abs(input) < 1.0 ? 2 : 0;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << input;
        // Return the formatted value as a string because
        // rounding isn't fun in c++
        return ss.str();
    }
};
