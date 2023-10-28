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
#include <iostream>

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
        Number * result = new Number(0);
        if (input.getValue() == INT_MAX) {
            result->setValue(INT_MAX);
        } else {
            result->setValue(value + input.getValue());
        }
        result->setMValue(Mvalue + input.getMvalue());
        return *result;
    }

    Number& Number::operator+(double input) {
        value += input;
        Mvalue += 0;
        return *this;
    }

    Number& Number::operator-(Number input) {
        Number * result = new Number(0);
        if (input.getValue() == INT_MAX) {
            result->setValue(-INT_MAX);
        } else {
            result->setValue(value + input.getValue());
        }
        result->setValue(value - input.getValue());
        result->setMValue(Mvalue - input.getMvalue());
        return *result;
    }

    Number& Number::operator-(double input) {
        value -= input;
        Mvalue -= 0;
        return *this;
    }

    Number& Number::operator*(Number input) {
        Number * result = new Number(0);
         // No scenarios where it multiplies with M and M
        result->setMValue(Mvalue*input.getValue() + input.getMvalue()*value); 
        result->setValue(value*input.getValue());
        return *result;
    }

    Number& Number::operator*(double input) {
        Number * result = new Number(0);
        result->setValue(value * input);
        result->setMValue(Mvalue * input);
        return *result;
    }

    Number& Number::operator/(Number input) {
        Number * result = new Number();
        // No scenarios where it divides by M too
        // But some Number instances might just be normal ints
        if (input.getValue() != 0) {
            result->setValue(value / input.getValue());
            result->setMValue(Mvalue / input.getValue());
        } else {
            result->setValue(value = INT_MAX);
        }
        return *result;
    }

    Number& Number::operator/(double input) {
        Number * result = new Number(0);
        if (input != 0) {
            result->setValue(value / input);
            result->setMValue(Mvalue / input);
        } else {
            result->setValue(INT_MAX);
        }
        return *result;
    }

    Number& Number::operator+=(Number input) {
        *this = *this + input;
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
        // std::cout   << "Value: " << value << std::endl
        //             << "MValue: " << Mvalue << std::endl
        //             << "input.Value: " << input.getValue() << std::endl
        //             << "input.Mvalue: " << input.getMvalue() << std::endl;
        // normal value is hopeless (0 and 0)
        if (Mvalue > 0) {
            // std::cout << "Return: " << (Mvalue > input.getMvalue()) << std::endl;
            return Mvalue > input.getMvalue();
        } else if (input.getMvalue() > 0) {
            // std::cout << "Return: " << (false) << std::endl;
            return false;
        } else if (Mvalue != 0 || input.getMvalue() != 0) { // Both not 0, so 0 > -1 or -1 > -1 or -1 > -4
            // std::cout << "Return: " << (Mvalue > input.getMvalue()) << std::endl;
            return Mvalue > input.getMvalue();
        }

        if (value > 0) {
            // std::cout << "Return: " << (value > input.getValue()) << std::endl;
            return value > input.getValue();
        } else if (input.getValue() > 0) {
            // std::cout << "Return: " << false << std::endl;
            return false;
        } else if (value != 0 || input.getValue() != 0) { // Both not 0, so 0 > -1 or -1 > -1 or -1 > -4
            // std::cout << "Return: " << (value > input.getValue()) << std::endl;
            return value > input.getValue();
        }

        return false;
    }

    bool Number::operator>(double input) {
        return (value > input);
    }

    bool Number::operator<(Number input) {
        // std::cout   << "Value: " << value << std::endl
        //             << "MValue: " << Mvalue << std::endl
        //             << "input.Value: " << input.getValue() << std::endl
        //             << "input.Mvalue: " << input.getMvalue() << std::endl;
        if (Mvalue > 0) {
            return Mvalue < input.getMvalue();
        } else if (input.getMvalue() > 0) {
            return false;
        } else if (Mvalue != 0 || input.getMvalue() != 0) { // Both not 0, so 0 > -1 or -1 > -1 or -1 > -4
            return Mvalue < input.getMvalue();
        }

        if (value > 0) {
            return value < input.getValue();
        } else if (input.getValue() > 0) {
            return false;
        } else if (value != 0 || input.getValue() != 0) { // Both not 0, so 0 > -1 or -1 > -1 or -1 > -4
            return value < input.getValue();
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

    bool Number::operator!=(Number input) {
        return value != input.getValue();
    }

    bool Number::operator!=(double input) {
        return value != input;
    }

    std::string Number::to_string() {
        if (value == INT_MAX) {
            return "∞";
        } else if (value == INT_MAX) {
            return "-∞";
        }

        if (Mvalue == 0 && value == 0) {
            return "0";
        } else if (Mvalue == 0) {
            return roundValue(value);
        } else if (value == 0) {
            return roundValue(Mvalue) + "*M";
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
        bool noDecimals = input == static_cast<int>(input);
        int precision = noDecimals ? 0 : 2;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << input;
        // Return the formatted value as a string because
        // rounding isn't fun in c++
        return ss.str();
    }
};
