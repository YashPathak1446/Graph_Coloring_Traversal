#include "proj6.h"

#include "CSVReader.h"
#include "InterferenceGraph.h"

#pragma once

#include <string>
#include <unordered_map>
#include <vector>


namespace proj6 {

    using Variable = std::string; // Variable is a string
    using Register = int; // Register is an integer
    using RegisterAssignment = std::unordered_map<Variable, Register>;// RegisterAssignment is a map from Variable to Register


    RegisterAssignment assignRegisters(const std::string& path_to_graph, int num_registers) noexcept;
    // This is the function you need to implement






    RegisterAssignment proj6::assignRegisters(const std::string& path_to_graph, int num_registers) noexcept
    {
        // Load the interference graph from the file
        InterferenceGraph<Variable> ig = CSVReader::load(path_to_graph);

        // Initialize the register assignment to be empty
        RegisterAssignment assignment;

        // Check if the number of registers is sufficient for the graph
        int max_degree = ig.getMaxDegree();// Get the maximum degree of any vertex in the graph
        if (max_degree + 1 > num_registers) {// If the number of registers is not sufficient
            return assignment;// Return an empty assignment
        }

        // Sort the vertices in decreasing order of degree
        auto sorted_vertices = ig.getVerticesSortedByDegree();

        // Allocate registers to vertices in the order given by sorted_vertices
        for (const auto& vertex : sorted_vertices) {// 
            // Collect the set of already-assigned registers that interfere with this vertex
            std::unordered_set<Register> interfering_registers;
            for (const auto& neighbor : ig.getNeighbors(vertex)) { // TODO: This is O(n^2) in the worst case
                if (assignment.find(neighbor) != assignment.end()) {// If the neighbor has already been assigned a register
                    interfering_registers.insert(assignment[neighbor]);// = (assignment[neighbor] + 1) % num_registers;
                }
            }

            // Find the lowest-numbered register that does not interfere with any neighbors
            Register chosen_register = 1; // Registers are 1-indexed
            while (interfering_registers.find(chosen_register) != interfering_registers.end()) {
                chosen_register++;// = (chosen_register + 1) % num_registers;
            }

            // Assign the chosen register to this vertex
            assignment[vertex] = chosen_register;
        }

        return assignment;
    }
}