/**
 * @file Number.hxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief Header file to define the representation of a number
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <map>
#include <vector>

namespace Value {

    class Number {

        private:

            double value;
            double Mvalue;

        public:

            Number();
            Number(std::string input);
            Number(double value, double Mvalue);
            Number(double value);
            ~Number();
            
            double getValue() { return value; }
            double getMvalue() { return Mvalue; }

            void setValue(double newValue) { value = newValue; }
            void setMValue(double newValue) { Mvalue = newValue; }

            Number& operator=(Number input);
            Number& operator=(double input);
            Number& operator=(std::string input);

            Number& operator+(Number input);
            Number& operator+(double input);

            Number& operator-(Number input);
            Number& operator-(double input);

            Number& operator*(Number input);
            Number& operator*(double input);

            Number& operator/(Number input);
            Number& operator/(double input);

            Number& operator+=(Number input);

            bool operator==(Number input);
            bool operator==(double input);

            bool operator>(Number input);
            bool operator>(double input);

            bool operator<(Number input);
            bool operator<(double input);

            bool operator>=(Number input);
            bool operator>=(double input);

            bool operator<=(Number input);
            bool operator<=(double input);

            std::string to_string();

            std::string to_string_no_m();
            
            bool hasBothValues();

            std::string roundValue(double input);
    };

};