/**
 * @file SolverMain.cxx
 * @author Gabriel Cezário (gabriel.cezario@pucpr.edu.br) and Milena Silvério (milena.silverio@pucpr.edu.br)
 * @brief File containing the main file for the Solver (TDE) for the Linear Systems Optimization (LSO)
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <string>
#include <iostream>
// #include "Representation/Values/Number.hxx"
#include "Representation/LinearSystems/Restriction.hxx"
#include "Representation/LinearSystems/System.hxx"
#include "Solver/Table.hxx"

int main () {
    // Get input from user
    // LinearSystems::Restriction * restriction = new LinearSystems::Restriction();
    LinearSystems::System * systemToSolve = new LinearSystems::System();
    Solver::Table table(systemToSolve);
    // Show final setup
}